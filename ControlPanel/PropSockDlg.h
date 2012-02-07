//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#if !defined(AFX_PROPSOCKDLG_H__F9AADBA4_74E5_11D3_AEE4_005004A1C5F3__INCLUDED_)
#define AFX_PROPSOCKDLG_H__F9AADBA4_74E5_11D3_AEE4_005004A1C5F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropSockDlg.h : header file
//

// CPropSockDlg dialog

//--------------------------------------------------------------------------------
class CPropSockDlg : public CDialog
	{
	// Construction
	public:
		CPropSockDlg(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
		//{{AFX_DATA(CPropSockDlg)
	enum { IDD = IDD_PROPDLG_SOCKET };
	UINT	m_nSockNo;
	//}}AFX_DATA


	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CPropSockDlg)
		protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
		//}}AFX_VIRTUAL

	// Implementation
	protected:

		// Generated message map functions
		//{{AFX_MSG(CPropSockDlg)
			// NOTE: the ClassWizard will add member functions here
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()
	};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPSOCKDLG_H__F9AADBA4_74E5_11D3_AEE4_005004A1C5F3__INCLUDED_)
