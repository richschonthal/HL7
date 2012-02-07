//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------
//WaitableTimer.h

#ifndef _WAITABLETIMER_H_
#define _WAITABLETIMER_H_

//--------------------------------------------------------------------------------
class CWaitableTimer : public CSyncObject
	{
	DECLARE_DYNAMIC(CWaitableTimer)
	public:
		CWaitableTimer(BOOL bManualReset = FALSE, LPCTSTR pName = NULL, LPSECURITY_ATTRIBUTES pAttribute = NULL);
		virtual ~CWaitableTimer();

	public:
		virtual BOOL Lock(DWORD dwTimeout = INFINITE);
		virtual BOOL Unlock() = 0;
	};


#endif // _WAITABLETIMER_H_
