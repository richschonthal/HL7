//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------


#include "stdafx.h"

#include <tchar.h>
#include <stdio.h>
#include <crtdbg.h>

#include <io.h>	
#include <fcntl.h>

#include "NTService.h"
#include "NTServiceEventLogMsg.h"

#include <CmdLine.h>

#include <allocstring.h>
#include <status.h>

#ifndef RSP_SIMPLE_SERVICE
	#define RSP_SIMPLE_SERVICE 1
#endif
#ifndef RSP_UNREGISTER_SERVICE
	#define RSP_UNREGISTER_SERVICE 0
#endif

BOOL CNTService::m_bInstance = FALSE;

static CNTService * g_pService = 0;

static LPCTSTR g_pAppRegKey = TEXT("SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\");

/////////////////////////////////////////////////////////////////////////////
// class CNTService -- construction/destruction

//--------------------------------------------------------------------------------
CNTService::CNTService(LPCTSTR pDependencies)
		: m_dwCheckPoint(0)
		, m_dwErr(0)
		, m_bDebug(FALSE)
		, m_sshStatusHandle(0)
		, m_dwControlsAccepted(SERVICE_ACCEPT_STOP)
		, m_pUserSID(0)
		, m_fConsoleReady(FALSE)
		, m_dwDesiredAccess(SERVICE_ALL_ACCESS)
		, m_dwServiceType(SERVICE_WIN32_OWN_PROCESS)
		, m_dwStartType(SERVICE_AUTO_START)
		, m_dwErrorControl(SERVICE_ERROR_NORMAL)
		, m_pszLoadOrderGroup(0)
		, m_dwTagID(0)
		, m_pszDependencies(pDependencies)
		, m_pName(NULL)
		, m_bExiting(false)
		, m_pStartName(NULL)
		, m_pPassword(NULL)

	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	_ASSERTE( ! m_bInstance );

	OSVERSIONINFO vi;
	vi.dwOSVersionInfoSize=sizeof(vi);  // init this.
	GetVersionEx(&vi);

	// must be running on NT
	if(vi.dwPlatformId != VER_PLATFORM_WIN32_NT)
		{
		return;
		}

	m_bInstance = TRUE;
	g_pService = this;
	CStatus(this);

	// SERVICE_STATUS members that rarely change
	m_ssStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	m_ssStatus.dwServiceSpecificExitCode = 0;

	/////////////////////////////////////////////////////////////////////////
	// Providing a SID (security identifier) was contributed by Victor
	// Vogelpoel (VictorV@Telic.nl).
	// The code from Victor was slightly modified.

	// Get security information of current user
	BYTE	security_identifier_buffer[ 4096 ];
	DWORD	dwSizeSecurityIdBuffer = sizeof( security_identifier_buffer );
	PSID	user_security_identifier = NULL;

	TCHAR sUserName[ 256 ];
	DWORD dwSizeUserName  =  255;

	TCHAR sDomainName[ 256 ];
	DWORD dwSizeDomainName = 255;

	SID_NAME_USE sidTypeSecurityId;

	::ZeroMemory(sUserName, sizeof( sUserName ) );
	::ZeroMemory(sDomainName, sizeof( sDomainName ) );
	::ZeroMemory(security_identifier_buffer, dwSizeSecurityIdBuffer );

	::GetUserName( sUserName, &dwSizeUserName );

	if( ::LookupAccountName(
				0,
				sUserName,
				&security_identifier_buffer,
				&dwSizeSecurityIdBuffer,
				sDomainName,
				&dwSizeDomainName,
				&sidTypeSecurityId
			))
		{
		if( ::IsValidSid( PSID(security_identifier_buffer) ) )
			{
			DWORD dwSidLen = ::GetLengthSid(PSID(security_identifier_buffer));
			m_pUserSID = PSID(new BYTE [dwSidLen]);
			::CopySid(dwSidLen, m_pUserSID, security_identifier_buffer);
			_ASSERTE(::EqualSid(m_pUserSID, security_identifier_buffer));
			}
		}
	}


