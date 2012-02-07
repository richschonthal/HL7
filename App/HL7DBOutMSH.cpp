//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "output.h"
#include "HL7DBOutMSH.h"

#include "globals.h"

#include "PACSDatabase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//--------------------------------------------------------------------------------
int CHL7OutMSH::m_nRefCount = 0;
CHL7OutMSHItemArray* CHL7OutMSH::m_pItems = NULL;

//--------------------------------------------------------------------------------
CHL7OutMSH::CHL7OutMSH()
	{
	ASSERT(m_pItems != NULL && m_pItems != (CHL7OutMSHItemArray*) 0xFFFFFFFF);

	m_nRefCount++;
	}

//--------------------------------------------------------------------------------
CHL7OutMSH::CHL7OutMSH(CPACSDatabase& db)
	{
	if(m_nRefCount == 0 && m_pItems == NULL)
		{
		CRecordset records(db.GetDBPtr());

		CString sSelect("select * from " + db.GetTableAccountName() + "HL7_OutMSH" + " where segid > 0 order by seqno");

		if(!records.Open(CRecordset::forwardOnly, sSelect))
			{
			m_pItems = (CHL7OutMSHItemArray*) 0xFFFFFFFF;
			SetResultCode(CResult::errOS);
			return;
			}

		m_pItems = new CHL7OutMSHItemArray;

		while(! records.IsEOF())
			{
			CDBVariant vTemp;
			records.GetFieldValue(1, vTemp, SQL_C_SLONG);

			CHL7OutMSHItem* pItem = new CHL7OutMSHItem;
			pItem->m_nSegment = vTemp.m_lVal;

			records.GetFieldValue(2, vTemp, SQL_C_SLONG);
			pItem->m_nType = vTemp.m_lVal;

			records.GetFieldValue(3, pItem->m_sValue);

			pItem->TraceDump();

			m_pItems->Add(pItem);

			records.MoveNext();
			}
		}

	m_nRefCount++;
	}

//--------------------------------------------------------------------------------
CHL7OutMSH::~CHL7OutMSH()
	{
	m_nRefCount--;
	if(m_nRefCount == 0 && m_pItems != (CHL7OutMSHItemArray*) 0xFFFFFFFF)
		{
		delete m_pItems;
		m_pItems = NULL;
		}
	}

//--------------------------------------------------------------------------------
const CHL7OutMSHItem* CHL7OutMSH::GetItem(int nIndex) const
	{
	if(m_pItems == NULL || (m_pItems == (CHL7OutMSHItemArray*) 0xFFFFFFFF))
		return NULL;

	if(nIndex < 0 || nIndex > m_pItems->GetUpperBound())
		return NULL;

	return m_pItems->GetAt(nIndex);
	}

//--------------------------------------------------------------------------------
int CHL7OutMSH::GetItemCount() const
	{
	if(m_pItems == NULL || (m_pItems == (CHL7OutMSHItemArray*) 0xFFFFFFFF))
		return 0;

	return m_pItems->GetSize();
	}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void CHL7OutMSHItem::TraceDump() const
	{
	GetIO()->FormatOutput(IOMASK_4, "HL7_OutMSH segment=%d, type=%d, value=\"%s\"", m_nSegment, m_nType, (LPCTSTR) m_sValue);
	}
