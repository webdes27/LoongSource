//-----------------------------------------------------------------------------
//!\file login_db.h
//!\author songg
//!
//!\date 2009-08-31
//! last 2009-08-31
//!
//!\brief login服务器数据库
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
// 游戏世界数据库
//-----------------------------------------------------------------------------
class LoginDB
{
public:
	//-------------------------------------------------------------------------
	// CONSTRUCT
	//-------------------------------------------------------------------------
	LoginDB() {}
	~LoginDB() {}

	//-------------------------------------------------------------------------
	// 初始化和销毁
	//-------------------------------------------------------------------------
	BOOL	Init();
	VOID	Update();
	VOID	Destroy();

	//-------------------------------------------------------------------------
	// 是否断开连接
	//-------------------------------------------------------------------------
	BOOL	IsConLost()				{ return m_DB.IsConnLost(); }

	//--------------------------------------------------------------------------
	// 账号控制函数
	//--------------------------------------------------------------------------
	BOOL	InsertAccout(LPCSTR szAccountName, LPCSTR szPsd, LPCSTR szMiBao, BOOL bGuard);

	//--------------------------------------------------------------------------
	// 辅助函数
	//--------------------------------------------------------------------------
	DWORD	GetAccountID(const LPCSTR szAccountName);

	BOOL	LockTables(LPCSTR szTableNames);
	BOOL	UnlockTables();
private:
	//--------------------------------------------------------------------------
	// 重连
	//--------------------------------------------------------------------------
	VOID	Reconnect();

private:
	DataBase				m_DB;				// Beton数据库

};

//-----------------------------------------------------------------------------
// 全局变量
//-----------------------------------------------------------------------------
extern LoginDB g_LoginDB;