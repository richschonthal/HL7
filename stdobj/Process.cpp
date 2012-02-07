//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// Process.cpp: implementation of the CProcess class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zProcess.h"

// Construction/Destruction

//--------------------------------------------------------------------------------
CProcess::CProcess()
	{

	}

//--------------------------------------------------------------------------------
CProcess::~CProcess()
	{
	}

//--------------------------------------------------------------------------------
bool CProcess::Create(LPCTSTR pName, LPTSTR pCmdLine, LPCTSTR pCurDir,
		DWORD dwCreationFlags,
		bool bInheritHandles,
		LPSTARTUPINFO lpStartupInfo,
		LPSECURITY_ATTRIBUTES lpProcessAttributes,
		LPSECURITY_ATTRIBUTES lpThreadAttributes,
		LPVOID lpEnvironment)
	{
	STARTUPINFO si;
	if(lpStartupInfo == NULL)
		{
		::ZeroMemory((void*) &si, sizeof(si));
		si.cb = sizeof(si);
		lpStartupInfo = &si;
		}

	bool bRv = ::CreateProcess(pName, pCmdLine, lpProcessAttributes, lpThreadAttributes,
		(BOOL) bInheritHandles, dwCreationFlags, lpEnvironment, pCurDir,
		lpStartupInfo, &m_procInfo) != 0;

	if(! bRv)
		{
		SetResultCode(errOS);
		SetOSResult(::GetLastError());
		}

	return bRv;
	}
