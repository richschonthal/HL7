//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "WaitableTimer.h"

//--------------------------------------------------------------------------------
CWaitableTimer::CWaitableTimer(BOOL bManualReset, LPCTSTR pName, LPSECURITY_ATTRIBUTES pAttribute)
		: CSyncObject(pName)
	{
	m_hObject = ::CreateWaitableTimer(pAttribute, bManualReset, pName);
	}