//--------------------------------------------------------------------------------
CNTService::~CNTService()
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	_ASSERTE( m_bInstance );
	delete [] LPBYTE(m_pUserSID);
	m_bInstance = FALSE;
	g_pService = 0;
	delete[] m_pStartName;
	delete[] m_pPassword;
	delete[] m_pName;
	}

/////////////////////////////////////////////////////////////////////////////
// class CNTService -- overridables


//--------------------------------------------------------------------------------
bool CNTService::PreprocessCommandLine()
	{
	return true;
	}

//--------------------------------------------------------------------------------
bool CNTService::ProcessCommandLine()
	{
	return true;
	}

//--------------------------------------------------------------------------------
bool CNTService::PreStart()
	{
	return true;
	}

//--------------------------------------------------------------------------------
bool CNTService::PreInstall()
	{
	return true;
	}

//--------------------------------------------------------------------------------
void CNTService::PostInstall()
	{
	}

//--------------------------------------------------------------------------------
BOOL CNTService::RegisterService( int argc, char ** argv )
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(! PreprocessCommandLine())
		return false;

	BOOL (CNTService::* fnc)() = &CNTService::StartDispatcher;

	CCmdLine cmdLine;
	CString sTemp;
	int nCmdIndex;

	if(cmdLine.HasParam('i', nCmdIndex))
		{
		fnc = &CNTService::InstallService;

		CStringArray sTemp;

		cmdLine.GetArgs(nCmdIndex, sTemp);
		if(sTemp.GetSize() == 2)
			{
			ALLOC_STRING(m_pStartName, sTemp[0]);
			ALLOC_STRING(m_pPassword, sTemp[1]);
			}
		}
	else if(cmdLine.HasParam('u', nCmdIndex))
		{
		fnc = &CNTService::RemoveService;
		}
	else if(cmdLine.HasParam('s', nCmdIndex))
		{
		fnc = &CNTService::StartupService;
		}
	else if(cmdLine.HasParam('e', nCmdIndex))
		{
		fnc = &CNTService::EndService;
		}

	if(! ProcessCommandLine())
		return false;

	m_bDebug = cmdLine.HasParam('d', nCmdIndex);

	if(m_bDebug)
		return DebugService(argc, argv, false);
	else
		return (this->*fnc)();
	}

//--------------------------------------------------------------------------------
BOOL CNTService::StartDispatcher()
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(! PreStart())
		return false;

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
		TCHAR szBuf[256];
        CStatus(GetLastErrorText(szBuf,255), EVENTLOG_ERROR_TYPE, -1);
		return false;
		}

	return true;
	}

//--------------------------------------------------------------------------------
BOOL CNTService::InstallService()
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(! PreInstall())
		return false;

    TCHAR szPath[1024];

	SetupConsole();

	if( GetModuleFileName( 0, szPath, 1023 ) == 0 )
		{
		TCHAR szErr[256];
		CStatus::Error(TEXT("Unable to install %s - %s\n"), GetDisplayName(), GetLastErrorText(szErr, 256));
		return FALSE;
		}

	BOOL bRet = FALSE;

	SC_HANDLE schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if(schSCManager == NULL)
		{
		TCHAR szErr[256];
		CStatus::Error(TEXT("OpenSCManager failed - %s\n"), GetLastErrorText(szErr,256));
		}
	else
		{
		SC_HANDLE schService =	CreateService(
									schSCManager,
									GetServiceName(),
									GetDisplayName(),
									m_dwDesiredAccess,
									m_dwServiceType,
									m_dwStartType,
									m_dwErrorControl,
									szPath,
									m_pszLoadOrderGroup,
									((m_dwServiceType == SERVICE_KERNEL_DRIVER ||
									  m_dwServiceType == SERVICE_FILE_SYSTEM_DRIVER) &&
									 (m_dwStartType == SERVICE_BOOT_START ||
									  m_dwStartType == SERVICE_SYSTEM_START)) ?
										&m_dwTagID : 0,
									m_pszDependencies,
									GetStartName(),
									GetPassword()
								);

		if( schService )
			{
			CStatus::Log(EVENTLOG_INFORMATION_TYPE, TEXT("%s installed.\n"), GetDisplayName() );
			CloseServiceHandle(schService);
			bRet = TRUE;
			}
		else
			{
			TCHAR szErr[256];
			CStatus::Error(TEXT("CreateService failed - %s\n"), GetLastErrorText(szErr, 256));
			}

			CloseServiceHandle(schSCManager);
		}

	if(bRet)
		{
		// installation succeeded. Now register the message file
		RegisterApplicationLog(
			szPath,		// the path to the application itself
			EVENTLOG_ERROR_TYPE | EVENTLOG_WARNING_TYPE | EVENTLOG_INFORMATION_TYPE // supported types
		);

		CStatus(TEXT("Service installed"), EVENTLOG_INFORMATION_TYPE);

		PostInstall();
		}

	return bRet;
	}


