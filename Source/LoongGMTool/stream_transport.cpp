//-----------------------------------------------------------------------------
//!\file stream_transport.cpp
//!\author Lyp
//!
//!\date 2003-03-17
//! last 2008-04-17
//!
//!\brief 广域网客户端通讯底层
/*
  in http://tangentsoft.net/wskfaq/newbie.html#howclose 

	The proper sequence for closing a TCP connection is:
	1.Finish sending data. 
	2.Call shutdown() with the how parameter set to 1. 
	3.Loop on recv() until it returns 0. 
	4.Call closesocket(). 

  in MSDN:
	To assure that all data is sent and received on a connected socket 
	before it is closed, an application should use shutdown to close connection 
	before calling closesocket. For example, to initiate a graceful disconnect:
	1.Call WSAAsyncSelect to register for FD_CLOSE notification. 
	2.Call shutdown with how=SD_SEND. 
	3.When FD_CLOSE received, call recv until zero returned, or SOCKET_ERROR. 
	4.Call closesocket. 

*/
// Copyright (c) 1985-2007 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include "stream_transport.h"

// 使用其它部件
#include "msg_queue_ts.h"

#define SECT_KIND			256
#define SECT_LEN			512
#define ONE_SECOND			1000

//-----------------------------------------------------------------------------
// construction / destruction
//-----------------------------------------------------------------------------
StreamTransport::StreamTransport()
{
	m_bEncrypt = FALSE;
	m_pSect = new BYTE[SECT_KIND*SECT_LEN];
	CreateSect();

	Clear();
}

StreamTransport::~StreamTransport()
{
	Destroy();
	SAFE_DEL_ARRAY(m_pSect);
	::WSACleanup();
}


//-----------------------------------------------------------------------------
// 创造加密源
//-----------------------------------------------------------------------------
VOID StreamTransport::CreateSect()
{
	INT nHolder = 0xDeadBeef;
	for(INT n=0; n<SECT_KIND*SECT_LEN; n++)
	{
		INT r = ((nHolder = nHolder * 214013L + 2531011L) >> 16) & 0x7fff;
		m_pSect[n] = (BYTE)r;
	}
}



//-----------------------------------------------------------------------------
// clear
//-----------------------------------------------------------------------------
VOID StreamTransport::Clear()
{
	m_pSendMsgQueue = NULL;
	m_pRecvMsgQueue = NULL;

	m_szIP[0] = 0;
	m_nPort = 0;
	m_Sock = 0;
	m_bNagle = TRUE;

	m_hConnect = 0;
	m_hSend = 0;
	m_hReceive = 0;

	::InterlockedExchange((LONG*)&m_bTerminateConnect, FALSE);
	::InterlockedExchange((LONG*)&m_bTerminateSend, FALSE);
	::InterlockedExchange((LONG*)&m_bTerminateReceive, FALSE);
	::InterlockedExchange((LONG*)&m_bConnected, FALSE);

	ZeroMemory(m_szHostName, sizeof(m_szHostName));
	m_bEncrypt = FALSE;
}



