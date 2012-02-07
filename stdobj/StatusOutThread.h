//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#if !defined(AFX_STATUSOUTTHREAD_H__DC1B71F1_2A3F_11D3_87BC_00104B9E6286__INCLUDED_)
#define AFX_STATUSOUTTHREAD_H__DC1B71F1_2A3F_11D3_87BC_00104B9E6286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatusOutThread.h : header file
//

#include <status.h>

//--------------------------------------------------------------------------------
class CStatusOutThreadMsgParam
	{
	public:
		CStatusOutThreadMsgParam(CStatus::eLevel nLevel, LPCTSTR pMsg, CResult* pRes, long nThreadId,
			LPCTSTR pObjName, LPCTSTR pFuncName, CStatus* pObj);

		CStatusOutThreadMsgParam(const CStatusOutThreadMsgParam&);

		~CStatusOutThreadMsgParam();

		long m_nSize;	// must be set to sizeof(CStatusOutThreadMsgParam)
		CStatus::eLevel m_nLevel;
		LPTSTR m_pMsg;
		CResult* m_pRes;
		long m_nThreadId;
		LPTSTR m_pObjName;
		LPTSTR m_pFuncName;
		CStatus* m_pStatusObj;
	};

//--------------------------------------------------------------------------------
// CStatusOutThread thread

//--------------------------------------------------------------------------------
class CStatusOutThread : public CWinThread
	{
	DECLARE_DYNCREATE(CStatusOutThread)
	protected:
		CStatusOutThread();           // protected constructor used by dynamic creation

	// Attributes
	public:
		enum
			{
			OUTPUT = WM_USER+1,
			QUIT
			};

	// Operations
	public:

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CStatusOutThread)
		public:
		virtual BOOL InitInstance();
		virtual int ExitInstance();
		//}}AFX_VIRTUAL

	// Implementation
	protected:
		virtual ~CStatusOutThread();

		// Generated message map functions
		//{{AFX_MSG(CStatusOutThread)
			// NOTE - the ClassWizard will add and remove member functions here.
		//}}AFX_MSG

		DECLARE_MESSAGE_MAP()

	public:
		virtual UINT Quit(WPARAM, LPARAM);
		virtual UINT DoOutput(WPARAM, LPARAM);
	};

//--------------------------------------------------------------------------------
class CStatusOutThreadArray : public CTypedPtrArray<CObArray, CStatusOutThread*>
	{
	public:
		~CStatusOutThreadArray()
			{
			for(int i = GetUpperBound(); i >= 0; i--)
				{
				CStatusOutThread* p = GetAt(i);
				p->PostThreadMessage(CStatusOutThread::QUIT, 0, 0);
				::WaitForSingleObject(p->m_hThread, 1000);
				RemoveAt(i);
				}
			};
	};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATUSOUTTHREAD_H__DC1B71F1_2A3F_11D3_87BC_00104B9E6286__INCLUDED_)
