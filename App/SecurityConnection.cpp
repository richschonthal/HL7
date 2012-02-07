//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"

#include <allocstring.h>
#include <LockedSocket.h>

#include "SecurityConnection.h"

#include <Certificate.h>
#include <AccessToken.h>
#include <WriteLock.h>
#include <ReadLock.h>

#ifndef _SECCON_NOFORCELIBS
#ifdef _DEBUG
#pragma comment(lib, "ServerLibD.lib")
#pragma comment(lib, "stdobjD.lib")
#pragma comment(lib, "SecurityServerLibD.lib")
#else
#pragma comment(lib, "ServerLib.lib")
#pragma comment(lib, "stdobj.lib")
#pragma comment(lib, "SecurityServerLib.lib")
#endif
// androgynous libs
#pragma comment(lib, "ws2_32.lib")
#endif

// socket time-out values
/*#ifdef _DEBUG
#define RECEIVEWAIT1	INFINITE
#define RECEIVEWAIT2	INFINITE
#define RECEIVEWAIT3	INFINITE
#define SENDWAIT1		INFINITE
#else*/
#define RECEIVEWAIT1	1000
#define RECEIVEWAIT2	2000
#define RECEIVEWAIT3	10000
#define SENDWAIT1		10000
//#endif

#define CONTAINER_LOCK_WAIT 3000

//ServerLibD.lib stdobjD.lib SecurityServerLibD.lib 
//--------------------------------------------------------------------------------
using namespace CCertificateCommands;

//--------------------------------------------------------------------------------
CSecurityConnection::CSecurityConnection(LPCTSTR pIP, UINT nPort, LPCTSTR pBackupIP, UINT nBackupPort)
		: CThreadObject(NULL)
		, m_pIP(NULL)
		, m_pBackupIP(NULL)
		, m_nSleepInterval(30003)
		, m_bObtained(false)
		, m_nTargetType(TargetNone)
		, m_nPort(nPort)
		, m_nBackupPort(nBackupPort)
		, m_bAutoConnectToBackup(true)
		, m_nUserClass(0)
		, m_bObtainedInitial(false)
	{
	// init winsock incase the app programmer forgot to
	AfxSocketInit();
	ALLOC_STRING(m_pIP, pIP);
	ALLOC_STRING(m_pBackupIP, pBackupIP);
	m_pContainer = new CCertificateContainer;
	m_pSocket = NULL;
	}

//--------------------------------------------------------------------------------
CSecurityConnection::~CSecurityConnection()
	{
	EndThread();
	ReleaseCertificate();
	delete[] m_pIP;
	delete[] m_pBackupIP;
	delete m_pContainer;
	delete m_pSocket;
	}

//--------------------------------------------------------------------------------
void CSecurityConnection::SetObtained(bool bObtained)
	{
	if(bObtained)
		m_evtObtained.SetEvent();
	else
		m_evtObtained.ResetEvent();

	m_bObtained = bObtained;
	}

//--------------------------------------------------------------------------------
bool CSecurityConnection::MainLoop()
	{
	CWriteLock lock1(this, false);
	if(! lock1.Lock(20))
		return true;

	CWriteLock lock2(m_pContainer, false);
	if(! lock2.Lock(20))
		return true;

	// this happens when we've lost a certificate
	if(m_pContainer->m_pCert == NULL && m_bObtainedInitial)
		if(! ReGetCertificate())
			return false;

	if(m_pContainer->m_pCert == NULL)
		return true;

	// try to refresh
	if(! RefreshCertificate())
		if(! ReGetCertificate())
			return false;

	return true;
	}

//--------------------------------------------------------------------------------
bool CSecurityConnection::ReGetCertificate()
	{
	DWORD nTokenId = 0;
	int nUpper = m_nTokenIdList.GetUpperBound();
	if(nUpper >= 0)
		nTokenId = m_nTokenIdList[0];

	// get a certificate
	if(GetNewCertificate(m_nUserClass, nTokenId) == (ULONG) -1)
		return false;

	::Sleep(1000);

	if(nUpper > 0)
		for(int i = nUpper; i > 0; i--)
			{
			nTokenId = m_nTokenIdList[i];
			ReleaseToken(nTokenId);
			::Sleep(1000);
			if(AquireToken(nTokenId))
				TRACE("re-Aquired %d\n", nTokenId);
			else
				TRACE("DID NOT re-Aquired %d\n", nTokenId);
			}

	return true;
	}

