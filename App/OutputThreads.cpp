//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// if the function runs as a thread and its part of COutputApp, you'll find the code here

#include "stdafx.h"
#include "Output.h"
#include "globals.h"

#include "HL7SystemMsg.h"

#include "PACSDatabase.h"
#include "HL7DBServerConfig.h"
#include "HL7DBPM_GetProcessAction.h"
#include "HL7DBPM_UnregisterProcess.h"
#include "HL7InputDaemonInit.h"
#include "OutputConfig.h"
#include "Monitor.h"
#include "PACMonitor.h"
#include "HL7PacOutThread.h"

#ifdef _EVALUATION_VERSION
#include <conio.h>
#endif

//--------------------------------------------------------------------------------
UINT COutputApp::StartMonitorsThread(COutputApp* that)
	{
	GetIO()->Output(IOMASK_7|IOMASK_CONST, "StartMonitorThread entry");
	that->m_pMonitors = new CMonitorList;

	CHL7InputDaemonInitList monitors;
	COutputConfig config;

	// create the init event so it exists when the monitors start up
	CEvent evtMonitorInit(false, false, "HL7MonitorInit");

	for(int i = 0; i < monitors.GetSize(); i++)
		{
		CThreadListAccess access(that);
		if(access.m_pThreads == NULL)
			{
			GetIO()->Output(IOMASK_ERR|IOMASK_CONST, "Internal Error (CThreadListAccess))");
			return false;
			}

		CString sKeyName;
		sKeyName.Format("%s\\%d", config.GetMonKeyName(), i);
		CRegKey rkMon(g_rkRoot, sKeyName);

		CHL7InputDaemonInit* pInputInit = new CHL7InputDaemonInit(rkMon);
		// create a new server info record
		CServerThreadInfo* pInfo = new CServerThreadInfo;

		// start the monitor
		CRuntimeClass* pClass = pInputInit->IsOutModule() ? RUNTIME_CLASS(CPACMonitorThread) : RUNTIME_CLASS(CMonitorThread);
		CWinThread* pThread = ::AfxBeginThread(pClass);
		that->m_pMonitors->AddTail(pThread);
		pInfo->SetMonitorPtr(pThread);

		// start the server thread
		pClass = pInputInit->IsOutModule() ? RUNTIME_CLASS(CHL7PacOutThread) : RUNTIME_CLASS(CServerThread);
		pInfo->SetThreadPtr((CServerThread*) ::AfxBeginThread(pClass));
		// add the server info to the thread list
		(*access.m_pThreads).Add(pInfo);
		// init the server thread
		pInfo->GetThreadPtr()->PostThreadMessage(INIT, (WPARAM) pInfo, (LPARAM) pInputInit);
		// init the monitor thread
		::PostThreadMessage(pThread->m_nThreadID, INIT, (WPARAM) pInputInit, (LPARAM) pInfo);

		// wait for it to init
		::WaitForSingleObject(evtMonitorInit, 20000);
		GetIO()->FormatOutput(IOMASK_1, "new connection complete (%s)", pInputInit->GetName());
		pInputInit->SetProcessId(that->RegisterProcess(pInputInit->GetName()));
		evtMonitorInit.ResetEvent();
		}

	GetIO()->Output(IOMASK_8|IOMASK_CONST, "StartMonitorThread exits");
	return 0;
	}

#if defined(DB_PRIVATE)
// this version does nothing except check for action so as to update the process monitor table
// communicates with and monitors the process monitor database
// via stored procedures
//--------------------------------------------------------------------------------
UINT COutputApp::ManageProcess(COutputApp* that)
	{
	CPACSDatabase* pDB = NULL;
	CHL7DBServerConfig config;

	for(;;)
		{
		if(that->m_bExit)
			break;

		::Sleep(config.GetProcessMonitorInterval());

		CThreadListAccess access(that);
		if(access.m_pThreads == NULL)
			continue;

		TRY
			{
			if(pDB == NULL)
				pDB = new CPACSDatabase;

			// get the action for the output daemon
			CHL7DBPM_GetProcessAction action(pDB->GetDBPtr(), that->m_nProcessId);

			// no action for us? then check for action for each monitor
			for(int i = 0; i <= (*access.m_pThreads).GetUpperBound(); i++)
				CHL7DBPM_GetProcessAction action(pDB->GetDBPtr(), (*access.m_pThreads)[i]->m_pThread->m_pInitInfo->GetProcessId());
			}
		CATCH_ALL(e)
			{
			GetIO()->Output(IOMASK_13|IOMASK_CONST, "Database error - retrying (ManageProcess)");
			delete pDB;
			pDB = NULL;
			}
		END_CATCH_ALL
		}

	delete pDB;
	return 0;
	}
