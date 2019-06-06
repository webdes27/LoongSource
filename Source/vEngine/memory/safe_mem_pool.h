//-----------------------------------------------------------------------------
// File: safe_mem_pool
// Desc: game tool mem pool 2.0
// Auth: Lyp
// Date: 2009-1-8	
// Last: 2009-3-18
//-----------------------------------------------------------------------------
#pragma once
namespace vEngine {
//-----------------------------------------------------------------------------
// �ڴ��(ע�⣺�ڴ�صĴ�С��ʾ:�ڴ�ص�ǰ�����ڴ�Ĵ�С)
// ��ʼ��ʱ��
// 1 �ⲿ�趨�ڴ�ص����������С
// 2 �ڴ�ز����κ��ڴ�Ԥ����
//
// �ⲿ�����ڴ�ʱ,�����жϣ�
// 1 ����ڴ���ж�Ӧ�ߴ�Ŀ��п�,ʹ�ÿ��п鷵��
// 2 û�п��п�ʱ,ֱ�������ڴ沢���ڴ�ǰ����tagMemItem,Ȼ�󷵻�
//
// �ⲿ�ͷ��ڴ�ʱ,�����ж�:
// 1 ����ͷ��ڴ���С>�ڴ������С,ֱ���ͷŵ�ϵͳ�ڴ�
// 2 �����ǰ�ڴ�ش�С+�ͷ��ڴ���С<�ڴ������С,ֱ�ӷ����ڴ��
// 3 ���������ռ��������ռ������¼������ĵ�2���������ͨ��������е�4��
// 4 ����ֱ���ͷ���ϵͳ�ڴ�
//-----------------------------------------------------------------------------
class VENGINE_API SafeMemPool
{
public:
	__forceinline LPVOID	Alloc(DWORD dwBytes);
	__forceinline VOID		Free(LPVOID pMem);
	__forceinline LPVOID	TryAlloc(DWORD dwBytes);	// ���Խ�����������
	__forceinline BOOL		TryFree(LPVOID pMem);		// ���Խ�����������
	__forceinline VOID		SetMaxSize(DWORD dwSize) { m_dwMaxSize = dwSize; }
	__forceinline DWORD		GetSize() { return m_dwCurrentSize; }
	__forceinline DWORD		GetMalloc() { return m_dwMalloc; }
	__forceinline DWORD		GetGC() { return m_dwGCTimes; }

	SafeMemPool(DWORD dwMaxPoolSize=16*1024*1024);
	~SafeMemPool();

private:
	struct tagNode	// �ڴ��ͷ����
	{
		tagNode*	pNext;
		tagNode*	pPrev;
		INT			nIndex;
		DWORD		dwSize;
		DWORD		dwUseTime;
		DWORD		pMem[1];	// ʵ���ڴ�ռ�
	};

	struct
	{
		tagNode*	pFirst;
		tagNode*	pLast;
	} m_Pool[16];

	DWORD m_dwMaxSize;		// �ⲿ�趨��������������ڴ�
	DWORD m_dwMalloc;			// ͳ���ã�ʵ��Malloc����
	DWORD m_dwGCTimes;		// ͳ���ã�ʵ�������ռ�����

	DWORD VOLATILE 	m_dwCurrentSize;	// �ڴ���п����ڴ�����
	LONG VOLATILE 	m_lLock;

	__forceinline VOID Lock() { while(InterlockedCompareExchange((LPLONG)&m_lLock, 1, 0)!= 0) Sleep(0); }
	__forceinline VOID Unlock() { InterlockedExchange((LPLONG)(&m_lLock), 0); }
	__forceinline bool TryLock() { return InterlockedCompareExchange((LPLONG)(&m_lLock), 1, 0) == 0;	}