//--------------------------------------------------------------------------------
bool CSecurityConnection::SwitchToBackup()
	{
	// if the refresh failed, and the result code is set (there was an error)
	// and auto-connect-to-backup is set and there's a backup IP and port # set...
	if(m_bAutoConnectToBackup && m_pBackupIP != NULL && m_nBackupPort != 0)
		{
		// swap the backup and main ip/port
		LPTSTR pIP = m_pBackupIP;
		m_pBackupIP = m_pIP;
		m_pIP = pIP;
		int nPort = m_nBackupPort;
		m_nBackupPort = m_nPort;
		m_nPort = nPort;

		::Sleep(3000);

		// get a new certificate
		TRACE("Switching to server at %s %d\n", pIP, nPort);
		return true;
		}

	return false;
	}

//--------------------------------------------------------------------------------
void CSecurityConnection::DoPostMessage(int nMsg)
	{
	if(nMsg == MsgNone)
		return;

	switch(m_nTargetType)
		{
		case TargetThread:
			m_target.pThrd->PostThreadMessage(m_nMsg, (WPARAM)m_pContainer->m_pCert, (LPARAM)nMsg);
			break;
		case TargetCWnd:
			m_target.pWnd->PostMessage(m_nMsg, (WPARAM)m_pContainer->m_pCert, (LPARAM)nMsg);
			break;
		case TargetHWND:
			::PostMessage(m_target.hWnd, m_nMsg, (WPARAM)m_pContainer->m_pCert, (LPARAM)nMsg);
			break;
		case TargetThreadId:
			::PostThreadMessage(m_target.nThreadId, m_nMsg, (WPARAM)m_pContainer->m_pCert, (LPARAM)nMsg);
			break;
		default:
			break;
		}
	}

//--------------------------------------------------------------------------------
DWORD CSecurityConnection::GetThreadLoopSleepInterval() const
	{
	return m_nSleepInterval;
	}

//--------------------------------------------------------------------------------
LPCTSTR CSecurityConnection::GetIP() const
	{
	return m_pIP;
	}

//--------------------------------------------------------------------------------
UINT CSecurityConnection::GetPort() const
	{
	return m_nPort;
	}

//--------------------------------------------------------------------------------
LPCTSTR CSecurityConnection::GetBackupIP() const
	{
	return m_pBackupIP;
	}

//--------------------------------------------------------------------------------
UINT CSecurityConnection::GetBackupPort() const
	{
	return m_nBackupPort;
	}

//--------------------------------------------------------------------------------
CCertificate* CSecurityConnection::GetCertificate() const
	{
	return m_pContainer->m_pCert;
	}

//--------------------------------------------------------------------------------
void CSecurityConnection::SetTarget(CWinThread* pThread, UINT nMsg)
	{
	m_target.pThrd = pThread;
	m_nMsg = nMsg;
	m_nTargetType = TargetThread;
	}

//--------------------------------------------------------------------------------
void CSecurityConnection::SetTarget(CWnd* pWnd, UINT nMsg)
	{
	m_target.pWnd = pWnd;
	m_nMsg = nMsg;
	m_nTargetType = TargetCWnd;
	}

//--------------------------------------------------------------------------------
void CSecurityConnection::SetTarget(HWND hWnd, UINT nMsg)
	{
	m_target.hWnd = hWnd;
	m_nMsg = nMsg;
	m_nTargetType = TargetHWND;
	}

//--------------------------------------------------------------------------------
void CSecurityConnection::SetTarget(DWORD nThreadId, UINT nMsg)
	{
	m_target.nThreadId = nThreadId;
	m_nMsg = nMsg;
	m_nTargetType = TargetThreadId;
	}

//--------------------------------------------------------------------------------
void CSecurityConnection::SetTargetNone()
	{
	memset(&m_target, 0, sizeof(m_target));
	m_nMsg = 0;
	m_nTargetType = TargetNone;
	}

