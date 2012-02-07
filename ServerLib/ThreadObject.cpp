//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#include "stdafx.h"
#include "ThreadObject.h"
#include "SubSystem.h"
#include "System.h"
 
#include <ReadLock.h>

#include <Process.h>

//--------------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(CThreadObject, CObject);

//--------------------------------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//--------------------------------------------------------------------------------
CThreadObject::CThreadObject(CSystemObject* pParent)
		: CSystemObject(pParent)
		, m_evtThreadBusy(FALSE, TRUE)
		, m_hThread(NULL)

	{
	memset(&m_msgCur, 0, sizeof(MSG));

	if(pParent != NULL)
		m_pState = pParent->m_pState;
	}

//--------------------------------------------------------------------------------
CThreadObject::~CThreadObject()
	{
	if(m_pParent != NULL && m_pState == m_pParent->m_pState)
		m_pState = NULL;
	}

//--------------------------------------------------------------------------------
bool CThreadObject::SetParent(CSubSystem* pParent)
	{
	if(m_pParent != NULL)
		return false;

	m_pParent = pParent;
	if(m_pState == NULL)
		m_pState = pParent->m_pState;
	return true;
	}

//--------------------------------------------------------------------------------
bool CThreadObject::StartThread()
	{
	if(m_hThread != NULL || m_hThread == INVALID_HANDLE_VALUE)
		return false;

	m_event.ResetEvent();

	// start our control loop
	m_hThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) StaticMain, this, 0, &m_nThreadId);

	if(m_hThread == INVALID_HANDLE_VALUE)
		return false;

	// wait for StaticMain to start
	return ::WaitForSingleObject(m_event, INFINITE) == WAIT_OBJECT_0;
	}

//--------------------------------------------------------------------------------
bool CThreadObject::EndThread()
	{
	if(this == NULL)
		return false;
	if(m_hThread == NULL || m_hThread == INVALID_HANDLE_VALUE)
		return false;
	::PostThreadMessage(m_nThreadId, WM_QUIT, 0, 0);
	::WaitForSingleObject(m_hThread, INFINITE);
	::CloseHandle(m_hThread);
	m_hThread = INVALID_HANDLE_VALUE;
	return true;
	}

//--------------------------------------------------------------------------------
UINT CThreadObject::StaticMain(CThreadObject* that)
	{
	int nHandCount = 0;
	HANDLE handsToWaitFor[2];
	// if we're part of a system then we add ourselves as a state client
	// so that the subsystem knows how many threads to wait for when
	// changing states (for state sync)
	CSystemObject* pParent = that->GetParent();
	const DWORD nStateWaitId = that->m_pState != NULL ? 0 : (DWORD) -1;
	if(that->m_pState != NULL)
		{
		if(pParent != NULL)
			pParent->AddStateClient();
		handsToWaitFor[nHandCount++] = that->m_pState->m_evtChanged;
		}

	HANDLE hTimer = NULL;
	// set up the timer that we'll use for the main loop interval
	DWORD nSleepTime = that->GetThreadLoopSleepInterval();
	DWORD nTimerWaitId;
	if(nSleepTime != (DWORD) -1)
		{
		hTimer = ::CreateWaitableTimer(NULL, FALSE, NULL);
		// negative values are relative time - this will make the timers first signal
		// happen (almost) immediatly
		__int64 nTimerDue = -1;
		::SetWaitableTimer(hTimer, (LARGE_INTEGER*) &nTimerDue, nSleepTime, NULL, NULL, FALSE);
		nTimerWaitId = nHandCount;
		handsToWaitFor[nHandCount++] = hTimer;
		}
	else
		nTimerWaitId = (DWORD) -1;

	DWORD nLastState = (DWORD) -1;

	bool bInitOk = that->Init();
	// signal that the thread is initialized
	that->m_event.SetEvent();

	if(bInitOk)
		TRY
			{
			for(;;)
				{
				DWORD nWait = ::MsgWaitForMultipleObjects(nHandCount, handsToWaitFor, FALSE, 20, QS_ALLEVENTS);

				// for debugging
				if(nWait == WAIT_FAILED || nWait == WAIT_ABANDONED || nWait == WAIT_TIMEOUT)
					continue;

				if(PeekMessage(&that->m_msgCur, NULL, 0, 0, PM_REMOVE))
					{
					UINT nMsg = that->m_msgCur.message;

					if(nMsg == WM_DESTROY)
						that->OnMessage(WM_QUIT, that->m_msgCur.wParam, that->m_msgCur.lParam);
					else	
						that->OnMessage(that->m_msgCur.message, that->m_msgCur.wParam, 
								that->m_msgCur.lParam);

					if(nMsg == WM_QUIT || nMsg == WM_DESTROY)
						break;
					}

				if(nTimerWaitId != (DWORD)-1 && nWait == nTimerWaitId)
					{
					if(! that->MainLoop())
						break;
					}
				else
					if(nStateWaitId != (DWORD)-1 && nWait == nStateWaitId)
						{
						that->OnStateChange(that->GetState());
						if(! that->CheckState())
							break;
						}

				// did the sleep interval change?
				if(nSleepTime != that->GetThreadLoopSleepInterval())
					{
					// if there was a timer, cancel it
					if(hTimer != NULL)
						{
						::CancelWaitableTimer(hTimer);
						::CloseHandle(hTimer);
						nHandCount--;
						}

					// get the new sleep interval
					nSleepTime = that->GetThreadLoopSleepInterval();
					if(nSleepTime != (DWORD) -1)
						{
						hTimer = ::CreateWaitableTimer(NULL, FALSE, NULL);
						// negative values are relative time - this will make the timers first signal
						// happen (almost) immediatly
						__int64 nTimerDue = -1;
						::SetWaitableTimer(hTimer, (LARGE_INTEGER*) &nTimerDue, nSleepTime, NULL, NULL, FALSE);
						nTimerWaitId = nHandCount;
						handsToWaitFor[nHandCount++] = hTimer;
						}
					else
						{
						hTimer = NULL;
						nTimerWaitId = (DWORD)-1;
						}
					}
				}
			}
		CATCH_ALL(e)
			{
			}
		END_CATCH_ALL;

	if(hTimer != NULL)
		{
		::CancelWaitableTimer(hTimer);
		::CloseHandle(hTimer);
		}

	that->Exit(bInitOk);

	that->AcknowledgeStateChange();

	if(pParent != NULL)
		pParent->RemoveStateClient();

	UINT nRv = that->GetResultCode();

	// basically this is 'delete this' which is kinda a no-no
	// but this is the one case i can think of where it's warrented
	// this class provides an easy to use thread encapsulation so
	// the thread should be able to delete itself, no?
	if(that->m_msgCur.message == WM_DESTROY)
		delete that;

	return nRv;
	}

