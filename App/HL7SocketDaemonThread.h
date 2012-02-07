//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// HL7SocketDaemonThread.h: interface for the CHL7SocketDaemonThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HL7SOCKETDAEMONTHREAD_H__D5D28A4E_02F2_11D3_87A7_00104B9E6286__INCLUDED_)
#define AFX_HL7SOCKETDAEMONTHREAD_H__D5D28A4E_02F2_11D3_87A7_00104B9E6286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
#include "HL7InputDaemonThread.h"
#include <SmallSocket.h>

//--------------------------------------------------------------------------------
class CSocketWrapper
	{
	private:
		CSmallSocket* m_pSocket;
		CMutex m_mutex;

	public:
		CSocketWrapper();
		~CSocketWrapper();

		void DestroySocket();
		void CreateNewSocket();
		CSmallSocket* GetSocket();
		bool IsValidSocket() const;

		CMutex& GetMutex()	{ return m_mutex; }
	};

//--------------------------------------------------------------------------------
class CHL7SocketDaemonThread : public CHL7InputDaemonThread  
	{
	DECLARE_DYNCREATE(CHL7SocketDaemonThread)

	public:
		CHL7SocketDaemonThread();
		virtual ~CHL7SocketDaemonThread();

	private:
		CSocketWrapper m_mainSock;
		CSocketWrapper m_curSock;

	public:
		virtual void Init(const CHL7InputDaemonInit*, CHL7InputDaemon*);

	protected:
		virtual bool SendString(LPCTSTR);

	private:
		virtual void MainLoop(WPARAM, LPARAM);

		bool GetConnection(bool = false);
	};

#endif // !defined(AFX_HL7SOCKETDAEMONTHREAD_H__D5D28A4E_02F2_11D3_87A7_00104B9E6286__INCLUDED_)
