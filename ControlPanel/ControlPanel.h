//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#if !defined(AFX_CONTROLPANEL_H__F1BF7B46_F926_11D3_AF1A_005004A1C5F3__INCLUDED_)
#define AFX_CONTROLPANEL_H__F1BF7B46_F926_11D3_AF1A_005004A1C5F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

//--------------------------------------------------------------------------------
#include <cpl.h>

//--------------------------------------------------------------------------------
class CControlPanelApp : public CWinApp
	{
	public:
		CControlPanelApp();

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CControlPanelApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

		//{{AFX_MSG(CControlPanelApp)
			// NOTE - the ClassWizard will add and remove member functions here.
			//    DO NOT EDIT what you see in these blocks of generated code !
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()


	public:
		static CControlPanelApp* m_pThis;

	public:
		static LONG APIENTRY CPlApplet(HWND hWnd, UINT uMsg, LONG lParam1, LONG lParam2);
		LONG OnDblclk(HWND hWnd, UINT uAppNum, LONG lData); 
		LONG OnExit();
		LONG OnGetCount();
		LONG OnInit();
		LONG OnInquire(UINT uAppNum, CPLINFO* pInfo);
		LONG OnNewInquire(UINT uAppNum, NEWCPLINFO* pInfo); 
		LONG OnStop(UINT uAppNum, LONG lData);
	};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTROLPANEL_H__F1BF7B46_F926_11D3_AF1A_005004A1C5F3__INCLUDED_)
