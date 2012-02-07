//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------
// HL7InputDaemon.h: interface for the CHL7InputDaemon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HL7INPUTDAEMON_H__A5625AAC_F0F9_11D2_87A6_00104B9E6286__INCLUDED_)
#define AFX_HL7INPUTDAEMON_H__A5625AAC_F0F9_11D2_87A6_00104B9E6286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HL7InputDaemonThread.h"
#include "HL7InputDaemonInit.h"

#include <commondefines.h>

//--------------------------------------------------------------------------------
class CHL7InputDaemon;
class CHL7Message;
class CHL7InputDaemonThread;
//class CMonitorThread;

//--------------------------------------------------------------------------------
class CHL7InputDaemon : public CObject  
	{
	friend class CHL7InputDaemonThread;
	friend class CHL7FileDaemonThread;

	public:
		CHL7InputDaemon(CWinThread*);
		virtual ~CHL7InputDaemon();

	private:
		CHL7InputDaemonThread* m_pThread;
		CWinThread* m_pMonitor;

	public:
		virtual bool Init(const CHL7InputDaemonInit*);

		bool Pause();
		void Continue();

		bool SendString(LPCTSTR);

		void SignalMsgComplete();
	};

#endif // !defined(AFX_HL7INPUTDAEMON_H__A5625AAC_F0F9_11D2_87A6_00104B9E6286__INCLUDED_)
