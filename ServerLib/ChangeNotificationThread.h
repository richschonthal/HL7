//--------------------------------------------------------------------------------
// Copyright (c) 2001 @COMPANY, Inc.
// Created...: 6/8/01
// Author....: Rich Schonthal
//--------------------------------------------------------------------------------

#if !defined(AFX_CHANGENOTIFICATIONTHREAD_H__1A4AB94F_5C82_11D5_B191_00A0CC271D0D__INCLUDED_)
#define AFX_CHANGENOTIFICATIONTHREAD_H__1A4AB94F_5C82_11D5_B191_00A0CC271D0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
#include <Directory.h>
#include <Handle.h>
#include <FileNotifyInformation.h>

#include "ThreadObject.h"

//--------------------------------------------------------------------------------
class CChangeNotificationSubSystem;

//--------------------------------------------------------------------------------
class CChangeNotificationThread : public CTypedThreadObject<CSystem, CChangeNotificationSubSystem>
	{
	DECLARE_DYNCREATE(CChangeNotificationThread);

	public:
		CChangeNotificationThread();
		virtual ~CChangeNotificationThread();

		virtual bool MainLoop();
		virtual DWORD GetThreadLoopSleepInterval() const;

		int GetPathIndex() const;

		int GetObjectId();
	};

#endif // !defined(AFX_CHANGENOTIFICATIONTHREAD_H__1A4AB94F_5C82_11D5_B191_00A0CC271D0D__INCLUDED_)
