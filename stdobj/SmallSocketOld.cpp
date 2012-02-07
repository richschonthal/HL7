//--------------------------------------------------------------------------------
//
// Copyright (c) 2001 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#include "stdafx.h"
#include "SmallSocket.h"
#include "Timer.h"

#define STRACE1

#include <atlconv.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//--------------------------------------------------------------------------------
CSmallSocket::CSmallSocket(bool bIsBlocking)
		: m_hSocket(INVALID_SOCKET)
		, m_bIsBlockingSocket(bIsBlocking)
	{
	}

//--------------------------------------------------------------------------------
CSmallSocket::~CSmallSocket()
	{
	if(m_hSocket != INVALID_SOCKET)
		Close();
	}

//--------------------------------------------------------------------------------
bool CSmallSocket::Attach(SOCKET hSocket)
	{
	if(m_hSocket != INVALID_SOCKET)
		return false;

	m_hSocket = hSocket;
	return true;
	}

//--------------------------------------------------------------------------------
SOCKET CSmallSocket::Detach()
	{
	SOCKET hSock = m_hSocket;
	m_hSocket = INVALID_SOCKET;
	return hSock;
	}

//--------------------------------------------------------------------------------
bool CSmallSocket::ConvertInAddr(LPCTSTR pIn, int nSocketPort, SOCKADDR_IN* pOut)
	{
	USES_CONVERSION;

	::ZeroMemory(pOut, sizeof(SOCKADDR_IN));

	LPSTR lpszAscii = T2A((LPTSTR) pIn);
	pOut->sin_family = AF_INET;

	if (lpszAscii == NULL)
		pOut->sin_addr.s_addr = htonl(INADDR_ANY);
	else
		{
		DWORD lResult = inet_addr(lpszAscii);
		if (lResult == INADDR_NONE)
			{
			SetLastError(WSAEINVAL);
			STRACE1("CSmallSocket::ConvertInAddr failed %ld\n", WSAEINVAL);
			return false;
			}
		pOut->sin_addr.s_addr = lResult;
		}

	pOut->sin_port = htons((u_short)nSocketPort);
	return true;
	}

//--------------------------------------------------------------------------------
bool CSmallSocket::Create(UINT nSocketPort, LPCTSTR lpszSocketAddress)
	{
	ASSERT(m_hSocket == INVALID_SOCKET);

	m_hSocket = socket(PF_INET, SOCK_STREAM, 0);

	bool bRv = m_hSocket != INVALID_SOCKET;
	if(nSocketPort != (UINT) -1 && bRv)
		bRv = Bind(nSocketPort,lpszSocketAddress);

	if(! bRv)
		STRACE1("CSmallSocket::Create failed %ld\n", GetLastError());

	if(bRv && ! m_bIsBlockingSocket)
		NonBlockingMode();

	return bRv;
	}

//--------------------------------------------------------------------------------
bool CSmallSocket::Accept(CSmallSocket& rConnectedSocket, SOCKADDR* lpSockAddr, int* lpSockAddrLen, DWORD nTimeout)
	{
	if(m_hSocket == INVALID_SOCKET)
		return false;

	CPulseTimer timer;
	if(nTimeout != INFINITE)
		timer.Set(nTimeout);

	for(;;)
		{
		if((rConnectedSocket.m_hSocket = accept(m_hSocket, lpSockAddr, lpSockAddrLen)) != INVALID_SOCKET)
			break;

		if(nTimeout == INFINITE)
			::Sleep(100);
		else
			if(timer.Wait(100))
				break;
		}
 
	bool bRv = rConnectedSocket.m_hSocket != INVALID_SOCKET;
	
	if(! bRv && GetLastError() != WSAEWOULDBLOCK)
		STRACE1("CSmallSocket::Accept failed %ld\n", GetLastError());

	if(bRv && ! m_bIsBlockingSocket)
		rConnectedSocket.NonBlockingMode();
	return bRv;
	}

