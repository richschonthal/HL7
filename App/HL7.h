// HL7.h : main header file for the HL7 application
//

#if !defined(AFX_HL7_H__C9EB6A85_4373_11D3_A6B6_005004A1C5F3__INCLUDED_)
#define AFX_HL7_H__C9EB6A85_4373_11D3_A6B6_005004A1C5F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CHL7App:
// See HL7.cpp for the implementation of this class
//

class CHL7App : public CWinApp
{
public:
	CHL7App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHL7App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CHL7App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HL7_H__C9EB6A85_4373_11D3_A6B6_005004A1C5F3__INCLUDED_)
