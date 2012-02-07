//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#if !defined(AFX_PROPFILEDLG_H__66268908_728A_11D3_AEE4_005004A1C5F3__INCLUDED_)
#define AFX_PROPFILEDLG_H__66268908_728A_11D3_AEE4_005004A1C5F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropFileDlg.h : header file
//

// CPropFileDlg dialog

//--------------------------------------------------------------------------------
class CPropFileDlg : public CDialog
	{
	// Construction
	public:
		CPropFileDlg(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
		//{{AFX_DATA(CPropFileDlg)
		enum { IDD = IDD_PROPDLG_FILE };
		CString	m_sInPath;
		CString	m_sOutPath;
		//}}AFX_DATA


	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CPropFileDlg)
		protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
		//}}AFX_VIRTUAL

	// Implementation
	protected:

		// Generated message map functions
		//{{AFX_MSG(CPropFileDlg)
	afx_msg void OnChoosepathIn();
	afx_msg void OnChoosepathOut();
	virtual void OnOK();
	//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

	private:
		void SelectPath(int);
	};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPFILEDLG_H__66268908_728A_11D3_AEE4_005004A1C5F3__INCLUDED_)
