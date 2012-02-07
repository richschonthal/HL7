//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// PropPageInput.cpp : implementation file
//

//--------------------------------------------------------------------------------
#include "stdafx.h"
#include "ControlPanel.h"
#include "PropPageInput.h"
#include "MonitorConfigDlg.h"
#include "SelectNewModuleTypeDlg.h"

#include <HL7InputDaemonInit.h>
#include <OutputConfig.h>
#include <LoadString.h>

//--------------------------------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CPropPageInput property page

//--------------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(CPropPageInput, CPropertyPage)

//--------------------------------------------------------------------------------
CPropPageInput::CPropPageInput()
		: CPropertyPage(CPropPageInput::IDD)
		, m_pConfigList(NULL)
		, m_pOutConfig(NULL)
	{
	//{{AFX_DATA_INIT(CPropPageInput)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	}

//--------------------------------------------------------------------------------
CPropPageInput::CPropPageInput(const COutputConfig* pOutConfig, CHL7InputDaemonInitList* pConfigList)
		: CPropertyPage(CPropPageInput::IDD)
		, m_pConfigList(pConfigList)
		, m_pOutConfig(pOutConfig)
	{
	}

//--------------------------------------------------------------------------------
CPropPageInput::~CPropPageInput()
	{
	}

//--------------------------------------------------------------------------------
void CPropPageInput::DoDataExchange(CDataExchange* pDX)
	{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageInput)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	}


//--------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CPropPageInput, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageInput)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkList)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_NOTIFY(NM_CLICK, IDC_LIST, OnClickList)
	ON_NOTIFY(NM_KILLFOCUS, IDC_LIST, OnKillfocusList)
	ON_NOTIFY(NM_SETFOCUS, IDC_LIST, OnSetfocusList)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST, OnKeydownList)
	ON_BN_CLICKED(IDC_DEL, OnDel)
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// CPropPageInput message handlers

//--------------------------------------------------------------------------------
BOOL CPropPageInput::OnInitDialog() 
	{
	CPropertyPage::OnInitDialog();
	
	CListCtrl* pCtrl = (CListCtrl*) GetDlgItem(IDC_LIST);
	if(pCtrl == NULL)
		return TRUE;

	pCtrl->InsertColumn(COL_TYPE, CLoadString::Get(IDS_COLNAME_TYPE), LVCFMT_LEFT, 50);
	pCtrl->InsertColumn(COL_NAME, CLoadString::Get(IDS_COLNAME_NAME), LVCFMT_LEFT, 96);
	pCtrl->InsertColumn(COL_VERSION, CLoadString::Get(IDS_COLNAME_VER), LVCFMT_LEFT, 50);
	pCtrl->InsertColumn(COL_CONFIG, CLoadString::Get(IDS_COLNAME_CONFIG), LVCFMT_LEFT, 750);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
	}

