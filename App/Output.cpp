//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "Output.h"
#include "HL7Message.h"

#include "Monitor.h"

#include "HL7SystemMsg.h"

#include "globals.h"
#include "defs.h"
#include "HL7InputDaemonInit.h"
#include "ProtocolObj.h"

#include <PACSDatabase.h>
#include <allocstring.h>
#include <registry.h>
#include <zProcess.h>

#include "OutputConfig.h"

#include "ServerThread.h"
#include "HL7PacOutThread.h"
#include "HL7DBMessagedef.h"
#include "HL7DBFieldDef.h"
#include "HL7DBMsgSegDef.h"
#include "HL7DBRuleDef.h"
#include "HL7DBSegmentDef.h"
#include "HL7DBVersionDef.h"
#include "HL7DBMsgSegFieldVal.h"
#include "HL7DBPM_RegisterProcess.h"
#include "HL7DBPM_UnregisterProcess.h"
#include "HL7DBPM_GetProcessAction.h"

#include "HL7MSAConfig.h"
#include "HL7DBServerConfig.h"

#include <IOSubSystem.h>
#include <System.h>
#include "HL7System.h"

#include "HL7NTService.h"

#ifdef _NO_SECURITY_SERVER
#pragma message("*** Warning! _NO_SECURITY_SERVER defined - no security checking in this version")
#endif

#ifdef _EVALUATION_VERSION
#pragma message("*** Evaluation version")
#endif

//--------------------------------------------------------------------------------
#define MAX_SERVER_COUNT 32
#define SERVICE_NAME "HL7Engine"

//--------------------------------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef _DEBUG
//--------------------------------------------------------------------------------
// global used for database error testing - when true, the CPACSDatabase will simulate a db error
bool g_bDBError = false;
#endif

static LPCTSTR g_pTraceNames[16] =
	{
	"Startup messages", "Timing info", "SQL statements", "Database listings", "Variables - general", "Variables - more detail",
		"Function entries", "Function exits", "HL7 messages", "Socket trace - general", "Socket trace - more detail",
		"File trace", "non-critical errors", "Actions - general", "Actions - more detail", "" 
	};

// a mutex and a lock to ensure only one HL7 is running
static CMutex* g_pMutex = NULL;
static CSingleLock* g_pLock = NULL;

