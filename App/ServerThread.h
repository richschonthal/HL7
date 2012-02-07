//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#if !defined(AFX_SERVERTHREAD_H__46F08FD1_F811_11D2_87A7_00104B9E6286__INCLUDED_)
#define AFX_SERVERTHREAD_H__46F08FD1_F811_11D2_87A7_00104B9E6286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ServerThread.h : header file
//

#include "PACSDatabase.h"

//--------------------------------------------------------------------------------
class CHL7Message;
class CMonitorThread;
class CProtocolObj;
class CHL7InputDaemonInit;

// for each CServerThread to have it's own DB connection define DB_PRIVATE
// to have all CServerThread's share one DB connection define DB_SHARE
// either way, they must be defined for the entire project
#ifdef DB_SHARE
#include "SharedDB.h"
#endif

//--------------------------------------------------------------------------------
class CServerThread : public CWinThread
	{
	friend class CServerThreadInfo;

	DECLARE_DYNCREATE(CServerThread)

	protected:
		CServerThread();           // protected constructor used by dynamic creation

	// Attributes
	public:

	// Operations
	public:

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CServerThread)
	public:
		virtual BOOL InitInstance();
		virtual int ExitInstance();
	//}}AFX_VIRTUAL

	// Implementation
	protected:
		virtual ~CServerThread();

		// Generated message map functions
		//{{AFX_MSG(CServerThread)
			// NOTE - the ClassWizard will add and remove member functions here.
		//}}AFX_MSG

		DECLARE_MESSAGE_MAP()

	public:
		CEvent m_evtInit;

		// holder to keep ref count > 1
		CString m_sLastMsgNumber;
		int m_nRetries;
		CServerThreadInfo* m_pInfo;
		CHL7InputDaemonInit* m_pInitInfo;

	protected:
		virtual void DoPause();
		virtual void DoContinue();
		virtual void DoInit(CServerThreadInfo*, CHL7InputDaemonInit*);
		void DoHL7Message(CHL7Message*, LPARAM);
		void DoGoodbye(WPARAM, LPARAM);

		void BuildResponse(CProtocolObj*, const CHL7Message*);
		CWinThread* GetMonitorPtr();
	};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERTHREAD_H__46F08FD1_F811_11D2_87A7_00104B9E6286__INCLUDED_)