//--------------------------------------------------------------------------------
DWORD CSecurityConnection::GetNewCertificate(DWORD nUserClass, DWORD nTokenId)
	{
	if(m_hThread == NULL)
		StartThread();

	DWORD nRv = DoGetNewCertificate(nUserClass, nTokenId);
	if(nRv == 0xFFFFFFFF)
		if(SwitchToBackup())
			nRv = DoGetNewCertificate(nUserClass, nTokenId);

	m_bObtainedInitial = m_bObtainedInitial || (nRv != 0xFFFFFFFF);
	return nRv;
	}

//--------------------------------------------------------------------------------
DWORD CSecurityConnection::DoGetNewCertificate(DWORD nUserClass, DWORD nTokenId)
	{
	CWriteLock lock(m_pContainer, false);
	if(! lock.Lock(CONTAINER_LOCK_WAIT))
		{
		TRACE("Lock failure (13)\n");
		return 0xFFFFFFFF;
		}

	delete m_pContainer->m_pCert;
	m_pContainer->m_pCert = NULL;

	SetObtained(false);
	m_nUserClass = nUserClass;
	m_nTokenIdList.SetAtGrow(0, nTokenId);

	delete m_pSocket;
	m_pSocket = NULL;

TRACE("GetNewCertificate %d\n", 1);
	UINT nPort = 0;

	{// begin scope of socket
	CSmallSocket socket;

	if(! socket.Create())
		{
		SetResultCode(ErrCantCreateSocket);
		DoPostMessage(MsgCommError);
		OnError();
		return 0xFFFFFFFF;
		}

TRACE("GetNewCertificate %d\n", 2);
	socket.NonBlockingMode();

	// open a connection to the server
	if(! socket.Connect(GetIP(), GetPort()))
		{
		SetResultCode(ErrCantConnectToServer);
		DoPostMessage(MsgCommError);
		OnError();
		return 0xFFFFFFFF;
		}

TRACE("GetNewCertificate %d\n", 3);
	// form our request
	char sTemp[4096];
	// get the server hello string
	if(socket.Receive(sTemp, 128, CSmallSocket::WAIT, RECEIVEWAIT1) == SOCKET_ERROR)
		{
		SetResultCode(ErrCantConnectToServer);
		DoPostMessage(MsgCommError);
		OnError();
		return 0xFFFFFFFF;
		}

TRACE("GetNewCertificate %d\n", 4);
	// the server isnt running so exit
	if(memcmp(sTemp, "000", 3) != 0)
		{
		SetResultCode(ErrSocketReceive);
		DoPostMessage(MsgCommError);
		OnError();
		return 0xFFFFFFFF;
		}

TRACE("GetNewCertificate %d\n", 5);
	delete m_pContainer->m_pCert;
	m_pContainer->m_pCert = new CCertificate(CMD_NEW, nUserClass, nTokenId);
	int nLen = m_pContainer->m_pCert->Encode(sTemp, false);

TRACE("GetNewCertificate %d\n", 6);
	// send the request
	if(socket.Send(sTemp, nLen, CSmallSocket::WAITFORWOULDBLOCK, SENDWAIT1) != nLen)
		{
		m_pContainer->DeleteCertificate();
		SetResultCode(ErrSocketSend);
		DoPostMessage(MsgCommError);
		OnError();
		return 0xFFFFFFFF;
		}

	nLen = socket.Receive(sTemp, 4096, CSmallSocket::WAIT, RECEIVEWAIT2);
TRACE("GetNewCertificate %d\n", 7);

	// get our new certificate
	if(nLen == SOCKET_ERROR)
		{
		m_pContainer->DeleteCertificate();
		SetResultCode(ErrSocketReceive);
		DoPostMessage(MsgCommError);
		OnError();
		return 0xFFFFFFFF;
		}

	::Sleep(100);

TRACE("GetNewCertificate %d\n", 8);
	m_pContainer->m_pCert->Decode(sTemp, nLen);

	while(socket.GetBytesAvail() == 0)
		::Sleep(100);

	nLen = socket.Receive(&nPort, sizeof(UINT), CSmallSocket::WAIT, RECEIVEWAIT2);

TRACE("GetNewCertificate %d\n", 9);
	// get our refresh port #
	if(nLen != sizeof(UINT))
		{
		TRACE("1\n");
		m_pContainer->DeleteCertificate();
		SetResultCode(ErrSocketReceive);
		DoPostMessage(MsgCommError);
		OnError();
		return 0xFFFFFFFF;
		}

TRACE("GetNewCertificate %d\n", 10);
	if(m_pContainer->m_pCert->GetSecondsToLive() < 30)
		m_nSleepInterval = m_pContainer->m_pCert->GetSecondsToLive() * 1000;
	else
		m_nSleepInterval = 30000;

	m_pSocket = new CSmallSocket;
	if(! m_pSocket->Create())
		{
		TRACE("2\n");
		m_pContainer->DeleteCertificate();
		SetResultCode(ErrCantCreateSocket);
		DoPostMessage(MsgCommError);
		OnError();
		return 0xFFFFFFFF;
		}
	} // end scope of socket

TRACE("GetNewCertificate %d\n", 11);
	m_pSocket->NonBlockingMode();	

	if(! m_pSocket->Connect(GetIP(), nPort))
		{
		TRACE("3\n");
		m_pContainer->DeleteCertificate();
		SetResultCode(ErrCantConnectToServer);
		SetOSResult(::WSAGetLastError());
		DoPostMessage(MsgCommError);
		OnError();
		return 0xFFFFFFFF;
		}

TRACE("GetNewCertificate %d\n", 12);
	m_pSocket->Send(&nPort, sizeof(UINT), CSmallSocket::WAITFORWOULDBLOCK, SENDWAIT1);

	if(m_pContainer->m_pCert->GetCommandParam() != INFO_OK || m_pContainer->m_pCert->GetId() == 0)
		{
		m_pContainer->DeleteCertificate();
		DoPostMessage(MsgNotIssued);
		OnError();
		return 0xFFFFFFFF;
		}

TRACE("GetNewCertificate %d\n", 13);

	SetObtained();
	DoPostMessage(MsgNew);
	OnNewCertificate();
	return m_pContainer->m_pCert->GetId();
	}

