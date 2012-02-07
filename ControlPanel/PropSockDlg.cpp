//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// PropSockDlg.cpp : implementation file
//

//--------------------------------------------------------------------------------
#include "stdafx.h"
#include "ControlPanel.h"
#include "PropSockDlg.h"

//--------------------------------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CPropSockDlg dialog

//--------------------------------------------------------------------------------
CPropSockDlg::CPropSockDlg(CWnd* pParent /*=NULL*/)
		: CDialog(CPropSockDlg::IDD, pParent)
	{
	//{{AFX_DATA_INIT(CPropSockDlg)
	m_nSockNo = 0;
	//}}AFX_DATA_INIT
	}

//--------------------------------------------------------------------------------
void CPropSockDlg::DoDataExchange(CDataExchange* pDX)
	{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropSockDlg)
	DDX_Text(pDX, IDC_SOCKETNUM, m_nSockNo);
	//}}AFX_DATA_MAP
	}

//--------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CPropSockDlg, CDialog)
	//{{AFX_MSG_MAP(CPropSockDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// CPropSockDlg message handlers
