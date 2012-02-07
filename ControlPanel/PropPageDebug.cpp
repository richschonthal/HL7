//--------------------------------------------------------------------------------
//
// Copyright (c) 2001 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// PropPageDebug.cpp : implementation file
//

#include "stdafx.h"
#include "controlpanel.h"
#include "PropPageDebug.h"

#include "OutputConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define NUMCHECKS 15

//--------------------------------------------------------------------------------
static int g_nCheckId[NUMCHECKS] =
	{
	IDC_CHECK1,IDC_CHECK2,IDC_CHECK3,IDC_CHECK4,IDC_CHECK5,IDC_CHECK6,
	IDC_CHECK7,IDC_CHECK8,IDC_CHECK9,IDC_CHECK10,IDC_CHECK11,IDC_CHECK12,
	IDC_CHECK13,IDC_CHECK14,IDC_CHECK15//,IDC_CHECK16
	};

//--------------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(CPropPageDebug, CPropertyPage)

// disable the annoying warning about using this in the base member initializer list
#pragma warning(disable:4355)

//--------------------------------------------------------------------------------
CPropPageDebug::CPropPageDebug(COutputConfig* pConfig)
		: CPropertyPage(CPropPageDebug::IDD)
		, m_pConfig(pConfig)
		, CChooseFileBtn(this)
	{
	//{{AFX_DATA_INIT(CPropPageDebug)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	}

#pragma warning(default:4355)

//--------------------------------------------------------------------------------
void CPropPageDebug::DoDataExchange(CDataExchange* pDX)
	{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageDebug)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	}