//-----------------------------------------------------------------------------
// init
//-----------------------------------------------------------------------------
BOOL StreamTransport::Init(BOOL bNagle, BOOL bPackageMode)
{
	Clear();

	m_bNagle = bNagle;
	m_bPackageMode = bPackageMode;
	WORD wVersionRequested;
	WSADATA wsaData;
 
	// 先尝试winsocket2.2
	wVersionRequested = MAKEWORD( 2, 2 );
 	INT nErr = WSAStartup( wVersionRequested, &wsaData );
	if( 0 != nErr ) 
	{
		// 尝试winsocket2.1
		wVersionRequested = MAKEWORD( 2, 1 );
	 	nErr = WSAStartup( wVersionRequested, &wsaData );
		if( 0 != nErr ) 
		{
			// 尝试winsocket1.1
			wVersionRequested = MAKEWORD( 1, 1 );
			nErr = WSAStartup( wVersionRequested, &wsaData );
			if( 0 != nErr ) 
			{
				return FALSE;
			}
		}
	}
	
	// 申请发送接受缓冲
	m_pSendMsgQueue = new MsgQueueTS(TRUE, m_bPackageMode);
	m_pRecvMsgQueue = new MsgQueueTS(FALSE, FALSE);
	if( NULL == m_pSendMsgQueue || NULL == m_pRecvMsgQueue )
	{
		SAFE_DEL(m_pSendMsgQueue);
		SAFE_DEL(m_pRecvMsgQueue);
		return FALSE;
	}

	//创建socket
	m_Sock = socket(AF_INET, SOCK_STREAM, 0);
	if( INVALID_SOCKET == m_Sock )
	{
		SAFE_DEL(m_pSendMsgQueue);
		SAFE_DEL(m_pRecvMsgQueue);
		return FALSE;
	}

	// 使用非阻塞方式,linger行为
	DWORD dwCmd = 1;
	INT nResult = ioctlsocket( m_Sock, FIONBIO, &dwCmd );
	if( SOCKET_ERROR == nResult )
	{
		closesocket( m_Sock );
		m_Sock = 0;
		SAFE_DEL(m_pSendMsgQueue);
		SAFE_DEL(m_pRecvMsgQueue);
		return FALSE;
	}

	// disables the Nagle algorithm for send coalescing
	if( !m_bNagle )
	{
		BOOL bNoDelay = TRUE;
		setsockopt(m_Sock, IPPROTO_TCP, TCP_NODELAY, (char*)&bNoDelay, sizeof(bNoDelay));
	}

	// set addr reusabel
	DWORD dwReUseAddr = TRUE;
	setsockopt( m_Sock, SOL_SOCKET, SO_REUSEADDR, 
		(char*)&dwReUseAddr, sizeof(dwReUseAddr) );

	// hard shut down
	linger lin;
	lin.l_linger = 0;
	lin.l_onoff = 1;
	setsockopt( m_Sock, SOL_SOCKET, SO_LINGER, (char*)&lin, sizeof(lin) );

	// 每秒流量统计
	m_dwLastStatOutTime = timeGetTime();
	m_dwLastStatInTime = timeGetTime();
	m_dwByteOut = 0;
	m_dwByteIn = 0;
	m_dwByteOutCounter = 0;
	m_dwByteInCounter = 0;

	m_dwSendPackNum = 0;
	m_dwRecvPackNum = 0;

	return TRUE;
}


//-----------------------------------------------------------------------------
// destroy
//-----------------------------------------------------------------------------
void StreamTransport::Destroy()
{
	// 设置结束线程标志
	::InterlockedExchange((LONG*)&m_bTerminateConnect, TRUE);
	::InterlockedExchange((LONG*)&m_bTerminateSend, TRUE);
	::InterlockedExchange((LONG*)&m_bTerminateReceive, TRUE);

	// 等待线程结束,一直等下去
	GracefulCloseThread(m_hConnect, INFINITE);
	GracefulCloseThread(m_hSend, INFINITE);
	GracefulCloseThread(m_hReceive, INFINITE);

	// 完美关闭socket
	if( TRUE == m_bConnected )
	{
		GracefulCloseSocket(m_Sock, 20);	// 重试 20 次
	}
	else
	{
		// m_bConnected尚未为真之前极短时间内可能已经连上
		// 所以先shutdown一下，并无副作用
		shutdown(m_Sock, SD_SEND);
		closesocket(m_Sock);
		m_Sock = 0;
	}

	// 关闭缓冲
	::InterlockedExchange((LONG*)&m_bConnected, FALSE);
	SAFE_DEL( m_pSendMsgQueue );
	SAFE_DEL( m_pRecvMsgQueue );
}



