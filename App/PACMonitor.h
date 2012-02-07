//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _PACMONITOR_H_
#define _PACMONITOR_H_

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

//--------------------------------------------------------------------------------
#include <result.h>
#include <registry.h>
#include <CmdLine.h>

#include "HL7SystemMsg.h"

//--------------------------------------------------------------------------------
class CHL7InputDaemon;
class CHL7InputDaemonInit;
class CHL7Message;
class CServerThreadInfo;

//--------------------------------------------------------------------------------
class CPACMonitorThread : public CWinThread
	{
	friend class CServerThread;
	friend class COutputApp;

	DECLARE_DYNCREATE(CPACMonitorThread)

	public:
		CPACMonitorThread();
		~CPACMonitorThread();

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CPACMonitorThread)
	public:
		virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	// Implementation

		//{{AFX_MSG(CPACMonitorThread)
			// NOTE - the ClassWizard will add and remove member functions here.
			//    DO NOT EDIT what you see in these blocks of generated code !
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

	private:
		virtual void DoInit(CHL7InputDaemonInit*, CServerThreadInfo*);
		static UINT MainLoopThread(CPACMonitorThread*);

	public:
		DWORD m_nProcessId;

	private:
		CRegKey* m_pParamKey;
		CHL7InputDaemonInit* m_pInputInit;
		CServerThreadInfo* m_pServerThreadInfo;
		CHL7InputDaemon* m_pInput;

	private:
		bool ConnectToServer();

		void DoMsgFromServer(LPTSTR pMsg, LPARAM);

		CServerThread* GetServerThread();
	};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // _PACMONITOR_H_

