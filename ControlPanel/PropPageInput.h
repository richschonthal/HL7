//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#if !defined(AFX_PROPPAGEINPUT_H__5257C8FB_6F70_11D3_AEE2_005004A1C5F3__INCLUDED_)
#define AFX_PROPPAGEINPUT_H__5257C8FB_6F70_11D3_AEE2_005004A1C5F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropPageInput.h : header file
//

//--------------------------------------------------------------------------------
class CHL7InputDaemonInitList;
class CHL7InputDaemonInit;
class COutputConfig;

/////////////////////////////////////////////////////////////////////////////
// CPropPageInput dialog

//--------------------------------------------------------------------------------
class CPropPageInput : public CPropertyPage
	{
	DECLARE_DYNCREATE(CPropPageInput)

	// Construction
	public:
		CPropPageInput();
		CPropPageInput(const COutputConfig*, CHL7InputDaemonInitList*);
		~CPropPageInput();

	// Dialog Data
		//{{AFX_DATA(CPropPageInput)
		enum { IDD = IDD_PROPPAGE_INPUT };
			// NOTE - ClassWizard will add data members here.
			//    DO NOT EDIT what you see in these blocks of generated code !
		//}}AFX_DATA


	// Overrides
		// ClassWizard generate virtual function overrides
		//{{AFX_VIRTUAL(CPropPageInput)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnApply();
	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
	protected:
		// Generated message map functions
		//{{AFX_MSG(CPropPageInput)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAdd();
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetfocusList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydownList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDel();
	afx_msg void OnEdit();
	//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

	private:
		const COutputConfig* m_pOutConfig;
		CHL7InputDaemonInitList* m_pConfigList;

	private:
		enum
			{
			COL_TYPE,
			COL_NAME,
			COL_VERSION,
			COL_CONFIG
			};

		void ConfigToList(int, CHL7InputDaemonInit*);
		void UpdateDisplay();

		bool CheckIsItemSelected();

		void EditItem(int);
	};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPPAGEINPUT_H__5257C8FB_6F70_11D3_AEE2_005004A1C5F3__INCLUDED_)
