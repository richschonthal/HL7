//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// Output.h : main header file for the OUTPUT application
//

#if !defined(AFX_OUTPUT_H__CFBF6365_4391_11D3_A6B7_005004A1C5F3__INCLUDED_)
#define AFX_OUTPUT_H__CFBF6365_4391_11D3_A6B7_005004A1C5F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// COutputApp:
// See Output.cpp for the implementation of this class
//
#include <registry.h>
#include <zProcess.h>

#include "ServerThread.h"
#include "ServerThreadInfo.h"
#include "protocolobj.h"
#include "ThreadListAccess.h"

//--------------------------------------------------------------------------------
class CRegKey;
class CHL7PacOutThread;
class CStaticHolder;
class CCmdLine;
class CMonitorList;
class CHL7System;
class CSecurityConnection;

//--------------------------------------------------------------------------------
class COutputApp : public CWinApp
	{
	friend class CStaticHolder;
	public:
		COutputApp();

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(COutputApp)
		public:
		virtual BOOL InitInstance();
		virtual int ExitInstance();
		//}}AFX_VIRTUAL

	// Implementation

		//{{AFX_MSG(COutputApp)
			// NOTE - the ClassWizard will add and remove member functions here.
			//    DO NOT EDIT what you see in these blocks of generated code !
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

	friend class CThreadListAccess;

	public:	// construction
		COutputApp(HINSTANCE);
		virtual ~COutputApp();

	public:
		CHL7System* GetSystem();

	private:
		void DoInit(WPARAM, LPARAM);
		// NT Service msg handlers
		void DoPause(WPARAM, LPARAM);
		void DoContinue(WPARAM, LPARAM);
		void DoStop(WPARAM, LPARAM);
		void DoShutdown(WPARAM, LPARAM);

	public:
		CSecurityConnection* m_pSecurity;

	private:
		// state switches
		bool m_bExit;
		bool m_bPaused;

		CHL7System* m_pSystem;

		CMonitorList* m_pMonitors;

		// the main loop thread
		CWinThread* m_pMainLoopThread;

		// the process monitor thread
		CWinThread* m_pProcMonThread;

		// the security connection handler
		CWinThread* m_pSecurityThread;

		// a container class for all the statics we need to declare
		// but don't really need to access
		// they need to be allocated so as to keep their ref counts > 0
		CStaticHolder* m_pStatics;

		// the name displayed for the service
		LPTSTR m_pName;

		// the process id returned by the process monitor
		long m_nProcessId;

		// the list of all the threads we have created
		CServerThreadInfoArray m_threads;

		CServerThreadInfoArray* LockThreadListPtr();
		void UnlockThreadListPtr();

		// events used to control the service
		CEvent	m_evtStop;
		CEvent	m_evtPause;

		// registry access routines
		bool CreateRegKey(CRegKey&);
		bool InitRegistry();
		bool InitRegValue(CRegKey&, LPCTSTR, LPCTSTR);
		bool InitRegValue(CRegKey&, LPCTSTR, DWORD);

		bool DoCommandLine();

		// process monitor management
		static UINT ManageProcess(COutputApp*);
		static bool GetProcessActionToken(int, CProtocolObj&);
		long RegisterProcess(LPCTSTR = NULL);
		long UnregisterProcess(long);

		// launch monitor processes
		void StartMonitors();
		static UINT StartMonitorsThread(COutputApp*);

		// the main looping thread
		static UINT MainLoopThread(COutputApp*);

		// security connection thread
		static UINT SecurityThread(COutputApp*);

		// send status into to control service
		void ReportStatus(DWORD, DWORD = 5000);

		LPCTSTR GetProcessName();
	};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OUTPUT_H__CFBF6365_4391_11D3_A6B7_005004A1C5F3__INCLUDED_)
