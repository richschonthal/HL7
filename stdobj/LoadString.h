//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// LoadString.h: interface for the CLoadString class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOADSTRING_H__BDAA0C66_72B0_11D3_AEE4_005004A1C5F3__INCLUDED_)
#define AFX_LOADSTRING_H__BDAA0C66_72B0_11D3_AEE4_005004A1C5F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
class CLoadString  
	{
	public:
		CLoadString();
		CLoadString(int, CString&);
		~CLoadString();

		static CString Get(int);
	};

#endif // !defined(AFX_LOADSTRING_H__BDAA0C66_72B0_11D3_AEE4_005004A1C5F3__INCLUDED_)