//-----------------------------------------------------------------------------
// 完美关闭socket
//-----------------------------------------------------------------------------
BOOL StreamTransport::GracefulCloseSocket(SOCKET& socket, INT nRetryTimes)
{
	if( 0 == socket )
		return TRUE;

	shutdown(socket, SD_SEND);
	
	INT nRetry = 0;
	fd_set fdread;
	timeval block_time;	// 设置阻塞时间
	block_time.tv_sec = 0;
	block_time.tv_usec = GT_NET_BLOCK_TIME;	// 150 ms 
	
	while( nRetry++ < nRetryTimes )
	{
		FD_ZERO(&fdread);
		FD_SET(socket, &fdread);
		if( 1 == select(0, &fdread, NULL, NULL, &block_time) )
		{
			CHAR chTemp;
			INT nResult = recv(socket, &chTemp, 1, 0);
			if( 0 == nResult )	// receive FIN, ok
			{
				closesocket( socket );
				socket = 0;
				return TRUE;
			}
			
			if( SOCKET_ERROR == nResult )	// network down
				break;
		}
	}
	

	closesocket( socket );
	// a hard shutdown
	socket = 0;
	return FALSE;
}


//-----------------------------------------------------------------------------
// 完美结束thread
//-----------------------------------------------------------------------------
BOOL StreamTransport::GracefulCloseThread(HANDLE& thread, INT nRetryTime)
{
	if( 0 == thread )
		return TRUE;

	DWORD dwResult = WaitForSingleObject(thread, nRetryTime);
	SAFE_CLOSE_HANDLE(thread);
	thread = 0;

	if( WAIT_TIMEOUT == dwResult )
		return FALSE;
	else
		return TRUE;
}




//-----------------------------------------------------------------------------
// 发送网络信息
//-----------------------------------------------------------------------------
BOOL StreamTransport::Send(LPVOID pMsg, DWORD dwMsgSize)
{
	ASSERT(NULL != pMsg);
	ASSERT(dwMsgSize > 0 && dwMsgSize < 0xFFFFFF);	// 数据过分长
	// 将发送信息添加到发送信息缓冲
	if( FALSE == m_bConnected )
		return FALSE;

	//初始化信息统计
	m_dwSendPackNum++;
	return m_pSendMsgQueue->AddMsg((LPBYTE)pMsg, dwMsgSize);
}



//-----------------------------------------------------------------------------
//! 接受网络信息,返回FALSE说明没有消息
//! 外部需要负责释放返回的内存指针
//-----------------------------------------------------------------------------
LPBYTE StreamTransport::Recv(DWORD& dwMsgSize)
{
	if( !P_VALID(m_pRecvMsgQueue) )
		return NULL;

	// 从接受缓冲接收消息
	return m_pRecvMsgQueue->GetMsg(dwMsgSize);
}



//-----------------------------------------------------------------------------
// 尝试连接指定地址
//-----------------------------------------------------------------------------
VOID StreamTransport::TryToConnect(LPCSTR szIP, INT nPort)
{
	Disconnect();
	
	strcpy(m_szIP, szIP);
	m_nPort = nPort;

	// 启动线程 connect
	m_hConnect = (HANDLE)_beginthreadex(NULL, 0, (THREADPROC)&StreamTransport::ThreadConnect, this, 0, NULL);
}


//-----------------------------------------------------------------------------
//! 返回是否正在尝试连接
//! 假如所有设置正确，但是远程主机并没有启动服务器，那么就会一直尝试连接
//! 但是如果网络发生错误，就有可能停止尝试连接
//-----------------------------------------------------------------------------
BOOL StreamTransport::IsTryingToConnect()
{
	if( !m_hConnect )
		return FALSE;

	DWORD dwConnectExitCode = STILL_ACTIVE;
	GetExitCodeThread(m_hConnect, &dwConnectExitCode);
	if( STILL_ACTIVE == dwConnectExitCode )
		return TRUE;
	else
		return FALSE;
}


//-----------------------------------------------------------------------------
// 放弃尝试连接
//-----------------------------------------------------------------------------
VOID StreamTransport::StopTryingConnect()
{
	// 如果我们已经连接上了，那么也会断开
	Destroy();
	Init(m_bNagle, m_bPackageMode);
}

//-----------------------------------------------------------------------------
// 正常断开连接
//-----------------------------------------------------------------------------
VOID StreamTransport::Disconnect()
{
	this->StopTryingConnect();
}


