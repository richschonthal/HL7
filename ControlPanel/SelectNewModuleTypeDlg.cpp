//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// SelectNewModuleTypeDlg.cpp : implementation file
//

//--------------------------------------------------------------------------------
#include "stdafx.h"
#include "ControlPanel.h"
#include <HL7InputDaemonInit.h>
#include "SelectNewModuleTypeDlg.h"

//--------------------------------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CSelectNewModuleTypeDlg dialog

//--------------------------------------------------------------------------------
CSelectNewModuleTypeDlg::CSelectNewModuleTypeDlg(CWnd* pParent /*=NULL*/)
		: CDialog(CSelectNewModuleTypeDlg::IDD, pParent)
		, m_nType(0)
	{
	//{{AFX_DATA_INIT(CSelectNewModuleTypeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	}

//--------------------------------------------------------------------------------
void CSelectNewModuleTypeDlg::DoDataExchange(CDataExchange* pDX)
	{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectNewModuleTypeDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	}

//--------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CSelectNewModuleTypeDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectNewModuleTypeDlg)
//	ON_BN_CLICKED(IDC_RADIO_DB, OnRadioDb)
	ON_BN_CLICKED(IDC_RADIO_FILE, OnRadioFile)
	ON_BN_CLICKED(IDC_RADIO_SERIAL, OnRadioSerial)
	ON_BN_CLICKED(IDC_RADIO_SOCKET, OnRadioSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// CSelectNewModuleTypeDlg message handlers
//--------------------------------------------------------------------------------
void CSelectNewModuleTypeDlg::OnRadioDb()
	{
	m_nType = CHL7InputDaemonInit::INPUT_DATABASE;
	}

//--------------------------------------------------------------------------------
void CSelectNewModuleTypeDlg::OnRadioFile()
	{
	m_nType = CHL7InputDaemonInit::INPUT_FILE;
	}

//--------------------------------------------------------------------------------
void CSelectNewModuleTypeDlg::OnRadioSerial() 
	{
	m_nType = CHL7InputDaemonInit::INPUT_SERIAL;
	}

//--------------------------------------------------------------------------------
void CSelectNewModuleTypeDlg::OnRadioSocket() 
	{
	m_nType = CHL7InputDaemonInit::INPUT_SOCKET;
	}

//--------------------------------------------------------------------------------
BOOL CSelectNewModuleTypeDlg::OnInitDialog() 
	{
	CDialog::OnInitDialog();

	m_nType = CHL7InputDaemonInit::INPUT_SOCKET;
	CheckDlgButton(IDC_RADIO_SOCKET, BST_CHECKED);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
	}
