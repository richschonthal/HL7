//--------------------------------------------------------------------------------
//
// Copyright (c) 2001 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "HL7SocketDaemonThread.h"
#include "MultiSocketLock.h"

//--------------------------------------------------------------------------------
CMultiSocketLock::CMultiSocketLock(CSocketWrapper* pSock1, CSocketWrapper* pSock2)
	{
	CSyncObject* sockLocks[2] = { &pSock1->GetMutex(), &pSock2->GetMutex() };
	m_pLock = new CMultiLock(sockLocks, 2);
	if(m_pLock->Lock() == -1)
		{
		delete m_pLock;
		m_pLock = NULL;
		}
	}

//--------------------------------------------------------------------------------
CMultiSocketLock::~CMultiSocketLock()
	{
	Unlock();
	}

//--------------------------------------------------------------------------------
bool CMultiSocketLock::IsLocked()
	{
	if(m_pLock == NULL)
		return false;

	return m_pLock->IsLocked(0) && m_pLock->IsLocked(1);
	}

//--------------------------------------------------------------------------------
void CMultiSocketLock::Unlock()
	{
	if(! IsLocked())
		return;

//	m_pLock->Unlock();
	delete m_pLock;
	m_pLock = NULL;
	}
