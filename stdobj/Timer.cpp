//--------------------------------------------------------------------------------
//
// Copyright (c) 2001 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "Timer.h"

//--------------------------------------------------------------------------------
CTimer::CTimer(LONG nPeriod)
		: m_hTimer(NULL)
	{
	if(nPeriod == -1)
		return;

	if(! Create())
		return;

	if(nPeriod == 0)
		return;

	Set(nPeriod);
	}

//--------------------------------------------------------------------------------
CTimer::~CTimer()
	{
	Close();
	}

//--------------------------------------------------------------------------------
bool CTimer::Create(bool bManual, LPCTSTR pName, SECURITY_ATTRIBUTES* pSecurity)
	{
	if(IsValid())
		{
		SetResultCode(ErrTimerAlreadySet);
		return false;
		}

	m_hTimer = ::CreateWaitableTimer(pSecurity, bManual, pName);

	if(IsValid())
		return true;

	SetResult(errOS, ::GetLastError());
	return false;
	}

//--------------------------------------------------------------------------------
bool CTimer::Open(LPCTSTR pName, DWORD nAccess,  bool bInherit)
	{
	if(IsValid())
		Close();

	m_hTimer = ::OpenWaitableTimer(nAccess, bInherit, pName);
	if(IsValid())
		return true;

	SetResult(errOS, ::GetLastError());
	return false;
	}

//--------------------------------------------------------------------------------
bool CTimer::Set(LONG nPeriod, __int64 nDue, PTIMERAPCROUTINE pProc, void* pParam, bool bResume)
	{
	if(! IsValid())
		{
		SetResultCode(ErrTimerNotOpen);
		return false;
		}

	if(::SetWaitableTimer(m_hTimer, (LARGE_INTEGER*) &nDue, nPeriod, pProc, pParam, bResume))
		return true;

	SetResult(errOS, ::GetLastError());
	return false;
	}

//--------------------------------------------------------------------------------
bool CTimer::Cancel()
	{
	if(! IsValid())
		return false;

	if(::CancelWaitableTimer(m_hTimer))
		return true;

	SetResult(errOS, ::GetLastError());
	return false;
	}

//--------------------------------------------------------------------------------
bool CTimer::Close()
	{
	if(! Cancel())
		return false;

	if(::CloseHandle(m_hTimer))
		{
		m_hTimer = NULL;
		return true;
		}

	m_hTimer = NULL;
	SetResult(errOS, ::GetLastError());
	return false;
	}

//--------------------------------------------------------------------------------
bool CTimer::Wait(DWORD nWait)
	{
	if(! IsValid())
		{
		::Sleep(nWait);
		return false;
		}

	return ::WaitForSingleObject(m_hTimer, nWait) == WAIT_OBJECT_0;
	}
