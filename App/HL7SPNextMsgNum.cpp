//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "ntserviceeventlogmsg.h"
#include "HL7SPNextMsgNum.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//--------------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CHL7SPNextMsgNum, CRecordset)

CHL7SPNextMsgNum::CHL7SPNextMsgNum(CDatabase* pdb)
		: CRecordset(pdb)
	{
	//{{AFX_FIELD_INIT(CHL7SPNextMsgNum)
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
	}

//--------------------------------------------------------------------------------
CString CHL7SPNextMsgNum::GetDefaultConnect()
	{
	return _T("");
	}

//--------------------------------------------------------------------------------
CString CHL7SPNextMsgNum::GetDefaultSQL()
	{
	return _T("");
	}

//--------------------------------------------------------------------------------
void CHL7SPNextMsgNum::DoFieldExchange(CFieldExchange* pFX)
	{
	//{{AFX_FIELD_MAP(CHL7SPNextMsgNum)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	//}}AFX_FIELD_MAP
	}

#ifdef _DEBUG
//--------------------------------------------------------------------------------
void CHL7SPNextMsgNum::AssertValid() const
	{
	CRecordset::AssertValid();
	}

//--------------------------------------------------------------------------------
void CHL7SPNextMsgNum::Dump(CDumpContext& dc) const
	{
	CRecordset::Dump(dc);
	}
#endif //_DEBUG