//--------------------------------------------------------------------------------
bool CSecurityConnection::RefreshCertificate()
	{
	CWriteLock lock(m_pContainer, false);
	if(! lock.Lock(CONTAINER_LOCK_WAIT))
		{
		TRACE("Lock failure (14)\n");
		return false;
		}

	SetObtained(false);
	if(m_pContainer->m_pCert == NULL)
		{ 
		SetResultCode(ErrNULLCertificate);
		DoPostMessage(MsgGeneralError);
		OnError();
		return false;
		}

	DWORD nId = m_pContainer->m_pCert->GetId();

	if(m_pSocket == NULL)
		{
		m_pContainer->DeleteCertificate();
		SetResultCode(ErrNULLSocket);
		DoPostMessage(MsgCommError);
		OnError();
		return false;
		}

	// form our request
	char sTemp[4096];
	m_pContainer->m_pCert->SetCommand(CMD_REFRESH);
	int nLen = m_pContainer->m_pCert->Encode(sTemp, false);

	// send the request
	if(m_pSocket->Send(sTemp, nLen, CSmallSocket::WAITFORWOULDBLOCK, SENDWAIT1) != nLen)
		{
		m_pContainer->DeleteCertificate();
		SetResultCode(ErrSocketSend);
		DoPostMessage(MsgCommError);
		OnError();
		return false;
		}

	nLen = m_pSocket->Receive(sTemp, 4096, CSmallSocket::WAITFORWOULDBLOCK, RECEIVEWAIT3);

	// get our new certificate
	if(nLen == SOCKET_ERROR)
		{
		m_pContainer->DeleteCertificate();
		SetResultCode(ErrSocketReceive);
		DoPostMessage(MsgCommError);
		OnError();
		return false;
		}

	m_pContainer->m_pCert->Decode(sTemp, nLen);

	if(m_pContainer->m_pCert->GetSecondsToLive() < 30)
		m_nSleepInterval = m_pContainer->m_pCert->GetSecondsToLive() * 1000;

	SetResultCode(0);
	SetOSResult(0);

	if(m_pContainer->m_pCert->GetCommandParam() != INFO_OK)
		{
		m_pContainer->DeleteCertificate();
		DoPostMessage(MsgRevoked);
		OnRevoked();
		return false;
		}

	SetObtained();
	DoPostMessage(MsgRefreshed);
	OnRefresh();
	return true;
	}

