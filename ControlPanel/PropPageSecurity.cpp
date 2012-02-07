//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "controlpanel.h"
#include "PropPageSecurity.h"

#include <IPAddress.h>

#include <OutputConfig.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//--------------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(CPropPageSecurity, CPropertyPage)

//--------------------------------------------------------------------------------
CPropPageSecurity::CPropPageSecurity(COutputConfig* pConfig)
		: CPropertyPage(CPropPageSecurity::IDD)
		, m_pConfig(pConfig)
	{
	//{{AFX_DATA_INIT(CPropPageSecurity)
	//}}AFX_DATA_INIT
	}

//--------------------------------------------------------------------------------
CPropPageSecurity::~CPropPageSecurity()
	{
	}

//--------------------------------------------------------------------------------
void CPropPageSecurity::DoDataExchange(CDataExchange* pDX)
	{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageSecurity)
	//}}AFX_DATA_MAP
	}

//--------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CPropPageSecurity, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageSecurity)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_BKUP_SS_IP, OnFieldchangedBkupSsIp)
	ON_EN_CHANGE(IDC_BKUP_SS_PORT, OnChangeBkupSsPort)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_MAIN_SS_IP, OnFieldchangedMainSsIp)
	ON_EN_CHANGE(IDC_MAIN_SS_PORT, OnChangeMainSsPort)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//--------------------------------------------------------------------------------
void CPropPageSecurity::SaveData()
	{
	if(m_pConfig == NULL)
		return;

	BYTE ip[4];
	CIPAddressCtrl* pCtrl = (CIPAddressCtrl*) GetDlgItem(IDC_MAIN_SS_IP);
	pCtrl->GetAddress(ip[0], ip[1], ip[2], ip[3]);
	CString sTemp;
	sTemp.Format("%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
	m_pConfig->SetSecServIP(sTemp);
	GetDlgItemText(IDC_MAIN_SS_PORT, sTemp);
	m_pConfig->SetSecServPort(atol(sTemp));

	pCtrl = (CIPAddressCtrl*) GetDlgItem(IDC_BKUP_SS_IP);
	pCtrl->GetAddress(ip[0], ip[1], ip[2], ip[3]);
	sTemp.Format("%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
	m_pConfig->SetSecServBkupIP(sTemp);
	GetDlgItemText(IDC_BKUP_SS_PORT, sTemp);
	m_pConfig->SetSecServBkupPort(atol(sTemp));
	}

//--------------------------------------------------------------------------------
BOOL CPropPageSecurity::OnApply() 
	{
	SaveData();
	m_pConfig->WriteConfig();
	return CPropertyPage::OnApply();
	}

//--------------------------------------------------------------------------------
BOOL CPropPageSecurity::OnKillActive() 
	{
	SaveData();
	return CPropertyPage::OnKillActive();
	}

//--------------------------------------------------------------------------------
BOOL CPropPageSecurity::OnSetActive() 
	{
	if(m_pConfig != NULL)
		{
		CIPAddress ip(m_pConfig->GetSecServIP());
		CIPAddressCtrl* pCtrl = (CIPAddressCtrl*) GetDlgItem(IDC_MAIN_SS_IP);
		pCtrl->SetAddress(ip);
		CString sTemp;
		sTemp.Format("%ld", m_pConfig->GetSecServPort());
		SetDlgItemText(IDC_MAIN_SS_PORT, sTemp);

		ip = m_pConfig->GetSecServBkupIP();
		pCtrl = (CIPAddressCtrl*) GetDlgItem(IDC_BKUP_SS_IP);
		pCtrl->SetAddress(ip);
		sTemp.Format("%ld", m_pConfig->GetSecServBkupPort());
		SetDlgItemText(IDC_BKUP_SS_PORT, sTemp);
		}

	return CPropertyPage::OnSetActive();
	}

//--------------------------------------------------------------------------------
void CPropPageSecurity::OnFieldchangedBkupSsIp(NMHDR* pNMHDR, LRESULT* pResult) 
	{
	*pResult = 0;
	SetModified();
	}

//--------------------------------------------------------------------------------
void CPropPageSecurity::OnChangeBkupSsPort() 
	{
	SetModified();
	}

//--------------------------------------------------------------------------------
void CPropPageSecurity::OnFieldchangedMainSsIp(NMHDR* pNMHDR, LRESULT* pResult) 
	{
	*pResult = 0;
	SetModified();
	}

//--------------------------------------------------------------------------------
void CPropPageSecurity::OnChangeMainSsPort() 
	{
	SetModified();
	}
