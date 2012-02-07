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
		, m_nLastError(0)
	{
	}

//--------------------------------------------------------------------------------
CSmallSocket::CSmallSocket(SOCKET sock, bool bIsBlocking)
		: m_hSocket(sock)
		, m_bIsBlockingSocket(bIsBlocking)
		, m_nLastError(0)
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
		{
		SetLastError(WSAEINVAL);
		return false;
		}

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
	SetLastError(::WSAGetLastError());
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

	if(bRv && ! m_bIsBlockingSocket)
		NonBlockingMode();

	SetLastError(::WSAGetLastError());
	return bRv;
	}

//--------------------------------------------------------------------------------
bool CSmallSocket::Accept(CSmallSocket& rConnectedSocket, SOCKADDR* lpSockAddr, int* lpSockAddrLen, DWORD nTimeout)
	{
	if(m_hSocket == INVALID_SOCKET)
		{
		SetLastError(WSAEINVAL);
		return false;
		}

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
	if(bRv && ! m_bIsBlockingSocket)
		rConnectedSocket.NonBlockingMode();
	SetLastError(::WSAGetLastError());
	return bRv;
	}

//--------------------------------------------------------------------------------
bool CSmallSocket::IOCtl(long lCommand, DWORD* lpArgument)
	{
	if(m_hSocket == INVALID_SOCKET)
		{
		SetLastError(WSAEINVAL);
		return false;
		}

	bool bRv = ::ioctlsocket(m_hSocket, lCommand, lpArgument) == 0;
	// if ioctksocket succeeded and we're setting blocking mode...
	if(bRv && lCommand == FIONBIO)
		m_bIsBlockingSocket = *lpArgument == 0;
	SetLastError(::WSAGetLastError());
	return bRv;
	}

//--------------------------------------------------------------------------------
bool CSmallSocket::AsyncSelect(HWND hWnd, UINT nMsg, long nEvent)
	{
	if(m_hSocket == INVALID_SOCKET)
		{
		SetLastError(WSAEINVAL);
		return false;
		}

	bool bRv = ::WSAAsyncSelect(m_hSocket, hWnd, nMsg, nEvent) == 0;
	if(bRv)
		// WSAAsyncSelect automatically sets the socket to non-blocking
		m_bIsBlockingSocket = false;

	SetLastError(::WSAGetLastError());
	return bRv;
	}

//--------------------------------------------------------------------------------
bool CSmallSocket::BlockingMode(bool bOn)
	{
	DWORD nCmd = bOn ? 0 : 1;
	bool bRv = IOCtl(FIONBIO, &nCmd);
	SetLastError(::WSAGetLastError());
	return bRv;
	}

//--------------------------------------------------------------------------------
bool CSmallSocket::NonBlockingMode(bool bOn)
	{
	DWORD nCmd = bOn ? 1 : 0;
	bool bRv = IOCtl(FIONBIO, &nCmd);
	SetLastError(::WSAGetLastError());
	return bRv;
	}

//--------------------------------------------------------------------------------
bool CSmallSocket::Bind(UINT nPort, LPCTSTR pAddr)
	{
	if(m_hSocket == INVALID_SOCKET)
		{
		SetLastError(WSAEINVAL);
		return false;
		}

 	SOCKADDR_IN sockAddr;
	if(! ConvertInAddr(pAddr, nPort, &sockAddr))
		return false;
	bool bRv = bind(m_hSocket, (SOCKADDR*)&sockAddr, sizeof(sockAddr)) == 0;
	SetLastError(::WSAGetLastError());
	return bRv;
	}

//--------------------------------------------------------------------------------
void CSmallSocket::Close()
	{
	if(m_hSocket == INVALID_SOCKET)
		{
		SetLastError(WSAEINVAL);
		return;
		}

	closesocket(m_hSocket);
	m_hSocket = INVALID_SOCKET;
	SetLastError(::WSAGetLastError());
	}

