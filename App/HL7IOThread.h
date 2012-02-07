//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _HL7IOTHREAD_H_
#define _HL7IOTHREAD_H_

//--------------------------------------------------------------------------------
#include <IOThread.h>

class CHL7IOSubSystem;

//--------------------------------------------------------------------------------
class CHL7IOThread : public CIOThread
	{
	public:
		CHL7IOThread(CHL7IOSubSystem*);
		virtual void OnMessage(UINT, WPARAM, LPARAM);
	};

#endif // _HL7IOTHREAD_H_