//--------------------------------------------------------------------------------
class CStaticHolder : public CResult
	{
	public:
		CHL7DBFieldDef* m_pFieldDef;
		CHL7DBMessageDef* m_pMsgDef;
		CHL7DBRuleDef* m_pRuleDef;
		CHL7DBSegmentDef* m_pSegDef;
		CHL7DBVersionDef* m_pVerDef;
		CHL7DBServerConfig* m_pServerConfig;
		CHL7MSAConfig* m_pMSAConfig;

		CStaticHolder(COutputApp* that, const COutputConfig& config)
				: m_pFieldDef(NULL)
				, m_pMsgDef(NULL)
				, m_pRuleDef(NULL)
				, m_pSegDef(NULL)
				, m_pVerDef(NULL)
				, m_pServerConfig(NULL)
				, m_pMSAConfig(NULL)

			{
			CRegKey rkDbKey(g_rkRoot, config.GetDBKeyName());
			if(! rkDbKey.OpenKey())
				{
				GetIO()->Output(IOMASK_ERR|IOMASK_CONST, "Couldn't open registry key value 'Database'");
				SetResultCode(errOS);
				return;
				}

			// configure the statics
			CPACSDatabase::Init((const CRegKey&) rkDbKey);
			CPACSDatabase db;

			GetIO()->Output(IOMASK_1|IOMASK_CONST, "success opening database");

			// read in all the static lists from the database
			// do this here so as to maintain a refcount of at least 1
			// so the list will remain in memory
			// and to be thread safe
			// we want the lists to be safely in memory before
			// the server threads start reading them
			m_pFieldDef = new CHL7DBFieldDef;
			if(! m_pFieldDef->Init(db))
				{
				GetIO()->Output(IOMASK_ERR|IOMASK_CONST, "Error in table HL7_FieldDef");
				SetResultCode(errOS);
				return;
				}

			m_pMsgDef = new CHL7DBMessageDef;
			if(! m_pMsgDef->Init(db))
				{
				GetIO()->Output(IOMASK_ERR|IOMASK_CONST, "Error in table HL7_MessageDef");
				SetResultCode(errOS);
				return;
				}

			m_pRuleDef = new CHL7DBRuleDef;
			if(! m_pRuleDef->Init(db))
				{
				GetIO()->Output(IOMASK_ERR|IOMASK_CONST, "Error in table HL7_RuleDef");
				SetResultCode(errOS);
				return;
				}

			m_pSegDef = new CHL7DBSegmentDef;
			if(! m_pSegDef->Init(db))
				{
				GetIO()->Output(IOMASK_ERR|IOMASK_CONST, "Error in table HL7_SegmentDef");
				SetResultCode(errOS);
				return;
				}

			m_pVerDef = new CHL7DBVersionDef;
			if(! m_pVerDef->Init(db))
				{
				GetIO()->Output(IOMASK_ERR|IOMASK_CONST, "Error in table HL7_VersionDef");
				SetResultCode(errOS);
				return;
				}

			m_pServerConfig = new CHL7DBServerConfig;
			if(! m_pServerConfig->Init(db))
				{
				GetIO()->Output(IOMASK_ERR|IOMASK_CONST, "Error in table HL7_ServerConfig");
				SetResultCode(errOS);
				return;
				}

			m_pMSAConfig = new CHL7MSAConfig;
			if(! m_pMSAConfig->Init(db))
				{
				GetIO()->Output(IOMASK_ERR|IOMASK_CONST, "Error in table HL7_MSA");
				SetResultCode(errOS);
				return;
				}

			GetIO()->FormatOutput(IOMASK_1, "Record count (MessageDef)..= %d", m_pMsgDef->GetRecordCount());
			GetIO()->FormatOutput(IOMASK_1, "Record count (SegmentDef)..= %d", m_pSegDef->GetRecordCount());
			GetIO()->FormatOutput(IOMASK_1, "Record count (FieldDef)....= %d", m_pFieldDef->GetRecordCount());
			GetIO()->FormatOutput(IOMASK_1, "Record count (RuleDef).....= %d", m_pRuleDef->GetRecordCount());
			GetIO()->FormatOutput(IOMASK_1, "Record count (VersionDef)..= %d", m_pVerDef->GetRecordCount());

			SetResultCode(0);
			SetOSResult(0);
			}

		~CStaticHolder()
			{
			delete m_pFieldDef;
			delete m_pMsgDef;
			delete m_pRuleDef;
			delete m_pSegDef;
			delete m_pVerDef;
			delete m_pServerConfig;
			delete m_pMSAConfig;
			}
	};

//--------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(COutputApp, CWinApp)
	//{{AFX_MSG_MAP(COutputApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
	ON_THREAD_MESSAGE(INIT, DoInit)

	ON_THREAD_MESSAGE(CNTService::MSG_PAUSE, DoPause)
	ON_THREAD_MESSAGE(CNTService::MSG_CONTINUE, DoContinue)
	ON_THREAD_MESSAGE(CNTService::MSG_STOP, DoStop)
	ON_THREAD_MESSAGE(CNTService::MSG_SHUTDOWN, DoShutdown)
END_MESSAGE_MAP()

//--------------------------------------------------------------------------------
COutputApp::COutputApp()
		: m_evtStop(false, true)
		, m_evtPause(false, true)
		, m_pName(NULL)
		, m_pStatics(NULL)
		, m_pProcMonThread(NULL)
		, m_pMainLoopThread(NULL)
		, m_bPaused(false)
		, m_bExit(false)
		, m_pMonitors(NULL)
		, m_pSecurity(NULL)
		, m_pSecurityThread(NULL)

	{
	m_pSystem = new CHL7System;
	m_pSystem->SetState(STATE_PAUSE);
	m_pSystem->WaitForStateSync();
	}

//--------------------------------------------------------------------------------
COutputApp::~COutputApp()
	{
	delete GetNTService();
	}

//--------------------------------------------------------------------------------
CHL7System* COutputApp::GetSystem()
	{
	return m_pSystem;
	}

