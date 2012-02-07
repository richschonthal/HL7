//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#if !defined(AFX_SUBSYSTEMCONTROLTHREAD_H__785892DD_8B64_11D3_B0DD_00A0CC271D0D__INCLUDED_)
#define AFX_SUBSYSTEMCONTROLTHREAD_H__785892DD_8B64_11D3_B0DD_00A0CC271D0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
#include "ThreadObject.h"
#include "System.h"
//--------------------------------------------------------------------------------
class CSubSystemControlThread : public CThreadObject
	{
	protected:
		CSubSystem* m_pSubSystem;
		DWORD m_nLastState;
		bool m_bRunning;

	public:
		CSubSystemControlThread(CSubSystem*);
		virtual ~CSubSystemControlThread();

		virtual void OnStateChange(DWORD);

		virtual bool MainLoop();

		CSystem* GetSystem()		{ return (CSystem*) GetParent(); }
		CSubSystem* GetSubSystem()	{ return m_pSubSystem; }
	};

#endif // !defined(AFX_SUBSYSTEMCONTROLTHREAD_H__785892DD_8B64_11D3_B0DD_00A0CC271D0D__INCLUDED_)
