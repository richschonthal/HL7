//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "NTService.h"
#include <CmdLine.h>
#include <allocstring.h>
#include <status.h>
#include <PlatformInfo.h>
#include <DebugPrintf.h>
#include <CmdLine.h>

// define this when building a DLL service
//#define AFX_MANAGE_STATE_IF_DLL AFX_MANAGE_STATE(AfxGetStaticModuleState());
#define AFX_MANAGE_STATE_IF_DLL

//--------------------------------------------------------------------------------
static CNTService* g_this = 0;
static LPCTSTR g_pAppRegKey = TEXT("SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\");

//--------------------------------------------------------------------------------
CNTService::CNTService(CWinApp* pMainApp)
		: m_dwErr(0)
		, m_sshStatusHandle(0)
		, m_pUserSID(0)
		, m_dwTagID(0)
		, m_pMainApp(pMainApp)
	{
	AFX_MANAGE_STATE_IF_DLL

	CPlatformInfo winVer;

	// must be running on NT
	if(! winVer.IsWinNT())
		return;

	g_this = this;

	// SERVICE_STATUS members that rarely change
	m_ssStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	m_ssStatus.dwServiceSpecificExitCode = 0;

	// Get security information of current user
	BYTE nSecurityIdBuffer[4096];
	DWORD dwSizeSecurityIdBuffer = sizeof(nSecurityIdBuffer);

	TCHAR sUserName[256];
	DWORD dwSizeUserName  =  255;

	TCHAR sDomainName[256];
	DWORD dwSizeDomainName = 255;

	SID_NAME_USE sidTypeSecurityId;

	::ZeroMemory(sUserName, sizeof(sUserName));
	::ZeroMemory(sDomainName, sizeof(sDomainName));
	::ZeroMemory(nSecurityIdBuffer, dwSizeSecurityIdBuffer);

	::GetUserName(sUserName, &dwSizeUserName);

	if(::LookupAccountName(0, sUserName, &nSecurityIdBuffer, &dwSizeSecurityIdBuffer, sDomainName, &dwSizeDomainName,
				&sidTypeSecurityId))
		{
		if(::IsValidSid( PSID(nSecurityIdBuffer)))
			{
			DWORD dwSidLen = ::GetLengthSid(PSID(nSecurityIdBuffer));
			m_pUserSID = PSID(new BYTE [dwSidLen]);
			::CopySid(dwSidLen, m_pUserSID, nSecurityIdBuffer);
			_ASSERTE(::EqualSid(m_pUserSID, nSecurityIdBuffer));
			}
		}
	}

//--------------------------------------------------------------------------------
CNTService::~CNTService()
	{
	AFX_MANAGE_STATE_IF_DLL

	ReportStatus(SERVICE_STOPPED);

	g_this = NULL;
	delete[] LPBYTE(m_pUserSID);
	}

//--------------------------------------------------------------------------------
BOOL CNTService::ProcessCommandLine()
	{
	return ProcessCommandLine(::GetCommandLine());
	}

//--------------------------------------------------------------------------------
BOOL CNTService::ProcessCommandLine(LPCTSTR pCmdLine)
	{
	AFX_MANAGE_STATE_IF_DLL

	CCmdLine cmdLine(pCmdLine);
	int nIndex;
	if(cmdLine.HasParam('D', nIndex) || cmdLine.HasParam(_T("-debug"), nIndex))
		return FALSE;

	if(cmdLine.HasParam('I', nIndex) || cmdLine.HasParam(_T("-install"), nIndex))
		{
		InstallService();
		return TRUE;
		}

	if(cmdLine.HasParam('R', nIndex) || cmdLine.HasParam(_T("-remove"), nIndex))
		{
		RemoveService();
		return TRUE;
		}

	if(cmdLine.HasParam('E', nIndex) || cmdLine.HasParam(_T("-end"), nIndex) || cmdLine.HasParam(_T("-stop"), nIndex))
		{
		EndService();
		return TRUE;
		}

	if(cmdLine.HasParam('S', nIndex) || cmdLine.HasParam(_T("-start"), nIndex))
		{
		StartupService();
		return TRUE;
		}

	return FALSE;
	}

//--------------------------------------------------------------------------------
BOOL CNTService::StartDispatcher()
	{
	AFX_MANAGE_STATE_IF_DLL

	// Default implementation creates a single threaded service.
	// Override this method and provide more table entries for
	// a multithreaded service (one entry for each thread).
	SERVICE_TABLE_ENTRY dispatchTable[] =
		{
			{ LPTSTR(GetServiceName()), (LPSERVICE_MAIN_FUNCTION)ServiceMain },
			{ 0, 0 }
		};

	if(! StartServiceCtrlDispatcher(dispatchTable))
		{
		DebugPrintf("StartServiceCtrlDispatcher err %d\n", GetLastError());
		return false;
		}

	return true;
	}

//--------------------------------------------------------------------------------
BOOL CNTService::InstallService()
	{
	AFX_MANAGE_STATE_IF_DLL

	TCHAR szPath[1024];

	if(GetModuleFileName( 0, szPath, 1023 ) == 0 )
		return FALSE;

	BOOL bRet = FALSE;

	SC_HANDLE schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if(schSCManager != NULL)
		{
		DWORD nServiceType = GetServiceType();
		DWORD nStartType = GetStartType();

		SC_HANDLE schService =	CreateService(
									schSCManager,
									GetServiceName(),
									GetDisplayName(),
									GetDesiredAccess(),
									nServiceType,
									nStartType,
									GetErrorControl(),
									szPath,
									GetLoadOrderGroup(),
									((nServiceType == SERVICE_KERNEL_DRIVER ||
									  nServiceType == SERVICE_FILE_SYSTEM_DRIVER) &&
									 (nStartType == SERVICE_BOOT_START ||
									  nStartType == SERVICE_SYSTEM_START)) ?
										&m_dwTagID : NULL,
									GetDependencies(),
									GetUserName(),
									GetPassword()
								);

		if(schService != NULL)
			{
			CloseServiceHandle(schService);
			bRet = TRUE;
			}

		CloseServiceHandle(schSCManager);
		}

	if(bRet)
		// installation succeeded. Now register the message file
		RegisterApplicationLog(szPath, EVENTLOG_ERROR_TYPE | EVENTLOG_WARNING_TYPE | EVENTLOG_INFORMATION_TYPE);

	return bRet;
	}

//--------------------------------------------------------------------------------
BOOL CNTService::RemoveService()
	{
	AFX_MANAGE_STATE_IF_DLL

	BOOL bRet = FALSE;

	SC_HANDLE schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(schSCManager != NULL)
		{
		SC_HANDLE schService =	OpenService(schSCManager, GetServiceName(), SERVICE_ALL_ACCESS);

		if(schService != NULL)
			{
			// try to stop the service
			if(ControlService(schService, SERVICE_CONTROL_STOP, &m_ssStatus) )
				{
				Sleep(1000);

				while(QueryServiceStatus(schService, &m_ssStatus) )
					{
					if(m_ssStatus.dwCurrentState == SERVICE_STOP_PENDING )
						::Sleep(1000);
					else
						break;
					}
				}

			// now remove the service
			bRet = DeleteService(schService) != 0;
			CloseServiceHandle(schService);
			}

		CloseServiceHandle(schSCManager);
		}

	if(bRet)
		DeregisterApplicationLog();

	return bRet;
	}

//--------------------------------------------------------------------------------
BOOL CNTService::EndService()
	{
	AFX_MANAGE_STATE_IF_DLL

	BOOL bRet = FALSE;

	SC_HANDLE schSCManager = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(schSCManager != NULL)
		{
		SC_HANDLE schService =	::OpenService(schSCManager, GetServiceName(), SERVICE_ALL_ACCESS);

		if(schService != NULL)
			{
			// try to stop the service
			if(::ControlService(schService, SERVICE_CONTROL_STOP, &m_ssStatus))
				{
				::Sleep(1000);

				while(::QueryServiceStatus(schService, &m_ssStatus))
					{
					if(m_ssStatus.dwCurrentState == SERVICE_STOP_PENDING)
						::Sleep(1000);
					else
						break;
					}

				bRet = (m_ssStatus.dwCurrentState == SERVICE_STOPPED);
				}

			::CloseServiceHandle(schService);
			}

		::CloseServiceHandle(schSCManager);
		}

	return bRet;
	}

//--------------------------------------------------------------------------------
BOOL CNTService::StartupService()
	{
	AFX_MANAGE_STATE_IF_DLL

	BOOL bRet = FALSE;

	SC_HANDLE schSCManager = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(schSCManager != NULL)
		{
		QUERY_SERVICE_CONFIG serviceConfig;

		::QueryServiceConfig(schSCManager, &serviceConfig, sizeof(serviceConfig), NULL);
		SC_HANDLE schService =	::OpenService(schSCManager, GetServiceName(), SERVICE_ALL_ACCESS);

		if(schService != NULL)
			{
			// try to start the service
			if(::StartService(schService, 0, 0) )
				{
				Sleep(1000);

				while(::QueryServiceStatus(schService, &m_ssStatus) )
					{
					if(m_ssStatus.dwCurrentState == SERVICE_START_PENDING )
						::Sleep(1000);
					else
						break;
					}

				bRet = (m_ssStatus.dwCurrentState == SERVICE_RUNNING);
				}

			::CloseServiceHandle(schService);
			}

		::CloseServiceHandle(schSCManager);
		}

	return bRet;
	}

//--------------------------------------------------------------------------------
DWORD CNTService::GetStopPendingTime()
	{
	return 5000;
	}

//--------------------------------------------------------------------------------
DWORD CNTService::GetPausePendingTime()
	{
	return 1000;
	}

//--------------------------------------------------------------------------------
DWORD CNTService::GetContinuePendingTime()
	{
	return 1000;
	}

//--------------------------------------------------------------------------------
void CNTService::Stop()
	{
	AFX_MANAGE_STATE_IF_DLL

	if(m_pMainApp == NULL)
		return;
	ReportStatus(SERVICE_STOP_PENDING, GetStopPendingTime());
	m_pMainApp->PostThreadMessage(MSG_STOP, 0, 0);
	}

//--------------------------------------------------------------------------------
void CNTService::Pause()
	{
	AFX_MANAGE_STATE_IF_DLL

	if(m_pMainApp == NULL)
		return;
	ReportStatus(SERVICE_PAUSE_PENDING, GetPausePendingTime());
	m_pMainApp->PostThreadMessage(MSG_PAUSE, 0, 0);
	}

//--------------------------------------------------------------------------------
void CNTService::Continue()
	{
	AFX_MANAGE_STATE_IF_DLL

	if(m_pMainApp == NULL)
		return;
	ReportStatus(SERVICE_CONTINUE_PENDING, GetContinuePendingTime());
	m_pMainApp->PostThreadMessage(MSG_CONTINUE, 0, 0);
	}

//--------------------------------------------------------------------------------
void CNTService::Shutdown()
	{
	AFX_MANAGE_STATE_IF_DLL

	if(m_pMainApp == NULL)
		return;
	ReportStatus(SERVICE_STOP_PENDING, GetStopPendingTime());
	m_pMainApp->PostThreadMessage(MSG_SHUTDOWN, 0, 0);
	}

//--------------------------------------------------------------------------------
void WINAPI CNTService::ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv)
	{
	AFX_MANAGE_STATE_IF_DLL

	// register our service control handler:
	g_this->m_sshStatusHandle =
		RegisterServiceCtrlHandler(g_this->GetServiceName(),CNTService::ServiceCtrl);

	g_this->ReportStatus(SERVICE_START_PENDING, 30000);
	}

