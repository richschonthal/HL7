//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// ServiceStatus.cpp: implementation of the CServiceStatus class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ServiceStatus.h"
#include <winsvc.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------
CServiceStatus::CServiceStatus(LPCTSTR pServiceName)
	{
	SC_HANDLE scmanager = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(scmanager == NULL)
		{
		SetResultCode(errOS);
		SetOSResult(GetLastError());
		return;
		}

	DWORD nSize = 0;
	DWORD nReturned = 1;

	char* pData = NULL;
	for(DWORD nResume = 0;;)
		{
		if(! ::EnumServicesStatus(scmanager, SERVICE_WIN32, SERVICE_STATE_ALL,
				(ENUM_SERVICE_STATUS*) pData, nSize, &nSize, &nReturned, &nResume))
			{
			if(GetLastError() != ERROR_MORE_DATA)
				break;
			pData = new char[nSize];
			}
		else
			if(pData != NULL)
				break;
		}
	ENUM_SERVICE_STATUS* pPtr = (ENUM_SERVICE_STATUS*) pData;
	for(int i = 0; i < (signed) nReturned; i++)
		TRACE(_T("%s - %s\n"), pPtr[i].lpServiceName, pPtr[i].lpDisplayName);
	delete[] pPtr;
	}

//--------------------------------------------------------------------------------
CServiceStatus::~CServiceStatus()
	{

	}

//--------------------------------------------------------------------------------
bool CServiceStatus::IsInstalled()
	{
	return true;
	}

//--------------------------------------------------------------------------------
bool CServiceStatus::IsRunning()
	{
	return true;
	}

//--------------------------------------------------------------------------------
bool CServiceStatus::IsAuto()
	{
	return true;
	}

//--------------------------------------------------------------------------------
bool CServiceStatus::IsSystem()
	{
	return true;
	}

//--------------------------------------------------------------------------------
bool CServiceStatus::IsManual()
	{
	return true;
	}

