#if !defined(AFX_HL7DBPM_UnregisterProcess_H__357DECB1_11E8_11D3_87B1_00104B9E6286__INCLUDED_)
#define AFX_HL7DBPM_UnregisterProcess_H__357DECB1_11E8_11D3_87B1_00104B9E6286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HL7DBPM_UnregisterProcess.h : header file
//

#include "StoredProcedure.h"

/////////////////////////////////////////////////////////////////////////////
// CHL7DBPM_UnregisterProcess recordset

//--------------------------------------------------------------------------------
class CHL7DBPM_UnregisterProcess : public CStoredProcedure
	{
	private:
		long m_nProcNum;

	public:
		CHL7DBPM_UnregisterProcess(CDatabase*, long);
		DECLARE_DYNAMIC(CHL7DBPM_UnregisterProcess)

	// Field/Param Data
		//{{AFX_FIELD(CHL7DBPM_UnregisterProcess, CRecordset)
		//}}AFX_FIELD


	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CHL7DBPM_UnregisterProcess)
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

#endif // !defined(AFX_HL7DBPM_UnregisterProcess_H__357DECB1_11E8_11D3_87B1_00104B9E6286__INCLUDED_)
