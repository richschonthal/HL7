//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#if !defined(AFX_HL7FILEDAEMONTHREAD_H__93330B28_F42C_11D2_87A7_00104B9E6286__INCLUDED_)
#define AFX_HL7FILEDAEMONTHREAD_H__93330B28_F42C_11D2_87A7_00104B9E6286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HL7FileDaemonThread.h : header file
//

#include "HL7InputDaemonThread.h"
#include "HL7Message.h"

/////////////////////////////////////////////////////////////////////////////
// CHL7FileDaemonThread thread

//--------------------------------------------------------------------------------
class CHL7FileDaemonThread : public CHL7InputDaemonThread
	{
		DECLARE_DYNCREATE(CHL7FileDaemonThread)
	protected:
		CHL7FileDaemonThread();           // protected constructor used by dynamic creation

	// Attributes
	public:

	// Operations
	public:

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CHL7FileDaemonThread)
	//}}AFX_VIRTUAL

	// Implementation
	protected:
		virtual ~CHL7FileDaemonThread();

		// Generated message map functions
		//{{AFX_MSG(CHL7FileDaemonThread)
			// NOTE - the ClassWizard will add and remove member functions here.
		//}}AFX_MSG

		DECLARE_MESSAGE_MAP()

	private:
		CStdioFile* m_pCurFile;
		CStdioFile* m_pAckFile;

	public:
		virtual void MainLoop(WPARAM, LPARAM);

		virtual bool SendString(LPCTSTR);
	};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HL7FILEDAEMONTHREAD_H__93330B28_F42C_11D2_87A7_00104B9E6286__INCLUDED_)
