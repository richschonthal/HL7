//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "HL7InputDaemon.h"
#include "HL7InputDaemonThread.h"
#include "HL7FileDaemonThread.h"
#include "HL7SocketDaemonThread.h"

#include "globals.h"
#include "waitlen.h"

//--------------------------------------------------------------------------------
CHL7InputDaemon::CHL7InputDaemon(CWinThread* pMonitor)
	{
	m_pMonitor = pMonitor;
	}

//--------------------------------------------------------------------------------
CHL7InputDaemon::~CHL7InputDaemon()
	{
	if(m_pThread != NULL)
		{
		m_pThread->PostThreadMessage(CHL7InputDaemonThread::ITWM_EXIT, 0, 0);
		::WaitForSingleObject(m_pThread->m_hThread, INFINITE);
		delete m_pThread;
		}
	}

//--------------------------------------------------------------------------------
bool CHL7InputDaemon::Init(const CHL7InputDaemonInit* pInit)
	{
	CRuntimeClass* pClass = NULL;

	switch(pInit->GetType())
		{
		case CHL7InputDaemonInit::INPUT_FILE:
			pClass = RUNTIME_CLASS(CHL7FileDaemonThread);
			break;
		case CHL7InputDaemonInit::INPUT_SOCKET:
			pClass = RUNTIME_CLASS(CHL7SocketDaemonThread);
			break;
//		case CHL7InputDaemonInit::INPUT_SERIAL:
//			pClass = RUNTIME_CLASS(CHL7SerialDaemonThread);
//			break;
		default:
			//TRACE("input type config error\n");
			return false;
		}

	m_pThread = (CHL7InputDaemonThread*) ::AfxBeginThread(pClass);

	// wait for the thread to init
	if(::WaitForSingleObject(m_pThread->m_evtInit, WAITLEN_INPUT_THREAD_INIT) != WAIT_OBJECT_0)
		{
		//TRACE("the input thread died on startup\n");
		}

	// wait for it to start running before sending any messages
	BOOL bStarted = false;

	for(int i = 0;i < 10;i++)
		{
		if(bStarted = m_pThread->PostThreadMessage(CHL7InputDaemonThread::ITWM_INIT, (WPARAM) pInit, (LPARAM) this))
			break;
		::Sleep(20);
		}

	if(! bStarted)
		{
		//TRACE("the input thread died on startup\n");
		}

	// we sent an INIT message so wait for init to complete
	if(::WaitForSingleObject(m_pThread->m_evtInit, WAITLEN_INPUT_THREAD_INIT) != WAIT_OBJECT_0)
		{
		//TRACE("the input thread died on init\n");
		}

	// crank that baby up
	m_pThread->PostThreadMessage(CHL7InputDaemonThread::ITWM_START, 0, 0);

	return true;
	}

//--------------------------------------------------------------------------------
bool CHL7InputDaemon::SendString(LPCTSTR pOrig)
	{
	LPTSTR pStr = new char[strlen(pOrig) + 1];
	strcpy(pStr, pOrig);
	return m_pThread->PostThreadMessage(CHL7InputDaemonThread::ITWM_SEND_STRING, (WPARAM) pStr, GetCurrentThreadId()) != 0;
	}

//--------------------------------------------------------------------------------
bool CHL7InputDaemon::Pause()
	{
	m_pThread->SuspendThread();
	GetIO()->Output(IOMASK_14|IOMASK_CONST, "CHL7InputDaemon Paused");
	return true;
	}

//--------------------------------------------------------------------------------
void CHL7InputDaemon::Continue()
	{
	m_pThread->ResumeThread();
	GetIO()->Output(IOMASK_14|IOMASK_CONST, "CHL7InputDaemon Continuing");
	}

//--------------------------------------------------------------------------------
void CHL7InputDaemon::SignalMsgComplete()
	{
	if(m_pThread == NULL)
		return;
	m_pThread->m_evtMsgComplete.SetEvent();
	}
