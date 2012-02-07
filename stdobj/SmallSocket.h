//--------------------------------------------------------------------------------
//
// Copyright (c) 2001 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#if !defined(AFX_SMALLSOCKET_H__4EDEDDAB_2360_11D3_87BA_00104B9E6286__INCLUDED_)
#define AFX_SMALLSOCKET_H__4EDEDDAB_2360_11D3_87BA_00104B9E6286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
#include "ReadWriteObject.h"

//--------------------------------------------------------------------------------
class CSmallSocket : public CObject
	{
	friend class CSmallSocketFile;

	public:
		enum eSmallSocketFlags
			{
			FLAGDFLT = 0,
			FLAGMASK_SMALLSOCKET = 0xFFFF0000,
			FLAGMASK_WINSOCK = 0x0000FFFF,
			// flag for Send and Receive
			// causing them to check for WSAEWOULDBLOCK
			// and waiting for it to clear
			WAITFORWOULDBLOCK = 0x10000,
			// causes receive to wait until there is some data read
			WAITFORDATA = 0x20000,
			WAIT = WAITFORWOULDBLOCK|WAITFORDATA,
			};

	protected:
		SOCKET m_hSocket;
		bool m_bIsBlockingSocket;
		bool ConvertInAddr(LPCTSTR, int, SOCKADDR_IN*);
		DWORD m_nLastError;

	public:
		CSmallSocket(bool bIsBlocking = false);
		CSmallSocket(SOCKET, bool bIsBlocking = false);
		virtual ~CSmallSocket();

		virtual bool Attach(SOCKET);
		virtual SOCKET Detach();

		virtual bool Create(UINT nSocketPort = 0, LPCTSTR lpszSocketAddress = NULL );
		virtual bool Bind(UINT nSocketPort, LPCTSTR lpszSocketAddress = NULL);
		virtual bool Connect(LPCTSTR lpszHostAddress, UINT nHostPort, DWORD nWait = 2000);

		virtual bool Listen(int nConnectionBacklog = 5 );
		virtual bool Accept(CSmallSocket& rConnectedSocket, SOCKADDR* = NULL, int* lpSockAddrLen = NULL, DWORD nTimeout = 2000);
		
		virtual void Close();

		virtual bool AsyncSelect(HWND, UINT = WM_USER, long lEvent = FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE );
		virtual bool IOCtl(long lCommand, DWORD* lpArgument);
		virtual bool BlockingMode(bool = true);
		virtual bool NonBlockingMode(bool = true);

		// nWait is only valid when WAITFORWOULDBLOCK is specified
		virtual int Receive(void* lpBuf, int nBufLen, int nFlags = 0, unsigned long nWait = 0);
		virtual int Receive(CString&, int nFlags = 0, unsigned long nWait = 0);

		virtual int Send(const void* lpBuf, int nBufLen, eSmallSocketFlags nFlags = FLAGDFLT, unsigned long nWait = 0);
		// returns true if all of sData was sent
		virtual bool Send(const CString& sData, eSmallSocketFlags nFlags = FLAGDFLT, unsigned long nWait = 0);

		virtual int GetBytesAvail();
		virtual bool WaitForBytesAvail(DWORD nTimeout, UINT nMinBytes = 0);

		virtual bool GetSockName(SOCKADDR* lpSockAddr, int* lpSockAddrLen);
		virtual bool GetSockName(CString& rSocketAddress, UINT& rSocketPort);

		virtual bool GetPeerName(SOCKADDR* lpSockAddr, int* lpSockAddrLen);
		virtual bool GetPeerName(CString& rSocketAddress, UINT& rSocketPort);

		virtual bool GetSockOpt(int level, int optname, void* optval, int* optlen);
		virtual bool SetSockOpt(int nLevel, int nOptName, const void* pOptVal, int nOptLen);

		virtual DWORD GetLastError();
		virtual void SetLastError(DWORD);

		virtual bool IsValid() const;
	};

//--------------------------------------------------------------------------------
inline DWORD CSmallSocket::GetLastError() { return m_nLastError; }
inline void CSmallSocket::SetLastError(DWORD nErr)
	{ InterlockedExchange((LONG*) &m_nLastError, nErr); ::WSASetLastError(nErr); }
inline bool CSmallSocket::IsValid() const { return m_hSocket != INVALID_SOCKET; }

//--------------------------------------------------------------------------------
class CSmallSocketList : public CList<CSmallSocket*, CSmallSocket*>, public CReadWriteObject
	{
	};

//--------------------------------------------------------------------------------
class CSmallSocketArray : public CArray<CSmallSocket*, CSmallSocket*>, public CReadWriteObject
	{
	};

//--------------------------------------------------------------------------------
class CSmallSocketPool : public CSmallSocketList
	{
	public:
		virtual ~CSmallSocketPool()
			{
			for(POSITION pos = GetHeadPosition(); pos != NULL; )
				delete GetNext(pos);
			};
	};

#endif // !defined(AFX_SMALLSOCKET_H__4EDEDDAB_2360_11D3_87BA_00104B9E6286__INCLUDED_)