//-----------------------------------------------------------------------------
// 得到未发送包的数量，可以以此来判断网络负荷
//-----------------------------------------------------------------------------
INT	StreamTransport::GetUnsendPackageNum()
{
	if( !m_bConnected )
		return 0;

	return m_pSendMsgQueue->GetMsgNum();
}


//-----------------------------------------------------------------------------
// 得到已接受包的数量，可以以此来判断网络负荷
//-----------------------------------------------------------------------------
INT	StreamTransport::GetReceivedPackageNum()
{
	if( !m_bConnected )
		return 0;

	return m_pRecvMsgQueue->GetMsgNum();
}


//-----------------------------------------------------------------------------
// thread connect
//-----------------------------------------------------------------------------
UINT StreamTransport::ThreadConnect(LPVOID param)
{
	StreamTransport* pStreamTransport = (StreamTransport*)param;
	INT nResult = 0;
	sockaddr_in address;
	ZeroMemory(&address, sizeof(address));
	address.sin_addr.s_addr = inet_addr(pStreamTransport->m_szIP);
	address.sin_family = AF_INET;
	address.sin_port = htons(pStreamTransport->m_nPort);

	// 检查IP是否合法
	if( INADDR_NONE == address.sin_addr.s_addr )
	{
		goto __connected_terminated;
	}
	
	fd_set fdwrite;
	timeval block_time;	// 设置阻塞时间
	block_time.tv_sec = 0;
	block_time.tv_usec = GT_NET_BLOCK_TIME;
	
	// 连接到服务器
	// For connection-oriented, nonblocking sockets, it is often not possible 
	// to complete the connection immediately. In such a case, "connect"
	// returns the error WSAEWOULDBLOCK. However, the operation proceeds.
	nResult = connect(pStreamTransport->m_Sock, (sockaddr*)&(address), sizeof(address));
	if( SOCKET_ERROR == nResult )
	{	
		if( WSAEWOULDBLOCK == WSAGetLastError() )
		{
			for(INT n=0; n<64; n++) // 不停重试,外部可以通过调用StopTryingConnect停止重试
			{
				FD_ZERO(&fdwrite);
				FD_SET(pStreamTransport->m_Sock, &fdwrite);

				if( 1 == select(0, NULL, &fdwrite, NULL, &block_time) )
					goto __connected;

				if( pStreamTransport->m_bTerminateConnect )
					goto __connected_terminated;
			}
		}

		// 连接尝试失败
		goto __connected_terminated;
	}
	
__connected:

	//初始化信息统计
	pStreamTransport->m_dwSendPackNum = 0;
	pStreamTransport->m_dwRecvPackNum = 0; 

	// 启动线程 send
	pStreamTransport->m_hSend = (HANDLE)_beginthreadex(NULL, 0, (THREADPROC)&StreamTransport::ThreadSend, pStreamTransport, 0, NULL);

	// 启动线程 receive
	if( pStreamTransport->m_bPackageMode )
	{
		pStreamTransport->m_hReceive = (HANDLE)_beginthreadex(NULL, 0, (THREADPROC)&StreamTransport::ThreadReceive, pStreamTransport, 0, NULL);
	}
	else
	{
		pStreamTransport->m_hReceive = (HANDLE)_beginthreadex(NULL, 0, (THREADPROC)&StreamTransport::ThreadStreamReceive, pStreamTransport, 0, NULL);
	}

	// 连接成功
	::InterlockedExchange((LONG*)&pStreamTransport->m_bConnected, TRUE);
	_endthreadex(0);
	return 0;

__connected_terminated:
		
	// 连接失败
	::InterlockedExchange((LONG*)&pStreamTransport->m_bConnected, FALSE);
	_endthreadex(0);
	return 0;
}