//--------------------------------------------------------------------------------
void COutputApp::DoInit(WPARAM, LPARAM)
	{
	g_pMutex = new CMutex(FALSE, "HL7ServerSingletonMutex");
	g_pLock = new CSingleLock(g_pMutex, FALSE);

	if(! g_pLock->Lock(10))
		{
		GetIO()->Output(IOMASK_ERR|IOMASK_CONST, "HL7 Engine already running");
		PostQuitMessage(-1);
		return;
		}

	// socket init
	if(!AfxSocketInit())
		{
		GetIO()->Output(IOMASK_ERR|IOMASK_CONST, "Error opening WinSock");
		PostQuitMessage(-1);
		return;
		}

	// create registry keys/values if they don't already exist
	InitRegistry();

	COutputConfig config;

	GetIO()->SetTraceLevel(config.GetTraceLevel());
	if(config.IsDailyLog())
		GetIO()->CreateLogFile(config.GenerateLogfileName());
	else
		GetIO()->CreateLogFile(config.GetLogFile());

	GetIO()->Output(IOMASK_7|IOMASK_CONST, "DoInit entry");

#ifndef _NO_SECURITY_SERVER
	m_pSecurity = new CSecurityConnection(config.GetSecServIP(), config.GetSecServPort(),
		config.GetSecServBkupIP(), config.GetSecServBkupPort());

	if(m_pSecurity->GetNewCertificate(1, SSTokenId::HL7_3) == -1)
		{
		delete m_pSecurity;
		m_pSecurity = NULL;
		GetIO()->Output(IOMASK_ERR|IOMASK_CONST, "Error connecting to SS or no token available");
		PostQuitMessage(-1);
		return;
		}

	m_pSecurityThread = ::AfxBeginThread((AFX_THREADPROC) SecurityThread, this);
	::Sleep(200);
	m_pSecurity->SetTarget(m_pSecurityThread, HL7_SECURITY_SERVER);
#endif

	if(GetIO()->IsDisplayed(IOMASK_1))
		{
		DWORD nMask = 1;
		DWORD nTrace = config.GetTraceLevel();

		for(int i = 0; i < 15; i++)
			{
			if(nTrace & nMask)
				GetIO()->FormatOutput(IOMASK_1, "Trace: %s", g_pTraceNames[i]);
			nMask <<= 1;
			}
		}

	for(;;)
		{
		TRY
			{
			// open database and load lists
			m_pStatics = new CStaticHolder(this, config);
			}
		CATCH(CDBException, e)
			{
			delete m_pStatics;
			m_pStatics = NULL;
			GetIO()->FormatOutput(IOMASK_ERR, "Database exception occured during init (%s)",
				(LPCTSTR) e->m_strError);
			if(::WaitForSingleObject(m_evtStop, 20000) == WAIT_OBJECT_0)
				return;

			continue;
			}
		CATCH_ALL(e)
			{
			GetIO()->Output(IOMASK_ERR|IOMASK_CONST, "Unknown exception occured during init (statics)");
			PostQuitMessage(-1);
			return;
			}
		END_CATCH_ALL
		break;
		}

	// report to the SCM that we're about to start
	ReportStatus(SERVICE_START_PENDING);

	// let the process monitor know we're here
	m_nProcessId = RegisterProcess();

	// startup the process monitor thread
	m_pProcMonThread = ::AfxBeginThread((AFX_THREADPROC) ManageProcess, this);

	// start up monitor modules
	StartMonitors();

	// report SERVICE_RUNNING to the SCM
	ReportStatus(SERVICE_RUNNING);

	// crank up the main loop and get outta here
	m_pMainLoopThread = ::AfxBeginThread((AFX_THREADPROC) MainLoopThread, this);

	GetIO()->Output(IOMASK_8|IOMASK_CONST, "DoInit exits");
	}

