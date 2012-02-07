//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// ServerThreadInfo.h: interface for the CServerThreadInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVERTHREADINFO_H__1008E06F_0872_11D3_87AA_00104B9E6286__INCLUDED_)
#define AFX_SERVERTHREADINFO_H__1008E06F_0872_11D3_87AA_00104B9E6286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ProtocolObj.h"

//--------------------------------------------------------------------------------
class CServerThread;

//--------------------------------------------------------------------------------
class CServerThreadInfo : public CObject  
	{
	public:
		CServerThreadInfo();
		virtual ~CServerThreadInfo();

	public:
		CServerThread* m_pThread;
		CWinThread* m_pMonitor;
		CTime m_ctLastPulse;
		CProtocolObjList m_msgQueue;

	public:
		CServerThread* GetThreadPtr()	{ return m_pThread; }
		void SetThreadPtr(CServerThread* p)	{ m_pThread = p; }

		CWinThread* GetMonitorPtr()	{ return m_pMonitor; }
		void SetMonitorPtr(CWinThread* p)	{ m_pMonitor = p; }

		void SetPulseTime()				{ m_ctLastPulse = CTime::GetCurrentTime();}
		CTimeSpan GetLastPulseSpan() const	{ return CTime::GetCurrentTime() - m_ctLastPulse; }

		CProtocolObjList& GetMsgQueue()	{ return m_msgQueue; }
		CProtocolObj* GetNextMsg();
	};

//--------------------------------------------------------------------------------
class CServerThreadInfoArray : public CTypedPtrArray<CObArray, CServerThreadInfo*>
	{
	public:
		~CServerThreadInfoArray();

		bool AddMsgToAll(CProtocolObj);

		CMutex m_mutex;
	};

#endif // !defined(AFX_SERVERTHREADINFO_H__1008E06F_0872_11D3_87AA_00104B9E6286__INCLUDED_)
