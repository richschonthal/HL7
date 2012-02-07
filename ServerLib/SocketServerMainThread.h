//--------------------------------------------------------------------------------
// Copyright (c) 2001 @COMPANY, Inc.
// Created...: 6/27/01
// Author....: Rich Schonthal
//--------------------------------------------------------------------------------

#ifndef _SOCKETSERVERMAINTHREAD_H_
#define _SOCKETSERVERMAINTHREAD_H_

//--------------------------------------------------------------------------------
#include "ThreadObject.h"
#include <SmallSocket.h>

//--------------------------------------------------------------------------------
class CSocketServerSubSystem;

//--------------------------------------------------------------------------------
class CSocketServerMainThread : public CThreadObject
	{
	protected:
		CSmallSocket m_socket;
		CSocketServerSubSystem* m_pSub;

	public:
		CSocketServerMainThread(CSocketServerSubSystem*);
		virtual ~CSocketServerMainThread();

		virtual bool MainLoop();
		virtual void OnMessage(UINT, WPARAM, LPARAM);
	};

#endif // _SOCKETSERVERMAINTHREAD_H_
