//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "SubSystem.h"
#include "ThreadObject.h"
#include "System.h"

#include <ReadLock.h>

//--------------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CThreadPoolSubSystem, CSubSystem)

//--------------------------------------------------------------------------------
CThreadPoolSubSystem::CThreadPoolSubSystem(CSystem* pParent)
		: CSubSystem(pParent)
		, m_nBusyThreadCount(0)
		, m_nProcessorCount(0)
	{
	}

//--------------------------------------------------------------------------------
CThreadPoolSubSystem::~CThreadPoolSubSystem()
	{
	CWriteLock lock(&m_threads);
	if(m_threads.GetCount() != 0)
		{
		for(POSITION pos = m_threads.GetHeadPosition(); pos != NULL; )
			{
			CThreadObject* pThread = m_threads.GetNext(pos);
			pThread->EndThread();
			delete pThread;
			}
		}
	}

//--------------------------------------------------------------------------------
CThreadObject* CThreadPoolSubSystem::CreateThread(CRuntimeClass* pRuntime)
	{
	if(pRuntime == NULL)
		return NULL;

	CThreadObject* pThread = (CThreadObject*) pRuntime->CreateObject();
	if(pThread == NULL)
		return NULL;

	pThread->SetParent(this);
	return pThread;
	}

//--------------------------------------------------------------------------------
bool CThreadPoolSubSystem::WaitForAllToExit(DWORD nWait)
	{
	CReadLock lock(&m_threads, false);
	if(! lock.Lock())
		return false;

	// get all the thread's handles so we can wait for them to signal
	int nCount = m_threads.GetCount();
	HANDLE* pHands = new HANDLE[nCount];
	POSITION pos = m_threads.GetHeadPosition();
	for(int i = 0; pos != NULL; i++)
		pHands[i] = m_threads.GetNext(pos)->GetThreadHandle();

	DWORD nResult = (nCount > 0) ? ::WaitForMultipleObjects(nCount, pHands, TRUE, nWait) : 0;

	delete[] pHands;

	return nResult >= WAIT_OBJECT_0 && nResult < WAIT_OBJECT_0 + nCount;
	}

//--------------------------------------------------------------------------------
bool CThreadPoolSubSystem::PostMessage(UINT nMsg, WPARAM wparam, LPARAM lparam)
	{
	CReadLock lock(&m_threads);
	if(m_threads.GetCount() == 0)
		return false;

	for(POSITION pos = m_threads.GetHeadPosition(); pos != NULL; )
		m_threads.GetNext(pos)->PostMessage(nMsg, wparam, lparam);

	return true;
	}

//--------------------------------------------------------------------------------
bool CThreadPoolSubSystem::PostNextThreadMessage(UINT nMsg, WPARAM wparam, LPARAM lparam)
	{
	CWriteLock lock(&m_threads);
	int nCount = m_threads.GetCount();
	if(nCount == 0 || nCount == m_nBusyThreadCount)
		return false;

	for(;;)
		{
		CThreadObject* pThread = m_threads.GetNextAndRequeue();
		if(pThread->IsThreadBusy(20))
			continue;
		return pThread->PostMessage(nMsg, wparam, lparam);
		}
	}

//--------------------------------------------------------------------------------
bool CThreadPoolSubSystem::AddThread(CRuntimeClass* pClass, int nCount)
	{
	if(nCount == 0)
		return false;
	// add a thread for each processor in the system
	if(nCount == -1)
		{
		if(m_nProcessorCount != 0)
			nCount = m_nProcessorCount;
		else
			{
			SYSTEM_INFO info;
			::GetSystemInfo(&info);
			nCount = m_nProcessorCount = info.dwNumberOfProcessors;
			}
		}

	for(int i = 0; i < nCount; i++)
		{
		CThreadObject* pThread = CreateThread(pClass);
		if(pThread == NULL)
			return false;
		pThread->StartThread();
		m_threads.Add(pThread);
		}

	return true;
	}

//--------------------------------------------------------------------------------
void CThreadPoolSubSystem::AddThread(CThreadObject* pThread)
	{
	if(pThread == NULL)
		return;

	pThread->StartThread();
	m_threads.Add(pThread);
	}