//--------------------------------------------------------------------------------
bool CThreadObject::OnActivate()
	{
	return true;
	}

//--------------------------------------------------------------------------------
bool CThreadObject::OnDeactivate()
	{
	GetEvent().SetEvent();
	return true;
	}

//--------------------------------------------------------------------------------
void CThreadObject::OnStateChange(DWORD)
	{
	AcknowledgeStateChange();
	}

//--------------------------------------------------------------------------------
void CThreadObject::OnMessage(UINT, WPARAM, LPARAM)
	{
	return;
	}

//--------------------------------------------------------------------------------
DWORD CThreadObject::GetThreadLoopSleepInterval() const
	{
	// the default is to not have a MainLoop so we return 0
	// see StaticMain (waitable timer stuff) for details
	return 100;
	}

//--------------------------------------------------------------------------------
DWORD CThreadObject::GetPauseWaitSleepInterval() const
	{
	return 0;
	}

//--------------------------------------------------------------------------------
bool CThreadObject::MainLoop()
	{
	return CheckState();
	}

//--------------------------------------------------------------------------------
bool CThreadObject::Init()
	{
	return true;
	}

//--------------------------------------------------------------------------------
void CThreadObject::Exit(bool)
	{
	}

//--------------------------------------------------------------------------------
void CThreadObject::SetThreadBusy(bool bIsBusy)
	{
	CSystemObject* pParent = GetParent();
	bool bIsPool = pParent != NULL && pParent->IsKindOf(RUNTIME_CLASS(CThreadPoolSubSystem));

	if(bIsBusy)
		{
		if(bIsPool)
			((CThreadPoolSubSystem*) pParent)->IncBusyThreadCount();
		m_evtThreadBusy.SetEvent();
		OnActivate();
		}
	else
		{
		if(bIsPool)
			((CThreadPoolSubSystem*) pParent)->DecBusyThreadCount();
		m_evtThreadBusy.ResetEvent();
		OnDeactivate();
		}
	}

//--------------------------------------------------------------------------------
bool CThreadObject::CheckState()
	{
	switch(GetState())
		{
		default:
		case STATE_INIT:
		case STATE_RUN:
		case STATE_PAUSE:
			return true;
		case STATE_UNKNOWN:
		case STATE_ERROR:
		case STATE_EXIT:
		case STATE_STOP:
			return false;
		}
	}

//--------------------------------------------------------------------------------
CSystemObject* CThreadObject::GetParentFromClass(const CRuntimeClass* pClass)
	{
	if(m_pParent == NULL)
		return NULL;
	for(CSystemObject* pObj = m_pParent; pObj != NULL; )
		{
		CRuntimeClass* pCurClass = pObj->GetRuntimeClass();
		if(pCurClass == NULL)
			return NULL;
		if(pCurClass->IsDerivedFrom(pClass))
			return pObj;
		pObj = pObj->m_pParent;
		}

	return NULL;
	}
