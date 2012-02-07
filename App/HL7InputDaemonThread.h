//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#if !defined(AFX_HL7INPUTDAEMONTHREAD_H__93330B21_F42C_11D2_87A7_00104B9E6286__INCLUDED_)
#define AFX_HL7INPUTDAEMONTHREAD_H__93330B21_F42C_11D2_87A7_00104B9E6286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HL7InputDaemonThread.h : header file
//

#include "HL7InputDaemonInit.h"
#include "HL7Message.h"

//--------------------------------------------------------------------------------
#include <result.h>

//--------------------------------------------------------------------------------
class CHL7InputDaemon;

/////////////////////////////////////////////////////////////////////////////
// CHL7InputDaemonThread thread


//--------------------------------------------------------------------------------
class CHL7InputDaemonThread : public CWinThread
	{
	DECLARE_DYNCREATE(CHL7InputDaemonThread)

	public:
		enum eInputThreadWM
			{
			ITWM_INIT = WM_USER + 1,
			ITWM_START,
			ITWM_EXIT,
			ITWM_SEND_STRING
			};

	protected:
		CHL7InputDaemonThread();           // protected constructor used by dynamic creation

	// Attributes
	public:

	// Operations
	public:

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CHL7InputDaemonThread)
	public:
		virtual BOOL InitInstance();
		virtual int ExitInstance();
	//}}AFX_VIRTUAL

	// Implementation
	protected:
		virtual ~CHL7InputDaemonThread();

		// Generated message map functions
		//{{AFX_MSG(CHL7InputDaemonThread)
			// NOTE - the ClassWizard will add and remove member functions here.
		//}}AFX_MSG

		DECLARE_MESSAGE_MAP()

	protected:
		CHL7InputDaemon* m_pParentObj;
		const CHL7InputDaemonInit* m_pInit;
		CEvent	m_evtStop;
		CEvent	m_evtPause;

	public:
		friend class CHL7InputDaemon;
		virtual void Init(const CHL7InputDaemonInit*, CHL7InputDaemon*);
		virtual void MainLoop(WPARAM, LPARAM);
		virtual void End(WPARAM, LPARAM);

		bool EventStopSignaled(DWORD = 20);
		bool EventPauseSignaled(DWORD = 20);
		bool EventContinueSignaled(DWORD = 20);

		virtual void ProcessMessage(CHL7Message*);

		// events
		CEvent m_evtMsgComplete;		// signaled when msg processing is complete
		CEvent m_evtInit;				// signaled when the thread has finished init

		void DoPause(WPARAM, LPARAM);
		void DoContinue(WPARAM, LPARAM);
		void DoStop(WPARAM, LPARAM);
		void DoShutdown(WPARAM, LPARAM);

		virtual void DoSendString(WPARAM pStringToSend, LPARAM nThreadIdOfSender);

	protected:
		virtual bool SendString(LPCTSTR);
	};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HL7INPUTDAEMONTHREAD_H__93330B21_F42C_11D2_87A7_00104B9E6286__INCLUDED_)
