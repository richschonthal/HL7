//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

//update HL7_Msg_Event set status = 6 where msgno=22000 and msgno=1

#include "stdafx.h"
#include "output.h"
#include "HL7DBPACOut.h"
#include "PACSDatabase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//--------------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CHL7DBPACOut, CRecordset)

//--------------------------------------------------------------------------------
CHL7DBPACOut::CHL7DBPACOut(CDatabase* pdb)
		: CRecordset(pdb)
	{
	//{{AFX_FIELD_INIT(CHL7DBPACOut)
	m_seqno = 0;
	m_msgno = 0;
	m_calluid = 0;
	m_version = 0;
	m_type = 0;
	m_data = _T("");
	m_nFields = 6;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
	}

//--------------------------------------------------------------------------------
CString CHL7DBPACOut::GetDefaultConnect()
	{
	return _T("");
	}

//--------------------------------------------------------------------------------
CString CHL7DBPACOut::GetDefaultSQL()
	{
	return _T(CPACSDatabase::GetTableAccountName() + "HL7_OutPAC");
	}

//--------------------------------------------------------------------------------
void CHL7DBPACOut::DoFieldExchange(CFieldExchange* pFX)
	{
	//{{AFX_FIELD_MAP(CHL7DBPACOut)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[seqno]"), m_seqno);
	RFX_Long(pFX, _T("[msgno]"), m_msgno);
	RFX_Long(pFX, _T("[calluid]"), m_calluid);
	RFX_Long(pFX, _T("[version]"), m_version);
	RFX_Long(pFX, _T("[pactype]"), m_type);
	RFX_Text(pFX, _T("[pacdata]"), m_data);
	//}}AFX_FIELD_MAP
	}

#ifdef _DEBUG
//--------------------------------------------------------------------------------
void CHL7DBPACOut::AssertValid() const
	{
	CRecordset::AssertValid();
	}

//--------------------------------------------------------------------------------
void CHL7DBPACOut::Dump(CDumpContext& dc) const
	{
	CRecordset::Dump(dc);
	}
#endif //_DEBUG
