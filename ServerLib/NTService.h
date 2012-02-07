//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef NTService_h
#define NTService_h

#include <winsvc.h>

//--------------------------------------------------------------------------------
class CNTService
	{
	public:
		enum
			{
			MSG_PAUSE = WM_USER + 200,
			MSG_CONTINUE,
			MSG_STOP,
			MSG_SHUTDOWN
			};

	protected:
		CWinApp* m_pMainApp;
		DWORD m_dwErr;
		SERVICE_STATUS m_ssStatus;
		SERVICE_STATUS_HANDLE m_sshStatusHandle;
		PSID m_pUserSID;				// the current user's security identifier
		DWORD m_dwTagID;

	public:
		CNTService(CWinApp* = NULL);
		virtual ~CNTService();

	protected:
		CNTService(const CNTService&);
		CNTService& operator=(const CNTService&);

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
		virtual LPCTSTR GetServiceName();
		virtual LPCTSTR GetDisplayName();

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

		void StartServiceThread();

		virtual DWORD GetStopPendingTime();
		virtual DWORD GetPausePendingTime();
		virtual DWORD GetContinuePendingTime();

		virtual BOOL ProcessCommandLine();
		virtual BOOL ProcessCommandLine(LPCTSTR);

	protected:
		virtual void RegisterApplicationLog(LPCTSTR lpszProposedMessageFile,DWORD dwProposedTypes);
		virtual void DeregisterApplicationLog();

		static UINT ServiceThread(CNTService*);

	public:
		LPTSTR GetLastErrorText(LPTSTR Buf, DWORD Size);
		BOOL ReportStatus(DWORD CurState, DWORD WaitHint = 3000, DWORD ErrExit = 0);
		virtual void AddToMessageLog(LPCTSTR Message, WORD EventType = EVENTLOG_ERROR_TYPE, DWORD dwEventID = -1);

		static void WINAPI ServiceCtrl(DWORD CtrlCode);
		static void WINAPI ServiceMain(DWORD argc, LPTSTR* argv);
	};

//--------------------------------------------------------------------------------
inline void CNTService::StartServiceThread()
	{
	CWinThread* pThread = ::AfxBeginThread((AFX_THREADPROC) CNTService::ServiceThread, this);
	pThread->m_bAutoDelete = TRUE;
	::Sleep(1000);
	ReportStatus(SERVICE_RUNNING);
	}

#endif	// NTService_h
