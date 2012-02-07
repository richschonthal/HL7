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

#if !defined(AFX_STDAFX_H__6598E961_F73E_11D2_87A7_00104B9E6286__INCLUDED_)
#define AFX_STDAFX_H__6598E961_F73E_11D2_87A7_00104B9E6286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#define _AFX_NOFORCE_LIBS

#include <afx.h>

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#include <winreg.h>
#include "registry.h"

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls

#include <afxsock.h>		// MFC socket extensions

#include <WINERROR.H>
#include <winbase.h>

#include <afxtempl.h>
#include <afxmt.h>

// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__6598E961_F73E_11D2_87A7_00104B9E6286__INCLUDED_)
