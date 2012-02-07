//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#if !defined(AFX_STOREDPROCEDURE_H__357DECAF_11E8_11D3_87B1_00104B9E6286__INCLUDED_)
#define AFX_STOREDPROCEDURE_H__357DECAF_11E8_11D3_87B1_00104B9E6286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXDB_H__
#include <afxdb.h>
#endif //__AFXDB_H__

//--------------------------------------------------------------------------------
class CStoredProcedure : public CRecordset
	{
	protected:
		LPTSTR m_pProcName;

	public:
		CStoredProcedure(CDatabase* pDatabase, LPCTSTR pProcedureName);
		~CStoredProcedure();

		DECLARE_DYNAMIC(CStoredProcedure)

	// Field/Param Data
		//{{AFX_FIELD(CStoredProcedure, CRecordset)
		//}}AFX_FIELD


	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CStoredProcedure)
		public:
		virtual CString GetDefaultConnect();    // Default connection string
		virtual CString GetDefaultSQL();    // Default SQL for Recordset
		virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
		//}}AFX_VIRTUAL

	// Implementation
	#ifdef _DEBUG
		virtual void AssertValid() const;
		virtual void Dump(CDumpContext& dc) const;
	#endif

	public:
		virtual BOOL Open(UINT nOpenType = AFX_DB_USE_DEFAULT_TYPE,
			LPCTSTR lpszSQL = NULL, DWORD dwOptions = none);
	};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STOREDPROCEDURE_H__357DECAF_11E8_11D3_87B1_00104B9E6286__INCLUDED_)
