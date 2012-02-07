//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "LockedSocket.h"

//--------------------------------------------------------------------------------
CLockedSocket::CLockedSocket()
	{

	}

//--------------------------------------------------------------------------------
CLockedSocket::~CLockedSocket()
	{

	}

//--------------------------------------------------------------------------------
bool CLockedSocket::Create(UINT nSocketPort, LPCTSTR lpszSocketAddress)
	{
	CSingleLock lock(&m_mutex, false);
	if(! lock.Lock(30000))
		return false;

	return CSmallSocket::Create(nSocketPort, lpszSocketAddress);
	}

//--------------------------------------------------------------------------------
bool CLockedSocket::Accept(CSmallSocket& rConnectedSocket, SOCKADDR* lpSockAddr, int* lpSockAddrLen)
	{
	CSingleLock lock(&m_mutex, false);
	if(! lock.Lock(30000))
		return false;
	return CSmallSocket::Accept(rConnectedSocket, lpSockAddr, lpSockAddrLen);
	}

//--------------------------------------------------------------------------------
void CLockedSocket::Close()
	{
	CSingleLock lock(&m_mutex, false);
	if(! lock.Lock(30000))
		return;

	CSmallSocket::Close();
	}

//--------------------------------------------------------------------------------
bool CLockedSocket::Connect(LPCTSTR lpszHostAddress, UINT nHostPort)
	{
	CSingleLock lock(&m_mutex, false);
	if(! lock.Lock(30000))
		return false;

	return CSmallSocket::Connect(lpszHostAddress, nHostPort);
	}

//--------------------------------------------------------------------------------
bool CLockedSocket::Listen(int nConnectionBacklog)
	{
	CSingleLock lock(&m_mutex, false);
	if(! lock.Lock(30000))
		return false;

	return CSmallSocket::Listen(nConnectionBacklog);
	}

//--------------------------------------------------------------------------------
int CLockedSocket::Receive(CString& sData, eSmallSocketFlags nFlags, unsigned long nWait)
	{
	LPTSTR pBuf = sData.GetBuffer(1024);
	int nRv = Receive(pBuf, 1024, nFlags, nWait);
	sData.ReleaseBuffer();
	return nRv;
	}

//--------------------------------------------------------------------------------
int CLockedSocket::Receive(void* lpBuf, int nBufLen, eSmallSocketFlags nFlags, unsigned long nWait)
	{
	CSingleLock lock(&m_mutex, false);
	if(! lock.Lock(30000))
		return -1;

	return CSmallSocket::Receive(lpBuf, nBufLen, nFlags, nWait);
	}

//--------------------------------------------------------------------------------
bool CLockedSocket::Send(const CString& sData, eSmallSocketFlags nFlags, unsigned long nWait)
	{
	int nLen = sData.GetLength();
	return Send(sData, nLen, nFlags, nWait) == nLen;
	}

//--------------------------------------------------------------------------------
int CLockedSocket::Send(const void* lpBuf, int nBufLen, eSmallSocketFlags nFlags, unsigned long nWait)
	{
	CSingleLock lock(&m_mutex, false);
	if(! lock.Lock(30000))
		return -1;

	return CSmallSocket::Send(lpBuf, nBufLen, nFlags, nWait);
	}

