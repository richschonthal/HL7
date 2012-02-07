//--------------------------------------------------------------------------------
// Copyright (c) 2001 @COMPANY, Inc.
// Created...: 6/8/01
// Author....: Rich Schonthal
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "ChangeNotificationThread.h"
#include "ChangeNotificationSubSystem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//--------------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(CChangeNotificationThread, CObject);

//--------------------------------------------------------------------------------
CChangeNotificationThread::CChangeNotificationThread()
		: CTypedThreadObject<CSystem, CChangeNotificationSubSystem>(NULL)
	{
	}

//--------------------------------------------------------------------------------
CChangeNotificationThread::~CChangeNotificationThread()
	{
	}

//--------------------------------------------------------------------------------
DWORD CChangeNotificationThread::GetThreadLoopSleepInterval() const
	{
	return 100;
	}

//--------------------------------------------------------------------------------
bool CChangeNotificationThread::MainLoop()
	{
	CChangeNotificationContextObj* pContext = GetSubSystem()->GetNextContext();
	if(pContext == NULL)
		return true;

	if(! pContext->IsValid())
		{
		GetSubSystem()->QueueContext(pContext);
		return true;
		}

	if(::WaitForSingleObject(pContext->m_event, 0) == WAIT_OBJECT_0)
		{
		if(! pContext->IsChangeReady())
			{
			GetSubSystem()->QueueContext(pContext);
			return true;
			}

		if(! pContext->PostChanges())
			{
			GetSubSystem()->QueueContext(pContext);
			return true;
			}

		if(! pContext->GetNextChange())
			{
			GetSubSystem()->QueueContext(pContext);
			return true;
			}
		}

	GetSubSystem()->QueueContext(pContext);
	return true;
	}

//--------------------------------------------------------------------------------
int CChangeNotificationThread::GetObjectId()
	{
	return OIDTM_THREAD + SLOID_CN;
	}