//--------------------------------------------------------------------------------
bool CSecurityConnection::ReleaseCertificate()
	{
	CWriteLock lock(m_pContainer, false);
	if(! lock.Lock(CONTAINER_LOCK_WAIT))
		{
		TRACE("Lock failure (15)\n");
		return false;
		}

	SetObtained(false);
	m_bObtainedInitial = false;
	if(m_pContainer->m_pCert == NULL)
		{
		SetResultCode(ErrNULLCertificate);
		DoPostMessage(MsgGeneralError);
		OnError();
		return false;
		}

	DWORD nId = m_pContainer->m_pCert->GetId();

	if(m_pSocket == NULL)
		{
		m_pContainer->DeleteCertificate();
		SetResultCode(ErrNULLSocket);
		DoPostMessage(MsgCommError);
		OnError();
		return false;
		}

	// form our request
	char sTemp[4096];
	m_pContainer->m_pCert->SetCommand(CMD_RELEASE);
	m_pContainer->m_pCert->SetCommandParam(0);

	int nLen = m_pContainer->m_pCert->Encode(sTemp, false);

	DoPostMessage(MsgReleased);

	m_pContainer->DeleteCertificate();

	// send the request
	if(m_pSocket->Send(sTemp, nLen, CSmallSocket::WAITFORWOULDBLOCK, SENDWAIT1) != nLen)
		{
		SetResultCode(ErrSocketSend);
		DoPostMessage(MsgCommError);
		OnError();
		return false;
		}

	SetResultCode(0);
	return true;
	}

//--------------------------------------------------------------------------------
bool CSecurityConnection::AquireToken(ULONG nTokenId)
	{
	CWriteLock lock(m_pContainer, false);
	if(! lock.Lock(CONTAINER_LOCK_WAIT))
		{
		TRACE("Lock failure (14)\n");
		return false;
		}

	if(m_pContainer->m_pCert == NULL)
		{ 
		SetResultCode(ErrNULLCertificate);
		DoPostMessage(MsgGeneralError);
		OnError();
		return false;
		}

	if(m_pSocket == NULL)
		{
		m_pContainer->DeleteCertificate();
		SetResultCode(ErrNULLSocket);
		DoPostMessage(MsgCommError);
		OnError();
		return false;
		}

	// form our request
	char sTemp[4096];
	m_pContainer->m_pCert->SetCommand(CMD_NEW);
	m_pContainer->m_pCert->SetCommandParam(nTokenId);

	int nLen = m_pContainer->m_pCert->Encode(sTemp, false);

	// send the request
	if(m_pSocket->Send(sTemp, nLen, CSmallSocket::WAITFORWOULDBLOCK, SENDWAIT1) != nLen)
		{
		m_pContainer->DeleteCertificate();
		SetResultCode(ErrSocketSend);
		DoPostMessage(MsgCommError);
		OnError();
		return false;
		}

	nLen = m_pSocket->Receive(sTemp, 4096, CSmallSocket::WAITFORWOULDBLOCK, RECEIVEWAIT3);

	// get our certificate with the new token
	if(nLen == SOCKET_ERROR)
		{
		m_pContainer->DeleteCertificate();
		SetResultCode(ErrSocketReceive);
		DoPostMessage(MsgCommError);
		OnError();
		return false;
		}

	m_pContainer->m_pCert->Decode(sTemp, nLen);

	if(m_pContainer->m_pCert->GetSecondsToLive() < 30)
		m_nSleepInterval = m_pContainer->m_pCert->GetSecondsToLive() * 1000;

	SetResultCode(0);
	SetOSResult(0);

	if(m_pContainer->m_pCert->GetCommandParam() != INFO_OK)
		{
		m_pContainer->DeleteCertificate();
		DoPostMessage(MsgRevoked);
		OnRevoked();
		return false;
		}

	m_nTokenIdList.Add(nTokenId);

	SetObtained();
	DoPostMessage(MsgNew);
	return true;
	}