//--------------------------------------------------------------------------------
bool COutputApp::DoCommandLine()
	{
#ifndef _EVALUATION_VERSION
	CCmdLine cmdLine;

	int nIndex;
	if(cmdLine.HasParam('D', nIndex) || cmdLine.HasParam("-debug", nIndex))
		return false;

	if(cmdLine.HasParam('I', nIndex) || cmdLine.HasParam("-install", nIndex))
		{
		GetNTService()->InstallService();
		return true;
		}

	if(cmdLine.HasParam('R', nIndex) || cmdLine.HasParam("-remove", nIndex))
		{
		GetNTService()->RemoveService();
		return true;
		}

	if(cmdLine.HasParam('E', nIndex) || cmdLine.HasParam("-end", nIndex) || cmdLine.HasParam("-stop", nIndex))
		{
		GetNTService()->EndService();
		return true;
		}

	if(cmdLine.HasParam('S', nIndex) || cmdLine.HasParam("-start", nIndex))
		{
		GetNTService()->StartupService();
		return true;
		}

	GetNTService()->StartServiceThread();
#endif

	return false;
	}

//--------------------------------------------------------------------------------
BOOL COutputApp::InitInstance()
	{
	// create an invisible window
	m_pMainWnd = new CWnd;
	m_pMainWnd->CreateEx(0, AfxRegisterWndClass(NULL), NULL, 0, 0, 0, 0, 0, NULL, NULL);

	m_pSystem->SetState(STATE_RUN);
	m_pSystem->WaitForStateSync();

	GetIO()->SetTraceLevel(1);
	GetIO()->Output(1|IOMASK_CONST, "initializing HL7...");
	::Sleep(100);
	GetIO()->SetTraceLevel(0);
	
	if(DoCommandLine())
		{
		delete m_pMainWnd;
		m_pMainWnd = NULL;
		return FALSE;
		}

	PostThreadMessage(INIT, 0, 0);

	return TRUE;
	}

//--------------------------------------------------------------------------------
int COutputApp::ExitInstance()
	{
	GetIO()->Output(IOMASK_7|IOMASK_CONST, "ExitInstance entry");

	// signal to worker threads to exit
	m_bExit = true;
	m_evtStop.SetEvent();

	if(m_pProcMonThread != NULL)
		::WaitForSingleObject(m_pProcMonThread->m_hThread, 60000);

	if(m_pMainLoopThread != NULL)
		::WaitForSingleObject(m_pMainLoopThread->m_hThread, 60000);

	if(m_pSecurityThread != NULL)
		{
		m_pSecurityThread->PostThreadMessage(WM_QUIT, 0, 0);
		::WaitForSingleObject(m_pSecurityThread->m_hThread, 60000);
		}

	// in case of future platform changes
	ASSERT(sizeof(HANDLE) == sizeof(void*));

	// wait for monitor apps to exit

	CDWordArray procIds;
	// save the proc id's of all the server threads so we can unregister them later
	for(int i = 0; i < m_threads.GetSize(); i++)
		{
		if(m_threads[i] != NULL)
			if(m_threads[i]->m_pThread != NULL)
				if(m_threads[i]->m_pThread->m_pInitInfo != NULL)
					procIds.Add(m_threads[i]->m_pThread->m_pInitInfo->GetProcessId());
		}

	// store monitor thread handles so we can wait on them to exit
	CPtrArray threads;

	if(m_pMonitors != NULL)
		{
		int nSize = m_pMonitors->GetCount();
		threads.SetSize(nSize);

		int i = 0;
		for(POSITION pos = m_pMonitors->GetHeadPosition(); pos != NULL; )
			{
			CWinThread* pThread = m_pMonitors->GetNext(pos);

			if(::PostThreadMessage(pThread->m_nThreadID, WM_QUIT, 0, 0))
				threads[i++] = pThread->m_hThread;
			}

		// wait for them to exit
		::WaitForMultipleObjects(threads.GetSize(), threads.GetData(), TRUE, INFINITE);
		}

	for(i = 0; i < procIds.GetSize(); i++)
		UnregisterProcess(procIds[i]);

	if(m_nProcessId > 0)
		UnregisterProcess(m_nProcessId);

	GetIO()->Output(IOMASK_8|IOMASK_CONST, "ExitInstance last message before I/O system shutdown");

	m_pSystem->SetState(STATE_EXIT);
	m_pSystem->WaitForStateSync();

	delete m_pMonitors;
	delete m_pStatics;

	delete m_pSystem;
	delete[] m_pName;

	if(g_pLock != NULL)
		if(g_pLock->IsLocked())
			g_pLock->Unlock();

	delete g_pLock;
	delete g_pMutex;

	delete m_pMainWnd;
	m_pMainWnd = NULL;

	CSharedDB db(false);
	db.DeleteConnection();

	GetNTService()->ReportStatus(SERVICE_STOPPED);

	// bu-bye
	return CWinApp::ExitInstance();
	}