//--------------------------------------------------------------------------------
bool CSmallSocket::IOCtl(long lCommand, DWORD* lpArgument)
	{
	if(m_hSocket == INVALID_SOCKET)
		return false;

	bool bRv = ::ioctlsocket(m_hSocket, lCommand, lpArgument) == 0;

	if(! bRv)
		STRACE1("CSmallSocket::IOCtl failed %ld\n", GetLastError());

	// if ioctksocket succeeded and we're setting blocking mode...
	if(bRv && lCommand == FIONBIO)
		m_bIsBlockingSocket = *lpArgument == 0;
	return bRv;
	}

//--------------------------------------------------------------------------------
bool CSmallSocket::AsyncSelect(HWND hWnd, UINT nMsg, long nEvent)
	{
	if(m_hSocket == INVALID_SOCKET)
		return false;

	bool bRv = ::WSAAsyncSelect(m_hSocket, hWnd, nMsg, nEvent) == 0;

	if(bRv)
		// WSAAsyncSelect automatically sets the socket to non-blocking
		m_bIsBlockingSocket = false;

	return bRv;
	}

//--------------------------------------------------------------------------------
bool CSmallSocket::BlockingMode(bool bOn)
	{
	DWORD nCmd = bOn ? 0 : 1;
	return IOCtl(FIONBIO, &nCmd);
	}

//--------------------------------------------------------------------------------
bool CSmallSocket::NonBlockingMode(bool bOn)
	{
	DWORD nCmd = bOn ? 1 : 0;
	return IOCtl(FIONBIO, &nCmd);
	}

//--------------------------------------------------------------------------------
bool CSmallSocket::Bind(UINT nPort, LPCTSTR pAddr)
	{
	if(m_hSocket == INVALID_SOCKET)
		return false;

 	SOCKADDR_IN sockAddr;
	if(! ConvertInAddr(pAddr, nPort, &sockAddr))
		return false;

	bool bRv = bind(m_hSocket, (SOCKADDR*)&sockAddr, sizeof(sockAddr)) == 0;

	if(! bRv)
		STRACE1("CSmallSocket::Bind failed %ld (%d %s)\n", GetLastError(), nPort, pAddr);

	return bRv;
	}

//--------------------------------------------------------------------------------
void CSmallSocket::Close()
	{
	if(m_hSocket == INVALID_SOCKET)
		return;

	closesocket(m_hSocket);
	m_hSocket = INVALID_SOCKET;
	}

//--------------------------------------------------------------------------------
bool CSmallSocket::Connect(LPCTSTR pHostAddr, UINT nHostPort, DWORD nWait)
	{ 
	if(m_hSocket == INVALID_SOCKET)
		return false;

 	SOCKADDR_IN sockAddr;
	if(! ConvertInAddr(pHostAddr, nHostPort, &sockAddr))
		return false;

	DWORD nErr = 0;
	CPulseTimer timer;
	if(nWait != INFINITE)
		timer.Set(nWait);

	for(;;)
		{
		if(connect(m_hSocket, (SOCKADDR*)&sockAddr, sizeof(sockAddr)) == 0)
			{
			SetLastError(0);
			return true;
			}

		nErr = GetLastError();
		if(nErr == WSAEISCONN)
			{
			SetLastError(0);
			return true;
			}

		// call select to check for writeability on m_hSocket
		struct timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 20;

		fd_set fdSet;
		fdSet.fd_count = 1;
        fdSet.fd_array[0] = m_hSocket;

		nErr = select(0, NULL, &fdSet, NULL, &timeout);

		if(nErr == SOCKET_ERROR)
			return false;

		if(nErr == 1)
			return true;

		if(nWait == 0)
			return false;

		if(nWait == INFINITE)
			::Sleep(100);
		else
			if(timer.Wait(10))
				{
				SetLastError(WSAETIMEDOUT);
				return false;
				}
		}
	}

//--------------------------------------------------------------------------------
bool CSmallSocket::Listen(int nConnectionBacklog)
	{
	if(m_hSocket == INVALID_SOCKET)
		return false;

	return listen(m_hSocket, nConnectionBacklog) == 0;
	}

//--------------------------------------------------------------------------------
int CSmallSocket::Receive(CString& sData, int nFlags, unsigned long nWait)
	{
	LPTSTR pBuf = sData.GetBuffer(1024);
	int nRv = Receive(pBuf, 1024, nFlags, nWait);
	sData.ReleaseBuffer(nRv > -1 ? nRv : 0);
	return nRv;
	}

