//--------------------------------------------------------------------------------
// Copyright (c) 2001 @COMPANY, Inc.
// Created...: 6/8/01
// Author....: Rich Schonthal
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "ChangeNotificationContextObj.h"
#include "ChangeNotificationObj.h"

#include <SystemObject.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//--------------------------------------------------------------------------------
CRegChangeNotificationContextObj::CRegChangeNotificationContextObj()
		: m_hKey(NULL)
	{
	}

//--------------------------------------------------------------------------------
bool CRegChangeNotificationContextObj::Create(HKEY hKey, bool bWatchSubDir, DWORD nFilter, CSystemObject* pTarget, int nMsgId)
	{
	if(nFilter == 0xffffffff)
		nFilter = REG_NOTIFY_CHANGE_NAME|REG_NOTIFY_CHANGE_ATTRIBUTES|REG_NOTIFY_CHANGE_LAST_SET|REG_NOTIFY_CHANGE_SECURITY;
	m_hKey = hKey;
	m_nFilter = nFilter;
	m_pTarget = pTarget;
	m_nMessageId = nMsgId;
	m_bWatchSubDir = bWatchSubDir;
	m_event.ResetEvent();
	return GetNextChange();
	}

//--------------------------------------------------------------------------------
CRegChangeNotificationContextObj::~CRegChangeNotificationContextObj()
	{
	}

//--------------------------------------------------------------------------------
bool CRegChangeNotificationContextObj::PostChanges()
	{
	CRegChangeNotificationObj* pInfo = new CRegChangeNotificationObj(m_hKey);
	m_pTarget->PostMessage(GetMessageId(), (WPARAM) pInfo);
	return true;
	}
