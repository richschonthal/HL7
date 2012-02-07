//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// MonitorConfigDlg.cpp : implementation file
//

//--------------------------------------------------------------------------------
#include "stdafx.h"
#include "ControlPanel.h"
#include "MonitorConfigDlg.h"
#include "PropFileDlg.h"
#include "PropSockDlg.h"
#include "ClientConfigDlg.h"

#include <HL7InputDaemonInit.h>
#include <LoadString.h>
#include <IPAddress.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// disable the annoying warning about using this in the base member initializer list
#pragma warning(disable:4355)

// CMonitorConfigDlg dialog
//--------------------------------------------------------------------------------
CMonitorConfigDlg::CMonitorConfigDlg(CHL7InputDaemonInit* pConfig, CHL7InputDaemonInitList* pList)
	: CDialog(CMonitorConfigDlg::IDD, NULL)
	, m_pOrigInit(pConfig)
	, m_pInit(new CHL7InputDaemonInit(*pConfig))
	, m_pInitList(pList)
	, CChooseFileBtn(this)
	{
	//{{AFX_DATA_INIT(CMonitorConfigDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	}

#pragma warning(default:4355)

//--------------------------------------------------------------------------------
CMonitorConfigDlg::~CMonitorConfigDlg()
	{
	delete m_pInit;
	}

//--------------------------------------------------------------------------------
void CMonitorConfigDlg::DoDataExchange(CDataExchange* pDX)
	{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMonitorConfigDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	}

//--------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CMonitorConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CMonitorConfigDlg)
	ON_BN_CLICKED(IDC_PROPERTIES, OnProperties)
	ON_BN_CLICKED(IDC_CLIENT, OnClient)
	ON_BN_CLICKED(IDC_CONFIG, OnConfig)
	ON_EN_CHANGE(IDC_VERSION, OnChangeVersion)
	ON_BN_CLICKED(IDC_IN, OnIn)
	ON_BN_CLICKED(IDC_OUT, OnOut)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//--------------------------------------------------------------------------------
BOOL CMonitorConfigDlg::OnInitDialog() 
	{
	ASSERT(m_pInit != NULL);

	CDialog::OnInitDialog();
	CheckConfigBtnEnable();
	ConfigToDlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
	}

//--------------------------------------------------------------------------------
void CMonitorConfigDlg::ConfigToDlg()
	{
	CWnd* pWnd = GetDlgItem(IDC_FRAME);

	struct
		{
		DWORD nTitleId;
		DWORD nTextId;
		} nLabelId[4] =
			{
			IDC_EDIT1_TITLE, IDC_EDIT1,
			IDC_EDIT2_TITLE, IDC_EDIT2,
			IDC_EDIT3_TITLE, IDC_EDIT3,
			IDC_EDIT4_TITLE, IDC_EDIT4
			};
	int nCurLabel = 0;

	CString sDesc;
	sDesc.LoadString(IDS_MODULEIOPROP);
	CString sTemp;
	CString sNotSet;
	sNotSet.LoadString(IDS_NOTSET);

	switch(m_pInit->GetType())
		{
		// currently unsupported so default to socket
		default:
		case CHL7InputDaemonInit::INPUT_OFF:
		case CHL7InputDaemonInit::INPUT_SERIAL:
		case CHL7InputDaemonInit::INPUT_DATABASE:

		case CHL7InputDaemonInit::INPUT_SOCKET:
			sDesc = CLoadString::Get(IDS_STRING_SOCKET) + " " + sDesc;
			if(m_pInit->GetPort() == 0)
				sTemp = sNotSet;
			else
				sTemp.Format("%d", m_pInit->GetPort());
			SetDlgItemText(nLabelId[nCurLabel].nTitleId, CLoadString::Get(IDS_STRING_PORT));
			SetDlgItemText(nLabelId[nCurLabel].nTextId, sTemp);

			nCurLabel++;
			break;

		case CHL7InputDaemonInit::INPUT_FILE:
			sDesc = CLoadString::Get(IDS_STRING_FILE) + " " + sDesc;
			if(m_pInit->GetPath() == NULL)
				sTemp = sNotSet;
			else
				sTemp = m_pInit->GetPath();

			SetDlgItemText(nLabelId[nCurLabel].nTitleId, CLoadString::Get(IDS_STRING_IN_PATH));
			SetDlgItemText(nLabelId[nCurLabel].nTextId, sTemp);
			nCurLabel++;

			if(m_pInit->GetOutPath() == NULL)
				sTemp = sNotSet;
			else
				sTemp = m_pInit->GetOutPath();

			SetDlgItemText(nLabelId[nCurLabel].nTitleId, CLoadString::Get(IDS_STRING_OUT_PATH));
			SetDlgItemText(nLabelId[nCurLabel].nTextId, sTemp);
			nCurLabel++;
			{// client mode doesnt apply so...
			// check/uncheck the client mode button
			CheckDlgButton(IDC_CLIENT, 0);
			CheckConfigBtnEnable();
			GetDlgItem(IDC_CLIENT)->EnableWindow(FALSE);
			}
			break;
		}

	pWnd->SetWindowText(sDesc);

	for(int i = 0; i < nCurLabel; i++)
		{
		GetDlgItemText(nLabelId[i].nTextId, sTemp);
		GetDlgItem(nLabelId[i].nTitleId)->EnableWindow(sTemp != sNotSet);
		GetDlgItem(nLabelId[i].nTextId)->EnableWindow(sTemp != sNotSet);
		}

	// set the Name field
	SetDlgItemText(IDC_DISPLAY_NAME, m_pInit->GetName());

	// populate version field
	sTemp.Format("%d", m_pInit->GetVersion());
	SetDlgItemText(IDC_VERSION, sTemp);

	// populate token id field
	sTemp.Format("%d", m_pInit->GetTokenId());
	SetDlgItemText(IDC_SECSERV_TOKEN, sTemp);

	// set the in/out buttons
	CheckDlgButton(IDC_IN,  m_pInit->IsOutModule() ? 0:1);
	CheckDlgButton(IDC_OUT, m_pInit->IsOutModule() ? 1:0);

	// check/uncheck the client mode button
	CheckDlgButton(IDC_CLIENT, m_pInit->IsClient() ? 1:0);
	CheckConfigBtnEnable();
	}

//--------------------------------------------------------------------------------
void CMonitorConfigDlg::DlgToConfig()
	{
	CString sTemp;

	GetDlgItemText(IDC_DISPLAY_NAME, sTemp);
	m_pInit->SetName(sTemp);

	GetDlgItemText(IDC_SECSERV_TOKEN, sTemp);
	m_pInit->SetTokenId(atol(sTemp));
	}

//--------------------------------------------------------------------------------
bool CMonitorConfigDlg::CheckConfigBtnEnable()
	{
	// enable the config button if the client mode check box is checked
	CWnd* pWnd = GetDlgItem(IDC_CONFIG);
	ASSERT(pWnd != NULL);
	bool bIsChecked = IsDlgButtonChecked(IDC_CLIENT) != 0;
	pWnd->EnableWindow(bIsChecked);
	return bIsChecked;
	}

//--------------------------------------------------------------------------------
void CMonitorConfigDlg::OnOK()
	{
	DlgToConfig();
	*m_pOrigInit = *m_pInit;
	CDialog::OnOK();
	}

//--------------------------------------------------------------------------------
void CMonitorConfigDlg::OnProperties()
	{
	DlgToConfig();

	switch(m_pInit->GetType())
		{
		case CHL7InputDaemonInit::INPUT_SOCKET:
			{
			CPropSockDlg dlg;

			dlg.m_nSockNo = m_pInit->GetPort();
			if(dlg.DoModal() == IDOK)
				{
				m_pInit->SetPort(dlg.m_nSockNo);
				ConfigToDlg();
				}
			}
			break;

		case CHL7InputDaemonInit::INPUT_FILE:
			{
			CPropFileDlg dlg;
			
			dlg.m_sInPath = m_pInit->GetPath();
			dlg.m_sOutPath = m_pInit->GetOutPath();

			if(dlg.DoModal())
				{
				m_pInit->SetPath(dlg.m_sInPath);
				m_pInit->SetOutPath(dlg.m_sOutPath);
				ConfigToDlg();
				}
			}
			break;
		}
	}

//--------------------------------------------------------------------------------
void CMonitorConfigDlg::OnChooselogpath() 
	{
	ChooseFile(IDC_LOG_FILE, IDS_STRING_LOG_FILE_FILTER);
	CString sTemp;
	GetDlgItemText(IDC_LOG_FILE, sTemp);
	m_pInit->SetLogFile(sTemp);
	}

//--------------------------------------------------------------------------------
void CMonitorConfigDlg::OnClient()
	{
	m_pInit->SetIsClient(IsDlgButtonChecked(IDC_CLIENT) != 0);

	if(CheckConfigBtnEnable())
		{
		if(m_pInit->GetOutPort() == 0 || m_pInit->GetOutAddr() == NULL)
			OnConfig();
		}

	ConfigToDlg();
	}

//--------------------------------------------------------------------------------
void CMonitorConfigDlg::OnConfig()
	{
	CClientConfigDlg dlg;
	// convert address as string to dword
	CIPAddress ip(m_pInit->GetOutAddr());
	// init the dialog's ip control
	dlg.m_ip = ip.GetAddress();
	dlg.m_nPortNo = m_pInit->GetOutPort();

	if(dlg.DoModal() == IDOK)
		{
		m_pInit->SetOutPort(dlg.m_nPortNo);
		ip.SetAddress(dlg.m_ip);
		CString sTemp;
		ip.GetAddress(sTemp);
		m_pInit->SetOutAddr(sTemp);
		}
	}

//--------------------------------------------------------------------------------
void CMonitorConfigDlg::OnChangeVersion()
	{
	CString sTemp;
	GetDlgItemText(IDC_VERSION, sTemp);
	m_pInit->SetVersion(atoi(sTemp));
	}

//--------------------------------------------------------------------------------
void CMonitorConfigDlg::OnIn()
	{
	m_pInit->SetIsOutModule(IsDlgButtonChecked(IDC_OUT) != 0);
	}

//--------------------------------------------------------------------------------
void CMonitorConfigDlg::OnOut() 
	{
	m_pInit->SetIsOutModule(IsDlgButtonChecked(IDC_OUT) != 0);
	}
