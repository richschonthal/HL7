//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "HL7InputDaemonThread.h"
#include "HL7InputDaemon.h"

#include "HL7NTService.h"

#include "globals.h"

#include <debugprintf.h>

#include "defs.h"
#include "waitlen.h"

#include "Monitor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//--------------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(CHL7InputDaemonThread, CWinThread)

//--------------------------------------------------------------------------------
CHL7InputDaemonThread::CHL7InputDaemonThread()
		: m_evtStop(false, true)
		, m_evtPause(false, true)
	{
	m_pParentObj = NULL;
	}

//--------------------------------------------------------------------------------
CHL7InputDaemonThread::~CHL7InputDaemonThread()
	{
	}

//--------------------------------------------------------------------------------
BOOL CHL7InputDaemonThread::InitInstance()
	{
	m_evtInit.SetEvent();
	return TRUE;
	}

//--------------------------------------------------------------------------------
int CHL7InputDaemonThread::ExitInstance()
	{
	return CWinThread::ExitInstance();
	}

//--------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CHL7InputDaemonThread, CWinThread)
	//{{AFX_MSG_MAP(CHL7InputDaemonThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP

	ON_THREAD_MESSAGE(ITWM_INIT, Init)
	ON_THREAD_MESSAGE(ITWM_START, MainLoop)
	ON_THREAD_MESSAGE(ITWM_EXIT, End)
	ON_THREAD_MESSAGE(ITWM_SEND_STRING, DoSendString)

	ON_THREAD_MESSAGE(CNTService::MSG_PAUSE, DoPause)
	ON_THREAD_MESSAGE(CNTService::MSG_CONTINUE, DoContinue)
	ON_THREAD_MESSAGE(CNTService::MSG_STOP, DoStop)
	ON_THREAD_MESSAGE(CNTService::MSG_SHUTDOWN, DoShutdown)

END_MESSAGE_MAP()

//--------------------------------------------------------------------------------
void CHL7InputDaemonThread::Init(const CHL7InputDaemonInit* pInit, CHL7InputDaemon* pParentObj)
	{
	m_pInit = pInit;
	m_pParentObj = pParentObj;
	m_evtInit.SetEvent();
	GetIO()->AddThreadName(m_pInit->GetName());
	}

//--------------------------------------------------------------------------------
void CHL7InputDaemonThread::DoPause(WPARAM, LPARAM)
	{
	m_evtPause.SetEvent();
	}

//--------------------------------------------------------------------------------
void CHL7InputDaemonThread::DoContinue(WPARAM, LPARAM)
	{
	m_evtPause.ResetEvent();
	}

//--------------------------------------------------------------------------------
void CHL7InputDaemonThread::DoStop(WPARAM, LPARAM)
	{
	m_evtStop.SetEvent();
	}

//--------------------------------------------------------------------------------
void CHL7InputDaemonThread::DoShutdown(WPARAM, LPARAM)
	{
	m_evtStop.SetEvent();
	}

//--------------------------------------------------------------------------------
bool CHL7InputDaemonThread::EventStopSignaled(DWORD nWaitTime)
	{
	return ::WaitForSingleObject(m_evtStop, nWaitTime) == WAIT_OBJECT_0;
	}

//--------------------------------------------------------------------------------
bool CHL7InputDaemonThread::EventPauseSignaled(DWORD nWaitTime)
	{
	return ::WaitForSingleObject(m_evtPause, nWaitTime) == WAIT_OBJECT_0;
	}

//--------------------------------------------------------------------------------
bool CHL7InputDaemonThread::EventContinueSignaled(DWORD nWaitTime)
	{
	return ::WaitForSingleObject(m_evtPause, nWaitTime) != WAIT_OBJECT_0;
	}

//--------------------------------------------------------------------------------
void CHL7InputDaemonThread::MainLoop(WPARAM, LPARAM)
	{
	if(EventStopSignaled(200))
		return;

	// call this at the end of the derived main loop to
	// keep the loop going
	PostThreadMessage(ITWM_START, 0, 0);
	}

//--------------------------------------------------------------------------------
void CHL7InputDaemonThread::End(WPARAM, LPARAM)
	{
	ExitThread(0);
	}

//--------------------------------------------------------------------------------
bool CHL7InputDaemonThread::SendString(LPCTSTR)
	{
	return false;
	}

//--------------------------------------------------------------------------------
void CHL7InputDaemonThread::ProcessMessage(CHL7Message* pMsg)
	{
	if(pMsg == NULL)
		return;

	m_pParentObj->m_pMonitor->PostThreadMessage(HL7_HL7MSG_IN, (WPARAM) pMsg, 0);
	}

//--------------------------------------------------------------------------------
void CHL7InputDaemonThread::DoSendString(WPARAM pString, LPARAM nThreadId)
	{
	DWORD nRv = (DWORD) SendString((LPCTSTR) pString);
	}