//-----------------------------------------------------------------------------
// thread send
//-----------------------------------------------------------------------------
UINT StreamTransport::ThreadSend(LPVOID param)
{
	StreamTransport* pStreamTransport = (StreamTransport*)param;
	fd_set fdwrite;
	timeval block_time;	// 设置发送阻塞时间
	block_time.tv_sec = 0;
	block_time.tv_usec = GT_NET_BLOCK_TIME;	// 150ms

	LPBYTE	pMsg = NULL, pTempPtr = NULL;
	DWORD	dwMsgSize = 0;
	INT		nReturn = 0;

	while( FALSE == pStreamTransport->m_bTerminateSend )
	{
		for(;;)	// 注意内部的break
		{
			// 流量统计
			DWORD dwTime = timeGetTime();
			if( dwTime - pStreamTransport->m_dwLastStatOutTime >= ONE_SECOND ) 
			{
				pStreamTransport->m_dwLastStatOutTime = dwTime;
				pStreamTransport->m_dwByteOut = pStreamTransport->m_dwByteOutCounter;
				pStreamTransport->m_dwByteOutCounter = 0;
			}

			pTempPtr = pMsg = pStreamTransport->m_pSendMsgQueue->GetMsg(dwMsgSize);
			if( NULL == pMsg )
				break;	// 发完为止

			if( pStreamTransport->m_bEncrypt )	// 开始加密
			{
				DWORD dwRand = rand() % SECT_KIND;
				if( 0 == dwRand )
					dwRand = 1;
				pStreamTransport->Encrypt(pMsg, dwMsgSize, dwRand);
			}

			// send 可能不能一次将数据发完
			while( dwMsgSize > 0 && FALSE == pStreamTransport->m_bTerminateSend )
			{
				nReturn = send(pStreamTransport->m_Sock, (char*)pTempPtr, dwMsgSize, 0);
				if( SOCKET_ERROR == nReturn )
				{
					switch(WSAGetLastError())
					{
					case WSAEWOULDBLOCK:
					case WSAENOBUFS:
						// no buffer space is available within the transport 
						// system to hold the data to be transmitted
						// 等待数据可以发送的时机
						FD_ZERO(&fdwrite);
						FD_SET(pStreamTransport->m_Sock, &fdwrite);
						select(0, NULL, &fdwrite, NULL, &block_time);
						break;

					default:	// 故障
						shutdown(pStreamTransport->m_Sock, SD_SEND);
						goto __thread_send_end;
					}
				}
				else
				{
					pTempPtr += nReturn;
					dwMsgSize -= nReturn;
					pStreamTransport->m_dwByteOutCounter += nReturn;	// 流量统计
				}
			}

			delete[](pMsg);
			pTempPtr = pMsg = NULL;
		}

		::WaitForSingleObject(pStreamTransport->m_pSendMsgQueue->GetEvent(), 120);
	}


__thread_send_end:
	::InterlockedExchange((LONG*)&pStreamTransport->m_bConnected, FALSE);
	_endthreadex(0);
	return 0;
}



