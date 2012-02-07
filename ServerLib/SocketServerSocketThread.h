//--------------------------------------------------------------------------------
// Copyright (c) 2001 @COMPANY, Inc.
// Created...: 6/27/01
// Author....: Rich Schonthal
//--------------------------------------------------------------------------------

#ifndef _SOCKETSERVERSOCKETTHREAD_H_
#define _SOCKETSERVERSOCKETTHREAD_H_

//--------------------------------------------------------------------------------
#include "ThreadObject.h"
#include "SocketServerQueue.h"

#include <SmallSocket.h>

//--------------------------------------------------------------------------------
class CSocketServerSocketThread : public CThreadObject
	{
	protected:
		friend class CSocketServerSubSystem;

		CSmallSocket m_socket;
		CSocketServerSubSystem* m_pSub;
		CSocketServerQueue m_queue;

	public:
		CSocketServerSocketThread(CSocketServerSubSystem*);
		virtual ~CSocketServerSocketThread();

		virtual bool MainLoop();
		virtual void OnMessage(UINT, WPARAM, LPARAM);
	};

#endif // _SOCKETSERVERSOCKETTHREAD_H_
