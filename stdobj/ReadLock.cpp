//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "ReadLock.h"
#include "WriteLock.h"

#include "DebugPrintf.h"

#ifdef _DEBUG
static LONG g_nReadLockNumber = 0;
#endif

//--------------------------------------------------------------------------------
CReadLock::CReadLock(CReadWriteObject* pRW, bool bInitialLock)
		: CReadWriteLock(pRW)
	{
#ifdef _DEBUG
	m_nLockNumber = ::InterlockedIncrement(&g_nReadLockNumber);
#endif

	if(bInitialLock)
		Lock(INFINITE);
	}

//--------------------------------------------------------------------------------
CReadLock::~CReadLock()
	{
	Unlock();
	}

//--------------------------------------------------------------------------------
bool CReadLock::Lock(DWORD nWait)
	{
	if(m_pRW == NULL)
		return false;

	if(m_bIsLocked)
		return true;

	HANDLE hands[2] = { (HANDLE) CReadWriteObject::m_hReaderMapMutex , m_pRW->m_mutexWriter };

	DWORD nRes = ::WaitForMultipleObjects(2, hands, TRUE, nWait);

	if(nRes == WAIT_OBJECT_0 || nRes == (WAIT_OBJECT_0 + 1))
		{
		// lookup or add our thread to the reader map for this CReadWriteObject
		LARGE_INTEGER nId = { GetCurrentThreadId(), (DWORD) m_pRW };
		LONG nCount = 0;
		CReadWriteObject::m_pReaderMap->Lookup(nId.QuadPart, nCount);
		nCount++;
		CReadWriteObject::m_pReaderMap->SetAt(nId.QuadPart, nCount);
		::InterlockedIncrement(&m_pRW->m_nReaderCount);
		m_bIsLocked = true;

		::ReleaseMutex(CReadWriteObject::m_hReaderMapMutex);
		m_pRW->m_mutexWriter.Unlock();
		}

#ifdef _DEBUG
//	TRACE("readlock # %d %s locked\n", m_nLockNumber, m_bIsLocked ? "is" : "failed to");
#endif

	return m_bIsLocked;
	}

//--------------------------------------------------------------------------------
bool CReadLock::Unlock()
	{
	bool bRv = true;
	if(m_pRW != NULL && m_bIsLocked && 
			::WaitForSingleObject(CReadWriteObject::m_hReaderMapMutex, INFINITE) == WAIT_OBJECT_0)
		{
		LARGE_INTEGER nId = { GetCurrentThreadId(), (DWORD) m_pRW };
		LONG nCount = 0;
		if(CReadWriteObject::m_pReaderMap->Lookup(nId.QuadPart, nCount))
			{
			nCount--;
			if(nCount == 0)
				CReadWriteObject::m_pReaderMap->RemoveKey(nId.QuadPart);
			else
				CReadWriteObject::m_pReaderMap->SetAt(nId.QuadPart, nCount);
			}

		::InterlockedDecrement(&m_pRW->m_nReaderCount);
		m_bIsLocked = false;
		::ReleaseMutex(CReadWriteObject::m_hReaderMapMutex);
		}
	else
		bRv = false;

#ifdef _DEBUG
//	TRACE("readlock # %d %s unlocked\n", m_nLockNumber, bRv ? "is" : "failed to");
#endif
	return bRv;
	}