//--------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CPropPageDebug, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageDebug)
	ON_BN_CLICKED(IDC_CHOOSEPATH, OnChoosepath)
	ON_EN_CHANGE(IDC_LOG_FILE, OnChangeLogFile)
	ON_BN_CLICKED(IDC_CHECK_ALL, OnCheckAll)
	ON_BN_CLICKED(IDC_CLEAR_ALL, OnClearAll)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	ON_BN_CLICKED(IDC_CHECK3, OnCheck3)
	ON_BN_CLICKED(IDC_CHECK4, OnCheck4)
	ON_BN_CLICKED(IDC_CHECK5, OnCheck5)
	ON_BN_CLICKED(IDC_CHECK6, OnCheck6)
	ON_BN_CLICKED(IDC_CHECK7, OnCheck7)
	ON_BN_CLICKED(IDC_CHECK8, OnCheck8)
	ON_BN_CLICKED(IDC_CHECK9, OnCheck9)
	ON_BN_CLICKED(IDC_CHECK10, OnCheck10)
	ON_BN_CLICKED(IDC_CHECK11, OnCheck11)
	ON_BN_CLICKED(IDC_CHECK12, OnCheck12)
	ON_BN_CLICKED(IDC_CHECK13, OnCheck13)
	ON_BN_CLICKED(IDC_CHECK14, OnCheck14)
	ON_BN_CLICKED(IDC_CHECK15, OnCheck15)
	ON_BN_CLICKED(IDC_DAILY_LOG, OnDailyLog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//--------------------------------------------------------------------------------
void CPropPageDebug::SaveData()
	{
	CString sTemp;
	GetDlgItemText(IDC_LOG_FILE, sTemp);
	m_pConfig->SetLogFile(sTemp);

	DWORD nMask = 1;
	DWORD nTrace = 0;

	for(int i = 0; i < NUMCHECKS; i++)
		{
		CButton* pBtn = (CButton*) GetDlgItem(g_nCheckId[i]);
		if(pBtn->GetCheck())
			nTrace += nMask;
		nMask <<= 1;
		}

	m_pConfig->SetTraceLevel(nTrace);

	CButton* pBtn = (CButton*) GetDlgItem(IDC_DAILY_LOG);
	m_pConfig->SetIsDailyLog(pBtn->GetCheck() != 0);
	}

//--------------------------------------------------------------------------------
BOOL CPropPageDebug::OnSetActive() 
	{	
	if(m_pConfig->GetLogFile() != NULL)
		SetDlgItemText(IDC_LOG_FILE, m_pConfig->GetLogFile());
	else
		SetDlgItemText(IDC_LOG_FILE, "");

	DWORD nMask = 1;
	DWORD nTrace = m_pConfig->GetTraceLevel();

	for(int i = 0; i < NUMCHECKS; i++)
		{
		CButton* pBtn = (CButton*) GetDlgItem(g_nCheckId[i]);
		pBtn->SetCheck(nMask & nTrace ? 1 : 0);
		nMask <<= 1;
		}

	CButton* pBtn = (CButton*) GetDlgItem(IDC_DAILY_LOG);
	pBtn->SetCheck(m_pConfig->IsDailyLog() ? 1 : 0);

	return CPropertyPage::OnSetActive();
	}

//--------------------------------------------------------------------------------
void CPropPageDebug::OnChoosepath() 
	{
	CString sName;
	GetDlgItemText(IDC_LOG_FILE, sName);
	if(sName.IsEmpty())
		SetDlgItemText(IDC_LOG_FILE, m_pConfig->GenerateLogfileName());
	if(ChooseFile(IDC_LOG_FILE, IDS_STRING_LOG_FILE_FILTER))
		SetModified();
	}

//--------------------------------------------------------------------------------
void CPropPageDebug::OnChangeLogFile() 
	{
	CString sTemp;
	GetDlgItemText(IDC_LOG_FILE, sTemp);
	if(m_pConfig->GetLogFile() != NULL && m_pConfig->GetLogFile() != sTemp)
		SetModified();
	}

//--------------------------------------------------------------------------------
void CPropPageDebug::OnCheckAll()
	{
	for(int i = 0; i < NUMCHECKS; i++)
		{
		CButton* pBtn = (CButton*) GetDlgItem(g_nCheckId[i]);
		if(pBtn->GetCheck() == 0)
			SetModified();
		pBtn->SetCheck(1);
		}
	}

//--------------------------------------------------------------------------------
void CPropPageDebug::OnClearAll() 
	{
	for(int i = 0; i < NUMCHECKS; i++)
		{
		CButton* pBtn = (CButton*) GetDlgItem(g_nCheckId[i]);
		if(pBtn->GetCheck() != 0)
			SetModified();
		pBtn->SetCheck(0);
		}
	}

//--------------------------------------------------------------------------------
void CPropPageDebug::OnDefault() 
	{
	DWORD nMask = 1;
	for(int i = 0; i < NUMCHECKS; i++)
		{
		CButton* pBtn = (CButton*) GetDlgItem(g_nCheckId[i]);
		int nVal = nMask & COutputConfig::m_nTraceLevelDefault ? 1 : 0;
		int nMod = nVal == 1 ? 0 : 1;
		if(pBtn->GetCheck() == nMod)
			SetModified();
		pBtn->SetCheck(nVal);
		nMask <<= 1;
		}
	}

//--------------------------------------------------------------------------------
BOOL CPropPageDebug::OnApply() 
	{
	SaveData();
	m_pConfig->WriteConfig();
	return CPropertyPage::OnApply();
	}

//--------------------------------------------------------------------------------
BOOL CPropPageDebug::OnKillActive() 
	{
	SaveData();	
	return CPropertyPage::OnKillActive();
	}

//--------------------------------------------------------------------------------
void CPropPageDebug::OnOK() 
	{
	CPropertyPage::OnOK();
	}

//--------------------------------------------------------------------------------
void CPropPageDebug::OnCheck1() 
	{
	SetModified();
	}

//--------------------------------------------------------------------------------
void CPropPageDebug::OnCheck2() 
	{
	SetModified();
	}

//--------------------------------------------------------------------------------
void CPropPageDebug::OnCheck3() 
	{
	SetModified();
	}

//--------------------------------------------------------------------------------
void CPropPageDebug::OnCheck4() 
	{
	SetModified();
	}

//--------------------------------------------------------------------------------
void CPropPageDebug::OnCheck5() 
	{
	SetModified();
	}

//--------------------------------------------------------------------------------
void CPropPageDebug::OnCheck6() 
	{
	SetModified();
	}

//--------------------------------------------------------------------------------
void CPropPageDebug::OnCheck7() 
	{
	SetModified();
	}

//--------------------------------------------------------------------------------
void CPropPageDebug::OnCheck8() 
	{
	SetModified();
	}

//--------------------------------------------------------------------------------
void CPropPageDebug::OnCheck9() 
	{
	SetModified();
	}

//--------------------------------------------------------------------------------
void CPropPageDebug::OnCheck10() 
	{
	SetModified();
	}

//--------------------------------------------------------------------------------
void CPropPageDebug::OnCheck11() 
	{
	SetModified();
	}

//--------------------------------------------------------------------------------
void CPropPageDebug::OnCheck12() 
	{
	SetModified();
	}

//--------------------------------------------------------------------------------
void CPropPageDebug::OnCheck13() 
	{
	SetModified();
	}

//--------------------------------------------------------------------------------
void CPropPageDebug::OnCheck14() 
	{
	SetModified();
	}

//--------------------------------------------------------------------------------
void CPropPageDebug::OnCheck15() 
	{
	SetModified();
	}


//--------------------------------------------------------------------------------
void CPropPageDebug::OnDailyLog() 
	{
	CButton* pBtn = (CButton*) GetDlgItem(IDC_DAILY_LOG);
	if(pBtn->GetCheck() != 0)
		SetDlgItemText(IDC_LOG_FILE, m_pConfig->GenerateLogfileName());
	SetModified();
	}
