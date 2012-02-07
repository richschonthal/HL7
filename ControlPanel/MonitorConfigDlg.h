//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#if !defined(AFX_MONITORCONFIGDLG_H__4B03F5B1_69E9_11D3_AEDF_005004A1C5F3__INCLUDED_)
#define AFX_MONITORCONFIGDLG_H__4B03F5B1_69E9_11D3_AEDF_005004A1C5F3__INCLUDED_

//--------------------------------------------------------------------------------
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
class CHL7InputDaemonInit;
class CHL7InputDaemonInitList;

//--------------------------------------------------------------------------------
#include "ChooseFileBtn.h"

//--------------------------------------------------------------------------------
class CMonitorConfigDlg : public CDialog, public CChooseFileBtn
	{
	// Construction
	public:
		CMonitorConfigDlg(CHL7InputDaemonInit* = NULL, CHL7InputDaemonInitList* = NULL);   // standard constructor
		~CMonitorConfigDlg();

		// Dialog Data
		//{{AFX_DATA(CMonitorConfigDlg)
		enum { IDD = IDD_INPUT_CONFIG };
			// NOTE: the ClassWizard will add data members here
		//}}AFX_DATA

		// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CMonitorConfigDlg)
		protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
		//}}AFX_VIRTUAL

	// Implementation
	protected:

		// Generated message map functions
		//{{AFX_MSG(CMonitorConfigDlg)
		virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnProperties();
	afx_msg void OnChooselogpath();
	afx_msg void OnClient();
	afx_msg void OnConfig();
	afx_msg void OnChangeVersion();
	afx_msg void OnIn();
	afx_msg void OnOut();
	//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

	public:
		CHL7InputDaemonInit* m_pOrigInit;
		CHL7InputDaemonInit* m_pInit;
		CHL7InputDaemonInitList* m_pInitList;
		void ConfigToDlg();
		void DlgToConfig();

		bool CheckConfigBtnEnable();
	};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MONITORCONFIGDLG_H__4B03F5B1_69E9_11D3_AEDF_005004A1C5F3__INCLUDED_)
