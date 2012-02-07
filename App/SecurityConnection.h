//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _SECURITYCONNECTION_H_
#define _SECURITYCONNECTION_H_

//--------------------------------------------------------------------------------
#include <ThreadObject.h>
#include <ReadWriteObject.h>
#include <Certificate.h>
#include <ReadWriteObject.h>

#ifndef __AFXSOCK_H__
#include <afxsock.h>
#endif //__AFXSOCK_H__

//--------------------------------------------------------------------------------
class CCertificateContainer;
class CSmallSocket;

//--------------------------------------------------------------------------------
class CSecurityConnection : public CThreadObject, public CReadWriteObject
	{
	public:
		enum
			{
			ErrCantCreateSocket = HighestResultCode,
			ErrCantOpenSocket,
			ErrCantConnectToServer,
			ErrSocketReceive,
			ErrSocketSend,
			ErrNULLCertificate,
			ErrNULLSocket
			};

		enum
			{
			// LPARAM values for target's messages
			MsgNone,
			// new certificate issued
			MsgNew,
			// no certificate issued (server is runnning)
			MsgNotIssued,
			// certificate refreshed
			MsgRefreshed,
			// certificate was issued but has been revoked
			MsgRevoked,
			// certificate released by client app
			MsgReleased,
			// communication error
			MsgCommError,
			// internal error
			MsgGeneralError
			};

	private:
		enum
			{
			TargetNone,
			// MFC targets
			TargetThread, TargetCWnd,
			// non-MFC targets
			TargetHWND, TargetThreadId
			} m_nTargetType;

	private:
		CCertificateContainer* m_pContainer;
		CSmallSocket* m_pSocket;
		LPTSTR m_pIP;
		UINT m_nPort;
		LPTSTR m_pBackupIP;
		UINT m_nBackupPort;
		DWORD m_nSleepInterval;
		// true when there is currently a certificate
		bool m_bObtained;
		// true when at least one certificate has been obtained
		bool m_bObtainedInitial;

		DWORD m_nUserClass;
		CDWordArray m_nTokenIdList;

		union
			{ 
			CWinThread* pThrd;
			CWnd* pWnd;
			HWND hWnd;
			DWORD nThreadId;
			} m_target;

	public:
		UINT m_nMsg;
		// when true, if the connection to the server is lost then
		// this connection tries to connect to the backup server
		// the default is true
		bool m_bAutoConnectToBackup;
		// fired when a certificate is obtained
		CEvent m_evtObtained;

	protected:
		bool SwitchToBackup();
		DWORD DoGetNewCertificate(DWORD nUserClass, DWORD nTokenId);
		bool ReGetCertificate();
		void SetObtained(bool = true);

	public:
		CSecurityConnection(LPCTSTR ip, UINT port, LPCTSTR backupip, UINT backupport);
		virtual ~CSecurityConnection();

		virtual bool MainLoop();
		virtual DWORD GetThreadLoopSleepInterval() const;

		void SetTarget(CWinThread*, UINT);
		void SetTarget(CWnd*, UINT);
		void SetTarget(HWND, UINT);
		void SetTarget(DWORD, UINT);

		void SetTargetNone();

		DWORD GetUserClass() const;

		LPCTSTR GetIP() const;
		UINT GetPort() const;

		LPCTSTR GetBackupIP() const;
		UINT GetBackupPort() const;

		CCertificate* GetCertificate() const;

		DWORD GetNewCertificate(DWORD nUserClass, DWORD nTokenId);
		bool RefreshCertificate();
		bool ReleaseCertificate();

		bool AquireToken(ULONG);
		bool ReleaseToken(ULONG);

		static LPCTSTR GetMessageText(int, int);

		virtual bool OnNewCertificate();
		virtual bool OnRefresh();
		virtual bool OnRevoked();
		virtual bool OnError();

		bool IsValidCertificate();
		bool HasToken(DWORD);

	protected:
		void DoPostMessage(int nMsg);
	};

//--------------------------------------------------------------------------------
class CCertificateContainer : public CReadWriteObject
	{
	public:
		CCertificate* m_pCert;

		CCertificateContainer() : m_pCert(NULL) {}
		~CCertificateContainer() { delete m_pCert; }

		void DeleteCertificate() { delete m_pCert; m_pCert = NULL; }
	};

#endif // _SECURITYCONNECTION_H_
