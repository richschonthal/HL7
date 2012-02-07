//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "LockedResource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//--------------------------------------------------------------------------------
CLockedResource::CLockedResource()
	{
	m_nCurrentUserCount = 0;
	}

//--------------------------------------------------------------------------------
CLockedResource::~CLockedResource()
	{
	ASSERT(m_nCurrentUserCount < 1);
	}

//--------------------------------------------------------------------------------
bool CLockedResource::AddUser(DWORD nWait)
	{
	CSingleLock lock(&m_mutex, false);
	if(! lock.Lock(nWait))
		return false;
	InterlockedIncrement(&m_nCurrentUserCount);
	return true;
	}

//--------------------------------------------------------------------------------
void CLockedResource::RemoveUser()
	{
	InterlockedDecrement(&m_nCurrentUserCount);
	}

//--------------------------------------------------------------------------------
long CLockedResource::GetCurrentUserCount() const
	{
	return m_nCurrentUserCount;
	}

//--------------------------------------------------------------------------------
CMutex& CLockedResource::GetMutex()
	{
	return m_mutex;
	}

//--------------------------------------------------------------------------------
CSingleLock* CLockedResource::CreateExclusiveLock(DWORD nWait)
	{
	CSingleLock* pLock = new CSingleLock(&m_mutex, false);

	if(! GetExclusiveLock(*pLock, nWait))
		{
		delete pLock;
		return NULL;
		}

	return pLock;
	}

//--------------------------------------------------------------------------------
bool CLockedResource::GetExclusiveLock(CSingleLock& rLock, DWORD nWait)
	{
	nWait /= 2;

	if(! rLock.Lock(nWait))
		return false;

	// if there are still readers then wait til they're done reading
	if(m_nCurrentUserCount > 0)
		{
		CTime ctStart(CTime::GetCurrentTime());

		::Sleep(20);

		while(m_nCurrentUserCount > 0)
			{
			if(nWait != INFINITE)
				// if the wait time is exceeded then bail
				if(ctStart + nWait < CTime::GetCurrentTime())
					{
					rLock.Unlock();
					return false;
					}

			::Sleep(20);
			}
		}

	m_nCurrentUserCount = 0;
	return true;
	}