	// �����ռ�
	__forceinline VOID GarbageCollect(DWORD dwExpectSize, DWORD dwUseTime);
	// ������ƥ��Ĵ�С
	__forceinline INT SizeToIndex(DWORD dwSize, DWORD& dwRealSize);
};


//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
LPVOID SafeMemPool::Alloc(DWORD dwBytes)
{
	DWORD dwRealSize = 0;
	INT nIndex = SizeToIndex(dwBytes, dwRealSize);
	if( GT_INVALID != nIndex )
	{
		if( m_Pool[nIndex].pFirst )	// ��ǰ����
		{
			Lock();
			if( m_Pool[nIndex].pFirst )	// �����У��ʹӳ������
			{
				tagNode* pNode = m_Pool[nIndex].pFirst;
				m_Pool[nIndex].pFirst = m_Pool[nIndex].pFirst->pNext;
				if( m_Pool[nIndex].pFirst )
					m_Pool[nIndex].pFirst->pPrev = NULL;
				else
					m_Pool[nIndex].pLast = NULL;
				m_dwCurrentSize -= dwRealSize;
				++pNode->dwUseTime;
				Unlock();
				return pNode->pMem;	
			}
			Unlock();
		}
	}

	++m_dwMalloc;
	tagNode* pNode = (tagNode*)malloc(dwRealSize + sizeof(tagNode) - sizeof(DWORD));
	if( !pNode )
		return NULL;

	pNode->nIndex = nIndex;
	pNode->dwSize = dwRealSize;
	pNode->pNext = NULL;
	pNode->pPrev = NULL;
	pNode->dwUseTime = 0;
	return pNode->pMem;	// ��ʵ���ڴ��з���
}


//-----------------------------------------------------------------------------
// �ͷ�
//-----------------------------------------------------------------------------
VOID SafeMemPool::Free(LPVOID pMem)
{
	tagNode* pNode = (tagNode*)(((LPBYTE)pMem) - sizeof(tagNode) + sizeof(DWORD));
	if( GT_INVALID != pNode->nIndex )
	{
		Lock();
		if( pNode->dwSize + m_dwCurrentSize > m_dwMaxSize && pNode->dwUseTime > 0 )
			GarbageCollect(pNode->dwSize*2, pNode->dwUseTime);	// �����ռ�

		if( pNode->dwSize + m_dwCurrentSize <= m_dwMaxSize ) // �ڴ�ؿ�������
		{
			pNode->pPrev = NULL;
			pNode->pNext = m_Pool[pNode->nIndex].pFirst;
			if( m_Pool[pNode->nIndex].pFirst )
				m_Pool[pNode->nIndex].pFirst->pPrev = pNode;
			else
				m_Pool[pNode->nIndex].pLast = pNode;

			m_Pool[pNode->nIndex].pFirst = pNode;
			m_dwCurrentSize += pNode->dwSize;
			Unlock();
			return;
		}
		Unlock();
	}

	free(pNode);
}


//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
LPVOID SafeMemPool::TryAlloc(DWORD dwBytes)
{
#if _WIN32_WINNT < 0x0400
	return Alloc(dwBytes);
#else
	DWORD dwRealSize = 0;
	INT nIndex = SizeToIndex(dwBytes, dwRealSize);
	if( GT_INVALID != nIndex )
	{
		if( !TryLock() )
			return NULL;

		if( m_Pool[nIndex].pFirst )	// �����У��ʹӳ������
		{
			tagNode* pNode = m_Pool[nIndex].pFirst;
			m_Pool[nIndex].pFirst = m_Pool[nIndex].pFirst->pNext;
			if( m_Pool[nIndex].pFirst )
				m_Pool[nIndex].pFirst->pPrev = NULL;
			else
				m_Pool[nIndex].pLast = NULL;
			m_dwCurrentSize -= dwRealSize;
			++pNode->dwUseTime;
			Unlock();
			return pNode->pMem;	
		}
		Unlock();
	}

	++m_dwMalloc;
	tagNode* pNode = (tagNode*)malloc(dwRealSize + sizeof(tagNode) - sizeof(DWORD));
	if( !pNode )
		return NULL;

	pNode->nIndex = nIndex;
	pNode->dwSize = dwRealSize;
	pNode->pNext = NULL;
	pNode->pPrev = NULL;
	pNode->dwUseTime = 0;
	return pNode->pMem;	// ��ʵ���ڴ��з���
#endif


}


//-----------------------------------------------------------------------------
// �ͷ�
//-----------------------------------------------------------------------------
BOOL SafeMemPool::TryFree(LPVOID pMem)
{
#if _WIN32_WINNT < 0x0400
	Free(pMem);
	return TRUE;
#else
	tagNode* pNode = (tagNode*)(((LPBYTE)pMem) - sizeof(tagNode) + sizeof(DWORD));
	if( GT_INVALID != pNode->nIndex )
	{
		if( !TryLock() )
			return FALSE;

		if( pNode->dwSize + m_dwCurrentSize > m_dwMaxSize && pNode->dwUseTime > 0 )
			GarbageCollect(pNode->dwSize*2, pNode->dwUseTime);	// �����ռ�

		if( pNode->dwSize + m_dwCurrentSize <= m_dwMaxSize ) // �ڴ�ؿ�������
		{
			pNode->pPrev = NULL;
			pNode->pNext = m_Pool[pNode->nIndex].pFirst;
			if( m_Pool[pNode->nIndex].pFirst )
				m_Pool[pNode->nIndex].pFirst->pPrev = pNode;
			else
				m_Pool[pNode->nIndex].pLast = pNode;

			m_Pool[pNode->nIndex].pFirst = pNode;
			m_dwCurrentSize += pNode->dwSize;
			Unlock();
			return TRUE;
		}
		Unlock();
	}
	free(pNode);
	return TRUE;
#endif
}



//-----------------------------------------------------------------------------
// �����ռ�
//-----------------------------------------------------------------------------
VOID SafeMemPool::GarbageCollect(DWORD dwExpectSize, DWORD dwUseTime)
{
	++m_dwGCTimes;
	DWORD dwFreeSize = 0;
	for(INT n=15; n>=0; --n)	// �����Ŀ�ʼ����
	{
		if( !m_Pool[n].pFirst )
			continue;

		tagNode* pNode = m_Pool[n].pLast; // �����ʼ�ͷţ���Ϊ�����Nodeʹ�ô�����
		while( pNode )
		{
			tagNode* pTempNode = pNode;
			pNode = pNode->pPrev;
			if( pTempNode->dwUseTime < dwUseTime )	// �ͷŴ˽ڵ�
			{
				if( pNode )
					pNode->pNext = pTempNode->pNext;
				if( pTempNode->pNext )
					pTempNode->pNext->pPrev = pNode;
				if( m_Pool[n].pLast == pTempNode )
					m_Pool[n].pLast = pNode;
				if( m_Pool[n].pFirst == pTempNode )
					m_Pool[n].pFirst = pTempNode->pNext;

				m_dwCurrentSize -= pTempNode->dwSize;
				dwFreeSize += pTempNode->dwSize;
				free(pTempNode);
			}

			if( dwFreeSize >= dwExpectSize )
				return;
		}
	}
}


//-----------------------------------------------------------------------------
// ������ƥ��Ĵ�С
//-----------------------------------------------------------------------------
INT SafeMemPool::SizeToIndex(DWORD dwSize, DWORD& dwRealSize)
{
	if( dwSize<=32 )		{ dwRealSize = 32;			return 0; }
	if( dwSize<=64 )		{ dwRealSize = 64;			return 1; }
	if( dwSize<=128 )		{ dwRealSize = 128;			return 2; }
	if( dwSize<=256 )		{ dwRealSize = 256;			return 3; }
	if( dwSize<=512 )		{ dwRealSize = 512;			return 4; }
	if( dwSize<=1024 )		{ dwRealSize = 1024;		return 5; }
	if( dwSize<=2*1024 )	{ dwRealSize = 2*1024;		return 6; }
	if( dwSize<=4*1024 )	{ dwRealSize = 4*1024;		return 7; }
	if( dwSize<=8*1024 )	{ dwRealSize = 8*1024;		return 8; }
	if( dwSize<=16*1024 )	{ dwRealSize = 16*1024;		return 9; }
	if( dwSize<=32*1024 )	{ dwRealSize = 32*1024;		return 10; }
	if( dwSize<=64*1024 )	{ dwRealSize = 64*1024;		return 11; }
	if( dwSize<=128*1024 )	{ dwRealSize = 128*1024;	return 12; }
	if( dwSize<=256*1024 )	{ dwRealSize = 256*1024;	return 13; }
	if( dwSize<=512*1024 )	{ dwRealSize = 512*1024;	return 14; }
	if( dwSize<=1024*1024 )	{ dwRealSize = 1024*1024;	return 15; }
	dwRealSize = dwSize;
	return GT_INVALID;
}
} // namespace vEngine {