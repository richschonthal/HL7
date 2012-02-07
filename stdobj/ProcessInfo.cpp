//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// ProcessInfo.cpp: implementation of the CProcessInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ProcessInfo.h"
//#include <psapi.h>
#include <Handle.h>

#pragma comment(lib, "psapi.lib")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------
CProcessInfo::CProcessInfo(LPCTSTR pName)
	{
/*
	DWORD nId[10000];
	DWORD nSize = sizeof(nId);

	::EnumProcesses(nId, nSize, &nSize);

	CString sName(pName);
	sName.MakeUpper();

	for(int i = 0; i < (int) (nSize/sizeof(DWORD)); i++)
		{
		CString sTemp;
		GetProcessName(nId[i], sTemp);
		sTemp.MakeUpper();
		if(sTemp == sName)
			m_nProcId.Add(nId[i]);
		}
*/
	}

//--------------------------------------------------------------------------------
CProcessInfo::~CProcessInfo()
	{

	}

//--------------------------------------------------------------------------------
void CProcessInfo::GetProcessName(DWORD nProcId, CString& sName)
	{
	/*
	sName.Empty();

	// Get a handle to the process
	CHandle hProcess(OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ, false, nProcId));

	// Get the process name
	if(hProcess.IsValid())
		{
		HMODULE hMod;
		DWORD cbNeeded;

		if(EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
			{
			GetModuleBaseName(hProcess, hMod, sName.GetBuffer(1024), 1024);
			sName.ReleaseBuffer();
			}
		}
	*/
	}
