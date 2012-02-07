//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "output.h"
#include "StoredProcedure.h"
#include "HL7DBPM_GetProcessAction.h"
#include <allocstring.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//--------------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CHL7DBPM_GetProcessAction, CStoredProcedure)

//--------------------------------------------------------------------------------
CHL7DBPM_GetProcessAction::CHL7DBPM_GetProcessAction(CDatabase* pdb, long nProcNum)
		: CStoredProcedure(pdb, "PM_GetProcessAction")
		, m_nAction(ACTION_NONE)
		, m_nProcNum(nProcNum)
	{
	//{{AFX_FIELD_INIT(CHL7DBPM_GetProcessAction)
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
	m_nParams = 2;
	Open();
	}

//--------------------------------------------------------------------------------
void CHL7DBPM_GetProcessAction::DoFieldExchange(CFieldExchange* pFX)
	{
	//{{AFX_FIELD_MAP(CHL7DBPM_GetProcessAction)
	//}}AFX_FIELD_MAP
	pFX->SetFieldType(CFieldExchange::inputParam);
	RFX_Long(pFX, _T(""), m_nProcNum);

	pFX->SetFieldType(CFieldExchange::outputParam);
	RFX_Long(pFX, _T(""), m_nAction);
	}

#ifdef _DEBUG
//--------------------------------------------------------------------------------
void CHL7DBPM_GetProcessAction::AssertValid() const
	{
	CRecordset::AssertValid();
	}

//--------------------------------------------------------------------------------
void CHL7DBPM_GetProcessAction::Dump(CDumpContext& dc) const
	{
	CRecordset::Dump(dc);
	}
#endif //_DEBUG