//--------------------------------------------------------------------------------
bool CSecurityConnection::ReleaseToken(ULONG nId)
	{
	CWriteLock lock(m_pContainer, false);
	if(! lock.Lock(CONTAINER_LOCK_WAIT))
		{
		TRACE("Lock failure (15)\n");
		return false;
		}

	if(m_pContainer->m_pCert == NULL)
		{
		SetResultCode(ErrNULLCertificate);
		DoPostMessage(MsgGeneralError);
		OnError();
		return false;
		}

	for(int i = 0; i < m_nTokenIdList.GetSize(); i++)
		if(m_nTokenIdList[i] == nId)
			{
			m_nTokenIdList.RemoveAt(i);
			break;
			}

	if(m_pSocket == NULL)
		{
		m_pContainer->DeleteCertificate();
		SetResultCode(ErrNULLSocket);
		DoPostMessage(MsgCommError);
		OnError();
		return false;
		}

	// form our request
	char sTemp[4096];
	m_pContainer->m_pCert->SetCommand(CMD_RELEASE);
	m_pContainer->m_pCert->SetCommandParam(nId);

	int nLen = m_pContainer->m_pCert->Encode(sTemp, false);

	// send the request
	if(m_pSocket->Send(sTemp, nLen, CSmallSocket::WAITFORWOULDBLOCK, SENDWAIT1) != nLen)
		{
		SetResultCode(ErrSocketSend);
		DoPostMessage(MsgCommError);
		OnError();
		return false;
		}

	SetResultCode(0);
	return true;
	}

//--------------------------------------------------------------------------------
bool CSecurityConnection::OnNewCertificate()
	{
	return true;
	}

//--------------------------------------------------------------------------------
bool CSecurityConnection::OnRevoked()
	{
	SetObtained(false);
	return true;
	}

//--------------------------------------------------------------------------------
bool CSecurityConnection::OnRefresh()
	{
	return true;
	}

//--------------------------------------------------------------------------------
bool CSecurityConnection::OnError()
	{
	return true;
	}

//--------------------------------------------------------------------------------
bool CSecurityConnection::IsValidCertificate()
	{
	if(m_pContainer == NULL)
		return false;

	CReadLock lock(m_pContainer);

	if(m_pContainer->m_pCert == NULL)
		return false;
	
	return (m_pContainer->m_pCert->GetCommand() == CMD_NEW || m_pContainer->m_pCert->GetCommand() == CMD_REFRESH) 
		&& m_pContainer->m_pCert->GetCommandParam() == INFO_OK;
	}

//--------------------------------------------------------------------------------
bool CSecurityConnection::HasToken(DWORD nId)
	{
	if(m_pContainer == NULL)
		return false;

	CReadLock lock(m_pContainer);

	if(m_pContainer->m_pCert == NULL)
		return false;

	return m_pContainer->m_pCert->HasToken(nId);
	}

//--------------------------------------------------------------------------------
LPCTSTR CSecurityConnection::GetMessageText(int n1, int n2)
	{
	static struct
		{
		int nId1;
		int nId2;
		LPCTSTR pMsg;
		} messages[] =

		{
		CMD_NEW,		INFO_OK,				"New certificate issued",
		CMD_NEW,		ERR_NO_SUCH_TOKEN,		"Certificate not issued - no such token",
		CMD_NEW,		ERR_TOKEN_NOT_AVAIL,	"Certificate not issued - token not available",
		CMD_NEW,		0,						"Certificate not issued - unknown error",
		CMD_REFRESH,	INFO_OK,				"Certificate refreshed",
		CMD_REFRESH,	ERR_NO_SUCH_CERT,		"Certificate not refreshed - no such certificate",
		CMD_REFRESH,	INFO_REVOKED,			"Certificate not refreshed - certificate revoked",
		CMD_REFRESH,	0,						"Certificate not refreshed - unknown error",
		CMD_RELEASE,	0,						"Certificate released",
		0,				0,						""
		};

	int nEndIndex = 0;

	// find an exact match
	for(int i = 0; ;i++)
		{
		if(n1 == messages[i].nId1 && n2 == messages[i].nId2)
			return messages[i].pMsg;

		if(messages[i].nId1 == 0 && messages[i].nId2 == 0)
			{
			nEndIndex = i;
			break;
			}
		}

	// ok so then match the first when the second is 0
	for(i = 0; i < nEndIndex;i++)
		{
		if(n1 == messages[i].nId1 && messages[i].nId2 == 0)
			return messages[i].pMsg;
		}

	// still no match so return the empty string
	return messages[nEndIndex].pMsg;
	}
