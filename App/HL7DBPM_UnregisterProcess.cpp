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
#include "HL7DBPM_UnregisterProcess.h"
#include <allocstring.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//--------------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CHL7DBPM_UnregisterProcess, CStoredProcedure)

//--------------------------------------------------------------------------------
CHL7DBPM_UnregisterProcess::CHL7DBPM_UnregisterProcess(CDatabase* pdb, long nProcNum)
		: CStoredProcedure(pdb, "PM_UnregisterProcess")
		, m_nProcNum(nProcNum)
	{
	//{{AFX_FIELD_INIT(CHL7DBPM_UnregisterProcess)
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
	m_nParams = 1;
	Open();
	}

//--------------------------------------------------------------------------------
void CHL7DBPM_UnregisterProcess::DoFieldExchange(CFieldExchange* pFX)
	{
	//{{AFX_FIELD_MAP(CHL7DBPM_UnregisterProcess)
	//}}AFX_FIELD_MAP
	pFX->SetFieldType(CFieldExchange::inputParam);
	RFX_Long(pFX, _T(""), m_nProcNum);
	}

#ifdef _DEBUG
//--------------------------------------------------------------------------------
void CHL7DBPM_UnregisterProcess::AssertValid() const
	{
	CRecordset::AssertValid();
	}

//--------------------------------------------------------------------------------
void CHL7DBPM_UnregisterProcess::Dump(CDumpContext& dc) const
	{
	CRecordset::Dump(dc);
	}
#endif //_DEBUG
