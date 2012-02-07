//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// ExecuteApp.cpp: implementation of the CExecuteApp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "result.h"
#include "ExecuteApp.h"
#include "DebugPrintf.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------
CExecuteApp::CExecuteApp(LPCTSTR pExe, LPCTSTR pParams, LPCTSTR pPath, DWORD nWaitTime)
		: m_hProcess(NULL)
		, m_nExitCode(0xffffffff)
	{
	SHELLEXECUTEINFO info;
	::ZeroMemory(&info, sizeof(SHELLEXECUTEINFO));
	info.cbSize = sizeof(SHELLEXECUTEINFO);
	info.fMask = SEE_MASK_FLAG_NO_UI|SEE_MASK_NOCLOSEPROCESS;
	info.lpFile = pExe;
	info.lpParameters = pParams;
	info.lpDirectory = pPath;
	info.nShow = SW_HIDE;

	SetResultCode(errOS);

	if(! ::ShellExecuteEx(&info))
		{
		SetOSResult(GetLastError());
		DebugPrintf(_T("ShellExecuteEx(%s,'%s','%s') error=%ld\n"), info.lpFile, pParams, pPath, GetOSResult());
		return;
		}

	m_hProcess = info.hProcess;

	if(nWaitTime != 0)
		{
		if(::WaitForSingleObject(m_hProcess, nWaitTime) != WAIT_OBJECT_0)
			{
			DebugPrintf(_T("wait on info.hProcess failed\n"));
			return;
			}

		BOOL bRunOk = ::GetExitCodeProcess(m_hProcess, &m_nExitCode);

		if(!bRunOk || m_nExitCode != 0)
			{
			DebugPrintf(_T("%s exits with %ld\n"), info.lpFile, m_nExitCode);
			return;
			}
		}
	}

//--------------------------------------------------------------------------------
CExecuteApp::~CExecuteApp()
	{

	}
