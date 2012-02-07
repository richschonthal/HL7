//--------------------------------------------------------------------------------
// Copyright (c) 2001 @COMPANY, Inc.
// Created...: 7/4/01
// Author....: Rich Schonthal
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#ifndef _SYSTEMTIMER_H_
#define _SYSTEMTIMER_H_

//--------------------------------------------------------------------------------
#ifndef __AFXMT_H__
#include <afxmt.h>
#endif

//--------------------------------------------------------------------------------
class CSystemTimer
	{
	protected:
		static __int64 m_nTickCount;
		static CMutex m_mutex;
		static LONG m_nResolution;
		static LONG m_nRefCount;
		static LONG m_nIsResolutionChanged;
		static CWinThread* m_pThread;

	protected:
		static UINT SystemTimerThread(LPVOID);

	public:
		CSystemTimer();
		virtual ~CSystemTimer();

		static __int64 GetTickCount();
		static void GetTickCount(DWORD& nHi, DWORD& nLo);
		static void SetResolution(LONG);
	};

#endif // _SYSTEMTIMER_H_