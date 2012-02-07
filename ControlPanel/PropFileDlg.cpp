//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// PropFileDlg.cpp : implementation file
//

//--------------------------------------------------------------------------------
#include "stdafx.h"
#include "ControlPanel.h"
#include "PropFileDlg.h"

#include <folder_dialog.h>
#include <PlatformInfo.h>
#include <Handle.h>

//--------------------------------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CPropFileDlg dialog

//--------------------------------------------------------------------------------
CPropFileDlg::CPropFileDlg(CWnd* pParent /*=NULL*/)
		: CDialog(CPropFileDlg::IDD, pParent)
	{
	//{{AFX_DATA_INIT(CPropFileDlg)
	m_sInPath = _T("");
	m_sOutPath = _T("");
	//}}AFX_DATA_INIT
	}

//--------------------------------------------------------------------------------
void CPropFileDlg::DoDataExchange(CDataExchange* pDX)
	{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropFileDlg)
	DDX_Text(pDX, IDC_INPUT_PATH, m_sInPath);
	DDX_Text(pDX, IDC_OUTPUT_PATH, m_sOutPath);
	//}}AFX_DATA_MAP
	}

//--------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CPropFileDlg, CDialog)
	//{{AFX_MSG_MAP(CPropFileDlg)
	ON_BN_CLICKED(IDC_CHOOSEPATH_IN, OnChoosepathIn)
	ON_BN_CLICKED(IDC_CHOOSEPATH_OUT, OnChoosepathOut)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// CPropFileDlg message handlers

//--------------------------------------------------------------------------------
void CPropFileDlg::OnChoosepathIn() 
	{
	SelectPath(IDC_INPUT_PATH);
	}

//--------------------------------------------------------------------------------
void CPropFileDlg::OnChoosepathOut() 
	{
	SelectPath(IDC_OUTPUT_PATH);
	}

//--------------------------------------------------------------------------------
void CPropFileDlg::SelectPath(int nItemId)
	{
	CString sPath;
	GetDlgItemText(nItemId, sPath);

	CString sCurDir;
	CPlatformInfo info;
	info.GetCurrentDirectory(sCurDir);

	CFolderDialog dlg(&sPath);
	if(dlg.DoModal() == IDOK)
		SetDlgItemText(nItemId, sPath);

	info.SetCurrentDirectory(sCurDir);
	}

//--------------------------------------------------------------------------------
void CPropFileDlg::OnOK() 
	{
	CString sIn;
	CString sOut;

	GetDlgItemText(IDC_INPUT_PATH, sIn);
	GetDlgItemText(IDC_OUTPUT_PATH, sOut);
	sIn.TrimLeft();
	sIn.TrimRight();
	sOut.TrimLeft();
	sOut.TrimRight();
	if(sIn.CompareNoCase(sOut) == 0 || sIn.IsEmpty() || sOut.IsEmpty())
		{
		::AfxMessageBox(IDS_IN_OUT_PATH_ERR);
		return;
		}

	CDialog::OnOK();
	}
