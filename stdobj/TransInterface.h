//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// TransInterface.h: interface for the CTransInterface class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRANSINTERFACE_H__C006BFFA_1506_11D3_87B1_00104B9E6286__INCLUDED_)
#define AFX_TRANSINTERFACE_H__C006BFFA_1506_11D3_87B1_00104B9E6286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <result.h>

//--------------------------------------------------------------------------------
class CSmallSocket;

//--------------------------------------------------------------------------------
class CTransInterface : public CResult
	{
	private:
		static HINSTANCE m_hLib;

		static bool (__cdecl * m_pStartSession)(LPCTSTR, LPCTSTR, LPCTSTR);
		static bool (__cdecl * m_pSendFile)(LPCTSTR, LPCTSTR, LPCTSTR);
		static bool (__cdecl * m_pEndSession)();
		static bool (__cdecl * m_pSendString)(CSmallSocket*, const CString&);
		static bool (__cdecl * m_pReceiveString)(CSmallSocket*, CString&);
		static CSmallSocket* (__cdecl * m_pGetSocket)();

		static int m_nRefCount;

	public:
		CTransInterface(LPCTSTR = NULL);
		virtual ~CTransInterface();

		bool StartSession(LPCTSTR pAddr, LPCTSTR pUser, LPCTSTR pPwd);
		bool SendFile(LPCTSTR pName, LPCTSTR pSourceDir = NULL, LPCTSTR pDestDir = NULL);
		bool SendString(const CString&);
		bool ReceiveString(CString&);
		bool EndSession();
		CSmallSocket* GetSocket();
	};

#endif // !defined(AFX_TRANSINTERFACE_H__C006BFFA_1506_11D3_87B1_00104B9E6286__INCLUDED_)
