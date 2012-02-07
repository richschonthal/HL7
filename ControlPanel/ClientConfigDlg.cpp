//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// ClientConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ControlPanel.h"
#include "ClientConfigDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CClientConfigDlg dialog

//--------------------------------------------------------------------------------
CClientConfigDlg::CClientConfigDlg(CWnd* pParent /*=NULL*/)
		: CDialog(CClientConfigDlg::IDD, pParent)
	{
	//{{AFX_DATA_INIT(CClientConfigDlg)
	m_nPortNo = 0;
	//}}AFX_DATA_INIT
	}

//--------------------------------------------------------------------------------
void CClientConfigDlg::DoDataExchange(CDataExchange* pDX)
	{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClientConfigDlg)
	DDX_Text(pDX, IDC_PORT, m_nPortNo);
	//}}AFX_DATA_MAP
	}

//--------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CClientConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CClientConfigDlg)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS, OnFieldchangedIpaddress)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// CClientConfigDlg message handlers

//--------------------------------------------------------------------------------
void CClientConfigDlg::OnFieldchangedIpaddress(NMHDR* pNMHDR, LRESULT* pResult) 
	{
	CIPAddressCtrl* pCtrl = (CIPAddressCtrl*) GetDlgItem(IDC_IPADDRESS);
	pCtrl->GetAddress(m_ip);

	*pResult = 0;
	}

BOOL CClientConfigDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CIPAddressCtrl* pCtrl = (CIPAddressCtrl*) GetDlgItem(IDC_IPADDRESS);
	pCtrl->SetAddress(m_ip);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
