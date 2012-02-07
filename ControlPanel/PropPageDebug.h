//--------------------------------------------------------------------------------
//
// Copyright (c) 2001 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#if !defined(AFX_PROPPAGEDEBUG_H__3759E9BC_F137_4E1A_9511_42B187D5840C__INCLUDED_)
#define AFX_PROPPAGEDEBUG_H__3759E9BC_F137_4E1A_9511_42B187D5840C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropPageDebug.h : header file
//

//--------------------------------------------------------------------------------
class COutputConfig;

//--------------------------------------------------------------------------------
#include "ChooseFileBtn.h"

//--------------------------------------------------------------------------------
class CPropPageDebug : public CPropertyPage, public CChooseFileBtn
	{
	DECLARE_DYNCREATE(CPropPageDebug)
	// Construction
	public:
		CPropPageDebug(COutputConfig* = NULL);   // standard constructor

	// Dialog Data
		//{{AFX_DATA(CPropPageDebug)
		enum { IDD = IDD_PROPPAGE_DEBUG };
			// NOTE: the ClassWizard will add data members here
		//}}AFX_DATA


	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CPropPageDebug)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnApply();
	virtual BOOL OnKillActive();
	virtual void OnOK();
	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
	protected:

		// Generated message map functions
		//{{AFX_MSG(CPropPageDebug)
	afx_msg void OnChoosepath();
	afx_msg void OnChangeLogFile();
	afx_msg void OnCheckAll();
	afx_msg void OnClearAll();
	afx_msg void OnDefault();
	afx_msg void OnCheck1();
	afx_msg void OnCheck2();
	afx_msg void OnCheck3();
	afx_msg void OnCheck4();
	afx_msg void OnCheck5();
	afx_msg void OnCheck6();
	afx_msg void OnCheck7();
	afx_msg void OnCheck8();
	afx_msg void OnCheck9();
	afx_msg void OnCheck10();
	afx_msg void OnCheck11();
	afx_msg void OnCheck12();
	afx_msg void OnCheck13();
	afx_msg void OnCheck14();
	afx_msg void OnCheck15();
	afx_msg void OnDailyLog();
	//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

	private:
		COutputConfig* m_pConfig;
		void SaveData();
	};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPPAGEDEBUG_H__3759E9BC_F137_4E1A_9511_42B187D5840C__INCLUDED_)
