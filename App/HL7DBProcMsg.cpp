//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "PACSDatabase.h"
#include "HL7DBProcMsg.h"
#include <allocstring.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//--------------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CHL7DBProcMsgRecordset, CRecordset)

//--------------------------------------------------------------------------------
CHL7DBProcMsg::CHL7DBProcMsg(CPACSDatabase& db, const CHL7DBMsgEvent& msgEvent)
	{
	m_nMsgNum = msgEvent.GetMsgNo();

	if(db.HasErrors())
		{
		(CResult) *this = db;
		return;
		}

	CHL7DBProcMsgRecordset recordset(db, this);

	if(! recordset.Open(CRecordset::snapshot))
		(CResult) *this = db;
	}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
CHL7DBProcMsgRecordset::CHL7DBProcMsgRecordset(CPACSDatabase& db, CHL7DBProcMsg* pWrapper)
		: CStoredProcedure(db.GetDBPtr(), "HL7_ProcessMessage")
	{
	m_pWrapper = pWrapper;
	m_nParams = 1;
	}

//--------------------------------------------------------------------------------
void CHL7DBProcMsgRecordset::DoFieldExchange(CFieldExchange* pFX)
	{
	//{{AFX_FIELD_MAP(CHL7DBProcMsg)
	//}}AFX_FIELD_MAP

	pFX->SetFieldType(CFieldExchange::inputParam);
	RFX_Long(pFX, _T(""), m_pWrapper->m_nMsgNum);
	}

