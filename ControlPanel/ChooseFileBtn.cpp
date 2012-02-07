//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// ChooseFileBtn.cpp: implementation of the CChooseFileBtn class.
//
//////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------
#include "stdafx.h"
#include "ControlPanel.h"
#include "ChooseFileBtn.h"

#include <PlatformInfo.h>
#include <LoadString.h>

//--------------------------------------------------------------------------------
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------
CChooseFileBtn::CChooseFileBtn(CDialog* pDlg)
		: m_pDlg(pDlg)
	{

	}

//--------------------------------------------------------------------------------
CChooseFileBtn::~CChooseFileBtn()
	{

	}

//--------------------------------------------------------------------------------
bool CChooseFileBtn::ChooseFile(int nDestId, int nFilterId)
	{
	CString sFilename;
	m_pDlg->GetDlgItemText(nDestId, sFilename);

	CString sCurDir;
	CPlatformInfo info;
	info.GetCurrentDirectory(sCurDir);

	CFileDialog dlg(FALSE, CLoadString::Get(IDS_STRING_DOT_TXT), sFilename, 
		OFN_CREATEPROMPT|OFN_NOCHANGEDIR|OFN_NOREADONLYRETURN|OFN_HIDEREADONLY
		|OFN_SHAREAWARE,
		CLoadString::Get(nFilterId));

	bool bRv = false;

	if(dlg.DoModal() == IDOK)
		{
		m_pDlg->SetDlgItemText(nDestId, dlg.GetPathName());
		bRv = true;
		}

	info.SetCurrentDirectory(sCurDir);

	return bRv;
	}
