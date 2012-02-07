//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// MainPropSheet.cpp : implementation file
//

//--------------------------------------------------------------------------------
#include "stdafx.h"
#include "ControlPanel.h"
#include "MainPropSheet.h"

#include <OutputConfig.h>

//--------------------------------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainPropSheet

//--------------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CMainPropSheet, CPropertySheet)

//--------------------------------------------------------------------------------
CMainPropSheet::CMainPropSheet(UINT nIDCaption, COutputConfig* pConfig,
								CHL7InputDaemonInitList* pInputConfig)
		: CPropertySheet(nIDCaption, NULL, 0)
		, m_page1(pConfig)
		, m_page2(pConfig, pInputConfig)
		, m_page3(pConfig)
		, m_page4(pConfig)
	{
	AddPage(&m_page1);
	AddPage(&m_page2);
	AddPage(&m_page3);
	AddPage(&m_page4);
	}

//--------------------------------------------------------------------------------
CMainPropSheet::~CMainPropSheet()
	{
	}

//--------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CMainPropSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CMainPropSheet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainPropSheet message handlers

//--------------------------------------------------------------------------------
BOOL CMainPropSheet::OnInitDialog() 
	{
	BOOL bResult = CPropertySheet::OnInitDialog();

	EndWaitCursor();

	return bResult;
	}
