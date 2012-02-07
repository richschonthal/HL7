//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#if !defined(AFX_PROPPAGESECURITY_H__2AE8D951_14DB_47E1_BB3B_0972AA543330__INCLUDED_)
#define AFX_PROPPAGESECURITY_H__2AE8D951_14DB_47E1_BB3B_0972AA543330__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropPageSecurity.h : header file
//

//--------------------------------------------------------------------------------
class COutputConfig;

//--------------------------------------------------------------------------------
class CPropPageSecurity : public CPropertyPage
	{
	DECLARE_DYNCREATE(CPropPageSecurity)

	// Construction
	public:
		CPropPageSecurity(COutputConfig* = NULL);
		~CPropPageSecurity();

	// Dialog Data
		//{{AFX_DATA(CPropPageSecurity)
	enum { IDD = IDD_PROPPAGE_SECURITY };
	//}}AFX_DATA

	// Overrides
		// ClassWizard generate virtual function overrides
		//{{AFX_VIRTUAL(CPropPageSecurity)
	public:
	virtual BOOL OnApply();
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
	protected:
		// Generated message map functions
		//{{AFX_MSG(CPropPageSecurity)
	afx_msg void OnFieldchangedBkupSsIp(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeBkupSsPort();
	afx_msg void OnFieldchangedMainSsIp(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeMainSsPort();
	//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

	private:
		COutputConfig* m_pConfig;

		void SaveData();
	};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPPAGESECURITY_H__2AE8D951_14DB_47E1_BB3B_0972AA543330__INCLUDED_)