//--------------------------------------------------------------------------------
void WINAPI CNTService::ServiceCtrl(DWORD nCtrlCode)
	{
	AFX_MANAGE_STATE_IF_DLL

	_ASSERTE( g_this != NULL );

	// Handle the requested control code.
	switch(nCtrlCode)
		{
		case SERVICE_CONTROL_STOP:
			// Stop the service.
			g_this->m_ssStatus.dwCurrentState = SERVICE_STOP_PENDING;
			g_this->Stop();
			break;

		case SERVICE_CONTROL_PAUSE:
			g_this->m_ssStatus.dwCurrentState = SERVICE_PAUSE_PENDING;
			g_this->Pause();
			break;

		case SERVICE_CONTROL_CONTINUE:
			g_this->m_ssStatus.dwCurrentState = SERVICE_CONTINUE_PENDING;
			g_this->Continue();
			break;

		case SERVICE_CONTROL_SHUTDOWN:
			g_this->Shutdown();
			break;

		case SERVICE_CONTROL_INTERROGATE:
			// Update the service status.
			g_this->ReportStatus(g_this->m_ssStatus.dwCurrentState);
			break;

		default:
			break;
		}

	}

//--------------------------------------------------------------------------------
BOOL CNTService::ReportStatus(DWORD dwCurrentState, DWORD dwWaitHint, DWORD dwErrExit)
	{
	AFX_MANAGE_STATE_IF_DLL

	BOOL fResult = TRUE;

	m_ssStatus.dwControlsAccepted = GetControlsAccepted();
	m_ssStatus.dwCurrentState = dwCurrentState;
	m_ssStatus.dwWin32ExitCode = NO_ERROR;
	m_ssStatus.dwWaitHint = dwWaitHint;

	m_ssStatus.dwServiceSpecificExitCode = dwErrExit;
	if (dwErrExit!=0)
		m_ssStatus.dwWin32ExitCode = ERROR_SERVICE_SPECIFIC_ERROR;

	if(dwCurrentState == SERVICE_RUNNING || dwCurrentState == SERVICE_STOPPED)
		m_ssStatus.dwCheckPoint = 0;
	else
		m_ssStatus.dwCheckPoint = GetServiceCheckPoint();

	// Report the status of the service to the service control manager.
	if(!(fResult = SetServiceStatus( m_sshStatusHandle, &m_ssStatus)))
		{
		TCHAR szErr[256];
		DebugPrintf(_T("SetServiceStatus() failed %s\n"), GetLastErrorText(szErr,255));
		}

	return fResult;
	}

