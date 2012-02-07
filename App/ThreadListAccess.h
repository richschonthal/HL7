//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// ThreadListAccess.h: interface for the CThreadListAccess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THREADLISTACCESS_H__7416F040_18F3_11D3_87B5_00104B9E6286__INCLUDED_)
#define AFX_THREADLISTACCESS_H__7416F040_18F3_11D3_87B5_00104B9E6286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
class COutputApp;

//--------------------------------------------------------------------------------
class CThreadListAccess
	{
	private:
		CSingleLock m_lock;

	public:
		CThreadListAccess(COutputApp*);
		~CThreadListAccess();

		CServerThreadInfoArray* m_pThreads;
	};


#endif // !defined(AFX_THREADLISTACCESS_H__7416F040_18F3_11D3_87B5_00104B9E6286__INCLUDED_)
