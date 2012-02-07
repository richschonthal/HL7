//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#if !defined(AFX_IOTHREAD_H__C10D0D11_CF6E_11D3_AF12_005004A1C5F3__INCLUDED_)
#define AFX_IOTHREAD_H__C10D0D11_CF6E_11D3_AF12_005004A1C5F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
#include "ThreadObject.h"

class CIOSubSystem;

//--------------------------------------------------------------------------------
class CIOThread : public CTypedThreadObject<CSystem, CIOSubSystem>
	{
	DECLARE_DYNAMIC(CIOThread);

	public:
		CIOThread(CIOSubSystem*);
		virtual ~CIOThread();

		virtual void Exit(bool);

//		CIOSubSystem* GetSubSystem()	{ return (CIOSubSystem*) CThreadObject::GetSubSystem(); }

		virtual void OnMessage(UINT, WPARAM, LPARAM);
		virtual void Output(LPCTSTR);

		// called by state clients to acknowldge state change
		virtual void AcknowledgeStateChange();

		virtual int GetObjectId();
	};

#endif // !defined(AFX_IOTHREAD_H__C10D0D11_CF6E_11D3_AF12_005004A1C5F3__INCLUDED_)
