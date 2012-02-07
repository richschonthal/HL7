//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// StatusOutThread.cpp : implementation file
//

#include "stdafx.h"
#include "StatusOutThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//--------------------------------------------------------------------------------
CStatusOutThreadMsgParam::CStatusOutThreadMsgParam(CStatus::eLevel nLevel, LPCTSTR pMsg, CResult* pRes, long nThreadId,
			LPCTSTR pObjName, LPCTSTR pFuncName, CStatus* pObj)
	{
	m_nSize = sizeof(CStatusOutThreadMsgParam);
	m_nLevel = nLevel;
	m_nThreadId = nThreadId;
	m_pStatusObj = pObj;

	if(pObjName != NULL)
		{
		m_pObjName = new char[strlen(pObjName) + 1];
		strcpy(m_pObjName, pObjName);
		}
	else
		m_pObjName = NULL;

	if(pFuncName != NULL)
		{
		m_pFuncName = new char[strlen(pFuncName) + 1];
		strcpy(m_pFuncName, pFuncName);
		}
	else
		m_pFuncName = NULL;

	if(pMsg != NULL)
		{
		m_pMsg = new char[strlen(pMsg) + 1];
		strcpy(m_pMsg, pMsg);
		}
	else
		m_pMsg = NULL;

	if(pRes != NULL)
		{
		m_pRes = new CResult;
		*m_pRes = *pRes;
		}
	else
		m_pRes = NULL;
	}

//--------------------------------------------------------------------------------
CStatusOutThreadMsgParam::CStatusOutThreadMsgParam(const CStatusOutThreadMsgParam& rv)
	{
	m_nSize = sizeof(CStatusOutThreadMsgParam);
	m_nLevel = rv.m_nLevel;
	m_nThreadId = rv.m_nThreadId;
	m_pStatusObj = rv.m_pStatusObj;

	if(rv.m_pObjName != NULL)
		{
		m_pObjName = new char[strlen(rv.m_pObjName) + 1];
		strcpy(m_pObjName, rv.m_pObjName);
		}
	else
		m_pObjName = NULL;

	if(rv.m_pFuncName != NULL)
		{
		m_pFuncName = new char[strlen(rv.m_pFuncName) + 1];
		strcpy(m_pFuncName, rv.m_pFuncName);
		}
	else
		m_pFuncName = NULL;

	if(rv.m_pMsg != NULL)
		{
		m_pMsg = new char[strlen(rv.m_pMsg) + 1];
		strcpy(m_pMsg, rv.m_pMsg);
		}
	else
		m_pMsg = NULL;

	if(rv.m_pRes != NULL)
		{
		m_pRes = new CResult;
		*m_pRes = *rv.m_pRes;
		}
	else
		m_pRes = NULL;
	}

//--------------------------------------------------------------------------------
CStatusOutThreadMsgParam::~CStatusOutThreadMsgParam()
	{
	delete[] m_pObjName;
	delete[] m_pFuncName;
	delete[] m_pMsg;
	delete m_pRes;
	}

/////////////////////////////////////////////////////////////////////////////
// CStatusOutThread

IMPLEMENT_DYNCREATE(CStatusOutThread, CWinThread)

//--------------------------------------------------------------------------------
CStatusOutThread::CStatusOutThread()
	{
	}

//--------------------------------------------------------------------------------
CStatusOutThread::~CStatusOutThread()
	{
	}

//--------------------------------------------------------------------------------
BOOL CStatusOutThread::InitInstance()
	{
	// TODO:  perform and per-thread initialization here
	return TRUE;
	}

//--------------------------------------------------------------------------------
int CStatusOutThread::ExitInstance()
	{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
	}

//--------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CStatusOutThread, CWinThread)
	//{{AFX_MSG_MAP(CStatusOutThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_THREAD_MESSAGE(OUTPUT, DoOutput)
	ON_THREAD_MESSAGE(QUIT, Quit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatusOutThread message handlers

//--------------------------------------------------------------------------------
UINT CStatusOutThread::DoOutput(WPARAM pWParam, LPARAM)
	{
	if(pWParam == NULL)
		{
		PostThreadMessage(OUTPUT, 0, 0);
		return 0;
		}

	const CStatusOutThreadMsgParam* pParam = (CStatusOutThreadMsgParam*) pWParam;
	if(pParam->m_nSize != sizeof(CStatusOutThreadMsgParam))
		{
		PostThreadMessage(OUTPUT, 0, 0);
		return 0;
		}

	CString sType;

	switch(pParam->m_nLevel)
		{
		case CStatus::S_ERROR:
			sType = "Error";
			break;
		case CStatus::S_LOG:
			sType = "Log";
			break;
		default:
			sType.Format("Trace_%d", pParam->m_nLevel);
			break;
		}

	CString sErrMsg;
	if(pParam->m_pRes != NULL)
		pParam->m_pRes->GetErrorMessage(sErrMsg);

	LPCTSTR pErrMsg = (LPCTSTR) sErrMsg;

	CString sThreadId;
	if(pParam->m_nThreadId!=0)
		sThreadId.Format("%ld", pParam->m_nThreadId);

	CString sResult;
	if(pParam->m_pRes != NULL)
		{
		CString sTemp;
		pParam->m_pRes->GetErrorMessage(sTemp);
		sResult.Format("%ld (%s)", pParam->m_pRes->GetOSResult(), (LPCTSTR) sTemp);
		}

	CString sTemp;
	sTemp.Format("%s\t%s\t%s\t%s\t%s\t%s\t%s\n",
		(LPCTSTR) sType,
		pParam->m_pObjName == NULL ? "n/a" : pParam->m_pObjName,
		pParam->m_pFuncName == NULL ? "n/a" : pParam->m_pFuncName,
		(LPCTSTR) sThreadId,
		pParam->m_pMsg,
		(LPCTSTR) sResult,
		pErrMsg
		);

	OutputDebugString((LPCTSTR) sTemp);

	if(pParam->m_pStatusObj->m_pFile != NULL)
		{
		pParam->m_pStatusObj->m_pFile->WriteString(sTemp);
		pParam->m_pStatusObj->m_pFile->Flush();
		}

	delete (CStatusOutThreadMsgParam*) pWParam;

	PostThreadMessage(OUTPUT, 0, 0);

	return 0;
	}

//--------------------------------------------------------------------------------
UINT CStatusOutThread::Quit(WPARAM, LPARAM)
	{
	::AfxEndThread(0);
	return 0;
	}