//--------------------------------------------------------------------------------
int CSmallSocket::Receive(void* lpBuf, int nBufLen, int nFlags, unsigned long nWait)
	{ 
	if(m_hSocket == INVALID_SOCKET)
		return SOCKET_ERROR;

	DWORD nCustFlags = nFlags & FLAGMASK_SMALLSOCKET;
	nFlags &= FLAGMASK_WINSOCK;

	// sleep in intervals of 100 or nWait milliseconds, whichever is smaller
	long nSleep = nWait < 100 ? nWait : 100;
	CPulseTimer timer;

	if(nWait != INFINITE)
		{
		timer.Set(nWait);
		STRACE1("Receive timer set to %ld\n", nWait);
		}

	for(;;)
		{
		// attempt to read
		int nRv = recv(m_hSocket, (char*) lpBuf, nBufLen, nFlags);
		if(nRv != SOCKET_ERROR)
			return nRv;

		nRv = GetLastError();

		// got an error other than WSAEWOULDBLOCK
		if(nRv != WSAEWOULDBLOCK)
			{
			STRACE1("CSmallSocket::Receive failed %ld\n", nRv);
			SetLastError(nRv);
			return SOCKET_ERROR;
			}

		if(! m_bIsBlockingSocket)
			{
			// in a non-blocking socket, WSAEWOULDBLOCK means there's no data to be read
			// if we're waiting for data...
			if((nCustFlags & WAITFORDATA) == WAITFORDATA)
				{
				if(nWait == INFINITE)
					::Sleep(nSleep);
				else
					// have we been at this too long?
					if(timer.Wait(nSleep))
						{
						STRACE1("Receive timer signaled\n");
						SetLastError(WSAEWOULDBLOCK);
						return SOCKET_ERROR;
						}
				}

			// we're not waiting for data so return immediately
			return SOCKET_ERROR;
			}

		// we're not waiting for WSAEWOULDBLOCK to clear so return now
		if((nCustFlags & WAITFORWOULDBLOCK) != WAITFORWOULDBLOCK)
			{
			SetLastError(WSAEWOULDBLOCK);
			return SOCKET_ERROR;
			}

		if(nWait == INFINITE)
			::Sleep(nSleep);
		else
			// have we been at this too long?
			if(timer.Wait(nSleep))
				{
				SetLastError(WSAEWOULDBLOCK);
				return SOCKET_ERROR;
				}
		}
	}

//--------------------------------------------------------------------------------
bool CSmallSocket::Send(const CString& sData, eSmallSocketFlags nFlags, unsigned long nWait)
	{
	int nLen = sData.GetLength();
	return Send(sData, nLen, nFlags, nWait) == nLen;
	}

//--------------------------------------------------------------------------------
int CSmallSocket::Send(const void* lpBuf, int nBufLen, eSmallSocketFlags nFlags, unsigned long nWait)
	{
	if(m_hSocket == INVALID_SOCKET)
		return SOCKET_ERROR;

	int nLeft = nBufLen;
	PBYTE pBuf = (PBYTE)lpBuf;
	int nWritten;

	bool bWaitForWouldBlock = (nFlags & WAITFORWOULDBLOCK) != 0;

	// remove our 'custom' flags
	nFlags = (eSmallSocketFlags) (((int)nFlags) & ((int)FLAGMASK_WINSOCK));

	// sleep in intervals of 100 or nWait milliseconds, whichever is smaller
	CPulseTimer timer;
	if(nWait != INFINITE)
		timer.Set(nWait);

	while(nLeft > 0)
		{
		for(;;)
			{
			nWritten = send(m_hSocket, (char*) pBuf, nBufLen, nFlags);
			if(nWritten != SOCKET_ERROR)
				break;
			else
				// it IS an error
				{
				int nErr = GetLastError();
				if(nErr != WSAEWOULDBLOCK || !bWaitForWouldBlock)
					{
					STRACE1("CSmallSocket::Send failed %ld\n", nErr);
					SetLastError(nErr);
					return SOCKET_ERROR;
					}

				// have we been at this too long?
				if(nWait == INFINITE)
					::Sleep(100);
				else
					if(timer.Wait(100))
						{
						SetLastError(WSAEWOULDBLOCK);
						return SOCKET_ERROR;
						}
				}
			}

		nLeft -= nWritten;
		pBuf += nWritten;
		}

	return nBufLen - nLeft;
	}