//--------------------------------------------------------------------------------
BOOL CNTService::RemoveService()
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	BOOL bRet = FALSE;

	SetupConsole();	

	SC_HANDLE schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(schSCManager == NULL)
		{
		TCHAR szErr[256];
		CStatus::Error(TEXT("OpenSCManager failed - %s\n"), GetLastErrorText(szErr,256));
		}
	else
		{
		SC_HANDLE schService =	OpenService(schSCManager, GetServiceName(), SERVICE_ALL_ACCESS);

		if(schService == NULL)
			{
			TCHAR szErr[256];
			CStatus::Error(TEXT("OpenService failed - %s\n"), GetLastErrorText(szErr,256));
			}
		else 
			{
			// try to stop the service
			if( ControlService(schService, SERVICE_CONTROL_STOP, &m_ssStatus) )
				{
				CStatus::Log(EVENTLOG_INFORMATION_TYPE, TEXT("Stopping %s."), GetDisplayName());
				Sleep(1000);

				while( QueryServiceStatus(schService, &m_ssStatus) )
					{
					if( m_ssStatus.dwCurrentState == SERVICE_STOP_PENDING )
						::Sleep(1000);
					else
						break;
					}

				if( m_ssStatus.dwCurrentState == SERVICE_STOPPED )
						CStatus::Log(EVENTLOG_INFORMATION_TYPE, TEXT("\n%s stopped.\n"), GetDisplayName());
				else
						CStatus::Error(TEXT("\n%s failed to stop.\n"), GetDisplayName());
				}

			// now remove the service
			if( DeleteService(schService) )
				{
				CStatus::Log(EVENTLOG_INFORMATION_TYPE, TEXT("%s removed.\n"), GetDisplayName());
				bRet = TRUE;
				}
			else
				{
				TCHAR szErr[256];
				CStatus::Error(TEXT("DeleteService failed - %s\n"), GetLastErrorText(szErr,256));
				}

			CloseServiceHandle(schService);
		}

	CloseServiceHandle(schSCManager);
	}

	if( bRet )
		DeregisterApplicationLog();

	return bRet;
	}


