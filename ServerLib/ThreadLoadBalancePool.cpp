//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include <stdafx.h>
#include "ThreadObjectPoolControlThread.h"
#include "ThreadObjectPool.h"
#include "ThreadObject.h"

//--------------------------------------------------------------------------------
CThreadObject* CThreadLoadBalancePool::GetNextWaiting(DWORD nWait)
	{
	DWORD nStart = CTime::GetCurrentTime().GetTime();
	CThreadObject* pThread = NULL;
	
	CSyncObject* pLocks[2];
	pLocks[0] = &GetWaitingThreads()->m_mutex;
	pLocks[1] = &GetActiveThreads()->m_mutex;

	CMultiLock lock(pLocks, 2, false);

	for(;;)
		{
		// have we waited long enough?
		if(nWait != INFINITE && (DWORD) CTime::GetCurrentTime().GetTime() > nStart + nWait)
			return NULL;

		if(lock.Lock() == -1)
			return NULL;

		// no threads available?
		if(GetWaitingThreads()->GetCount() == 0)
			{
			ASSERT(GetMaxSize() != 0);

			// have we maxed out the pool?
			if(GetActiveThreads()->GetCount() == (long) GetMaxSize())
				{
				lock.Unlock();
				::Sleep(100);
				continue;
				}

			lock.Unlock();

			::Sleep(100);

			// cant add any more?
			if(! IncreasePool())
				continue;

			if(lock.Lock() == -1)
				return NULL;
			}

		break;
		}

	POSITION pos = GetWaitingThreads()->GetHeadPosition();
	if(pos == NULL)
		return NULL;

	pThread = GetWaitingThreads()->GetAt(pos);
	GetWaitingThreads()->RemoveAt(pos);
	GetWaitingThreads()->AddTail(pThread);
	lock.Unlock();

	// here's the big difference between this function and the
	// version in the ThreadPool
	// the difference is that here we dont move the thread to the active list
	// it will move itself when it's load is "full"
	pThread->OnActivate();

	return pThread;
	}

//--------------------------------------------------------------------------------
bool CThreadLoadBalancePool::ReleaseActive(CThreadObject* pThread)
	{
	return SetThreadWaiting(pThread);
	}

//--------------------------------------------------------------------------------
bool CThreadLoadBalancePool::SetThreadActive(CThreadObject* pThread)
	{
	CSyncObject* pLocks[2];
	pLocks[0] = &GetWaitingThreads()->m_mutex;
	pLocks[1] = &GetActiveThreads()->m_mutex;

	CMultiLock lock(pLocks, 2, false);

	if(lock.Lock() == -1)
		return false;

	POSITION pos = GetWaitingThreads()->Find(pThread);
	if(pos != NULL)
		GetWaitingThreads()->RemoveAt(pos);

	pos = GetActiveThreads()->Find(pThread);
	if(pos != NULL)
		GetActiveThreads()->RemoveAt(pos);

	pThread->m_bActive = true;
	GetActiveThreads()->AddTail(pThread);
	return pThread->OnActivate();
	}

//--------------------------------------------------------------------------------
bool CThreadLoadBalancePool::SetThreadWaiting(CThreadObject* pThread)
	{
	CSyncObject* pLocks[2];
	pLocks[0] = &GetWaitingThreads()->m_mutex;
	pLocks[1] = &GetActiveThreads()->m_mutex;

	CMultiLock lock(pLocks, 2, false);

	if(lock.Lock() == -1)
		return false;

	POSITION pos = GetActiveThreads()->Find(pThread);
	if(pos != NULL)
		GetActiveThreads()->RemoveAt(pos);

	pos = GetWaitingThreads()->Find(pThread);
	if(pos != NULL)
		GetWaitingThreads()->RemoveAt(pos);

	pThread->m_bActive = false;
	GetWaitingThreads()->AddTail(pThread);
	return pThread->OnDeactivate();
	}
