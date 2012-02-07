//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#include "stdafx.h"
#include "Monitor.h"

#include <DebugPrintf.h>

#include "defs.h"
#include "waitlen.h"
#include "globals.h"
#include "HL7SystemMsg.h"
#include "HL7InputDaemon.h"
#include "HL7Message.h"
#include "HL7InputDaemonInit.h"
#include "OutputConfig.h"

#include "ProtocolObj.h"
#include <DebugPrintf.h>
#include <cmdline.h>
#include <allocstring.h>
#include <registry.h>
#include "ConnectionException.h"
#include <PlatformInfo.h>
#include <PathString.h>
#include "ServerThreadInfo.h"
#include "ServerThread.h"

//--------------------------------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CMonitorThread
//--------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CMonitorThread, CWinThread)
	//{{AFX_MSG_MAP(CMonitorThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_THREAD_MESSAGE(INIT, DoInit)
	ON_THREAD_MESSAGE(HL7_HL7MSG_IN, DoMsgFromInput)
	ON_THREAD_MESSAGE(HL7_PROTOCOL, DoMsgFromServer)
END_MESSAGE_MAP()

//--------------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(CMonitorThread, CWinThread)

// CMonitorThread construction
//--------------------------------------------------------------------------------
CMonitorThread::CMonitorThread()
		: m_pInputInit(NULL)
		, m_pInput(NULL)
		, m_nProcessId(0)
		, m_pParamKey(NULL)
	{
	}

//--------------------------------------------------------------------------------
CMonitorThread::~CMonitorThread()
	{
	delete m_pInput;
	delete m_pInputInit;
	delete m_pParamKey;
	}

// CMonitorThread initialization
//--------------------------------------------------------------------------------
BOOL CMonitorThread::InitInstance()
	{
	// this process doesnt do much at all until it receives
	// an INIT message
	return TRUE;
	}

//--------------------------------------------------------------------------------
void CMonitorThread::DoInit(CHL7InputDaemonInit* pInit, CServerThreadInfo* pServerThreadInfo)
	{
	GetIO()->FormatOutput(IOMASK_7, "CMonitorThread::DoInit entry (%s)", pInit->GetName());
	TRY
		{
		COutputConfig config;
		m_pInputInit = pInit;
		m_pServerThreadInfo = pServerThreadInfo;

		GetIO()->AddThreadName(m_pInputInit->GetName());

		if(m_pInputInit->GetType() == CHL7InputDaemonInit::INPUT_FILE)
			{
			// create our output paths
			CPlatformInfo info;
			info.CreatePath(m_pInputInit->GetPath());
			info.CreatePath(m_pInputInit->GetOutPath());
			CPathString sLogPath(m_pInputInit->GetLogFile());
			sLogPath.RemoveFileSpec();
			info.CreatePath(sLogPath);
			}

		m_pInput = new CHL7InputDaemon(this);
		m_pInput->Init(m_pInputInit);

		CEvent evtMonitorInit(false, false, "HL7MonitorInit");
		evtMonitorInit.SetEvent();
		}
	CATCH_ALL(e)
		{
		CString sTemp;
		e->GetErrorMessage(sTemp.GetBuffer(1024), 1024);
		sTemp.ReleaseBuffer();
		GetIO()->FormatOutput(IOMASK_ERR, "Exception occured during init - exiting %s", (LPCTSTR) sTemp);
		PostQuitMessage(-1);
		}
	END_CATCH_ALL
	GetIO()->FormatOutput(IOMASK_8, "CMonitorThread::DoInit exit (%s)", pInit->GetName());
	}

//--------------------------------------------------------------------------------
int CMonitorThread::ExitInstance() 
	{
	MSG msg;
	while(::PeekMessage(&msg, NULL, HL7_HL7MSG_IN, HL7_HL7MSG_IN, PM_REMOVE))
		delete (CHL7Message*) msg.wParam;

	while(::PeekMessage(&msg, NULL, HL7_PROTOCOL, HL7_PROTOCOL, PM_REMOVE))
		delete (CHL7Message*) msg.wParam;

	GetServerThread()->PostThreadMessage(HL7_GOODBYE, 0, 0);
	return CWinThread::ExitInstance();
	}

//--------------------------------------------------------------------------------
void CMonitorThread::DoMsgFromInput(CHL7Message* pMsg, LPARAM)
	{
	GetIO()->Output(IOMASK_7|IOMASK_CONST, "CMonitorThread::DoMsgFromInput entry");

	if(pMsg == NULL)
		{
		GetIO()->Output(IOMASK_ERR|IOMASK_CONST, "received a null msg - this should not happen");
		return;
		}

	pMsg->TraceDump();

	// version can be set for each monitor (the default is 0)
	// if version is set to 0 then the version is extracted from
	// the MSH record by the output daemon
	pMsg->SetVersion(m_pInputInit->GetVersion());
	pMsg->SetSourceId(m_nProcessId);

	// tell the server that an HL7 msg is comming
	if(! GetServerThread()->PostThreadMessage(HL7_HL7MSG, (WPARAM) pMsg, 0))
		{
		delete pMsg;
		GetIO()->Output(IOMASK_13|IOMASK_CONST, "throwing CConnectionException (1)");
		return;
		}

	GetIO()->Output(IOMASK_8|IOMASK_CONST, "CMonitorThread::DoMsgFromInput exit");
	}

//--------------------------------------------------------------------------------
void CMonitorThread::DoMsgFromServer(CProtocolObj* pObj, LPARAM)
	{
#if 0 // small packets
#pragma message("*** using \"small packets\" for HL7 response")
	// send in 2 packets
	// send MSH and MSA
	m_pInput->SendString(pObj->GetParam(0));
	m_pInput->SendString(pObj->GetParam(1));
#else // large packets
#pragma message("*** using \"large packets\" for HL7 response")
	// send in 1 packet
	// the new way
	CString sTemp(pObj->GetParam(0));
	sTemp += pObj->GetParam(1);
	m_pInput->SendString(sTemp);
#endif

	delete pObj;
	}

//--------------------------------------------------------------------------------
CServerThread* CMonitorThread::GetServerThread()
	{
	return m_pServerThreadInfo->GetThreadPtr();
	}
