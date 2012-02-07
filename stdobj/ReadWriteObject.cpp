//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "ReadWriteObject.h"

//--------------------------------------------------------------------------------
CMap<__int64, __int64, LONG, LONG>* CReadWriteObject::m_pReaderMap = NULL;
HANDLE CReadWriteObject::m_hReaderMapMutex = NULL;
LONG CReadWriteObject::m_nReaderMapRefCount = 0;

//--------------------------------------------------------------------------------
CReadWriteObject::CReadWriteObject()
		: m_nReaderCount(0)
	{
	if(::InterlockedIncrement(&m_nReaderMapRefCount) == 1)
		{
		m_hReaderMapMutex = ::CreateMutex(NULL, FALSE, NULL);
		m_pReaderMap = new CMap<__int64, __int64, LONG, LONG>;
		m_pReaderMap->InitHashTable(2207);
		}

	}

//--------------------------------------------------------------------------------
CReadWriteObject::~CReadWriteObject()
	{
	if(::InterlockedDecrement(&m_nReaderMapRefCount) == 0)
		{
		delete m_pReaderMap;
		m_pReaderMap = NULL;
		::CloseHandle(m_hReaderMapMutex);
		}
	}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
CReadWriteLock::CReadWriteLock(CReadWriteObject* pRW)
		: m_pRW(pRW)
		, m_bIsLocked(false)
	{
#ifdef _DEBUG
	m_nLockNumber = 0;
#endif
	}

//--------------------------------------------------------------------------------
bool CReadWriteLock::Attach(CReadWriteObject* pRW)
	{
	if(m_bIsLocked || m_pRW != NULL)
		return false;
	m_pRW = pRW;
	return true;
	}

