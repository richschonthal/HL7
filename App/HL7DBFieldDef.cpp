//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "PACSDatabase.h"
#include "HL7DBFieldDef.h"
#include <allocstring.h>

#include "globals.h"

//--------------------------------------------------------------------------------
CHL7DBFieldDefItemList* CHL7DBFieldDef::m_pFieldDefList = NULL;
LPCTSTR CHL7DBFieldDef::pTableName = _T("HL7_FieldDef");
int CHL7DBFieldDef::m_nRefCount = 0;

//--------------------------------------------------------------------------------
CHL7DBFieldDef::CHL7DBFieldDef()
	{
	m_nVersion = 0;
	m_nRefCount++;
	}

//--------------------------------------------------------------------------------
CHL7DBFieldDef::CHL7DBFieldDef(int nVersion)
	{
	ASSERT(m_pFieldDefList != NULL && m_pFieldDefList != (CHL7DBFieldDefItemList*) 0xFFFFFFFF);

	// bail if there were errors
	if(m_pFieldDefList == NULL || m_pFieldDefList == (CHL7DBFieldDefItemList*) 0xFFFFFFFF)
		return;

	m_nVersion = nVersion;
	m_nRefCount++;
	}

//--------------------------------------------------------------------------------
CHL7DBFieldDef::CHL7DBFieldDef(int nVersion, int nSegId)
	{
	ASSERT(m_pFieldDefList != NULL && m_pFieldDefList != (CHL7DBFieldDefItemList*) 0xFFFFFFFF);

	// bail if there were errors
	if(m_pFieldDefList == NULL || m_pFieldDefList == (CHL7DBFieldDefItemList*) 0xFFFFFFFF)
		return;
	m_nVersion = nVersion;
	Find(nSegId, m_index);
	m_nRefCount++;
	}

//--------------------------------------------------------------------------------
CHL7DBFieldDef::~CHL7DBFieldDef()
	{
	m_nRefCount --;

	if(m_pFieldDefList != (CHL7DBFieldDefItemList*) 0xFFFFFFFF && m_nRefCount < 1)
		{
		delete m_pFieldDefList;
		m_pFieldDefList = NULL;
		}
	}

//--------------------------------------------------------------------------------
bool CHL7DBFieldDef::Init(CPACSDatabase& database)
	{
	ASSERT(m_pFieldDefList == NULL && m_pFieldDefList != (CHL7DBFieldDefItemList*) 0xFFFFFFFF);

	// if its null then this is the first time
	// a CHL7DBFieldDef object is being instansiated so
	// we must populate the list
	// if its 0xFFFFFFFF then we tried this before and got errors

	// in case of an error we can just return
	m_pFieldDefList = (CHL7DBFieldDefItemList*) 0xFFFFFFFF;

	if(database.HasErrors())
		{
		(CResult) *this = (CResult) database;
		return false;
		}

	CRecordset records(database.GetDBPtr());

	if(! records.Open(CRecordset::forwardOnly, 
			CString("select * from ") + database.GetTableAccountName() + pTableName))
		{
		return false;
		}

	m_pFieldDefList = new CHL7DBFieldDefItemList;
	if(! m_pFieldDefList)
		{
		m_pFieldDefList = (CHL7DBFieldDefItemList*) 0xFFFFFFFF;
		records.Close();
		return false;
		}

	// now we've opened and read the database and have
	// created our list - now we move the data into the list
	// and we're done
	while(! records.IsEOF())
		{
		CDBVariant vTemp;
		records.GetFieldValue((int) 0, vTemp, SQL_C_SLONG);

		CHL7DBFieldDefItem* pItem = new CHL7DBFieldDefItem;
		pItem->m_nSegId = vTemp.m_lVal;

		records.GetFieldValue(1, vTemp, SQL_C_SLONG);
		pItem->m_nFieldId = vTemp.m_lVal;

		CString sTemp;

		records.GetFieldValue(2, sTemp);
		sTemp.TrimLeft();
		sTemp.TrimRight();
		ALLOC_STRING(pItem->m_pDesc, sTemp);

		records.GetFieldValue(3, vTemp, SQL_C_SLONG);
		pItem->m_nRuleId = vTemp.m_lVal;

		records.GetFieldValue(4, vTemp, SQL_C_SLONG);
		pItem->m_nVersionId = vTemp.m_lVal;

		records.GetFieldValue(5, vTemp, SQL_C_SLONG);
		pItem->m_nFieldLen = vTemp.m_lVal;

		records.GetFieldValue(6, sTemp);
		sTemp.TrimLeft();
		sTemp.TrimRight();
		ALLOC_STRING(pItem->m_pDatatype, sTemp);

		records.GetFieldValue(7, sTemp);
		sTemp.TrimLeft();
		sTemp.TrimRight();
		pItem->m_bMandatory = (sTemp == "M");

		records.GetFieldValue(8, vTemp, SQL_C_SLONG);
		pItem->m_nCalluid = vTemp.m_lVal;

		records.GetFieldValue(9, vTemp, SQL_C_SLONG);
		pItem->m_nWrapLen = vTemp.m_lVal;

		pItem->TraceDump();

		m_pFieldDefList->AddTail(pItem);

		records.MoveNext();
		}

	records.Close();
	return true;
	}