//-----------------------------------------------------------------------------
// receive thread function
//-----------------------------------------------------------------------------
UINT StreamTransport::ThreadReceive(LPVOID param)
{
	StreamTransport* pStreamTransport = (StreamTransport*)param;
	fd_set fdread;
	timeval block_time;	// 设置阻塞时间
	block_time.tv_sec = 0;
	block_time.tv_usec = GT_NET_BLOCK_TIME;

	INT nResult = 0;
	DWORD dwMsgSize = 0;
	DWORD dwTempSize = 0;
	CHAR* pTempPtr = NULL;

	// 循环获得数据
	while( FALSE == pStreamTransport->m_bTerminateReceive )
	{
		// 先接受DWORD型的信息长度信息
		dwTempSize = sizeof(DWORD);
		pTempPtr = (char*)&dwMsgSize;

		// 流量统计
		DWORD dwTime = timeGetTime();
		if( dwTime - pStreamTransport->m_dwLastStatInTime >= ONE_SECOND )
		{
			pStreamTransport->m_dwLastStatInTime = dwTime;
			pStreamTransport->m_dwByteIn = pStreamTransport->m_dwByteInCounter;
			pStreamTransport->m_dwByteInCounter = 0;
		}

		while( dwTempSize > 0 )
		{
			// 先取得数据的大小
			FD_ZERO(&fdread);
			FD_SET(pStreamTransport->m_Sock, &fdread);
			if( 1 == select(0, &fdread, NULL, NULL, &block_time) )
			{
				nResult = recv(pStreamTransport->m_Sock, pTempPtr, dwTempSize, 0);
				if( 0 == nResult )	// FIN
					goto __thread_receive_end;

				if( SOCKET_ERROR == nResult )
				{
					// 网络故障
					shutdown(pStreamTransport->m_Sock, SD_SEND);
					goto __thread_receive_end;
				}
				else
				{
					dwTempSize -= nResult;
					pTempPtr += nResult;

					// 流量统计
					pStreamTransport->m_dwByteInCounter += nResult;
				}
			}
			
			if( pStreamTransport->m_bTerminateReceive )
				goto __thread_receive_end;
		}

		BYTE byEncryptType = (BYTE)(dwMsgSize >> 24);	// 最高8位为加密状态
		dwMsgSize &= 0xffffff;	// 底24位为真正消息长度

		// ASSERT(dwMsgSize < 40960);	// 异常的数据
		if( SOCKET_ERROR != nResult )
		{
			char* pTemp = (char*)new BYTE[dwMsgSize];
			dwTempSize = dwMsgSize;
			pTempPtr = pTemp;
			while( dwTempSize > 0 )
			{
				FD_ZERO(&fdread);
				FD_SET(pStreamTransport->m_Sock, &fdread);
				if( 1 == select(0, &fdread, NULL, NULL, &block_time) )
				{
					nResult = recv(pStreamTransport->m_Sock, pTempPtr, dwTempSize, 0);
					if( 0 == nResult )	// FIN
					{
						// IMSG(_T("client get a FIN\r\n"));
						delete[](pTemp);
						goto __thread_receive_end;
					}

					if( nResult == SOCKET_ERROR )
					{
						// 网络故障
						shutdown(pStreamTransport->m_Sock, SD_SEND);
						goto __thread_receive_end;
					}
					else
					{
						dwTempSize -= nResult;
						pTempPtr += nResult;

						// 流量统计
						pStreamTransport->m_dwByteInCounter += nResult;
					}
					
					if( pStreamTransport->m_bTerminateReceive )
					{
						delete[](pTemp);
						goto __thread_receive_end;
					}
				}
			}
			
			if( 0 != byEncryptType )
			{
				// 设置加密标志
				::InterlockedExchange((LONG*)&pStreamTransport->m_bEncrypt, TRUE);
				pStreamTransport->Decrypt(pTemp, dwMsgSize, (DWORD)byEncryptType);
			}

			//初始化信息统计
			pStreamTransport->m_dwRecvPackNum++; 
			pStreamTransport->m_pRecvMsgQueue->AddMsg(pTemp, dwMsgSize);
			delete[](pTemp);
		}
	}

__thread_receive_end:
	::InterlockedExchange((LONG*)&pStreamTransport->m_bConnected, FALSE);
	_endthreadex(0);
	return 0;
}



//-----------------------------------------------------------------------------
// stream receive thread function
//-----------------------------------------------------------------------------
UINT StreamTransport::ThreadStreamReceive(LPVOID param)
{
	StreamTransport* pStreamTransport = (StreamTransport*)param;
	fd_set fdread;
	timeval block_time;	// 设置阻塞时间
	block_time.tv_sec = 0;
	block_time.tv_usec = GT_NET_BLOCK_TIME;

	CONST INT RECV_BUFF_SIZE = 2048;	// 2KB recv buff
	INT nResult = 0;
	LPBYTE pBuffer = new BYTE[RECV_BUFF_SIZE];

	// 循环获得数据
	while( FALSE == pStreamTransport->m_bTerminateReceive )
	{
		// 流量统计
		DWORD dwTime = timeGetTime();
		if( dwTime - pStreamTransport->m_dwLastStatInTime >= ONE_SECOND )
		{
			pStreamTransport->m_dwLastStatInTime = dwTime;
			pStreamTransport->m_dwByteIn = pStreamTransport->m_dwByteInCounter;
			pStreamTransport->m_dwByteInCounter = 0;
		}

		FD_ZERO(&fdread);
		FD_SET(pStreamTransport->m_Sock, &fdread);
		if( 1 == select(0, &fdread, NULL, NULL, &block_time) )
		{
			nResult = recv(pStreamTransport->m_Sock, (CHAR*)pBuffer, RECV_BUFF_SIZE, 0);
			if( 0 == nResult )	// FIN
			{
				goto __thread_stream_receive_end;
			}
	
			if( nResult == SOCKET_ERROR )
			{
				// 网络故障
				shutdown(pStreamTransport->m_Sock, SD_SEND);
				goto __thread_stream_receive_end;
			}
			else
			{
				// 流量统计
				pStreamTransport->m_dwByteInCounter += nResult;
				pStreamTransport->m_dwRecvPackNum++;

				// 保存数据
				pStreamTransport->m_pRecvMsgQueue->AddMsg(pBuffer, nResult);
			}
		}

	}

__thread_stream_receive_end:
	SAFE_DEL_ARRAY(pBuffer);
	::InterlockedExchange((LONG*)&pStreamTransport->m_bConnected, FALSE);
	_endthreadex(0);
	return 0;
}