//--------------------------------------------------------------------------------
int CSmallSocket::GetBytesAvail()
	{
	if(m_hSocket == INVALID_SOCKET)
		return -1;
#ifdef _DEBUG
	ULONG nBytes=0;
#else
	ULONG nBytes;
#endif
	if(::ioctlsocket(m_hSocket, FIONREAD, &nBytes) == SOCKET_ERROR)
		return -1;
	int nErr = GetLastError();

	return (int) nBytes;
	}

//--------------------------------------------------------------------------------
bool CSmallSocket::WaitForBytesAvail(DWORD nTimeout, UINT nMinBytes)
	{
	if(m_hSocket == INVALID_SOCKET)
		return false;

	CPulseTimer timer;
	if(nTimeout != INFINITE)
		timer.Set(nTimeout);

	for(ULONG nBytes = 0;;)
		{
		if(::ioctlsocket(m_hSocket, FIONREAD, &nBytes) == SOCKET_ERROR)
			return false;

		if(nBytes > nMinBytes)
			return true;

		if(nTimeout == INFINITE)
			::Sleep(100);
		else
			if(timer.Wait(100))
				return false;
		}
	}

//--------------------------------------------------------------------------------
bool CSmallSocket::GetSockName(SOCKADDR* lpSockAddr, int* lpSockAddrLen)
	{
	if(m_hSocket == INVALID_SOCKET)
		return false;

	return (SOCKET_ERROR != getsockname(m_hSocket, lpSockAddr, lpSockAddrLen));
	}

//--------------------------------------------------------------------------------
bool CSmallSocket::GetSockName(CString& rSocketAddress, UINT& rSocketPort)
	{
	if(m_hSocket == INVALID_SOCKET)
		return false;

	SOCKADDR_IN sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));

	int nSockAddrLen = sizeof(sockAddr);

	bool bResult = GetSockName((SOCKADDR*)&sockAddr, &nSockAddrLen);
	if(bResult)
		{
		rSocketPort = ntohs(sockAddr.sin_port);
		rSocketAddress = inet_ntoa(sockAddr.sin_addr);
		}

	return bResult;
	}

//--------------------------------------------------------------------------------
bool CSmallSocket::GetPeerName(SOCKADDR* lpSockAddr, int* lpSockAddrLen)
	{
	if(m_hSocket == INVALID_SOCKET)
		return false;

	return (SOCKET_ERROR != getpeername(m_hSocket, lpSockAddr, lpSockAddrLen));
	}

//--------------------------------------------------------------------------------
bool CSmallSocket::GetPeerName(CString& rSocketAddress, UINT& rSocketPort)
	{
	if(m_hSocket == INVALID_SOCKET)
		return false;

	SOCKADDR_IN sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));

	int nSockAddrLen = sizeof(sockAddr);

	bool bResult = GetPeerName((SOCKADDR*)&sockAddr, &nSockAddrLen);
	if(bResult)
		{
		rSocketPort = ntohs(sockAddr.sin_port);
		rSocketAddress = inet_ntoa(sockAddr.sin_addr);
		}

	return bResult;
	}

//--------------------------------------------------------------------------------
bool CSmallSocket::GetSockOpt(int nLevel, int nOptName, void* pOptVal, int* pOptLen)
	{
	if(m_hSocket == INVALID_SOCKET)
		return false;

	return ::getsockopt(m_hSocket, nLevel, nOptName, (char*) pOptVal, pOptLen) != SOCKET_ERROR;
	}

//--------------------------------------------------------------------------------
bool CSmallSocket::SetSockOpt(int nLevel, int nOptName, const void* pOptVal, int nOptLen)
	{
	if(m_hSocket == INVALID_SOCKET)
		return false;

	return ::setsockopt(m_hSocket, nLevel, nOptName, (const char*) pOptVal, nOptLen) != SOCKET_ERROR;
	}