//--------------------------------------------------------------------------------
void CNTService::AddToMessageLog(LPCTSTR lpszMsg, WORD wEventType, DWORD dwEventID)
	{
	AFX_MANAGE_STATE_IF_DLL

	m_dwErr = GetLastError();

	// Use event logging to log the error.
	HANDLE hEventSource = RegisterEventSource(0, GetServiceName());

	if(hEventSource != 0 )
		{
		LPCTSTR lpszMessage = lpszMsg;

		ReportEvent(
			hEventSource,	// handle of event source
			wEventType,		// event type
			0,				// event category
			dwEventID,		// event ID
			m_pUserSID,		// current user's SID
			1,				// strings in lpszStrings
			0,				// no bytes of raw data
			&lpszMessage,	// array of error strings
			0				// no raw data
		);

		::DeregisterEventSource(hEventSource);
		}
	}


//--------------------------------------------------------------------------------
LPTSTR CNTService::GetLastErrorText(LPTSTR lpszBuf, DWORD dwSize)
	{
	AFX_MANAGE_STATE_IF_DLL

	DWORD nErr = GetLastError();

	LPTSTR lpszTemp = 0;

	DWORD dwRet =	::FormatMessage(
						FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_ARGUMENT_ARRAY,
						0,
						nErr,
						LANG_NEUTRAL,
						(LPTSTR)&lpszTemp,
						0,
						0
					);

	if(!dwRet || (dwSize < dwRet+14) )
		lpszBuf[0] = TEXT('\0');
	else
		{
		lpszTemp[_tcsclen(lpszTemp)-2] = TEXT('\0');  //remove cr/nl characters
		_tcscpy(lpszBuf, lpszTemp);
		}

	if(lpszTemp )
		LocalFree(HLOCAL(lpszTemp));

	return lpszBuf;
	}