//--------------------------------------------------------------------------------
BOOL CNTService::EndService()
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	BOOL bRet = FALSE;

	SC_HANDLE schSCManager = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if( schSCManager == NULL)
		{
		TCHAR szErr[256];
		CStatus::Error(TEXT("OpenSCManager failed - %s\n"), GetLastErrorText(szErr,256));
		}
	else
		{
		SC_HANDLE schService =	::OpenService(schSCManager, GetServiceName(), SERVICE_ALL_ACCESS);

		if( schService == NULL)
			{
			TCHAR szErr[256];
			CStatus::Error(TEXT("OpenService failed - %s\n"), GetLastErrorText(szErr,256));
			}
		else
			{
			// try to stop the service
			if( ::ControlService(schService, SERVICE_CONTROL_STOP, &m_ssStatus) )
				{
				CStatus::Log(EVENTLOG_INFORMATION_TYPE, TEXT("Stopping %s."), GetDisplayName());
				::Sleep(1000);

				while( ::QueryServiceStatus(schService, &m_ssStatus) )
					{
					if( m_ssStatus.dwCurrentState == SERVICE_STOP_PENDING )
						::Sleep(1000);
					else
						break;
					}

				if( m_ssStatus.dwCurrentState == SERVICE_STOPPED )
					{
					bRet = TRUE;
					CStatus::Log(EVENTLOG_INFORMATION_TYPE, TEXT("\n%s stopped.\n"), GetDisplayName());
					}
                else
                    CStatus::Error(TEXT("\n%s failed to stop.\n"), GetDisplayName());
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
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	BOOL bRet = FALSE;

	if(! PreStart())
		return bRet;

	SC_HANDLE schSCManager = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if( schSCManager == NULL)
		{
		TCHAR szErr[256];
		CStatus::Error(TEXT("OpenSCManager failed - %s\n"), GetLastErrorText(szErr,256));
		}
	else
		{
		QUERY_SERVICE_CONFIG serviceConfig;

		bool b = ::QueryServiceConfig(schSCManager, &serviceConfig, sizeof(serviceConfig), NULL);
		SC_HANDLE schService =	::OpenService(schSCManager, GetServiceName(), SERVICE_ALL_ACCESS);

		if( schService == NULL)
			{
			TCHAR szErr[256];
			CStatus::Error(TEXT("OpenService failed - %s\n"), GetLastErrorText(szErr,256));
			}
		else
			{
			// try to start the service
			CStatus::Log(EVENTLOG_INFORMATION_TYPE, TEXT("Starting up %s."), GetDisplayName());
			if( ::StartService(schService, 0, 0) )
				{
				Sleep(1000);

				while( ::QueryServiceStatus(schService, &m_ssStatus) )
					{
					if( m_ssStatus.dwCurrentState == SERVICE_START_PENDING )
						::Sleep(1000);
					else
						break;
					}

				if( m_ssStatus.dwCurrentState == SERVICE_RUNNING )
					{
					bRet = TRUE;
					CStatus::Log(EVENTLOG_INFORMATION_TYPE, TEXT("\n%s started.\n"), GetDisplayName());
					}
                else
                    CStatus::Error(TEXT("\n%s failed to start.\n"), GetDisplayName());
				}
			else
				{
				// StartService failed
				TCHAR szErr[256];
				CStatus::Error(TEXT("\n%s failed to start: %s\n"), GetDisplayName(), GetLastErrorText(szErr,256));
				}

			::CloseServiceHandle(schService);
			}

        ::CloseServiceHandle(schSCManager);
		}

	return bRet;
}


//--------------------------------------------------------------------------------
BOOL CNTService::DebugService(int argc, char ** argv, BOOL)
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

    DWORD dwArgc;
    LPTSTR *lpszArgv;

#ifdef UNICODE
    lpszArgv = CommandLineToArgvW(GetCommandLineW(), &(dwArgc) );
#else
    dwArgc   = (DWORD) argc;
    lpszArgv = argv;
#endif

	SetupConsole();
	CStatus::Log(EVENTLOG_INFORMATION_TYPE, TEXT("Debugging %s.\n"), GetDisplayName());
	SetConsoleCtrlHandler(ControlHandler, TRUE);

    Run(dwArgc, lpszArgv);

#ifdef UNICODE
	::GlobalFree(HGLOBAL)lpszArgv);
#endif

	return TRUE;
	}


//--------------------------------------------------------------------------------
void CNTService::Pause()
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	}


//--------------------------------------------------------------------------------
void CNTService::Continue()
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	}


//--------------------------------------------------------------------------------
void CNTService::Shutdown()
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	}

/////////////////////////////////////////////////////////////////////////////
// class CNTService -- default handlers

//--------------------------------------------------------------------------------
void WINAPI CNTService::ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv)
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	_ASSERTE( g_pService != 0 );

	// register our service control handler:
	g_pService->m_sshStatusHandle =
		RegisterServiceCtrlHandler(g_pService->GetServiceName(),CNTService::ServiceCtrl);

	if( g_pService->m_sshStatusHandle )

		// report the status to the service control manager.
		if( g_pService->ReportStatus(SERVICE_START_PENDING) )
			g_pService->Run( dwArgc, lpszArgv );

	// try to report the stopped status to the service control manager.
	if( g_pService->m_sshStatusHandle )
		g_pService->ReportStatus(SERVICE_STOPPED);
	}


