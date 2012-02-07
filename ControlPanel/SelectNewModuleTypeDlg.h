//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#if !defined(AFX_SELECTNEWMODULETYPEDLG_H__A9F05DD7_7037_11D3_AEE2_005004A1C5F3__INCLUDED_)
#define AFX_SELECTNEWMODULETYPEDLG_H__A9F05DD7_7037_11D3_AEE2_005004A1C5F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectNewModuleTypeDlg.h : header file
//

// CSelectNewModuleTypeDlg dialog

//--------------------------------------------------------------------------------
class CSelectNewModuleTypeDlg : public CDialog
	{
	// Construction
	public:
		CSelectNewModuleTypeDlg(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
		//{{AFX_DATA(CSelectNewModuleTypeDlg)
		enum { IDD = IDD_NEW_MODULE };
			// NOTE: the ClassWizard will add data members here
		//}}AFX_DATA


	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CSelectNewModuleTypeDlg)
		protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
		//}}AFX_VIRTUAL

	// Implementation
	protected:

		// Generated message map functions
		//{{AFX_MSG(CSelectNewModuleTypeDlg)
		afx_msg void OnRadioDb();
		afx_msg void OnRadioFile();
	afx_msg void OnRadioSerial();
	afx_msg void OnRadioSocket();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

	private:
		DWORD m_nType;

	public:
		DWORD GetType() const { return m_nType; }
	};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTNEWMODULETYPEDLG_H__A9F05DD7_7037_11D3_AEE2_005004A1C5F3__INCLUDED_)