//--------------------------------------------------------------------------------
void CNTService::RegisterApplicationLog( LPCTSTR lpszFileName, DWORD dwTypes )
	{
	AFX_MANAGE_STATE_IF_DLL

	TCHAR szKey[256];
	_tcscpy(szKey, g_pAppRegKey);
	_tcscat(szKey, GetServiceName());
	HKEY hKey = 0;
	LONG lRet = ERROR_SUCCESS;
	
	// Create a key for that application and insert values for
	// "EventMessageFile" and "TypesSupported"
	if(::RegCreateKey(HKEY_LOCAL_MACHINE, szKey, &hKey) == ERROR_SUCCESS )
		{
		lRet =	::RegSetValueEx(
					hKey,						// handle of key to set value for
					TEXT("EventMessageFile"),	// address of value to set
					0,							// reserved
					REG_EXPAND_SZ,				// flag for value type
					(CONST BYTE*)lpszFileName,	// address of value data
					_tcslen(lpszFileName) + 1	// size of value data
				);

		// Set the supported types flags.
		lRet =	::RegSetValueEx(
					hKey,					// handle of key to set value for
					TEXT("TypesSupported"),	// address of value to set
					0,						// reserved
					REG_DWORD,				// flag for value type
					(CONST BYTE*)&dwTypes,	// address of value data
					sizeof(DWORD)			// size of value data
				);
		::RegCloseKey(hKey);
		}

	// Add the service to the "Sources" value

	lRet =	::RegOpenKeyEx( 
				HKEY_LOCAL_MACHINE,	// handle of open key 
				g_pAppRegKey,		// address of name of subkey to open 
				0,					// reserved 
				KEY_ALL_ACCESS,		// security access mask 
				&hKey				// address of handle of open key 
			);
	if(lRet == ERROR_SUCCESS )
		{
		DWORD dwSize;

		// retrieve the size of the needed value
		lRet =	::RegQueryValueEx(
					hKey,			// handle of key to query 
					TEXT("Sources"),// address of name of value to query 
					0,				// reserved 
					0,				// address of buffer for value type 
					0,				// address of data buffer 
					&dwSize			// address of data buffer size 
				);

 		if(lRet == ERROR_SUCCESS )
			{
			DWORD dwType;
			DWORD dwNewSize = dwSize+_tcslen(GetServiceName())+1;
			LPBYTE Buffer = LPBYTE(::GlobalAlloc(GPTR, dwNewSize));

			lRet =	::RegQueryValueEx(
						hKey,			// handle of key to query 
						TEXT("Sources"),// address of name of value to query 
						0,				// reserved 
						&dwType,		// address of buffer for value type 
						Buffer,			// address of data buffer 
						&dwSize			// address of data buffer size 
					);
			if(lRet == ERROR_SUCCESS )
				{
				_ASSERTE(dwType == REG_MULTI_SZ);

				// check whether this service is already a known source
				register LPTSTR p = LPTSTR(Buffer);
				for(; *p; p += _tcslen(p)+1 )
					{
					if(_tcscmp(p, GetServiceName()) == 0 )
						break;
					}

				if(! *p)
					{
					// We're at the end of the stringarray
					// and the service does still not exist in the "Sources".
					// Now insert it at this point.
					// Note that we have already enough memory allocated
					// (see GlobalAlloc() above). We also don't need to append
					// an additional '\0'. This is done in GlobalAlloc() above
					// too.
					_tcscpy(p, GetServiceName());

					// OK - now store the modified value back into the
					// registry.
					lRet =	::RegSetValueEx(
								hKey,			// handle of key to set value for
								TEXT("Sources"),// address of value to set
								0,				// reserved
								dwType,			// flag for value type
								Buffer,			// address of value data
								dwNewSize		// size of value data
							);
					}
				}

			::GlobalFree(HGLOBAL(Buffer));
			}

		::RegCloseKey(hKey);
		}
	}


