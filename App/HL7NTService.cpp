//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "HL7NTService.h"
#include "globals.h"

//--------------------------------------------------------------------------------
CHL7NTService::CHL7NTService()
		: CNTService(GetApp())
	{
	}

//--------------------------------------------------------------------------------
LPCTSTR CHL7NTService::GetServiceName()
	{
	return "HL7Engine";
	}

//--------------------------------------------------------------------------------
LPCTSTR CHL7NTService::GetDisplayName()
	{
	return "HL7Engine";
	}

#ifdef _EVALUATION_VERSION
DWORD CHL7NTService::GetControlsAccepted() {return NULL; }
DWORD CHL7NTService::GetServiceCheckPoint() {return NULL; }
DWORD CHL7NTService::GetDesiredAccess() {return NULL; }
DWORD CHL7NTService::GetServiceType() {return NULL; }
DWORD CHL7NTService::GetStartType() {return NULL; }
DWORD CHL7NTService::GetErrorControl() {return NULL; }
LPCTSTR CHL7NTService::GetLoadOrderGroup() {return NULL; }
LPCTSTR CHL7NTService::GetDependencies() {return NULL; }
LPCTSTR CHL7NTService::GetUserName() {return NULL; }
LPCTSTR CHL7NTService::GetPassword() {return NULL; }

void CHL7NTService::Stop() {}
void CHL7NTService::Pause() {}
void CHL7NTService::Continue() {}
void CHL7NTService::Shutdown() {}
BOOL CHL7NTService::StartDispatcher() {return FALSE;}
BOOL CHL7NTService::InstallService() {return FALSE;}
BOOL CHL7NTService::RemoveService() {return FALSE;}
BOOL CHL7NTService::EndService() {return FALSE;}
BOOL CHL7NTService::StartupService() {return FALSE;}

BOOL CHL7NTService::StartServiceThread() {return FALSE;}

DWORD CHL7NTService::GetStopPendingTime(){ return NULL; }
DWORD CHL7NTService::GetPausePendingTime(){ return NULL; }
DWORD CHL7NTService::GetContinuePendingTime(){ return NULL; }

BOOL CHL7NTService::ProcessCommandLine() { return FALSE; }
BOOL CHL7NTService::ProcessCommandLine(LPCTSTR) { return FALSE; }

void CHL7NTService::RegisterApplicationLog(LPCTSTR,DWORD) {}
void CHL7NTService::DeregisterApplicationLog() {}

LPTSTR CHL7NTService::GetLastErrorText(LPTSTR, DWORD) {return NULL;}
BOOL CHL7NTService::ReportStatus(DWORD, DWORD, DWORD) {return FALSE;}
void CHL7NTService::AddToMessageLog(LPCTSTR, WORD, DWORD) {}
#endif
