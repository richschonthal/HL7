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
#include "ThreadObject.h"

//--------------------------------------------------------------------------------
CThreadObjectPool::CThreadObjectPool(DWORD nFlags, DWORD nMaxSize, DWORD nMaxWaiting, DWORD nCleanUpInterval)
		: m_nFlags(nFlags)
		, m_nMaxSize(nMaxSize)
		, m_nMaxWaiting(nMaxWaiting)
		, m_nCleanUpInterval(nCleanUpInterval)
	{
	m_pActiveThreads = new CThreadObList;
	m_pWaitingThreads = new CThreadObList;
	if(m_nCleanUpInterval > 0)
		{
		m_pControlThread = new CThreadObjectPoolControlThread(this);
		m_pControlThread->StartThread();
		}
	else
		m_pControlThread = NULL;
	}

//--------------------------------------------------------------------------------
CThreadObjectPool::~CThreadObjectPool()
	{
	m_pControlThread->EndThread();
	delete m_pControlThread;
	m_pControlThread = NULL;

	if(m_pActiveThreads != NULL)
		m_pActiveThreads->WaitForAllToExit(30000);

	if(m_pWaitingThreads != NULL)
		m_pWaitingThreads->WaitForAllToExit(30000);

	delete m_pActiveThreads;
	delete m_pWaitingThreads;
	}

//--------------------------------------------------------------------------------
DWORD CThreadObjectPool::GetFlags() const
	{
	return m_nFlags;
	}

//--------------------------------------------------------------------------------
void CThreadObjectPool::SetFlags(DWORD nFlags)
	{
	m_nFlags = nFlags;
	}

//--------------------------------------------------------------------------------
DWORD CThreadObjectPool::GetMaxSize() const
	{
	return m_nMaxSize;
	}

//--------------------------------------------------------------------------------
void CThreadObjectPool::SetMaxSize(DWORD nSize)
	{
	m_nMaxSize = nSize;
	}

//--------------------------------------------------------------------------------
DWORD CThreadObjectPool::GetMaxWaiting() const
	{
	return m_nMaxWaiting;
	}

//--------------------------------------------------------------------------------
void CThreadObjectPool::SetMaxWaiting(DWORD nSize)
	{
	m_nMaxWaiting = nSize;
	}

//--------------------------------------------------------------------------------
DWORD CThreadObjectPool::GetCleanUpInterval() const
	{
	return m_nCleanUpInterval;
	}

//--------------------------------------------------------------------------------
void CThreadObjectPool::SetCleanUpInterval(DWORD nInterval)
	{
	if(nInterval > 0 && m_pControlThread == NULL)
		{
		m_pControlThread = new CThreadObjectPoolControlThread(this);
		m_pControlThread->StartThread();
		}
	else
		if(nInterval == 0 && m_pControlThread != NULL)
			{
			m_pControlThread->EndThread();
			delete m_pControlThread;
			m_pControlThread = NULL;
			}

	m_nCleanUpInterval = nInterval;
	}

//--------------------------------------------------------------------------------
CThreadObList* CThreadObjectPool::GetActiveThreads() const
	{
	return m_pActiveThreads;
	}

//--------------------------------------------------------------------------------
CThreadObList* CThreadObjectPool::GetWaitingThreads() const
	{
	return m_pWaitingThreads;
	}

//--------------------------------------------------------------------------------
bool CThreadObjectPool::AddThread(CThreadObject* pThread, bool bActive)
	{
	ASSERT(sizeof(DWORD) == sizeof(long));

	bool bRv;

	pThread->m_bActive = bActive;
	if(bActive)
		bRv = GetActiveThreads()->AddTail(pThread) != NULL;
	else
		bRv = GetWaitingThreads()->AddTail(pThread) != NULL;
	if(! bRv)
		{
		pThread->EndThread();
		delete pThread;
		}
	return bRv;
	}

//--------------------------------------------------------------------------------
bool CThreadObjectPool::SetThreadActive(CThreadObject* pThread)
	{
	CSingleLock lock(&GetActiveThreads()->m_mutex, false);
	if(! lock.Lock())
		return false;

	pThread->m_bActive = true;
	GetActiveThreads()->AddHead(pThread);
	return pThread->OnActivate();
	}

//--------------------------------------------------------------------------------
bool CThreadObjectPool::SetThreadWaiting(CThreadObject* pThread)
	{
	CSingleLock lock(&GetWaitingThreads()->m_mutex, false);
	if(! lock.Lock())
		return false;

	pThread->m_bActive = false;

	if((GetFlags() & TP_KEEPCLEAN) && GetWaitingThreads()->GetCount() >= (int) GetMaxWaiting())
		{
		pThread->PostMessage(WM_QUIT);
		return ::WaitForSingleObject(pThread->GetThreadHandle(), 3000) == WAIT_OBJECT_0;
		}

	GetWaitingThreads()->AddHead(pThread);
	return pThread->OnDeactivate();
	}

//--------------------------------------------------------------------------------
CThreadObject* CThreadObjectPool::GetNextWaiting(DWORD nWait)
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
	lock.Unlock();

	SetThreadActive(pThread);

	return pThread;
	}

//--------------------------------------------------------------------------------
bool CThreadObjectPool::IncreasePool()
	{
	return false;
	}

//--------------------------------------------------------------------------------
bool CThreadObjectPool::CanIncreasePool() const
	{
	return true;
	}

//--------------------------------------------------------------------------------
bool CThreadObjectPool::ReleaseActive(CThreadObject* pThread)
	{
	CSyncObject* pLocks[2];
	pLocks[0] = &GetWaitingThreads()->m_mutex;
	pLocks[1] = &GetActiveThreads()->m_mutex;

	CMultiLock lock(pLocks, 2, false);

	if(lock.Lock() == -1)
		return false;

	pThread->m_bActive = false;

	POSITION pos = GetActiveThreads()->Find(pThread);
	if(pos != NULL)
		GetActiveThreads()->RemoveAt(pos);

	pos = GetWaitingThreads()->Find(pThread);

	if((GetFlags() & TP_KEEPCLEAN) && GetWaitingThreads()->GetCount() >= (int) GetMaxWaiting())
		{
		if(pos != NULL)
			GetWaitingThreads()->RemoveAt(pos);

		lock.Unlock();
		// WM_DESTROY will cause the thread to exit, as if it received WM_QUIT
		// and then it will delete itself
		pThread->PostMessage(WM_DESTROY);
		return true;
		}

	if(pos == NULL)
		GetWaitingThreads()->AddTail(pThread);

	lock.Unlock();

	return pThread->OnDeactivate();
	}
