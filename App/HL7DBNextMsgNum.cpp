//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "PACSDatabase.h"
#include "HL7DBNextMsgNum.h"
#include <allocstring.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//--------------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CHL7DBNextMsgNumRecordset, CRecordset)

//--------------------------------------------------------------------------------
CHL7DBNextMsgNum::CHL7DBNextMsgNum(CPACSDatabase& db)
	{
	m_nMsgNum = 0;

	if(db.HasErrors())
		{
		(CResult) *this = db;
		return;
		}

	CHL7DBNextMsgNumRecordset recordset(db, this);

	if(! recordset.Open(CRecordset::forwardOnly))
		{
		(CResult) *this = recordset;
		return;
		}
	}

//--------------------------------------------------------------------------------
CHL7DBNextMsgNum::operator long() const
	{
	return m_nMsgNum;
	}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
CString CHL7DBNextMsgNumRecordset::m_sCounterName = "HL7_Message_Counter";

//--------------------------------------------------------------------------------
CHL7DBNextMsgNumRecordset::CHL7DBNextMsgNumRecordset(CPACSDatabase& db, CHL7DBNextMsgNum* pWrapper)
		: CStoredProcedure(db.GetDBPtr(), "CNT_GetNextValue")
	{
	m_pWrapper = pWrapper;
	m_nParams = 2;
	m_nFields = 0;
	}

//--------------------------------------------------------------------------------
void CHL7DBNextMsgNumRecordset::DoFieldExchange(CFieldExchange* pFX)
	{
	//{{AFX_FIELD_MAP(CHL7DBNextMsgNum)
	//}}AFX_FIELD_MAP

	pFX->SetFieldType(CFieldExchange::inputParam);
	RFX_Text(pFX, _T(""), m_sCounterName);

	pFX->SetFieldType(CFieldExchange::outputParam);
	RFX_Long(pFX, _T(""), m_pWrapper->m_nMsgNum);
	}

