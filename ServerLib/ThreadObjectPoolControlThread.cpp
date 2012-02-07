//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "ThreadObjectPoolControlThread.h"
#include "ThreadObjectPool.h"

//--------------------------------------------------------------------------------
CThreadObjectPoolControlThread::CThreadObjectPoolControlThread(CThreadObjectPool* pPool)
		: CThreadObject(NULL)
		, m_pPool(pPool)
	{
	m_ctLastCheck = CTime::GetCurrentTime();
	}

//--------------------------------------------------------------------------------
CThreadObjectPoolControlThread::~CThreadObjectPoolControlThread()
	{
	}

//--------------------------------------------------------------------------------
CThreadObjectPool* CThreadObjectPoolControlThread::GetPool() const
	{
	return m_pPool;
	}

//--------------------------------------------------------------------------------
bool CThreadObjectPoolControlThread::MainLoop()
	{
	CTime ctNow(CTime::GetCurrentTime());
	// is it time to check the thread pool?
	if(m_ctLastCheck + GetPool()->GetCleanUpInterval() < ctNow)
		{
		// 'now' is the last time the pool was checked
		m_ctLastCheck = ctNow;
		// attempt to lock the thread lists
		CSyncObject* locks[2];
		locks[0] = &GetPool()->GetWaitingThreads()->m_mutex;
		locks[1] = &GetPool()->GetActiveThreads()->m_mutex;

		CMultiLock lock(locks, 2, false);
		if(! lock.Lock(3000))
			return true;

		int nMax = (int) GetPool()->GetMaxWaiting();

		// are there too many waiting threads?
		while(GetPool()->GetWaitingThreads()->GetCount() > nMax)
			{
			POSITION pos = GetPool()->GetWaitingThreads()->GetHeadPosition();
			CThreadObject* pThread = GetPool()->GetWaitingThreads()->GetAt(pos);
			pThread->EndThread();
			delete pThread;
			GetPool()->GetWaitingThreads()->RemoveAt(pos);
			}
		}

	return true;
	}
