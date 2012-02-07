//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#if !defined(AFX_LOCKEDSOCKET_H__FB136E33_D27C_11D3_AF12_005004A1C5F3__INCLUDED_)
#define AFX_LOCKEDSOCKET_H__FB136E33_D27C_11D3_AF12_005004A1C5F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
#include "SmallSocket.h"
#include "LockedResource.h"

//--------------------------------------------------------------------------------
class CLockedSocket : public CSmallSocket, public CLockedResource
	{
	private:
		CMutex m_mutex;

	public:
		CLockedSocket();
		virtual ~CLockedSocket();

		virtual bool Create(UINT nSocketPort = 0, LPCTSTR lpszSocketAddress = NULL );

		virtual bool Accept(CSmallSocket& rConnectedSocket, SOCKADDR* lpSockAddr = NULL, int* lpSockAddrLen = NULL );
		virtual void Close( );
		virtual bool Connect( LPCTSTR lpszHostAddress, UINT nHostPort );
		virtual bool Listen( int nConnectionBacklog = 5 );
		
		virtual int Receive(void* lpBuf, int nBufLen, eSmallSocketFlags nFlags = FLAGDFLT, unsigned long nWait = 0);
		virtual int Receive(CString&, eSmallSocketFlags nFlags = FLAGDFLT, unsigned long nWait = 0);
		
		virtual int Send(const void* lpBuf, int nBufLen, eSmallSocketFlags nFlags = FLAGDFLT, unsigned long nWait = 0);
		virtual bool Send(const CString&, eSmallSocketFlags nFlags = FLAGDFLT, unsigned long nWait = 0);
	};

#endif // !defined(AFX_LOCKEDSOCKET_H__FB136E33_D27C_11D3_AF12_005004A1C5F3__INCLUDED_)
