#if !defined(AFX_HL7SPNEXTMSGNUM_H__7CBF903B_FBDE_11D2_87A7_00104B9E6286__INCLUDED_)
#define AFX_HL7SPNEXTMSGNUM_H__7CBF903B_FBDE_11D2_87A7_00104B9E6286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HL7SPNextMsgNum.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHL7SPNextMsgNum recordset

class CHL7SPNextMsgNum : public CRecordset
{
public:
	CHL7SPNextMsgNum(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CHL7SPNextMsgNum)

// Field/Param Data
	//{{AFX_FIELD(CHL7SPNextMsgNum, CRecordset)
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHL7SPNextMsgNum)
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

#endif // !defined(AFX_HL7SPNEXTMSGNUM_H__7CBF903B_FBDE_11D2_87A7_00104B9E6286__INCLUDED_)
