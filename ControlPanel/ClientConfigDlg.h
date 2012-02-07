//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#if !defined(AFX_CLIENTCONFIGDLG_H__F9AADBA8_74E5_11D3_AEE4_005004A1C5F3__INCLUDED_)
#define AFX_CLIENTCONFIGDLG_H__F9AADBA8_74E5_11D3_AEE4_005004A1C5F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClientConfigDlg.h : header file
//

// CClientConfigDlg dialog

//--------------------------------------------------------------------------------
class CClientConfigDlg : public CDialog
	{
	// Construction
	public:
		CClientConfigDlg(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
		//{{AFX_DATA(CClientConfigDlg)
	enum { IDD = IDD_CLIENT_CONFIG };
	UINT	m_nPortNo;
	//}}AFX_DATA


	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CClientConfigDlg)
		protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
		//}}AFX_VIRTUAL

	// Implementation
	protected:

		// Generated message map functions
		//{{AFX_MSG(CClientConfigDlg)
	afx_msg void OnFieldchangedIpaddress(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

	public:
		DWORD m_ip;
	};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTCONFIGDLG_H__F9AADBA8_74E5_11D3_AEE4_005004A1C5F3__INCLUDED_)
