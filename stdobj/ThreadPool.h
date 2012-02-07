//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#if !defined(AFX_THREADPOOL_H__5097F278_A674_11D3_AF06_005004A1C5F3__INCLUDED_)
#define AFX_THREADPOOL_H__5097F278_A674_11D3_AF06_005004A1C5F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
class CThreadPoolList : public CTypedPtrList<CObList, CWinThread*>
	{
	public:
		CMutex m_mutex;
	};

//--------------------------------------------------------------------------------
class CThreadPoolMsg
	{
	public:
		CThreadPoolMsg(UINT m, WPARAM w, LPARAM l)
			{
			m_nMsg = m;
			m_wparam = w;
			m_lparam = l;
			}

		UINT m_nMsg;
		WPARAM m_wparam;
		LPARAM m_lparam;
	};

//--------------------------------------------------------------------------------
class CThreadPool : public CWinThread
	{
	DECLARE_DYNCREATE(CThreadPool)
	protected:
		CThreadPool();           // protected constructor used by dynamic creation

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CThreadPool)
		public:
		virtual BOOL InitInstance();
		virtual int ExitInstance();
		//}}AFX_VIRTUAL

	// Implementation
	protected:
		virtual ~CThreadPool();

		// Generated message map functions
		//{{AFX_MSG(CThreadPool)
			// NOTE - the ClassWizard will add and remove member functions here.
		//}}AFX_MSG

		DECLARE_MESSAGE_MAP()

	private:
		CThreadPoolList m_active;
		CThreadPoolList m_waiting;

	public:
		int GetActiveCount();
		int GetWaitingCount();
		int GetPoolSize();

		bool SetThreadActive(CWinThread*);
		bool SetThreadWaiting(CWinThread*);

		CWinThread* GetNextWaiting();

		void DoMsg(CThreadPoolMsg*, LPARAM);
		void DoInit(DWORD nCount, CRuntimeClass*);

		enum
			{
			TPMSG_MESSAGE = WM_USER,
			TPMSG_INIT
			};
	};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THREADPOOL_H__5097F278_A674_11D3_AF06_005004A1C5F3__INCLUDED_)
