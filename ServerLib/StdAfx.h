//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__42D4F253_830D_11D3_AEFF_005004A1C5F3__INCLUDED_)
#define AFX_STDAFX_H__42D4F253_830D_11D3_AEFF_005004A1C5F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#define _AFX_NOFORCE_LIBS

#include <afx.h>
#include <afxwin.h>

#include <afxmt.h>
#include <afxtempl.h>

#include <commondefines.h>

#define ReleaseTimeslice() ::Sleep(0)

// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__42D4F253_830D_11D3_AEFF_005004A1C5F3__INCLUDED_)