//--------------------------------------------------------------------------------
void WINAPI CNTService::ServiceCtrl(DWORD nCtrlCode)
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	_ASSERTE( g_pService != 0 );

	// Handle the requested control code.
	switch(nCtrlCode)
		{
		case SERVICE_CONTROL_STOP:
			// Stop the service.
			g_pService->m_ssStatus.dwCurrentState = SERVICE_STOP_PENDING;
			g_pService->Stop();
			break;

		case SERVICE_CONTROL_PAUSE:
			g_pService->m_ssStatus.dwCurrentState = SERVICE_PAUSE_PENDING;
			g_pService->Pause();
			break;

		case SERVICE_CONTROL_CONTINUE:
			g_pService->m_ssStatus.dwCurrentState = SERVICE_CONTINUE_PENDING;
			g_pService->Continue();
			break;

		case SERVICE_CONTROL_SHUTDOWN:
			g_pService->Shutdown();
			break;

		case SERVICE_CONTROL_INTERROGATE:
			// Update the service status.
			g_pService->ReportStatus(g_pService->m_ssStatus.dwCurrentState);
			break;

		default:
			break;
		}

	}

//--------------------------------------------------------------------------------
BOOL WINAPI CNTService::ControlHandler(DWORD dwCtrlType)
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	_ASSERTE(g_pService != 0);
	switch( dwCtrlType )
		{
		case CTRL_BREAK_EVENT:  // use Ctrl+C or Ctrl+Break to simulate
		case CTRL_C_EVENT:      // SERVICE_CONTROL_STOP in debug mode
			CStatus::Log(EVENTLOG_INFORMATION_TYPE, TEXT("Stopping %s.\n"), g_pService->GetDisplayName());
			g_pService->Stop();
			return TRUE;
		}

	return FALSE;
	}


/////////////////////////////////////////////////////////////////////////////
// class CNTService -- helpers

//!! TCW MOD - added DWORD dwErrExit for error exit value. Defaults to zero
//--------------------------------------------------------------------------------
BOOL CNTService::ReportStatus(DWORD dwCurrentState, DWORD dwWaitHint, DWORD dwErrExit)
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	BOOL fResult = TRUE;

	if(! m_bDebug)
		{ 
		// when debugging we don't report to the SCM
//		if( dwCurrentState == SERVICE_START_PENDING)
//           m_ssStatus.dwControlsAccepted = 0;
//        else
            m_ssStatus.dwControlsAccepted = m_dwControlsAccepted;

        m_ssStatus.dwCurrentState = dwCurrentState;
        m_ssStatus.dwWin32ExitCode = NO_ERROR;
        m_ssStatus.dwWaitHint = dwWaitHint;

		m_ssStatus.dwServiceSpecificExitCode = dwErrExit;
		if (dwErrExit!=0)
			m_ssStatus.dwWin32ExitCode = ERROR_SERVICE_SPECIFIC_ERROR;

        if( dwCurrentState == SERVICE_RUNNING || dwCurrentState == SERVICE_STOPPED )
            m_ssStatus.dwCheckPoint = 0;
        else
            m_ssStatus.dwCheckPoint = ++m_dwCheckPoint;

        // Report the status of the service to the service control manager.
        if (!(fResult = SetServiceStatus( m_sshStatusHandle, &m_ssStatus)))
            CStatus(TEXT("SetServiceStatus() failed"), EVENTLOG_ERROR_TYPE);
		}

    return fResult;
	}


