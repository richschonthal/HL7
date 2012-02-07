//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#if !defined(AFX_TEXTSOCKET_H__93A55BD1_961A_11D3_AF01_005004A1C5F3__INCLUDED_)
#define AFX_TEXTSOCKET_H__93A55BD1_961A_11D3_AF01_005004A1C5F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
#include "SmallSocket.h"

//--------------------------------------------------------------------------------
class CTextSocket : public CSmallSocket  
	{
	protected:
		DWORD m_nTimeOut;
		CString m_sCurLine;
		CStringList m_buffer;

	public:
		CTextSocket();
		virtual ~CTextSocket();

		void SetTimeOut(DWORD);
		DWORD GetTimeOut();

		int Receive(CString&);
		int Send(const CString&);
	};

#endif // !defined(AFX_TEXTSOCKET_H__93A55BD1_961A_11D3_AF01_005004A1C5F3__INCLUDED_)
