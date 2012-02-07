//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _HL7NTSERVICE_H_
#define _HL7NTSERVICE_H_

#include <NTService.h>

//--------------------------------------------------------------------------------
class CHL7NTService : public CNTService
	{
	public:
		CHL7NTService();

		virtual LPCTSTR GetServiceName();
		virtual LPCTSTR GetDisplayName();

#ifdef _EVALUATION_VERSION
	protected:
		virtual DWORD GetControlsAccepted();
		virtual DWORD GetServiceCheckPoint();
		virtual DWORD GetDesiredAccess();
		virtual DWORD GetServiceType();
		virtual DWORD GetStartType();
		virtual DWORD GetErrorControl();
		virtual LPCTSTR GetLoadOrderGroup();
		virtual LPCTSTR GetDependencies();
		virtual LPCTSTR GetUserName();
		virtual LPCTSTR GetPassword();

	public:
		virtual void Stop();
		virtual void Pause();
		virtual void Continue();
		virtual void Shutdown();
		virtual BOOL StartDispatcher();
		virtual BOOL InstallService();
		virtual BOOL RemoveService();
		virtual BOOL EndService();
		virtual BOOL StartupService();

		virtual BOOL StartServiceThread();

		virtual DWORD GetStopPendingTime();
		virtual DWORD GetPausePendingTime();
		virtual DWORD GetContinuePendingTime();

		virtual BOOL ProcessCommandLine();
		virtual BOOL ProcessCommandLine(LPCTSTR);

	protected:
		virtual void RegisterApplicationLog(LPCTSTR lpszProposedMessageFile,DWORD dwProposedTypes);
		virtual void DeregisterApplicationLog();

	public:
		LPTSTR GetLastErrorText(LPTSTR Buf, DWORD Size);
		BOOL ReportStatus(DWORD CurState, DWORD WaitHint = 3000, DWORD ErrExit = 0);
		virtual void AddToMessageLog(LPCTSTR Message, WORD EventType = EVENTLOG_ERROR_TYPE, DWORD dwEventID = -1);
#endif
	};

#endif //_HL7NTSERVICE_H_