//--------------------------------------------------------------------------------
#elif defined(DB_SHARE)
// this version does nothing except check for action so as to update the process monitor table
// communicates with and monitors the process monitor database
// via stored procedures
//--------------------------------------------------------------------------------
UINT COutputApp::ManageProcess(COutputApp* that)
	{
	CHL7DBServerConfig config;

	for(;;)
		{
		if(that->m_bExit)
			break;

		::Sleep(config.GetProcessMonitorInterval());

		CThreadListAccess access(that);
		if(access.m_pThreads == NULL)
			continue;

		TRY
			{
			CSharedDB db;
			CPACSDatabase* pDB = db.GetDB();
			if(pDB == NULL)
				throw new CException;

			// get the action for the output daemon
			CHL7DBPM_GetProcessAction action(pDB->GetDBPtr(), that->m_nProcessId);

			// no action for us? then check for action for each monitor
			for(int i = 0; i <= (*access.m_pThreads).GetUpperBound(); i++)
				CHL7DBPM_GetProcessAction action(pDB->GetDBPtr(), (*access.m_pThreads)[i]->m_pThread->m_pInitInfo->GetProcessId());
			}
		CATCH_ALL(e)
			{
			CSharedDB db(false);
			db.DeleteConnection();
			GetIO()->Output(IOMASK_13|IOMASK_CONST, "Database error - retrying (ManageProcess)");
			}
		END_CATCH_ALL
		}

	return 0;
	}
#endif// DB_SHARE

#ifdef BULLWINKLE
//--------------------------------------------------------------------------------
UINT COutputApp::ManageProcess(COutputApp* that)
	{
	CPACSDatabase* pDB = NULL;
	CHL7DBServerConfig config;

	for(;;)
		{
		if(that->m_bExit)
			break;

		::Sleep(config.GetProcessMonitorInterval());

		CThreadListAccess access(that);
		if(access.m_pThreads == NULL)
			continue;

		TRY
			{
			if(pDB == NULL)
				pDB = new CPACSDatabase;

			// get the action for the output daemon
			CHL7DBPM_GetProcessAction action(pDB->GetDBPtr(), that->m_nProcessId);

			CProtocolObj obj;
			if(GetProcessActionToken(action.m_nAction, obj))
				{
				switch(obj.GetCommand())
					{
					case CProtocolObj::GOODBYE:
						(*access.m_pThreads).AddMsgToAll(obj);
						that->m_evtStop.SetEvent();
						break;

					case CProtocolObj::PAUSE:
						(*access.m_pThreads).AddMsgToAll(obj);
						that->m_evtPause.SetEvent();
						break;

					case CProtocolObj::CONTINUE:
						(*access.m_pThreads).AddMsgToAll(obj);
						that->m_evtPause.ResetEvent();
						break;

					default:
						break;
					}
				}
			else
				// no action for us? then check for action for each monitor
				for(int i = 0; i <= (*access.m_pThreads).GetUpperBound(); i++)
					{
					CHL7DBPM_GetProcessAction action(pDB->GetDBPtr(), (*access.m_pThreads)[i]->GetProcessId());

					// no action for this monitor?
					if(! GetProcessActionToken(action.m_nAction, obj))
						continue;

					if((*access.m_pThreads)[i]->GetMsgQueue().Lock())
						{
						// add the msg to the queue for this monitor
						CProtocolObj* pObj = new CProtocolObj(obj);
						(*access.m_pThreads)[i]->GetMsgQueue().AddTail(pObj);
						(*access.m_pThreads)[i]->GetMsgQueue().Unlock();
						}
					}
			}
		CATCH_ALL(e)
			{
			GetIO()->Output(IOMASK_13|IOMASK_CONST, "Database error - retrying (ManageProcess)");
			delete pDB;
			pDB = NULL;
			}
		END_CATCH_ALL
		}

	delete pDB;
	return 0;
	}
#endif

