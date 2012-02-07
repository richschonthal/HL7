//--------------------------------------------------------------------------------
// Copyright (c) 2001 @COMPANY, Inc.
// Created...: 6/9/01
// Author....: Rich Schonthal
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "ChangeNotificationDirThread.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//--------------------------------------------------------------------------------
CChangeNotificationDirThread::CChangeNotificationDirThread(CChangeNotificationSubSystem* pParent)
		: CThreadObject(pParent)
		, m_pSubSystem(pParent)
	{
	}

//--------------------------------------------------------------------------------
CChangeNotificationDirThread::~CChangeNotificationDirThread()
	{
	}

//--------------------------------------------------------------------------------
void CChangeNotificationDirThread::OnMessage(UINT nMsg, WPARAM wparam, LPARAM lparam)
	{
	switch(nMsg)
		{
		case CChangeNotificationSubSystem::MsgChangeNotify:
			{
			m_pSubSystem->AddToQueue((FILE_NOTIFY_INFORMATION*) wparam, (int) lparam);
			break;
			}
		}
	}