//--------------------------------------------------------------------------------
void COutputApp::DoPause(WPARAM, LPARAM)
	{
	}

//--------------------------------------------------------------------------------
void COutputApp::DoContinue(WPARAM, LPARAM)
	{
	}

//--------------------------------------------------------------------------------
void COutputApp::DoStop(WPARAM, LPARAM)
	{
	PostThreadMessage(WM_QUIT, 0, 0);
	}

//--------------------------------------------------------------------------------
void COutputApp::DoShutdown(WPARAM, LPARAM)
	{
	PostThreadMessage(WM_QUIT, 0, 0);
	}

//--------------------------------------------------------------------------------
void COutputApp::StartMonitors()
	{
	::AfxBeginThread((AFX_THREADPROC) StartMonitorsThread, (LPVOID) this);
	}

//--------------------------------------------------------------------------------
void COutputApp::ReportStatus(DWORD nStatus, DWORD nTime)
	{
	GetNTService()->ReportStatus(nStatus, nTime);
	}

// translates action codes from the process monitor database into
// CProtocolObj commands
//--------------------------------------------------------------------------------
bool COutputApp::GetProcessActionToken(int nAction, CProtocolObj& obj)
	{
	switch(nAction)
		{
		default:
		case CHL7DBPM_GetProcessAction::ACTION_NONE:
			return false;

		case CHL7DBPM_GetProcessAction::ACTION_RESTART:
			obj.SetCommand(CProtocolObj::CONTINUE);
			break;

		case CHL7DBPM_GetProcessAction::ACTION_EMERGENCY:
		case CHL7DBPM_GetProcessAction::ACTION_TERMINATE:
		case CHL7DBPM_GetProcessAction::ACTION_SUSPEND:
			obj.SetCommand(CProtocolObj::PAUSE);
			break;
		}

	return true;
	}

//--------------------------------------------------------------------------------
LPCTSTR COutputApp::GetProcessName()
	{
	return "Engine";
	}

#if defined(DB_PRIVATE)
// register with the process monitor
//--------------------------------------------------------------------------------
long COutputApp::RegisterProcess(LPCTSTR pParam)
	{
	CString sName;
	sName.Format("HL7%s", (pParam == NULL) ? GetProcessName() : pParam);
	CPACSDatabase* pDB = NULL;
	long nRv = -1;

	for(;;)
		{
		TRY
			{
			if(pDB == NULL)
				pDB = new CPACSDatabase;
			CHL7DBPM_RegisterProcess reg(pDB->GetDBPtr(), sName);
			TRACE("procnum=%ld\n", reg.m_nProcNum);
			nRv = reg.m_nProcNum;
			delete pDB;
			}
		CATCH_ALL(e)
			{
			GetIO()->Output(IOMASK_ERR|IOMASK_CONST, "Database exception occured during RegisterProcess");
			delete pDB;
			pDB = NULL;
			// is stop signaled?
			if(::WaitForSingleObject(m_evtStop, 1000) == WAIT_OBJECT_0)
				return -1;
			continue;
			}
		END_CATCH_ALL

		return nRv;
		}
	}

// register with the process monitor
//--------------------------------------------------------------------------------
long COutputApp::UnregisterProcess(long nProcId)
	{
	CPACSDatabase* pDB = NULL;
	long nRv = -1;

	TRY
		{
		if(pDB == NULL)
			pDB = new CPACSDatabase;
		CHL7DBPM_UnregisterProcess reg(pDB->GetDBPtr(), nProcId);
		delete pDB;
		nRv = 0;
		}
	CATCH_ALL(e)
		{
		GetIO()->Output(IOMASK_ERR|IOMASK_CONST, "Database exception occured during UnregisterProcess");
		delete pDB;
		pDB = NULL;
		}
	END_CATCH_ALL

	return nRv;
	}