#ifndef _EVALUATION_VERSION
// this is the regular version main loop
//--------------------------------------------------------------------------------
UINT COutputApp::MainLoopThread(COutputApp* that)
	{
	GetIO()->Output(IOMASK_7|IOMASK_CONST, "MainThreadLoop entry");
	GetIO()->Output(IOMASK_1|IOMASK_CONST, "HL7 Started");

	CTime ctStart(CTime::GetCurrentTime());
	CTime ctLast(ctStart);

	for(;;)
		{
		// is stop signaled?
		if(::WaitForSingleObject(that->m_evtStop, 50) == WAIT_OBJECT_0)
			{
			OutputDebugString("output: stop\n");
			break;
			}

		// if pause signaled?
		if(::WaitForSingleObject(that->m_evtPause, 50) == WAIT_OBJECT_0)
			{
			that->m_bPaused = true;

			// wait for continue
			while(::WaitForSingleObject(that->m_evtPause, 50) == WAIT_OBJECT_0)
				// stop signaled while we're paused?
				if(::WaitForSingleObject(that->m_evtStop, 20) == WAIT_OBJECT_0)
					break;

			that->m_bPaused = false;
			}

		// if the day has changed since we started then start a new log file
		CTime ctNow(CTime::GetCurrentTime());
		if(ctNow.GetMinute() != ctLast.GetMinute())
			{
			ctLast = ctNow;
			COutputConfig config;
			if(config.IsDailyLog() && ctStart.GetDay() != ctNow.GetDay())
				{
				ctStart = ctNow;
				GetIO()->CreateLogFile(config.GenerateLogfileName());
				}
			}
		}

	GetIO()->Output(IOMASK_8|IOMASK_CONST, "MainThreadLoop exits");

#ifdef _DEBUG
	that->m_evtStop.SetEvent();
#endif

	// send a quit msg to the main thread
	that->PostThreadMessage(WM_QUIT, 0, 0);

	return 0;
	}

//--------------------------------------------------------------------------------
#else // it's the _EVALUATION_VERSION
//--------------------------------------------------------------------------------
UINT COutputApp::MainLoopThread(COutputApp* that)
	{
	AllocConsole();
	SetConsoleTitle("HL7 Engine Evaluation");
	__int64 nStartTick = ::GetTickCount();
	int nLastReportedHours = -1;

	for(;;)
		{
		__int64 nTick = ::GetTickCount();
		if(nTick < nStartTick)
			nTick += 0xffffffff;

		int nMinutes = (nTick - nStartTick) / 60000;

		if(nMinutes > 240)
			break;

		if(nMinutes > 180)
			_cprintf("This application will exit in %d minutes\n", 240 - nMinutes);
		else
			if((nMinutes / 60) != nLastReportedHours)
				{
				nLastReportedHours = (nMinutes / 60);
				_cprintf("This application will exit in %d hour%c\n", 4 - nLastReportedHours, nLastReportedHours==3?' ':'s');
				}

		::Sleep(60000);
		}

#ifdef _DEBUG
	that->m_evtStop.SetEvent();
#endif

	// send a quit msg to the main thread
	that->PostThreadMessage(WM_QUIT, 0, 0);
	return 0;
	}

#endif //_EVALUATION_VERSION

//--------------------------------------------------------------------------------
UINT COutputApp::SecurityThread(COutputApp* that)
	{
	GetIO()->Output(IOMASK_7|IOMASK_CONST, "SecurityThread Entry");

#ifndef _NO_SECURITY_SERVER
	for(;;)
		{
		MSG msg;
		// any messages from the security connection?
		if(::PeekMessage(&msg, NULL, HL7_SECURITY_SERVER, HL7_SECURITY_SERVER, TRUE))
			{
			switch(msg.message)
				{
				case WM_QUIT:
					goto exit;

				case HL7_SECURITY_SERVER:
					// is it an error or revocation of the certificate?
					if(msg.lParam != CSecurityConnection::MsgNew && msg.lParam != CSecurityConnection::MsgRefreshed)
						{
						// cancel messages
						that->m_pSecurity->SetTargetNone();
						// wait 30 seconds for a new certificate to be obtained
						if(::WaitForSingleObject(that->m_pSecurity->m_evtObtained, 30000) == WAIT_OBJECT_0)
							// we've got it so restart messages and relax
							that->m_pSecurity->SetTarget(that->m_pSecurityThread, HL7_SECURITY_SERVER);
						else
							{
							// we lost our certificate and did not obtain a new one within 30 seconds
							GetIO()->Output(IOMASK_ERR|IOMASK_CONST, "Certificate lost - must exit");
							goto exit;
							}
						}
					break;
				}
			}
		::Sleep(20);
		}

exit:;
	delete that->m_pSecurity;
	that->m_pSecurity = NULL;
	that->m_evtStop.SetEvent();

#endif

	GetIO()->Output(IOMASK_8|IOMASK_CONST, "SecurityThread exits");

	return 0;
	}
