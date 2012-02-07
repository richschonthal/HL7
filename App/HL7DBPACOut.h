//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#if !defined(AFX_HL7DBPACOUT_H__F8C4E385_1CE0_11D3_87B7_00104B9E6286__INCLUDED_)
#define AFX_HL7DBPACOUT_H__F8C4E385_1CE0_11D3_87B7_00104B9E6286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HL7DBPACOut.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHL7DBPACOut recordset

//--------------------------------------------------------------------------------
class CHL7DBPACOut : public CRecordset
	{
	public:
		enum
			{
			PACDT_EMPTY_FIELD,
			PACDT_SEGMENT_NAME,
			PACDT_RESERVED1,
			PACDT_CURRENT_DATE,
			PACDT_STRING,
			PACDT_RESERVED2,
			PACDT_CHAR_CODE,
			PACDT_RESERVED3
			};

	public:
		CHL7DBPACOut(CDatabase* pDatabase = NULL);
		DECLARE_DYNAMIC(CHL7DBPACOut)

	// Field/Param Data
		//{{AFX_FIELD(CHL7DBPACOut, CRecordset)
		long	m_seqno;
		long	m_msgno;
		long	m_calluid;
		long	m_version;
		long	m_type;
		CString	m_data;
		//}}AFX_FIELD


	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CHL7DBPACOut)
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
	};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HL7DBPACOUT_H__F8C4E385_1CE0_11D3_87B7_00104B9E6286__INCLUDED_)
