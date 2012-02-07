//--------------------------------------------------------------------------------
// Copyright (c) 2001 @COMPANY, Inc.
// Created...: 7/4/01
// Author....: Rich Schonthal
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "SystemTimer.h"
#include "Timer.h"

//--------------------------------------------------------------------------------
__int64 CSystemTimer::m_nTickCount = 0;
CMutex CSystemTimer::m_mutex;
LONG CSystemTimer::m_nResolution = 2;
LONG CSystemTimer::m_nRefCount = 0;
LONG CSystemTimer::m_nIsResolutionChanged = 0;
CWinThread* CSystemTimer::m_pThread = NULL;

//--------------------------------------------------------------------------------
CSystemTimer::CSystemTimer()
	{
	if(InterlockedIncrement(&m_nRefCount) == 1)
		m_pThread = AfxBeginThread((AFX_THREADPROC) SystemTimerThread, NULL);
	}

//--------------------------------------------------------------------------------
CSystemTimer::~CSystemTimer()
	{
	if(InterlockedDecrement(&m_nRefCount) == 0)
		{
		if(m_pThread != NULL)
			{
			m_pThread->PostThreadMessage(WM_QUIT, 0, 0);
			::WaitForSingleObject(m_pThread->m_hThread, INFINITE);
			}
		}
	}

//--------------------------------------------------------------------------------
__int64 CSystemTimer::GetTickCount()
	{
	CSingleLock lock(&m_mutex);
	if(! lock.Lock(INFINITE))
		return -1;
	return m_nTickCount;
	}

//--------------------------------------------------------------------------------
void CSystemTimer::GetTickCount(DWORD& nHi, DWORD& nLo)
	{
	LARGE_INTEGER nTick;
	nTick.QuadPart = GetTickCount();
	nHi = nTick.HighPart;
	nLo = nTick.LowPart;
	}

//--------------------------------------------------------------------------------
void CSystemTimer::SetResolution(LONG nResolution)
	{
	InterlockedExchange(&m_nResolution, nResolution);
	InterlockedExchange(&m_nIsResolutionChanged, 1);
	}

//--------------------------------------------------------------------------------
UINT CSystemTimer::SystemTimerThread(LPVOID)
	{
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
	CPulseTimer timer;
	timer.Set(1, true);
	for(;;)
		{
		if(InterlockedExchange(&m_nIsResolutionChanged, 0) == 1)
			return 0;

		if(timer.IsSignaled())
			m_nTickCount++;

		::Sleep(1);
		}
	}

#if 0
//--------------------------------------------------------------------------------
UINT CSystemTimer::SystemTimerThread(LPVOID)
	{
#define Range(nMin, nMax, nVal) __max((nMin), __min((nMax), (nVal)))

	CPulseTimer* pTimer = NULL;
	LONG nRes = m_nResolution;
	nRes = Range(2, 1000, nRes);
	LONG nWait = Range(5, 20, nRes / 2);
	for(;;)
		{
		MSG msg;
		if(PeekMessage(&msg, NULL, WM_QUIT, WM_QUIT, PM_REMOVE))
			return 0;

		// see if the resolution's changed
		if(InterlockedExchange(&m_nIsResolutionChanged, 0) == 1)
			{
			nRes = m_nResolution;
			nRes = Range(2, 1000, nRes);
			nWait = Range(5, 20, nRes / 2);
			delete pTimer;
			pTimer = NULL;
			}

		if(pTimer == NULL)
			{
			pTimer = new CPulseTimer;
			pTimer->Set(nRes, true);
			}

		if(pTimer->Wait(nWait))
			{
			CSingleLock lock(&m_mutex);
			if(! lock.Lock(INFINITE))
				continue;
			m_nTickCount += nRes + 1;
			}
		}
	}
#endif
