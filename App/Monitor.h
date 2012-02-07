//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#if !defined(AFX_MONITOR_H__11FEDA27_44F7_11D3_A6B7_005004A1C5F3__INCLUDED_)
#define AFX_MONITOR_H__11FEDA27_44F7_11D3_A6B7_005004A1C5F3__INCLUDED_

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
class CProtocolObj;

//--------------------------------------------------------------------------------
class CMonitorThread : public CWinThread
	{
	friend class CServerThread;
	friend class COutputApp;

	DECLARE_DYNCREATE(CMonitorThread)

	public:
		CMonitorThread();
		~CMonitorThread();

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CMonitorThread)
	public:
		virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	// Implementation

		//{{AFX_MSG(CMonitorThread)
			// NOTE - the ClassWizard will add and remove member functions here.
			//    DO NOT EDIT what you see in these blocks of generated code !
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

	private:
		virtual void DoInit(CHL7InputDaemonInit*, CServerThreadInfo*);
		static UINT MainLoopThread(CMonitorThread*);

	public:
		DWORD m_nProcessId;

	private:
		CRegKey* m_pParamKey;
		CHL7InputDaemonInit* m_pInputInit;
		CServerThreadInfo* m_pServerThreadInfo;
		CHL7InputDaemon* m_pInput;

	private:
		bool ConnectToServer();

		void DoMsgFromInput(CHL7Message*, LPARAM);
		void DoMsgFromServer(CProtocolObj*, LPARAM);

		CServerThread* GetServerThread();
	};

//--------------------------------------------------------------------------------
class CMonitorList : public CTypedPtrList<CObList, CWinThread*>
	{
	};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MONITOR_H__11FEDA27_44F7_11D3_A6B7_005004A1C5F3__INCLUDED_)
