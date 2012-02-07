//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "output.h"
#include "HL7DBOutMsgFormat.h"

#include "globals.h"

#include "PACSDatabase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//--------------------------------------------------------------------------------
int CHL7OutMsgFormat::m_nRefCount = 0;
CHL7OutMsgFormatItemArray* CHL7OutMsgFormat::m_pItems = NULL;

//--------------------------------------------------------------------------------
CHL7OutMsgFormat::CHL7OutMsgFormat(int nVersion)
	{
	ASSERT(m_pItems != NULL && m_pItems != (CHL7OutMsgFormatItemArray*) 0xFFFFFFFF);

	for(int i = 0; i < m_pItems->GetSize(); i++)
		{
		if(m_pItems->GetAt(i)->m_nVersion == nVersion)
			m_index.Add(m_pItems->GetAt(i));
		}

#ifdef _DEBUG
	{
	for(int i = 0; i <= m_pItems->GetUpperBound(); i++)
		m_pItems->GetAt(i)->TraceDump();
	}
#endif

	m_nRefCount++;
	}

//--------------------------------------------------------------------------------
CHL7OutMsgFormat::CHL7OutMsgFormat(CPACSDatabase& db)
	{
	if(m_nRefCount == 0 && m_pItems == NULL)
		{
		CRecordset records(db.GetDBPtr());

		CString sSelect("select * from " + db.GetTableAccountName() + "HL7_OutMsgFormat" + " where fmttype > 0 order by seqno");

		if(!records.Open(CRecordset::forwardOnly, sSelect))
			{
			m_pItems = (CHL7OutMsgFormatItemArray*) 0xFFFFFFFF;
			SetResultCode(CResult::errOS);
			return;
			}

		m_pItems = new CHL7OutMsgFormatItemArray;

		while(! records.IsEOF())
			{
			CDBVariant vTemp;
			records.GetFieldValue(1, vTemp, SQL_C_SLONG);

			CHL7OutMsgFormatItem* pItem = new CHL7OutMsgFormatItem;
			pItem->m_nVersion = vTemp.m_lVal;

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
CHL7OutMsgFormat::~CHL7OutMsgFormat()
	{
	m_nRefCount--;
	if(m_nRefCount == 0 && m_pItems != (CHL7OutMsgFormatItemArray*) 0xFFFFFFFF)
		{
		delete m_pItems;
		m_pItems = NULL;
		}
	}

//--------------------------------------------------------------------------------
const CHL7OutMsgFormatItem* CHL7OutMsgFormat::GetItem(int nIndex) const
	{
	if(m_index.GetSize() > 0)
		{
		if(nIndex < 0 || nIndex > m_index.GetUpperBound())
			return NULL;

		return m_index.GetAt(nIndex);
		}

	if(m_pItems == NULL || (m_pItems == (CHL7OutMsgFormatItemArray*) 0xFFFFFFFF))
		return NULL;

	if(nIndex < 0 || nIndex > m_pItems->GetUpperBound())
		return NULL;

	return m_pItems->GetAt(nIndex);
	}

//--------------------------------------------------------------------------------
int CHL7OutMsgFormat::GetItemCount() const
	{
	if(m_index.GetSize() > 0)
		return m_index.GetSize();

	if(m_pItems == NULL || (m_pItems == (CHL7OutMsgFormatItemArray*) 0xFFFFFFFF))
		return 0;

	return m_pItems->GetSize();
	}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void CHL7OutMsgFormatItem::TraceDump() const
	{
	GetIO()->FormatOutput(IOMASK_4, "HL7_OutMsgFormat version=%ld, type=%ld, value=\"%s\"", m_nVersion, m_nType, (LPCTSTR) m_sValue);
	}
