//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// PropPageMain.cpp : implementation file
//

//--------------------------------------------------------------------------------
#include "stdafx.h"
#include "ControlPanel.h"
#include "PropPageMain.h"

#include <OutputConfig.h>

#include <PathString.h>
#include <PlatformInfo.h>
#include <LoadString.h>

//--------------------------------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CPropPageMain property page

//--------------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(CPropPageMain, CPropertyPage)

//--------------------------------------------------------------------------------
CPropPageMain::CPropPageMain()
		: CPropertyPage(CPropPageMain::IDD)
		, m_pConfig(NULL)
	{
	}

//--------------------------------------------------------------------------------
CPropPageMain::CPropPageMain(COutputConfig* pConfig)
		: CPropertyPage(CPropPageMain::IDD)
		, m_pConfig(pConfig)
	{
	//{{AFX_DATA_INIT(CPropPageMain)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	}

//--------------------------------------------------------------------------------
CPropPageMain::~CPropPageMain()
	{
	}

//--------------------------------------------------------------------------------
void CPropPageMain::DoDataExchange(CDataExchange* pDX)
	{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageMain)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	}

//--------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CPropPageMain, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageMain)
	ON_EN_CHANGE(IDC_ODBCSTRING, OnChangeOdbcstring)
	ON_EN_CHANGE(IDC_PROCACCNT, OnChangeProcaccnt)
	ON_EN_CHANGE(IDC_TABLEACCNT, OnChangeTableaccnt)
	ON_CBN_SELCHANGE(IDC_DBTYPE, OnSelchangeDbtype)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// CPropPageMain message handlers
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
BOOL CPropPageMain::OnInitDialog() 
	{
	CPropertyPage::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
	}

//--------------------------------------------------------------------------------
void CPropPageMain::OnOK() 
	{
	CPropertyPage::OnOK();
	}

//--------------------------------------------------------------------------------
void CPropPageMain::SaveData()
	{
	CString sTemp;

	{	
	CComboBox* pList = (CComboBox*) GetDlgItem(IDC_DBTYPE);
	pList->GetLBText(pList->GetCurSel(), sTemp);
	sTemp.MakeUpper();
	m_pConfig->SetDBType(sTemp);
	}

	GetDlgItemText(IDC_ODBCSTRING, sTemp);
	m_pConfig->SetODBCString(sTemp);

	GetDlgItemText(IDC_PROCACCNT, sTemp);
	m_pConfig->SetProcAccnt(sTemp);

	GetDlgItemText(IDC_TABLEACCNT, sTemp);
	m_pConfig->SetTableAccnt(sTemp);
	}

//--------------------------------------------------------------------------------
BOOL CPropPageMain::OnApply() 
	{
	SaveData();

	m_pConfig->WriteConfig();

	return CPropertyPage::OnApply();
	}

//--------------------------------------------------------------------------------
void CPropPageMain::OnSelchangeDbtype() 
	{
	SetModified();
	}

//--------------------------------------------------------------------------------
void CPropPageMain::OnChangeOdbcstring() 
	{
	CString sTemp;
	GetDlgItemText(IDC_ODBCSTRING, sTemp);
	if(m_pConfig->GetODBCString() != NULL && m_pConfig->GetODBCString() != sTemp)
		SetModified();
	}

//--------------------------------------------------------------------------------
void CPropPageMain::OnChangeProcaccnt() 
	{
	CString sTemp;
	GetDlgItemText(IDC_PROCACCNT, sTemp);
	if(m_pConfig->GetProcAccnt() != NULL && m_pConfig->GetProcAccnt() != sTemp)
		SetModified();
	}

//--------------------------------------------------------------------------------
void CPropPageMain::OnChangeTableaccnt() 
	{
	CString sTemp;
	GetDlgItemText(IDC_TABLEACCNT, sTemp);
	if(m_pConfig->GetTableAccnt() != NULL && m_pConfig->GetTableAccnt() != sTemp)
		SetModified();
	}

//--------------------------------------------------------------------------------
BOOL CPropPageMain::OnSetActive() 
	{
	CComboBox* pList = (CComboBox*) GetDlgItem(IDC_DBTYPE);
	if(pList == NULL)
		return FALSE;

	if(m_pConfig->GetDBType() != NULL)
		{
		if(strcmp(m_pConfig->GetDBType(), "SYBASE") == 0)
			pList->SetCurSel(0);
		else
			if(strcmp(m_pConfig->GetDBType(), "ORACLE") == 0)
				pList->SetCurSel(1);
			else
				if(strcmp(m_pConfig->GetDBType(), "MSSQL") == 0)
					pList->SetCurSel(2);
				else

					pList->SetCurSel(-1);
		}

	if(m_pConfig->GetODBCString() != NULL)
		SetDlgItemText(IDC_ODBCSTRING, m_pConfig->GetODBCString());
	else
		SetDlgItemText(IDC_ODBCSTRING, "");

	if(m_pConfig->GetProcAccnt() != NULL)
		SetDlgItemText(IDC_PROCACCNT, m_pConfig->GetProcAccnt());
	else
		SetDlgItemText(IDC_PROCACCNT, "");

	if(m_pConfig->GetTableAccnt() != NULL)
		SetDlgItemText(IDC_TABLEACCNT, m_pConfig->GetTableAccnt());
	else
		SetDlgItemText(IDC_TABLEACCNT, "");
	
	return CPropertyPage::OnSetActive();
	}

//--------------------------------------------------------------------------------
BOOL CPropPageMain::OnKillActive() 
	{
	SaveData();

	return CPropertyPage::OnKillActive();
	}