//--------------------------------------------------------------------------------
void CPropPageInput::ConfigToList(int nIndex, CHL7InputDaemonInit* pConfig)
	{
	CListCtrl* pCtrl = (CListCtrl*) GetDlgItem(IDC_LIST);
	if(pCtrl == NULL)
		return;

	CString sTemp;
	switch(pConfig->GetType())
		{
		case CHL7InputDaemonInit::INPUT_SOCKET:
			CLoadString(IDS_STRING_SOCKET, sTemp);
			break;
		case CHL7InputDaemonInit::INPUT_FILE:
			CLoadString(IDS_STRING_FILE, sTemp);
			break;
		}

	if(nIndex >= pCtrl->GetItemCount())
		nIndex = pCtrl->InsertItem(nIndex, sTemp);
	else
		pCtrl->SetItem(nIndex, COL_TYPE, LVIF_TEXT, sTemp, 0, 0, 0, NULL);

	pCtrl->SetItem(nIndex, COL_NAME, LVIF_TEXT, pConfig->GetName(), 0, 0, 0, NULL);
	if(pConfig->GetVersion() != 0)
		sTemp.Format("%d", pConfig->GetVersion());
	else
		sTemp = ' ';
	pCtrl->SetItem(nIndex, COL_VERSION, LVIF_TEXT, sTemp, 0, 0, 0, NULL);

	sTemp.Empty();
	if(pConfig->IsOutModule())
		CLoadString(IDS_STRING_OUTMODULE, sTemp);
	else
		CLoadString(IDS_STRING_INMODULE, sTemp);

	CString sNum;
	if(pConfig->IsClient())
		{
		if(! sTemp.IsEmpty())
			sTemp += ", ";
		sTemp += CLoadString::Get(IDS_STRING_CLIENT_MODE) + ": ";
		sTemp += CLoadString::Get(IDS_STRING_PORT) + '=';
		sNum.Format("%d", pConfig->GetOutPort());
		sTemp += sNum + ' ';
		sTemp += CLoadString::Get(IDS_STRING_IP_ADDRESS) + '=';
		sTemp += pConfig->GetOutAddr();
		}

	switch(pConfig->GetType())
		{
		case CHL7InputDaemonInit::INPUT_SOCKET:
			if(pConfig->GetPort()!=0)
				{
				sNum.Format("%d", pConfig->GetPort());
				if(! sTemp.IsEmpty())
					sTemp += ", ";
				sTemp += CLoadString::Get(IDS_STRING_INPORT) + " = " + sNum;
				}
			if(pConfig->GetOutPort()!=0)
				{
				sNum.Format("%d", pConfig->GetOutPort());
				if(! sTemp.IsEmpty())
					sTemp += ", ";
				sTemp += ", " + CLoadString::Get(IDS_STRING_OUTPORT) + " = " + sNum;
				}
			break;

		case CHL7InputDaemonInit::INPUT_FILE:
			if(pConfig->GetPath() != NULL && *pConfig->GetPath() != 0)
				{
				if(! sTemp.IsEmpty())
					sTemp += ", ";
				sTemp += CLoadString::Get(IDS_STRING_IN_PATH) + " = ";
				sTemp += pConfig->GetPath();
				}

			if(pConfig->GetOutPath() != NULL && *pConfig->GetOutPath() != 0)
				{
				if(! sTemp.IsEmpty())
					sTemp += ", ";
				sTemp += ", " + CLoadString::Get(IDS_STRING_OUT_PATH) + " = ";
				sTemp += pConfig->GetOutPath();
				}
			break;
		}

	pCtrl->SetItem(nIndex, COL_CONFIG, LVIF_TEXT, sTemp, 0, 0, 0, NULL);

	// set the list item data to point to the config record
	pCtrl->SetItemData(nIndex, (DWORD) pConfig);
	}

//--------------------------------------------------------------------------------
void CPropPageInput::EditItem(int nItem)
	{
	CListCtrl* pCtrl = (CListCtrl*) GetDlgItem(IDC_LIST);
	if(pCtrl == NULL)
		return;

	CHL7InputDaemonInit* pConfig = 
		(CHL7InputDaemonInit*) pCtrl->GetItemData(nItem);
	CMonitorConfigDlg dlg(pConfig, m_pConfigList);
	if(dlg.DoModal() == IDOK)
		{
		UpdateDisplay();
		SetModified();
		}
	}

//--------------------------------------------------------------------------------
void CPropPageInput::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
	{
	NMLISTVIEW* pAction = (NMLISTVIEW*) pNMHDR;

	if(pAction->iItem > -1)
		EditItem(pAction->iItem);

	*pResult = 0;
	}

//--------------------------------------------------------------------------------
void CPropPageInput::UpdateDisplay()
	{
	CListCtrl* pCtrl = (CListCtrl*) GetDlgItem(IDC_LIST);
	if(pCtrl == NULL)
		return;

	pCtrl->DeleteAllItems();

	int nIndex = -1;
	for(int i = 0; i < m_pConfigList->GetSize(); i++)
		ConfigToList(i, m_pConfigList->GetAt(i));
	CheckIsItemSelected();
	}

