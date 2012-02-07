//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#include "stdafx.h"
#include "LockObject.h"

//--------------------------------------------------------------------------------
CLockObject::CLockObject(CSyncObject* pLock, DWORD nFlags)
	{
	m_pLock = pLock;
	m_nFlags = nFlags;
	}

//--------------------------------------------------------------------------------
CLockObject::~CLockObject()
	{
	if((m_nFlags & AUTO_DELETE) == AUTO_DELETE)
		{
		delete m_pLock;
		m_pLock = NULL;
		}
	}

//--------------------------------------------------------------------------------
CSyncObject* CLockObject::GetLockPtr()
	{
	return m_pLock;
	}

//--------------------------------------------------------------------------------
void CLockObject::SetLockPtr(CSyncObject* pLock, DWORD nFlags)
	{
	m_pLock = pLock;
	m_nFlags = nFlags;
	}

//--------------------------------------------------------------------------------
HANDLE CLockObject::GetLockHandle()
	{
	if(m_pLock == NULL)
		return INVALID_HANDLE_VALUE;
	return *m_pLock;
	}
