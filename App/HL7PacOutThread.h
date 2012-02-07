//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#if !defined(AFX_HL7PACOUTTHREAD_H__85EA4789_1DA7_11D3_87B9_00104B9E6286__INCLUDED_)
#define AFX_HL7PACOUTTHREAD_H__85EA4789_1DA7_11D3_87B9_00104B9E6286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
#include "ServerThread.h"

//--------------------------------------------------------------------------------
class CPACSDatabase;

//--------------------------------------------------------------------------------
class CHL7PacOutThread : public CServerThread
	{
	public:
		enum
			{
			STATUS_MSG_AVAIL = 6,
			STATUS_MSG_PROCESSING = 7,
			STATUS_MSG_OK = 8,
			STATUS_MSG_ERROR = -1,
			STATUS_MSG_ERROR_RESEND = STATUS_MSG_AVAIL
			};

	DECLARE_DYNCREATE(CHL7PacOutThread)
	protected:
		CHL7PacOutThread();           // protected constructor used by dynamic creation

	// Attributes
	public:

	// Operations
	public:

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CHL7PacOutThread)
		public:
		virtual BOOL InitInstance();
		virtual int ExitInstance();
		//}}AFX_VIRTUAL

	// Implementation
	protected:
		virtual ~CHL7PacOutThread();

		// Generated message map functions
		//{{AFX_MSG(CHL7PacOutThread)
			// NOTE - the ClassWizard will add and remove member functions here.
		//}}AFX_MSG

		DECLARE_MESSAGE_MAP()

	private:
#if defined(DB_PRIVATE)
		CRecordset* m_pRS;
#elif defined(DB_SHARE)
		CList<__int64, __int64>* m_pList;
#endif
		int m_nVersion;
		// to keep record of how many times a message has been sent
		long m_nLastMsgNo;
		int m_nRetries;

	private:
		virtual void Loop(WPARAM, LPARAM);
		void DoGoodbye(WPARAM, LPARAM);

		void DoInit(CServerThreadInfo*, CHL7InputDaemonInit*);
		int DoPACMessage(CPACSDatabase*, long nMsgNo, long nMsgId);
		bool BuildString(CPACSDatabase*, long nMsgNo, long nMsgId, CString&);
	};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HL7PACOUTTHREAD_H__85EA4789_1DA7_11D3_87B9_00104B9E6286__INCLUDED_)