//--------------------------------------------------------------------------------
void CNTService::DeregisterApplicationLog()
	{
	AFX_MANAGE_STATE_IF_DLL

	TCHAR szKey[256];
	_tcscpy(szKey, g_pAppRegKey);
	_tcscat(szKey, GetServiceName());
	HKEY hKey = 0;
	LONG lRet = ERROR_SUCCESS;

	lRet = ::RegDeleteKey(HKEY_LOCAL_MACHINE, szKey);

	// now we have to delete the application from the "Sources" value too.
	lRet =	::RegOpenKeyEx( 
				HKEY_LOCAL_MACHINE,	// handle of open key 
				g_pAppRegKey,		// address of name of subkey to open 
				0,					// reserved 
				KEY_ALL_ACCESS,		// security access mask 
				&hKey				// address of handle of open key 
			);
	if(lRet == ERROR_SUCCESS )
		{
		DWORD dwSize;

		// retrieve the size of the needed value
		lRet =	::RegQueryValueEx(
					hKey,			// handle of key to query 
					TEXT("Sources"),// address of name of value to query 
					0,				// reserved 
					0,				// address of buffer for value type 
					0,				// address of data buffer 
					&dwSize			// address of data buffer size 
				);

 		if(lRet == ERROR_SUCCESS )
			{
			DWORD dwType;
			LPBYTE Buffer = LPBYTE(::GlobalAlloc(GPTR, dwSize));
			LPBYTE NewBuffer = LPBYTE(::GlobalAlloc(GPTR, dwSize));

			lRet =	::RegQueryValueEx(
						hKey,			// handle of key to query 
						TEXT("Sources"),// address of name of value to query 
						0,				// reserved 
						&dwType,		// address of buffer for value type 
						Buffer,			// address of data buffer 
						&dwSize			// address of data buffer size 
					);
			if(lRet == ERROR_SUCCESS )
				{
				_ASSERTE(dwType == REG_MULTI_SZ);

				// check whether this service is already a known source
				register LPTSTR p = LPTSTR(Buffer);
				register LPTSTR pNew = LPTSTR(NewBuffer);
				BOOL bNeedSave = FALSE;	// assume the value is already correct
				for(; *p; p += _tcslen(p)+1)
					{
					// except ourself: copy the source string into the destination
					if(_tcscmp(p, GetServiceName()) != 0 )
						{
						_tcscpy(pNew, p);
						pNew += _tcslen(pNew)+1;
						}
					else 
						{
						bNeedSave = TRUE;		// *this* application found
						dwSize -= _tcslen(p)+1;	// new size of value
						}
					}
				if(bNeedSave)
					{
					// OK - now store the modified value back into the
					// registry.
					lRet =	::RegSetValueEx(
								hKey,			// handle of key to set value for
								TEXT("Sources"),// address of value to set
								0,				// reserved
								dwType,			// flag for value type
								NewBuffer,		// address of value data
								dwSize			// size of value data
							);
					}
				}

			::GlobalFree(HGLOBAL(Buffer));
			::GlobalFree(HGLOBAL(NewBuffer));
			}

		::RegCloseKey(hKey);
		}
	}