//--------------------------------------------------------------------------------
const CHL7DBFieldDefItem* CHL7DBFieldDef::Find(int nCalluid) const
	{
	return m_pFieldDefList->Find(m_nVersion, nCalluid);
	}

//--------------------------------------------------------------------------------
const CHL7DBFieldDefItem* CHL7DBFieldDef::Find(int nSegId, int nFieldId) const
	{
	return m_pFieldDefList->Find(m_nVersion, nSegId, nFieldId);
	}

//--------------------------------------------------------------------------------
int CHL7DBFieldDef::Find(int nSegId, CHL7DBFieldDefItemArray& aResults) const
	{
	return m_pFieldDefList->Find(m_nVersion, nSegId, aResults);
	}

//--------------------------------------------------------------------------------
const CHL7DBFieldDefItem* CHL7DBFieldDef::GetIndexedItem(int nIndex) const
	{
	if(nIndex < 0 || nIndex > m_index.GetUpperBound())
		return NULL;
	return m_index.GetAt(nIndex);
	}

//--------------------------------------------------------------------------------
int CHL7DBFieldDef::GetRecordCount() const
	{
	if(m_index.GetSize() > 0)
		return m_index.GetSize();
	else
		return m_pFieldDefList->GetCount();
	}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
const CHL7DBFieldDefItem* CHL7DBFieldDefItemList::Find(int nVersion, int nCalluid) const
	{
	if(this == NULL)
		return NULL;

	for(POSITION pos = GetHeadPosition(); pos; GetNext(pos))
		{
		CHL7DBFieldDefItem* pInfo = GetAt(pos);
		if(pInfo->m_nCalluid == nCalluid && pInfo->m_nVersionId == nVersion)
			return pInfo;
		}

	return NULL;
	}

//--------------------------------------------------------------------------------
const CHL7DBFieldDefItem* CHL7DBFieldDefItemList::Find(int nVersion, int nSegId, int nFieldId) const
	{
	if(this == NULL)
		return NULL;

	for(POSITION pos = GetHeadPosition(); pos; GetNext(pos))
		{
		CHL7DBFieldDefItem* pInfo = GetAt(pos);
		if(pInfo->m_nSegId == nSegId && pInfo->m_nFieldId == nFieldId && pInfo->m_nVersionId == nVersion)
			return pInfo;
		}

	return NULL;
	}

//--------------------------------------------------------------------------------
int CHL7DBFieldDefItemList::Find(int nVersion, int nSegId, CHL7DBFieldDefItemArray& aResults) const
	{
	if(this == NULL)
		return 0;

	aResults.RemoveAll();

	for(POSITION pos = GetHeadPosition(); pos; GetNext(pos))
		{
		CHL7DBFieldDefItem* pInfo = GetAt(pos);
		if(pInfo->m_nSegId == nSegId && pInfo->m_nVersionId == nVersion)
			aResults.Add(pInfo);
		}

	return aResults.GetSize();
	}

//--------------------------------------------------------------------------------
CHL7DBFieldDefItem* CHL7DBFieldDefItemList::FindFirst(int nVersion, int nSegId) const
	{
	if(this == NULL)
		return NULL;

	for(POSITION pos = GetHeadPosition(); pos; )
		{
		CHL7DBFieldDefItem* pInfo = GetNext(pos);
		if(pInfo->m_nSegId == nSegId && pInfo->m_nVersionId == nVersion)
			return pInfo;
		}

	return NULL;
	}

//--------------------------------------------------------------------------------
CHL7DBFieldDefItem* CHL7DBFieldDefItemList::FindLast(int nVersion, int nSegId) const
	{
	if(this == NULL)
		return NULL;

	for(POSITION pos = GetTailPosition(); pos; )
		{
		CHL7DBFieldDefItem* pInfo = GetPrev(pos);
		if(pInfo->m_nSegId == nSegId && pInfo->m_nVersionId == nVersion)
			return pInfo;
		}

	return NULL;
	}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void CHL7DBFieldDefItem::TraceDump()
	{
	GetIO()->FormatOutput(IOMASK_4, "HL7_FieldDef segid=%ld, fieldid=%ld, desc=\"%s\", ruleid=%ld, version=%ld, fieldlen=%ld, datatype=\"%s\", mandatory=%d, calluid=%ld",
		m_nSegId, m_nFieldId, m_pDesc, m_nRuleId, m_nVersionId, m_nFieldLen, m_pDatatype, (int) m_bMandatory, m_nCalluid);
	}

