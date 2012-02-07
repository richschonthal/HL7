//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#if !defined(AFX_PROPPAGEMAIN_H__5257C8FA_6F70_11D3_AEE2_005004A1C5F3__INCLUDED_)
#define AFX_PROPPAGEMAIN_H__5257C8FA_6F70_11D3_AEE2_005004A1C5F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropPageMain.h : header file
//

//--------------------------------------------------------------------------------
class COutputConfig;

// CPropPageMain dialog
//--------------------------------------------------------------------------------
class CPropPageMain : public CPropertyPage
	{
		DECLARE_DYNCREATE(CPropPageMain)

	// Construction
	public:
		CPropPageMain();
		CPropPageMain(COutputConfig*);
		~CPropPageMain();

	// Dialog Data
		//{{AFX_DATA(CPropPageMain)
		enum { IDD = IDD_PROPPAGE_MAIN };
			// NOTE - ClassWizard will add data members here.
			//    DO NOT EDIT what you see in these blocks of generated code !
		//}}AFX_DATA


	// Overrides
		// ClassWizard generate virtual function overrides
		//{{AFX_VIRTUAL(CPropPageMain)
	public:
	virtual void OnOK();
	virtual BOOL OnApply();
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
	protected:
		// Generated message map functions
		//{{AFX_MSG(CPropPageMain)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeOdbcstring();
	afx_msg void OnChangeProcaccnt();
	afx_msg void OnChangeTableaccnt();
	afx_msg void OnSelchangeDbtype();
	//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

	private:
		COutputConfig* m_pConfig;

		void SaveData();
	};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPPAGEMAIN_H__5257C8FA_6F70_11D3_AEE2_005004A1C5F3__INCLUDED_)
