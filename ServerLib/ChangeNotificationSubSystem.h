//--------------------------------------------------------------------------------
// Copyright (c) 2001 @COMPANY, Inc.
// Created...: 6/11/01
// Author....: Rich Schonthal
//--------------------------------------------------------------------------------

#if !defined(AFX_CHANGENOTIFICATIONSUBSYSTEM_H__B66F74A9_E5CF_11D3_B10F_00A0CC271D0D__INCLUDED_)
#define AFX_CHANGENOTIFICATIONSUBSYSTEM_H__B66F74A9_E5CF_11D3_B10F_00A0CC271D0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
#include "SubSystem.h"
#include "System.h"
#include "ChangeNotificationThread.h"
#include "ChangeNotificationContextObj.h"

#include <Directory.h>
#include <StdobjQueue.h>
#include <ContextSwitcher.h>

#ifndef __AFXTEMPL_H__
#include <afxtempl.h>
#endif

//--------------------------------------------------------------------------------
class CChangeNotificationSubSystem : public CThreadPoolSubSystem, public CContextSwitcher<CChangeNotificationContextObj>
	{
	// needs to access AddToQueue and MsgAddToQueue
	friend class CChangeNotificationThread;

	public:
		DECLARE_DYNAMIC(CChangeNotificationSubSystem);
		enum
			{
			MsgChangeNotify = CSystem::MSG_DERIVED_START
			};

	public:
		CChangeNotificationSubSystem(CSystem*);
		virtual ~CChangeNotificationSubSystem();

		bool AddWatch(LPCTSTR pPath, CSystemObject* pTarget, int nMsgId = WM_USER,
			bool bWatchSubDir = true, DWORD nFilter = 0xffffffff);

		bool AddWatch(HKEY, CSystemObject* pTarget, int nMsgId = WM_USER,
			bool bWatchSubDir = true, DWORD nFilter = 0xffffffff);

		virtual int GetObjectId();

	protected:
		// cant do this with this sub system since each thread is watching a different directory
		virtual bool PostNextThreadMessage(UINT, WPARAM = 0, LPARAM = 0);
	};

#endif // !defined(AFX_CHANGENOTIFICATIONSUBSYSTEM_H__B66F74A9_E5CF_11D3_B10F_00A0CC271D0D__INCLUDED_)