//--------------------------------------------------------------------------------
BOOL CPropPageInput::OnSetActive() 
	{
	UpdateDisplay();
	return CPropertyPage::OnSetActive();
	}

//--------------------------------------------------------------------------------
BOOL CPropPageInput::OnApply()
	{
	m_pConfigList->WriteList(m_pOutConfig->GetMonKeyName());
	return CPropertyPage::OnApply();
	}

//--------------------------------------------------------------------------------
void CPropPageInput::OnAdd() 
	{
	// find out what type of module we're adding
	CSelectNewModuleTypeDlg newDlg;
	if(newDlg.DoModal() != IDOK)
		return;

	CHL7InputDaemonInit* pConfig = new CHL7InputDaemonInit(newDlg.GetType());
	CMonitorConfigDlg dlg(pConfig, m_pConfigList);
	if(dlg.DoModal() == IDOK)
		{
		m_pConfigList->Add(pConfig);
		UpdateDisplay();
		SetModified();
		}
	else
		delete pConfig;
	}

//--------------------------------------------------------------------------------
bool CPropPageInput::CheckIsItemSelected()
	{
	CListCtrl* pList = (CListCtrl*) GetDlgItem(IDC_LIST);
	bool bIsSelected = pList->GetFirstSelectedItemPosition() != NULL;
	GetDlgItem(IDC_DEL)->EnableWindow(bIsSelected);
	GetDlgItem(IDC_EDIT)->EnableWindow(bIsSelected);
	return bIsSelected;
	}

//--------------------------------------------------------------------------------
void CPropPageInput::OnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
	{
	CheckIsItemSelected();
	*pResult = 0;
	}

//--------------------------------------------------------------------------------
void CPropPageInput::OnKillfocusList(NMHDR* pNMHDR, LRESULT* pResult) 
	{
	CheckIsItemSelected();
	*pResult = 0;
	}

//--------------------------------------------------------------------------------
void CPropPageInput::OnSetfocusList(NMHDR* pNMHDR, LRESULT* pResult) 
	{
	CheckIsItemSelected();
	*pResult = 0;
	}

//--------------------------------------------------------------------------------
void CPropPageInput::OnKeydownList(NMHDR* pNMHDR, LRESULT* pResult) 
	{
	CheckIsItemSelected();
	*pResult = 0;
	}

//--------------------------------------------------------------------------------
void CPropPageInput::OnDel()
	{
	// see if there's anything really selected
	CListCtrl* pList = (CListCtrl*) GetDlgItem(IDC_LIST);
	POSITION pos = pList->GetFirstSelectedItemPosition();
	if(pos == NULL)
		return;
	// confirm deletion
	if(::AfxMessageBox(IDS_STRING_CONFIRM_DEL, MB_YESNO) == IDNO)
		return;
	CDWordArray id;
	// get indexes of selected items
	while(pos != NULL)
		id.Add(pList->GetNextSelectedItem(pos));
	for(int i = id.GetUpperBound(); i >= 0; i--)
		{
		// get the config ptr
		CHL7InputDaemonInit* pItem = (CHL7InputDaemonInit*) pList->GetItemData(id[i]);
		// remove it from the list
		for(int n = 0; n < m_pConfigList->GetSize(); n++)
			if(m_pConfigList->GetAt(n) == pItem)
				{
				m_pConfigList->RemoveAt(n);
				delete pItem;
				break;
				}

		// delete from display list ctrl
		pList->DeleteItem(id[i]);
		}

	SetModified();
	UpdateDisplay();
	}

//--------------------------------------------------------------------------------
void CPropPageInput::OnEdit()
	{
	CListCtrl* pList = (CListCtrl*) GetDlgItem(IDC_LIST);
	POSITION pos = pList->GetFirstSelectedItemPosition();
	if(pos == NULL)
		return;
	EditItem(pList->GetNextSelectedItem(pos));
	}
