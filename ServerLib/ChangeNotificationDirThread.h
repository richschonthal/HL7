//--------------------------------------------------------------------------------
// Copyright (c) 2001 @COMPANY, Inc.
// Created...: 6/8/01
// Author....: Rich Schonthal
//--------------------------------------------------------------------------------

#if !defined(AFX_CHANGENOTIFICATIONDIRTHREAD_H__1A4AB94F_5C82_11D5_B191_00A0CC271D0D__INCLUDED_)
#define AFX_CHANGENOTIFICATIONDIRTHREAD_H__1A4AB94F_5C82_11D5_B191_00A0CC271D0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
#include <Directory.h>
#include <Handle.h>

#include "ThreadObject.h"
#include "ChangeNotificationSubSystem.h"

//--------------------------------------------------------------------------------
class CChangeNotificationDirThread : public CTypedThreadObject<CSystem, CChangeNotificationSubSystem>
	{
	protected:
		CChangeNotificationSubSystem* m_pSubSystem;

	public:
		CChangeNotificationDirThread(CChangeNotificationSubSystem*);
		virtual ~CChangeNotificationDirThread();

		virtual void OnMessage(UINT, WPARAM, LPARAM);
	};

#endif // !defined(AFX_CHANGENOTIFICATIONDIRTHREAD_H__1A4AB94F_5C82_11D5_B191_00A0CC271D0D__INCLUDED_)
