//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// Status.cpp: implementation of the CStatus class.
//
//////////////////////////////////////////////////////////////////////

#define _STATUS_CPP_

#include "stdafx.h"

#undef _STATUS_CPP_

#include "Status.h"
#include "StatusOutThread.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//CNTService* CStatus::m_pService = NULL;
CStdioFile* CStatus::m_pFile = NULL;

CStatus::eLevel CStatus::m_nTraceLevel =

#ifdef _DEBUG
	CStatus::S_TRACE_3;
#else
	CStatus::S_NO_TRACE;
#endif

int CStatus::m_nRefCount = 0;
CStatusOutThread* CStatus::m_pOutThread = NULL;
CStatusOutThreadArray* CStatus::m_pOutThreads = NULL;

//--------------------------------------------------------------------------------
CStatus::CStatus()
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//if(CStatus::m_pService != NULL)
		m_nRefCount++;
	}

//--------------------------------------------------------------------------------
CStatus::CStatus(eLevel nTraceLevel)
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_nRefCount++;

	if(nTraceLevel >= S_NO_TRACE && nTraceLevel <= S_TRACE_HIGHEST)
		m_nTraceLevel = nTraceLevel;
	}

//--------------------------------------------------------------------------------
CStatus::CStatus(LPCTSTR pFilename)
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_nRefCount++;

	SetLogFile(pFilename, (CResult*) this);
	}

//--------------------------------------------------------------------------------
CStatus::CStatus(LPCTSTR lpszMsg, WORD wEventType, DWORD dwEventID)
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_nRefCount++;

	eLevel nLevel = S_LOG;

	CString sEventType;
	switch(wEventType)
		{
		case EVENTLOG_ERROR_TYPE:
			sEventType = "EVENTLOG_ERROR_TYPE";
			nLevel = S_ERROR;
			break;
		case EVENTLOG_WARNING_TYPE:
			sEventType = "EVENTLOG_WARNING_TYPE";
			break;
		case EVENTLOG_INFORMATION_TYPE:
			sEventType = "EVENTLOG_INFORMATION_TYPE";
			break;
		case EVENTLOG_AUDIT_SUCCESS:
			sEventType = "EVENTLOG_AUDIT_SUCCESS";
			break;
		case EVENTLOG_AUDIT_FAILURE:
			sEventType = "EVENTLOG_AUDIT_FAILURE";
			break;
		default:
			sEventType = "Unknown";
			break;
		}

	sEventType = "Type: " + sEventType;

	CString sEventId;
	sEventId.Format("ID: %ld", dwEventID);

	CStatus(nLevel, lpszMsg, NULL, 0, sEventType, sEventId);
	}

//--------------------------------------------------------------------------------
CStatus::CStatus(eLevel nLevel, LPCTSTR pMsg, CResult* pRes, long nThreadId,
				LPCTSTR pObjName, LPCTSTR pFuncName)
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_nRefCount++;

	if(! IsDisplayed(nLevel))
		return;

	CStatusOutThreadMsgParam* pParam =
		new CStatusOutThreadMsgParam(nLevel, pMsg, pRes, nThreadId, pObjName, pFuncName, this);

	if(m_pOutThread == NULL)
		{
		m_pOutThread = (CStatusOutThread*) ::AfxBeginThread(RUNTIME_CLASS(CStatusOutThread));
		if(m_pOutThread == NULL)
			return;
		::Sleep(1000);
		}

	m_pOutThread->PostThreadMessage(CStatusOutThread::OUTPUT, (WPARAM)pParam, 0);

	if(m_pOutThreads != NULL)
		{
		for(int i = 0; i < m_pOutThreads->GetSize(); i++)
			{
			CStatusOutThreadMsgParam* pParam2 = new CStatusOutThreadMsgParam(*pParam);
			m_pOutThreads->GetAt(i)->PostThreadMessage(CStatusOutThread::OUTPUT, (WPARAM)pParam2, 0);
			}
		}
	}