//-----------------------------------------------------------------------------
// 得到本地主机名
//-----------------------------------------------------------------------------
CHAR* StreamTransport::GetHostName()
{
	gethostname(m_szHostName, sizeof(m_szHostName)-10);
	return m_szHostName;
}


//-----------------------------------------------------------------------------
// 得到指定主机的IP
//-----------------------------------------------------------------------------
BOOL StreamTransport::GetIPByHostName(DWORD& dwIP, LPCSTR lpHostName)
{
	ASSERT(lpHostName);
	hostent* p = gethostbyname(lpHostName);
	if( p )
	{
		INT n=0;
		while((DWORD*)p->h_addr_list[n])
		{
			dwIP = *((DWORD*)p->h_addr_list[n]);
			n++;
		}
		return TRUE;
	}
	else
		return FALSE;
}


//-----------------------------------------------------------------------------
// 得到本机局域网地址
//-----------------------------------------------------------------------------
BOOL StreamTransport::GetLocalLANIP(DWORD& dwIP)
{
    SOCKET sd = WSASocket(AF_INET, SOCK_DGRAM, 0, 0, 0, 0);
    if( sd == SOCKET_ERROR )
	{
		return FALSE;
    }

    INTERFACE_INFO InterfaceList[128];
    DWORD dwBytesReturned;
    if( SOCKET_ERROR == WSAIoctl(sd, SIO_GET_INTERFACE_LIST, 0, 0, 
		&InterfaceList,	sizeof(InterfaceList), &dwBytesReturned, 0, 0) ) 
	{
		closesocket(sd);
		return FALSE;
    }

	closesocket(sd);

	INT nNumInterfaces = dwBytesReturned / sizeof(INTERFACE_INFO);
    for( INT i = 0; i < nNumInterfaces; ++i) 
	{
		u_long nFlags = InterfaceList[i].iiFlags;
		if( !(nFlags & IFF_UP) )	// 必须可用
			continue;

		if( nFlags & IFF_LOOPBACK ) // 不能是 loopback
			continue;

        sockaddr_in *pAddress;
        pAddress = (sockaddr_in *) & (InterfaceList[i].iiAddress);
        dwIP = pAddress->sin_addr.S_un.S_addr;

		if( pAddress->sin_addr.S_un.S_un_b.s_b1 == 10 
			|| (pAddress->sin_addr.S_un.S_un_b.s_b1 == 172 && pAddress->sin_addr.S_un.S_un_b.s_b2 >= 16 && pAddress->sin_addr.S_un.S_un_b.s_b2 <= 31)
			|| (pAddress->sin_addr.S_un.S_un_b.s_b1 == 192 && pAddress->sin_addr.S_un.S_un_b.s_b2 == 168) )
			return TRUE;
    }

	return FALSE;
}


