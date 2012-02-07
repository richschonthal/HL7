//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#include "stdafx.h"
#include "PACMonitor.h"

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

// CPACMonitorThread
//--------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CPACMonitorThread, CWinThread)
	//{{AFX_MSG_MAP(CPACMonitorThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_THREAD_MESSAGE(INIT, DoInit)
	ON_THREAD_MESSAGE(HL7_HL7MSG, DoMsgFromServer)
END_MESSAGE_MAP()

//--------------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(CPACMonitorThread, CWinThread)

// CPACMonitorThread construction
//--------------------------------------------------------------------------------
CPACMonitorThread::CPACMonitorThread()
		: m_pInputInit(NULL)
		, m_pInput(NULL)
		, m_nProcessId(0)
		, m_pParamKey(NULL)
	{
	}

//--------------------------------------------------------------------------------
CPACMonitorThread::~CPACMonitorThread()
	{
	delete m_pInput;
	delete m_pInputInit;
	delete m_pParamKey;
	}

// CPACMonitorThread initialization
//--------------------------------------------------------------------------------
BOOL CPACMonitorThread::InitInstance()
	{
	// this process doesnt do much at all until it receives
	// an INIT message
	return TRUE;
	}

//--------------------------------------------------------------------------------
void CPACMonitorThread::DoInit(CHL7InputDaemonInit* pInit, CServerThreadInfo* pServerThreadInfo)
	{
	GetIO()->FormatOutput(IOMASK_7, "CPACMonitorThread::DoInit entry (%s)", pInit->GetName());

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

	GetIO()->Output(IOMASK_8|IOMASK_CONST, "CPACMonitorThread::DoInit exit");
	}

//--------------------------------------------------------------------------------
int CPACMonitorThread::ExitInstance() 
	{
	MSG msg;
	while(::PeekMessage(&msg, NULL, HL7_HL7MSG_IN, HL7_HL7MSG_IN, PM_REMOVE))
		delete (CHL7Message*) msg.wParam;

	GetServerThread()->PostThreadMessage(HL7_GOODBYE, 0, 0);
	return CWinThread::ExitInstance();
	}

//--------------------------------------------------------------------------------
void CPACMonitorThread::DoMsgFromServer(LPTSTR pData, LPARAM)
	{
	GetIO()->Output(IOMASK_7|IOMASK_CONST, "CPACMonitorThread::DoMsgFromServer entry");

	if(! m_pInput->SendString(pData))
		GetIO()->Output(IOMASK_13|IOMASK_CONST, "Message not sent!");

	::Sleep(100);

	MSG msg;
	CTime ctStart(CTime::GetCurrentTime());

	// wait up to 15 seconds for a message to arrive
	for(;;)
		{
		if(::PeekMessage(&msg, NULL, HL7_HL7MSG_IN, HL7_HL7MSG_IN, PM_REMOVE) != 0)
			break;
		if( (CTime::GetCurrentTime() - ctStart).GetTotalSeconds() > 15)
			{
			msg.wParam = NULL;
			msg.lParam = 1;
			break;
			}

		::Sleep(20);
		}

	GetServerThread()->PostThreadMessage(HL7_HL7MSG, msg.wParam, msg.lParam);
	GetIO()->Output(IOMASK_15|IOMASK_CONST, "PAC msg done");
	// let the input thread know we're done
	delete[] pData;
	GetIO()->Output(IOMASK_8|IOMASK_CONST, "CPACMonitorThread::DoMsgFromServer exit");
	}

//--------------------------------------------------------------------------------
CServerThread* CPACMonitorThread::GetServerThread()
	{
	return m_pServerThreadInfo->GetThreadPtr();
	}