//--------------------------------------------------------------------------------
LPCTSTR CNTService::GetServiceName()
	{
	return _T("AAAA Important NT Service");
	}

//--------------------------------------------------------------------------------
LPCTSTR CNTService::GetDisplayName()
	{
	return _T("AAAA Important NT Service");
	}

//--------------------------------------------------------------------------------
DWORD CNTService::GetControlsAccepted()
	{
	return SERVICE_ACCEPT_STOP|SERVICE_ACCEPT_PAUSE_CONTINUE;
	}

//--------------------------------------------------------------------------------
DWORD CNTService::GetServiceCheckPoint()
	{
	return 0;
	}

//--------------------------------------------------------------------------------
DWORD CNTService::GetDesiredAccess()
	{
	return SERVICE_ALL_ACCESS;
	}

//--------------------------------------------------------------------------------
DWORD CNTService::GetServiceType()
	{
	return SERVICE_WIN32_OWN_PROCESS;
	}

//--------------------------------------------------------------------------------
DWORD CNTService::GetStartType()
	{
	return SERVICE_AUTO_START;
	}

//--------------------------------------------------------------------------------
DWORD CNTService::GetErrorControl()
	{
	return SERVICE_ERROR_NORMAL;
	}

//--------------------------------------------------------------------------------
LPCTSTR CNTService::GetLoadOrderGroup()
	{
	return NULL;
	}

//--------------------------------------------------------------------------------
LPCTSTR CNTService::GetDependencies()
	{
	return NULL;
	}

//--------------------------------------------------------------------------------
LPCTSTR CNTService::GetUserName()
	{
	return NULL;
	}

//--------------------------------------------------------------------------------
LPCTSTR CNTService::GetPassword()
	{
	return NULL;
	}

//--------------------------------------------------------------------------------
UINT CNTService::ServiceThread(CNTService* that)
	{
	AFX_MANAGE_STATE_IF_DLL

	UINT nRv = 0;

	if(! that->StartDispatcher())
		nRv = 1;

	DebugPrintf("stopping\n");

	that->ReportStatus(SERVICE_STOP_PENDING);

	if(AfxGetApp() != NULL)
		AfxGetApp()->PostThreadMessage(WM_QUIT, 0, 0);

	return nRv;
	}