//--------------------------------------------------------------------------------
void CNTService::AddToMessageLog(LPCTSTR lpszMsg, WORD wEventType, DWORD dwEventID)
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_dwErr = GetLastError();

	// use default message-IDs
	if( dwEventID == DWORD(-1) )
		{
		switch( wEventType )
			{
			case EVENTLOG_ERROR_TYPE:
				dwEventID = MSG_ERROR_1;
				break;
			case EVENTLOG_WARNING_TYPE:
				dwEventID = MSG_WARNING_1;
				break;
			case EVENTLOG_INFORMATION_TYPE:
				dwEventID = MSG_INFO_1;
				break;
			case EVENTLOG_AUDIT_SUCCESS:
				dwEventID = MSG_INFO_1;
				break;
			case EVENTLOG_AUDIT_FAILURE:
				dwEventID = MSG_INFO_1;
				break;
			default:
				dwEventID = MSG_INFO_1;
				break;
			}
		}

	// Use event logging to log the error.
	HANDLE hEventSource = RegisterEventSource(0, GetServiceName());

	if( hEventSource != 0 )
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
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

    LPTSTR lpszTemp = 0;
	
	DWORD nErr = GetLastError();

    DWORD dwRet =	::FormatMessage(
						FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_ARGUMENT_ARRAY,
						0,
						nErr,
						LANG_NEUTRAL,
						(LPTSTR)&lpszTemp,
						0,
						0
					);

    if( !dwRet || (dwSize < dwRet+14) )
        lpszBuf[0] = TEXT('\0');
    else
		{
        lpszTemp[_tcsclen(lpszTemp)-2] = TEXT('\0');  //remove cr/nl characters
        _tcscpy(lpszBuf, lpszTemp);
		}

    if( lpszTemp )
        LocalFree(HLOCAL(lpszTemp));

    return lpszBuf;
	}

/////////////////////////////////////////////////////////////////////////////
// class CNTService -- implementation

//--------------------------------------------------------------------------------
void CNTService::RegisterApplicationLog( LPCTSTR lpszFileName, DWORD dwTypes )
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	TCHAR szKey[256];
	_tcscpy(szKey, g_pAppRegKey);
	_tcscat(szKey, GetServiceName());
	HKEY hKey = 0;
	LONG lRet = ERROR_SUCCESS;
	
	// Create a key for that application and insert values for
	// "EventMessageFile" and "TypesSupported"
	if( ::RegCreateKey(HKEY_LOCAL_MACHINE, szKey, &hKey) == ERROR_SUCCESS )
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
	if( lRet == ERROR_SUCCESS )
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

 		if( lRet == ERROR_SUCCESS )
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
			if( lRet == ERROR_SUCCESS )
				{
				_ASSERTE(dwType == REG_MULTI_SZ);

				// check whether this service is already a known source
				register LPTSTR p = LPTSTR(Buffer);
				for(; *p; p += _tcslen(p)+1 )
					{
					if( _tcscmp(p, GetServiceName()) == 0 )
						break;
					}

				if( ! * p )
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
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

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
	if( lRet == ERROR_SUCCESS )
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

 		if( lRet == ERROR_SUCCESS )
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
			if( lRet == ERROR_SUCCESS )
				{
				_ASSERTE(dwType == REG_MULTI_SZ);

				// check whether this service is already a known source
				register LPTSTR p = LPTSTR(Buffer);
				register LPTSTR pNew = LPTSTR(NewBuffer);
				BOOL bNeedSave = FALSE;	// assume the value is already correct
				for(; *p; p += _tcslen(p)+1)
					{
					// except ourself: copy the source string into the destination
					if( _tcscmp(p, GetServiceName()) != 0 )
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
				if( bNeedSave )
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

////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------
void CNTService::SetupConsole()
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if( !m_fConsoleReady )
		{
		AllocConsole();	// you only get 1 console.

		DWORD astds[3]={STD_OUTPUT_HANDLE,STD_ERROR_HANDLE,STD_INPUT_HANDLE};
		FILE *atrgs[3]={stdout,stderr,stdin};
		for( register int i=0; i<3; i++ )
			{
			long hand=(long)GetStdHandle(astds[i]);
			if( hand!=(long)INVALID_HANDLE_VALUE )
				{
				int osf=_open_osfhandle(hand,_O_TEXT);
				if( osf!=-1 )
					{
					FILE *fp=_fdopen(osf,(astds[i]==STD_INPUT_HANDLE) ? "r" : "w");
					if( fp!=NULL )
						{
						*(atrgs[i])=*fp;
						setvbuf(fp,NULL,_IONBF,0);
						}
					}
				}
			}

		m_fConsoleReady=TRUE;
		}
	}
