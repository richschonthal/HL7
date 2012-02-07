//--------------------------------------------------------------------------------
//
// Copyright (c) 2001 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _TIMER_H_
#define _TIMER_H_

//--------------------------------------------------------------------------------
#include "result.h"

//--------------------------------------------------------------------------------
class CTimer : public CResult
	{
	public:
		// period = the length of time to call set with
		// if period == 0, the timer is created but not set
		// if period == -1 the timer is not created
		CTimer(LONG period = 0);

	public:
		HANDLE m_hTimer;

	public:
		enum
			{
			ErrTimerAlreadySet = HighestResultCode,
			ErrTimerNotOpen
			};

	public:
		virtual ~CTimer();

		virtual bool Create(bool manual = false, LPCTSTR = NULL, SECURITY_ATTRIBUTES* = NULL);
		bool Open(LPCTSTR name, DWORD = TIMER_ALL_ACCESS,  bool inherit = false);
		virtual bool Set(LONG period, __int64 due = 0, PTIMERAPCROUTINE = NULL, void* param = NULL, bool resume = true);
		bool Cancel();
		bool Close();

		bool IsSignaled(DWORD wait = 0);
		bool Wait(DWORD wait = INFINITE);

		bool IsValid();

		operator HANDLE();
	};

//--------------------------------------------------------------------------------
inline bool CTimer::IsValid() { return m_hTimer != NULL && m_hTimer != INVALID_HANDLE_VALUE; }
inline bool CTimer::IsSignaled(DWORD nWait) { return ::WaitForSingleObject(m_hTimer, nWait) == WAIT_OBJECT_0; }
inline CTimer::operator HANDLE() { return m_hTimer; }

//--------------------------------------------------------------------------------
class CPulseTimer : public CTimer
	{
	public:
		CPulseTimer() : CTimer(0) {}
		virtual bool Set(LONG period, PTIMERAPCROUTINE = NULL, void* param = NULL, bool resume = true);
		virtual bool Set(LONG period, bool bEatFirstPulse);
	};

//--------------------------------------------------------------------------------
inline bool CPulseTimer::Set(LONG nPeriod, PTIMERAPCROUTINE pProc, void* pParam, bool bResume)
	{
	if(! CTimer::Set(nPeriod, NULL, pProc, pParam, bResume))
		return false;
	// clear the first signal
	IsSignaled();
	return true;
	}

//--------------------------------------------------------------------------------
inline bool CPulseTimer::Set(LONG nPeriod, bool bEatFirstPulse)
	{
	if(! CTimer::Set(nPeriod, NULL, NULL, NULL, true))
		return false;
	if(bEatFirstPulse)
		// clear the first signal
		IsSignaled();
	return true;
	}

#endif // _TIMER_H_