//--------------------------------------------------------------------------------
CStatus::~CStatus()
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_nRefCount--;

	if(m_nRefCount == 0)
		{
		if(m_pFile != NULL)
			{
			m_pFile->Close();
			delete m_pFile;
			m_pFile = NULL;
			}

		if(m_pOutThread != NULL)
			{
			m_pOutThread->PostThreadMessage(CStatusOutThread::QUIT, 0, 0);
			::WaitForSingleObject(m_pOutThread->m_hThread, 1000);
			m_pOutThread = NULL;
			}

		if(m_pOutThreads != NULL)
			{
			for(int i = 0; i < m_pOutThreads->GetSize(); i++)
				{
				m_pOutThreads->GetAt(i)->PostThreadMessage(CStatusOutThread::QUIT, 0, 0);
				::WaitForSingleObject(m_pOutThreads->GetAt(i)->m_hThread, 1000);
				m_pOutThreads->RemoveAt(i);
				}
			}
		}
	}

//--------------------------------------------------------------------------------
int CStatus::Trace(CStatus::eLevel nLevel, LPCTSTR pFormat, ...)
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(! IsDisplayed(nLevel))
		return 0;

	va_list marker;

	va_start(marker, pFormat);
	char temp[2048];

	int nCount = vsprintf(temp, pFormat, marker);
	CStatus(nLevel, temp);

	return nCount;
	}

//--------------------------------------------------------------------------------
int CStatus::Log(WORD nEventType, LPCTSTR pFormat, ...)
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(! IsDisplayed())
		return 0;

	va_list marker;

	va_start(marker, pFormat);
	char temp[2048];

	int nCount = vsprintf(temp, pFormat, marker);
	CStatus(temp, nEventType);

	return nCount;
	}

//--------------------------------------------------------------------------------
int CStatus::Error(LPCTSTR pFormat, ...)
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(! IsDisplayed())
		return 0;

	va_list marker;

	va_start(marker, pFormat);
	char temp[2048];

	int nCount = vsprintf(temp, pFormat, marker);
	CStatus(temp, EVENTLOG_ERROR_TYPE);

	return nCount;
	}

//--------------------------------------------------------------------------------
bool CStatus::SetLogFile(LPCTSTR pFilename, CResult* pRes)
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_nRefCount++;

	if(m_pFile != NULL)
		{
		m_pFile->Close();
		delete m_pFile;
		m_pFile = NULL;
		}

	bool bRv = true;
	CFileException e;
	m_pFile = new CStdioFile;
	if(m_pFile->Open(pFilename,
			CFile::modeReadWrite|CFile::modeCreate|CFile::modeNoTruncate|
			CFile::shareDenyWrite|CFile::typeText, &e))
		{
		m_pFile->SeekToEnd();

		CString sLine('-', 160);
		CString sTemp;
		sTemp.Format("\n%s\nLog file started: %s\n%s\n%s\n%s\n", (LPCTSTR) sLine,
			(LPCTSTR) CTime::GetCurrentTime().Format("%c"), (LPCTSTR) sLine,
			"Type\tObjName\tFuncName\tThreadId\tMsg\tOS err id\tOS err msg",  (LPCTSTR) sLine);
		m_pFile->WriteString(sTemp);

/*		if(m_pService != NULL)
			if(m_pService->m_bDebug)
				printf("%s", (LPCTSTR) sTemp);
*/
		}
	else
		{
		if(pRes != NULL)
			{
			pRes->SetResultCode(CResult::errOS);
			pRes->SetOSResult(e.m_lOsError);
			}

		delete m_pFile;
		m_pFile = NULL;
		bRv = false;
		}

	return bRv;
	}

//--------------------------------------------------------------------------------
bool CStatus::SetTraceLevel(eLevel nTraceLevel)
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(nTraceLevel >= S_NO_TRACE && nTraceLevel <= S_TRACE_HIGHEST)
		m_nTraceLevel = nTraceLevel;

	return m_nTraceLevel == nTraceLevel;
	}

//--------------------------------------------------------------------------------
CStatus::eLevel CStatus::GetTraceLevel()
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return m_nTraceLevel;
	}

//--------------------------------------------------------------------------------
bool CStatus::IsDisplayed(eLevel nLvl)
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return (m_nTraceLevel != S_NO_OUTPUT) && 
		((nLvl > S_NO_TRACE && nLvl <= m_nTraceLevel) || nLvl > S_TRACE_HIGHEST);
	}

//--------------------------------------------------------------------------------
bool CStatus::IsDisplayed()
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return m_nTraceLevel != S_NO_OUTPUT;
	}

