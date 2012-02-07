//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "output.h"
#include "HL7MSAConfig.h"

#include "globals.h"

#include "PACSDatabase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//--------------------------------------------------------------------------------
int CHL7MSAConfig::m_nRefCount = 0;
CHL7MSAConfigItemArray* CHL7MSAConfig::m_pItems = NULL;

//--------------------------------------------------------------------------------
CHL7MSAConfig::CHL7MSAConfig()
	{
	m_nRefCount++;
	}

//--------------------------------------------------------------------------------
CHL7MSAConfig::CHL7MSAConfig(int nVersion)
	{
	ASSERT(m_pItems != NULL && m_pItems != (CHL7MSAConfigItemArray*) 0xFFFFFFFF);

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
bool CHL7MSAConfig::Init(CPACSDatabase& db)
	{
	CRecordset records(db.GetDBPtr());

	CString sSelect();

	if(! records.Open(CRecordset::forwardOnly,
			"select * from " + db.GetTableAccountName() + "HL7_MSA" + " where segid > 0 order by seqno"))
		{
		m_pItems = (CHL7MSAConfigItemArray*) 0xFFFFFFFF;
		SetResultCode(CResult::errOS);
		return false;
		}

	m_pItems = new CHL7MSAConfigItemArray;

	while(! records.IsEOF())
		{
		CDBVariant vTemp;
		records.GetFieldValue(1, vTemp, SQL_C_SLONG);

		CHL7MSAConfigItem* pItem = new CHL7MSAConfigItem;
		pItem->m_nVersion = vTemp.m_lVal;

		records.GetFieldValue(2, vTemp, SQL_C_SLONG);
		pItem->m_nSegment = vTemp.m_lVal;

		records.GetFieldValue(3, vTemp, SQL_C_SLONG);
		pItem->m_nType = vTemp.m_lVal;

		records.GetFieldValue(4, pItem->m_sValue);
		pItem->m_sValue.TrimRight();

		pItem->TraceDump();
		m_pItems->Add(pItem);
		records.MoveNext();
		}

	return true;
	}

//--------------------------------------------------------------------------------
CHL7MSAConfig::~CHL7MSAConfig()
	{
	m_nRefCount--;
	if(m_nRefCount < 1 && m_pItems != (CHL7MSAConfigItemArray*) 0xFFFFFFFF)
		{
		delete m_pItems;
		m_pItems = NULL;
		}
	}

//--------------------------------------------------------------------------------
const CHL7MSAConfigItem* CHL7MSAConfig::GetItem(int nIndex) const
	{
	if(m_index.GetSize() > 0)
		{
		if(nIndex < 0 || nIndex > m_index.GetUpperBound())
			return NULL;

		return m_index.GetAt(nIndex);
		}

	if(m_pItems == NULL || (m_pItems == (CHL7MSAConfigItemArray*) 0xFFFFFFFF))
		return NULL;

	if(nIndex < 0 || nIndex > m_pItems->GetUpperBound())
		return NULL;

	return m_pItems->GetAt(nIndex);
	}

//--------------------------------------------------------------------------------
int CHL7MSAConfig::GetItemCount() const
	{
	if(m_index.GetSize() > 0)
		return m_index.GetSize();

	if(m_pItems == NULL || (m_pItems == (CHL7MSAConfigItemArray*) 0xFFFFFFFF))
		return 0;

	return m_pItems->GetSize();
	}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void CHL7MSAConfigItem::TraceDump() const
	{
	GetIO()->FormatOutput(IOMASK_4, "HL7_MSA segment=%d, type=%d, value=\"%s\"", m_nSegment, m_nType, (LPCTSTR) m_sValue);
	}