//--------------------------------------------------------------------------------
bool CSmallSocket::Connect(LPCTSTR pHostAddr, UINT nHostPort, DWORD nWait)
	{ 
	if(m_hSocket == INVALID_SOCKET)
		{
		SetLastError(WSAEINVAL);
		return false;
		}

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

		nErr = ::WSAGetLastError();
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
			{
			SetLastError(::WSAGetLastError());
			return false;
			}

		if(nErr == 1)
			{
			SetLastError(0);
			return true;
			}

		if(nWait == 0)
			{
			SetLastError(::WSAGetLastError());
			return false;
			}

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
		{
		SetLastError(WSAEINVAL);
		return false;
		}

	bool bRv = listen(m_hSocket, nConnectionBacklog) == 0;
	SetLastError(::WSAGetLastError());
	return bRv;
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
		{
		SetLastError(WSAEINVAL);
		return SOCKET_ERROR;
		}

	DWORD nCustFlags = nFlags & FLAGMASK_SMALLSOCKET;
	nFlags &= FLAGMASK_WINSOCK;

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
			{
			SetLastError(0);
			return nRv;
			}

		nRv = ::WSAGetLastError();

		// got an error other than WSAEWOULDBLOCK
		if(nRv != WSAEWOULDBLOCK)
			{
			STRACE1("CSmallSocket::Receive failed %ld\n", nRv);
			SetLastError(nRv);
			return SOCKET_ERROR;
			}

		if(! m_bIsBlockingSocket)
			{
			if(((nCustFlags & WAITFORDATA) == WAITFORDATA) && WaitForBytesAvail(nWait, 0))
				continue;
			STRACE1("CSmallSocket::Receive No data available\n");
			SetLastError(WSAEWOULDBLOCK);
			return SOCKET_ERROR;
			}

		// we're not waiting for WSAEWOULDBLOCK to clear so return now
		if((nCustFlags & WAITFORWOULDBLOCK) != WAITFORWOULDBLOCK)
			{
			STRACE1("CSmallSocket::Receive socket is blocked \n");
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
		{
		SetLastError(WSAEINVAL);
		return SOCKET_ERROR;
		}

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
				int nErr = ::WSAGetLastError();
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

	SetLastError(::WSAGetLastError());
	return nBufLen - nLeft;
	}

//--------------------------------------------------------------------------------
int CSmallSocket::GetBytesAvail()
	{
	if(m_hSocket == INVALID_SOCKET)
		{
		SetLastError(WSAEINVAL);
		return -1;
		}

	ULONG nBytes;
	if(::ioctlsocket(m_hSocket, FIONREAD, &nBytes) == SOCKET_ERROR)
		{
		SetLastError(::WSAGetLastError());
		return -1;
		}

	SetLastError(::WSAGetLastError());
	return (int) nBytes;
	}

//--------------------------------------------------------------------------------
bool CSmallSocket::WaitForBytesAvail(DWORD nTimeout, UINT nMinBytes)
	{
	if(m_hSocket == INVALID_SOCKET)
		{
		SetLastError(WSAEINVAL);
		return false;
		}

	CPulseTimer timer;
	if(nTimeout != INFINITE)
		timer.Set(nTimeout);

	for(ULONG nBytes = 0;;)
		{
		if(::ioctlsocket(m_hSocket, FIONREAD, &nBytes) == SOCKET_ERROR)
			{
			SetLastError(::WSAGetLastError());
			return false;
			}

		if(nBytes > nMinBytes)
			{
			SetLastError(::WSAGetLastError());
			return true;
			}

		if(nTimeout == INFINITE)
			::Sleep(100);
		else
			if(timer.Wait(100))
				{
				SetLastError(WSAETIMEDOUT);
				return false;
				}
		}
	}

//--------------------------------------------------------------------------------
bool CSmallSocket::GetSockName(SOCKADDR* lpSockAddr, int* lpSockAddrLen)
	{
	if(m_hSocket == INVALID_SOCKET)
		{
		SetLastError(WSAEINVAL);
		return false;
		}

	bool bRv = getsockname(m_hSocket, lpSockAddr, lpSockAddrLen) != SOCKET_ERROR;
	SetLastError(::WSAGetLastError());
	return bRv;
	}

//--------------------------------------------------------------------------------
bool CSmallSocket::GetSockName(CString& rSocketAddress, UINT& rSocketPort)
	{
	if(m_hSocket == INVALID_SOCKET)
		{
		SetLastError(WSAEINVAL);
		return false;
		}

	SOCKADDR_IN sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));

	int nSockAddrLen = sizeof(sockAddr);

	bool bResult = GetSockName((SOCKADDR*)&sockAddr, &nSockAddrLen);
	if(bResult)
		{
		rSocketPort = ntohs(sockAddr.sin_port);
		rSocketAddress = inet_ntoa(sockAddr.sin_addr);
		}

	SetLastError(::WSAGetLastError());
	return bResult;
	}

//--------------------------------------------------------------------------------
bool CSmallSocket::GetPeerName(SOCKADDR* lpSockAddr, int* lpSockAddrLen)
	{
	if(m_hSocket == INVALID_SOCKET)
		{
		SetLastError(WSAEINVAL);
		return false;
		}

	bool bRv = getpeername(m_hSocket, lpSockAddr, lpSockAddrLen) != SOCKET_ERROR;
	SetLastError(::WSAGetLastError());
	return bRv;
	}

//--------------------------------------------------------------------------------
bool CSmallSocket::GetPeerName(CString& rSocketAddress, UINT& rSocketPort)
	{
	if(m_hSocket == INVALID_SOCKET)
		{
		SetLastError(WSAEINVAL);
		return false;
		}

	SOCKADDR_IN sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));

	int nSockAddrLen = sizeof(sockAddr);

	bool bResult = GetPeerName((SOCKADDR*)&sockAddr, &nSockAddrLen);
	if(bResult)
		{
		rSocketPort = ntohs(sockAddr.sin_port);
		rSocketAddress = inet_ntoa(sockAddr.sin_addr);
		}

	SetLastError(::WSAGetLastError());
	return bResult;
	}

//--------------------------------------------------------------------------------
bool CSmallSocket::GetSockOpt(int nLevel, int nOptName, void* pOptVal, int* pOptLen)
	{
	if(m_hSocket == INVALID_SOCKET)
		{
		SetLastError(WSAEINVAL);
		return false;
		}

	bool bRv = ::getsockopt(m_hSocket, nLevel, nOptName, (char*) pOptVal, pOptLen) != SOCKET_ERROR;
	SetLastError(::WSAGetLastError());
	return bRv;
	}

//--------------------------------------------------------------------------------
bool CSmallSocket::SetSockOpt(int nLevel, int nOptName, const void* pOptVal, int nOptLen)
	{
	if(m_hSocket == INVALID_SOCKET)
		{
		SetLastError(WSAEINVAL);
		return false;
		}

	bool bRv = ::setsockopt(m_hSocket, nLevel, nOptName, (const char*) pOptVal, nOptLen) != SOCKET_ERROR;
	SetLastError(::WSAGetLastError());
	return bRv;
	}
