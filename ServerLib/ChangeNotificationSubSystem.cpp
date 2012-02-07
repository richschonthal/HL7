//--------------------------------------------------------------------------------
// Copyright (c) 2001 @COMPANY, Inc.
// Created...: 6/9/01
// Author....: Rich Schonthal
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#include "stdafx.h"
#include "ChangeNotificationSubSystem.h"
#include "ChangeNotificationThread.h"
#include "ChangeNotificationDirThread.h"

#include <Directory.h>

//--------------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CChangeNotificationSubSystem, CSubSystem)

//--------------------------------------------------------------------------------
void AFXAPI DestructElements<CFileNotifyInformation*>(CFileNotifyInformation** pElements, int nCount)
	{
	for(int i = 0; i < nCount; i++)
		delete *pElements;
	}

//--------------------------------------------------------------------------------
CChangeNotificationSubSystem::CChangeNotificationSubSystem(CSystem* pParent)
		: CThreadPoolSubSystem(pParent)
	{
	AddThread(RUNTIME_CLASS(CChangeNotificationThread));
	}

//--------------------------------------------------------------------------------
CChangeNotificationSubSystem::~CChangeNotificationSubSystem()
	{
	}

//--------------------------------------------------------------------------------
bool CChangeNotificationSubSystem::AddWatch(LPCTSTR pPath, CSystemObject* pTarget, int nMsgId, bool bWatchSubDir, DWORD nFilter)
	{
	CFileChangeNotificationContextObj* pContext = new CFileChangeNotificationContextObj;
	if(! pContext->Create(pPath, bWatchSubDir, nFilter, pTarget, nMsgId))
		{
		delete pContext;
		return false;
		}
	QueueContext((CChangeNotificationContextObj*) pContext);
	return true;
	}

//--------------------------------------------------------------------------------
bool CChangeNotificationSubSystem::AddWatch(HKEY hKey, CSystemObject* pTarget, int nMsgId, bool bWatchSubDir, DWORD nFilter)
	{
	CRegChangeNotificationContextObj* pContext = new CRegChangeNotificationContextObj;
	if(! pContext->Create(hKey, bWatchSubDir, nFilter, pTarget, nMsgId))
		{
		delete pContext;
		return false;
		}
	QueueContext((CChangeNotificationContextObj*) pContext);
	return true;
	}

//--------------------------------------------------------------------------------
int CChangeNotificationSubSystem::GetObjectId()
	{
	return OIDTM_SUBSYSTEM + SLOID_CN;
	}

//--------------------------------------------------------------------------------
bool CChangeNotificationSubSystem::PostNextThreadMessage(UINT, WPARAM, LPARAM)
	{
	return false;
	}
