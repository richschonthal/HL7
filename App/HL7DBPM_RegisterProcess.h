#if !defined(AFX_HL7DBPM_REGISTERPROCESS_H__357DECB1_11E8_11D3_87B1_00104B9E6286__INCLUDED_)
#define AFX_HL7DBPM_REGISTERPROCESS_H__357DECB1_11E8_11D3_87B1_00104B9E6286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HL7DBPM_RegisterProcess.h : header file
//

#include "StoredProcedure.h"

/////////////////////////////////////////////////////////////////////////////
// CHL7DBPM_RegisterProcess recordset

//--------------------------------------------------------------------------------
class CHL7DBPM_RegisterProcess : public CStoredProcedure
	{
	private:
		CString m_sParam;

	public:
		long m_nProcNum;

	public:
		CHL7DBPM_RegisterProcess(CDatabase*, LPCTSTR);
		DECLARE_DYNAMIC(CHL7DBPM_RegisterProcess)

	// Field/Param Data
		//{{AFX_FIELD(CHL7DBPM_RegisterProcess, CRecordset)
		//}}AFX_FIELD


	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CHL7DBPM_RegisterProcess)
		public:
		virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
		//}}AFX_VIRTUAL

	// Implementation
	#ifdef _DEBUG
		virtual void AssertValid() const;
		virtual void Dump(CDumpContext& dc) const;
	#endif
	};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HL7DBPM_REGISTERPROCESS_H__357DECB1_11E8_11D3_87B1_00104B9E6286__INCLUDED_)
