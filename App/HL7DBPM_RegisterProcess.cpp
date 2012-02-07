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
#include "HL7DBPM_RegisterProcess.h"
#include <allocstring.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//--------------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CHL7DBPM_RegisterProcess, CStoredProcedure)

//--------------------------------------------------------------------------------
CHL7DBPM_RegisterProcess::CHL7DBPM_RegisterProcess(CDatabase* pdb, LPCTSTR pProcessName)
		: CStoredProcedure(pdb, "PM_RegisterProcess")
		, m_sParam(pProcessName)
		, m_nProcNum(-1)
	{
	//{{AFX_FIELD_INIT(CHL7DBPM_RegisterProcess)
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
	m_nParams = 2;
	Open();
	}

//--------------------------------------------------------------------------------
void CHL7DBPM_RegisterProcess::DoFieldExchange(CFieldExchange* pFX)
	{
	//{{AFX_FIELD_MAP(CHL7DBPM_RegisterProcess)
	//}}AFX_FIELD_MAP
	pFX->SetFieldType(CFieldExchange::inputParam);
	RFX_Text(pFX, _T(""), m_sParam);

	pFX->SetFieldType(CFieldExchange::outputParam);
	RFX_Long(pFX, _T(""), m_nProcNum);
	}

#ifdef _DEBUG
//--------------------------------------------------------------------------------
void CHL7DBPM_RegisterProcess::AssertValid() const
	{
	CRecordset::AssertValid();
	}

//--------------------------------------------------------------------------------
void CHL7DBPM_RegisterProcess::Dump(CDumpContext& dc) const
	{
	CRecordset::Dump(dc);
	}
#endif //_DEBUG
