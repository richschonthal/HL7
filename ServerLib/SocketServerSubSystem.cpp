//--------------------------------------------------------------------------------
// Copyright (c) 2001 @COMPANY, Inc.
// Created...: 6/26/01
// Author....: Rich Schonthal
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#include "StdAfx.h"
#include "SocketServerSubSystem.h"
#include "SocketServerMainThread.h"

#include <ReadLock.h>
#include <WriteLock.h>

//--------------------------------------------------------------------------------
//IMPLEMENT_DYNAMIC(CSocketServerSubSystem, CThreadPoolSubSystem)

//--------------------------------------------------------------------------------
CSocketServerSubSystem::CSocketServerSubSystem(CSystem* pParent)
		: CThreadPoolSubSystem(pParent)
		, m_nPort(0)
		, m_pIP(NULL)
		, m_nMaxThreads(0)
	{
	m_evtPaused.SetEvent();
	}

//--------------------------------------------------------------------------------
CSocketServerSubSystem::~CSocketServerSubSystem()
	{
	}

//--------------------------------------------------------------------------------
void CSocketServerSubSystem::SetMaxThread(int nMax)
	{
	}

//--------------------------------------------------------------------------------
bool CSocketServerSubSystem::IsClosePortsOnPause()
	{
	return false;
	}

//--------------------------------------------------------------------------------
void CSocketServerSubSystem::StartSubSystem()
	{
	if(m_pMainThread == NULL)
		{
		m_pMainThread = new CSocketServerMainThread(this);
		m_pMainThread->StartThread();
		}

	CSubSystem::StartSubSystem();
	}

//--------------------------------------------------------------------------------
void CSocketServerSubSystem::SetIP(LPCTSTR pIP)
	{
	CWriteLock lock(this);

	AllocString(m_pIP, pIP);
	}

//--------------------------------------------------------------------------------
void CSocketServerSubSystem::SetPort(int nPort)
	{
	CWriteLock lock(this);

	m_nPort = nPort;
	}

//--------------------------------------------------------------------------------
LPCTSTR CSocketServerSubSystem::GetIP() const
	{
	CReadLock lock(const_cast<CSocketServerSubSystem*>(this));
	return m_pIP;
	}

//--------------------------------------------------------------------------------
int CSocketServerSubSystem::GetPort() const
	{
	CReadLock lock(const_cast<CSocketServerSubSystem*>(this));
	return m_nPort;
	}

//--------------------------------------------------------------------------------
int CSocketServerSubSystem::GetMaxThreads() const
	{
	CReadLock lock(const_cast<CSocketServerSubSystem*>(this));
	return m_nMaxThreads;
	}