//-----------------------------------------------------------------------------
// 得到本机广域网地址
//-----------------------------------------------------------------------------
BOOL StreamTransport::GetLocalWANIP(DWORD& dwIP)
{
    SOCKET sd = WSASocket(AF_INET, SOCK_DGRAM, 0, 0, 0, 0);
    if( sd == SOCKET_ERROR )
	{
		return FALSE;
    }

    INTERFACE_INFO InterfaceList[128];
    DWORD dwBytesReturned;
    if( SOCKET_ERROR == WSAIoctl(sd, SIO_GET_INTERFACE_LIST, 0, 0, 
		&InterfaceList,	sizeof(InterfaceList), &dwBytesReturned, 0, 0) ) 
	{
		closesocket(sd);
		return FALSE;
    }

	closesocket(sd);

	INT nNumInterfaces = dwBytesReturned / sizeof(INTERFACE_INFO);
    for( INT i = 0; i < nNumInterfaces; ++i) 
	{
		u_long nFlags = InterfaceList[i].iiFlags;
		if( !(nFlags & IFF_UP) )	// 必须可用
			continue;

		if( nFlags & IFF_LOOPBACK ) // 不能是 loopback
			continue;

        sockaddr_in *pAddress;
        pAddress = (sockaddr_in *) & (InterfaceList[i].iiAddress);
        dwIP = pAddress->sin_addr.S_un.S_addr;

   		if( !( pAddress->sin_addr.S_un.S_un_b.s_b1 == 10 
			|| (pAddress->sin_addr.S_un.S_un_b.s_b1 == 172 && pAddress->sin_addr.S_un.S_un_b.s_b2 >= 16 && pAddress->sin_addr.S_un.S_un_b.s_b2 <= 31)
			|| (pAddress->sin_addr.S_un.S_un_b.s_b1 == 192 && pAddress->sin_addr.S_un.S_un_b.s_b2 == 168) ))
			return TRUE;
    }

	return FALSE;
}



//-----------------------------------------------------------------------------
// 将真正IP地址转化为IP地址字符串
//-----------------------------------------------------------------------------
CHAR* StreamTransport::IP2String(DWORD dwIP)
{
	in_addr addr;
	addr.S_un.S_addr = dwIP;
	return inet_ntoa(addr);
}

//-----------------------------------------------------------------------------
// 将IP地址字符串转化为真正IP地址
//-----------------------------------------------------------------------------
DWORD StreamTransport::StringIP2IP(CHAR* szIP)
{
	return inet_addr(szIP);
}


//-----------------------------------------------------------------------------
// 加密
//-----------------------------------------------------------------------------
VOID StreamTransport::Encrypt(LPVOID pMsg, DWORD dwRealSize, DWORD dwEncryptType)
{
	// 注意这里传进来的dwRealSize包括前面一个DWORD长度
	INT nSect = dwEncryptType % SECT_KIND;
	PBYTE p1 = (PBYTE)pMsg;
	*(p1+sizeof(DWORD)-1) = BYTE(nSect);
	PBYTE p2_start = &m_pSect[nSect*SECT_LEN];
	PBYTE p2 = p2_start;

	// 包头作特殊处理
	p1 += sizeof(DWORD);

	DWORD dwSize = dwRealSize / 8 * 8;
	if( dwSize > dwRealSize - sizeof(DWORD) )
		dwSize = dwRealSize - sizeof(DWORD);

	while( dwSize-- )
	{
		*p1++ = (*p1)^(*p2++);
		if( p2 >= p2_start + SECT_LEN )
			p2 = p2_start;
	}
}


//-----------------------------------------------------------------------------
// 解密
//-----------------------------------------------------------------------------
VOID StreamTransport::Decrypt(LPVOID pMsg, DWORD dwRealSize, DWORD dwEncryptType)
{
	INT nSect = dwEncryptType % SECT_KIND;
	PBYTE p1 = (PBYTE)pMsg;
	PBYTE p2_start = &m_pSect[nSect*SECT_LEN];
	PBYTE p2 = p2_start;

	dwRealSize = ((dwRealSize+sizeof(DWORD)) / 8 * 8) - sizeof(DWORD);

	// 包头作特殊处理
	p2 += sizeof(DWORD);
	while(dwRealSize--)
	{
		*p1++ = (*p1)^(*p2++);
		if( p2 >= p2_start + SECT_LEN )
			p2 = p2_start;
	}

}
