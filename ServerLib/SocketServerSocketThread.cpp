//--------------------------------------------------------------------------------
// Copyright (c) 2001 @COMPANY, Inc.
// Created...: 6/30/01
// Author....: Rich Schonthal
//--------------------------------------------------------------------------------

#include "StdAfx.h"
#include "SocketServerSocketThread.h"
#include "SocketServerSubSystem.h"

//--------------------------------------------------------------------------------
CSocketServerSocketThread::CSocketServerSocketThread(CSocketServerSubSystem* pParent)
		: CThreadObject(pParent)
		, m_pSub(pParent)
	{
	}

//--------------------------------------------------------------------------------
CSocketServerSocketThread::~CSocketServerSocketThread()
	{
	}

//--------------------------------------------------------------------------------
bool CSocketServerSocketThread::MainLoop()
	{
	DWORD nState = GetState();
	if(nState == STATE_PAUSE)
		{
		if(m_pSub->IsClosePortsOnPause())
			m_socket.Close();
		return true;
		}

	if(m_socket.IsValid())
		{
		if(m_socket.Listen(5))
			{
			CSmallSocket sock;
			if(m_socket.Accept(sock))
				{
				SOCKET hSocket = sock.Detach();
				if(! m_pSub->PostNextThreadMessage(CSocketServerSubSystem::MsgConnect, (WPARAM) hSocket, 0))
					sock.Attach(hSocket);
				}
			}
		}

	return CheckState();
	}
 
//--------------------------------------------------------------------------------
void CSocketServerSocketThread::OnMessage(UINT nMsg, WPARAM wparam, LPARAM lparam)
	{
	switch(nMsg)
		{
		case CSocketServerSubSystem::MsgConnect:
			m_socket.Attach((SOCKET) wparam);
			break;
		}
	}