//--------------------------------------------------------------------------------
#elif defined(DB_SHARE)

// register with the process monitor
//--------------------------------------------------------------------------------
long COutputApp::RegisterProcess(LPCTSTR pParam)
	{
	CString sName;
	sName.Format("HL7%s", (pParam == NULL) ? GetProcessName() : pParam);
	long nRv = -1;

	for(;;)
		{
		TRY
			{
			CSharedDB db;
			CPACSDatabase* pDB = db.GetDB();
			if(pDB == NULL)
				throw new CException;
			CHL7DBPM_RegisterProcess reg(pDB->GetDBPtr(), sName);
			TRACE("procnum=%ld\n", reg.m_nProcNum);
			nRv = reg.m_nProcNum;
			}
		CATCH_ALL(e)
			{
			CSharedDB db(false);
			db.DeleteConnection();
			GetIO()->Output(IOMASK_ERR|IOMASK_CONST, "Database exception occured during RegisterProcess");
			// is stop signaled?
			if(::WaitForSingleObject(m_evtStop, 1000) == WAIT_OBJECT_0)
				return -1;
			continue;
			}
		END_CATCH_ALL

		return nRv;
		}
	}

// register with the process monitor
//--------------------------------------------------------------------------------
long COutputApp::UnregisterProcess(long nProcId)
	{
	CPACSDatabase* pDB = NULL;
	long nRv = -1;

	TRY
		{
		CSharedDB db;
		CPACSDatabase* pDB = db.GetDB();
		if(pDB == NULL)
			throw new CException;
		CHL7DBPM_UnregisterProcess reg(pDB->GetDBPtr(), nProcId);
		nRv = 0;
		}
	CATCH_ALL(e)
		{
		CSharedDB db(false);
		db.DeleteConnection();
		GetIO()->Output(IOMASK_ERR|IOMASK_CONST, "Database exception occured during UnregisterProcess");
		}
	END_CATCH_ALL

	return nRv;
	}

#endif // DB_SHARE

//--------------------------------------------------------------------------------
bool COutputApp::CreateRegKey(CRegKey& rkNew)
	{
	// create and/or open the root
	if(! rkNew.CreateKey())
		{
		GetIO()->FormatOutput(IOMASK_ERR, "Error creating and/or opening registry key: %s", rkNew.GetKeyName());
		return false;
		}

	return true;
	}

//--------------------------------------------------------------------------------
bool COutputApp::InitRegValue(CRegKey& rk, LPCTSTR pName, LPCTSTR pDefault)
	{
	CRegValueSz rvTemp(rk, pName);
	CString sTemp;
	if(! rvTemp.ReadValue(sTemp))
		return rvTemp.WriteValue(pDefault) == 1;
	return true;
	}

//--------------------------------------------------------------------------------
bool COutputApp::InitRegValue(CRegKey& rk, LPCTSTR pName, DWORD dwDefault)
	{
	CRegValueDWord rvTemp(rk, pName);
	DWORD dwTemp;
	if(! rvTemp.ReadValue(&dwTemp))
		return rvTemp.WriteValue(dwDefault) == 1;
	return true;
	}

//--------------------------------------------------------------------------------
bool COutputApp::InitRegistry()
	{
	// create and/or open the root
	if(! g_rkRoot.CreateKey())
		{
		GetIO()->FormatOutput(IOMASK_ERR, "Error creating and/or opening registry key: %s", g_rkRoot.GetKeyName());
		return false;
		}

	COutputConfig config;

	InitRegValue(g_rkRoot, "DBKeyName", "Database");
	InitRegValue(g_rkRoot, "MonKeyName", REG_MONITOR_KEYNAME);

	CRegKey rkDB(g_rkRoot, config.GetDBKeyName());
	if(! CreateRegKey(rkDB))
		return false;
	InitRegValue(rkDB, "dbtype", "");
	InitRegValue(rkDB, "ConnectString", "");
	InitRegValue(rkDB, "TableAccount", "");
	InitRegValue(rkDB, "ProcedureAccount", "");
	return true;
	}
