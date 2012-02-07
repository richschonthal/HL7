//--------------------------------------------------------------------------------
// Copyright (c) 2001 @COMPANY, Inc.
// Created...: 6/8/01
// Author....: Rich Schonthal
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "ChangeNotificationContextObj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//--------------------------------------------------------------------------------
ReadDirectoryChangesWType CChangeNotificationContextObj::m_pReadDirectoryChangesW =
	(ReadDirectoryChangesWType) GetProcAddress(GetModuleHandle("kernel32.dll"), "ReadDirectoryChangesW");

//--------------------------------------------------------------------------------
CChangeNotificationContextObj::CChangeNotificationContextObj()
		: m_event(FALSE, TRUE)
		, m_pTarget(NULL)
		, m_bWatchSubDir(false)
		, m_nFilter(0)
	{
	}

//--------------------------------------------------------------------------------
CChangeNotificationContextObj::~CChangeNotificationContextObj()
	{
	}
