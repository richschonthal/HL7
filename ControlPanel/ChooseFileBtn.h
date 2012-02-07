//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// ChooseFileBtn.h: interface for the CChooseFileBtn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHOOSEFILEBTN_H__6626890B_728A_11D3_AEE4_005004A1C5F3__INCLUDED_)
#define AFX_CHOOSEFILEBTN_H__6626890B_728A_11D3_AEE4_005004A1C5F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// intended for use as an additional base class for a CDialog
//--------------------------------------------------------------------------------
class CChooseFileBtn  
	{
	friend class CDialog;
	friend class CPropertyPage;

	private:
		CDialog* m_pDlg;

	protected:
		CChooseFileBtn(CDialog*);
		virtual ~CChooseFileBtn();

		bool ChooseFile(int nDestId, int nFilterId);
	};

#endif // !defined(AFX_CHOOSEFILEBTN_H__6626890B_728A_11D3_AEE4_005004A1C5F3__INCLUDED_)
