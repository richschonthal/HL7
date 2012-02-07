//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#if !defined(AFX_MAINPROPSHEET_H__5257C8F9_6F70_11D3_AEE2_005004A1C5F3__INCLUDED_)
#define AFX_MAINPROPSHEET_H__5257C8F9_6F70_11D3_AEE2_005004A1C5F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainPropSheet.h : header file
//

//--------------------------------------------------------------------------------
#include "PropPageMain.h"
#include "PropPageInput.h"
#include "PropPageSecurity.h"
#include "PropPageDebug.h"

//--------------------------------------------------------------------------------
class COutputConfig;
class CHL7InputDaemonInitList;

//--------------------------------------------------------------------------------
class CMainPropSheet : public CPropertySheet
	{
	DECLARE_DYNAMIC(CMainPropSheet)

	// Construction
	public:
		CMainPropSheet(UINT nIDCaption, COutputConfig*, CHL7InputDaemonInitList*);

	// Attributes
	public:

	// Operations
	public:

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CMainPropSheet)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

	// Implementation
	public:
		virtual ~CMainPropSheet();

		// Generated message map functions
	protected:
		//{{AFX_MSG(CMainPropSheet)
	//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

	private:
		CPropPageMain m_page1;
		CPropPageInput m_page2;
		CPropPageDebug m_page3;
		CPropPageSecurity m_page4;
	};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINPROPSHEET_H__5257C8F9_6F70_11D3_AEE2_005004A1C5F3__INCLUDED_)
