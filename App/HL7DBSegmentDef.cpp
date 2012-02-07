//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "PACSDatabase.h"
#include "HL7DBSegmentDef.h"
#include <allocstring.h>

#include "globals.h"

//--------------------------------------------------------------------------------
CHL7DBSegmentDefItemList* CHL7DBSegmentDef::m_pSegDefList = NULL;
LPCTSTR CHL7DBSegmentDef::pTableName = _T("HL7_SegmentDef");
int CHL7DBSegmentDef::m_nRefCount = 0;

//--------------------------------------------------------------------------------
CHL7DBSegmentDef::CHL7DBSegmentDef()
	{
	m_nRefCount++;
	}

//--------------------------------------------------------------------------------
bool CHL7DBSegmentDef::Init(CPACSDatabase& database)
	{
	ASSERT(m_pSegDefList == NULL && m_pSegDefList != (CHL7DBSegmentDefItemList*) 0xFFFFFFFF);

	// if its null then this is the first time
	// a CHL7DBSegmentDef object is being instansiated so
	// we must populate the list
	// if its 0xFFFFFFFF then we tried this before and got errors

	// in case of an error we can just return
	m_pSegDefList = (CHL7DBSegmentDefItemList*) 0xFFFFFFFF;

	if(database.HasErrors())
		{
		(CResult) *this = (CResult) database;
		return false;
		}

	CRecordset records(database.GetDBPtr());

	if(! records.Open(CRecordset::forwardOnly, "select * from " + database.GetTableAccountName() + pTableName))
		return false;

	m_pSegDefList = new CHL7DBSegmentDefItemList;
	if(m_pSegDefList == NULL)
		{
		m_pSegDefList = (CHL7DBSegmentDefItemList*) 0xFFFFFFFF;
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

		CHL7DBSegmentDefItem* pItem = new CHL7DBSegmentDefItem;
		pItem->m_nSegId = vTemp.m_lVal;

		CString sTemp;

		records.GetFieldValue(1, sTemp);
		sTemp.TrimLeft();
		sTemp.TrimRight();
		ALLOC_STRING(pItem->m_pSegName, sTemp);

		records.GetFieldValue(2, sTemp);
		sTemp.TrimLeft();
		sTemp.TrimRight();
		ALLOC_STRING(pItem->m_pSegDesc, sTemp);

		pItem->TraceDump();

		m_pSegDefList->AddTail(pItem);

		records.MoveNext();
		}

	records.Close();
	return true;
	}

//--------------------------------------------------------------------------------
CHL7DBSegmentDef::~CHL7DBSegmentDef()
	{
	m_nRefCount--;
	if(m_pSegDefList != (CHL7DBSegmentDefItemList*) 0xFFFFFFFF && m_nRefCount < 1)
		{
		delete m_pSegDefList;
		m_pSegDefList = NULL;
		}
	}

//--------------------------------------------------------------------------------
const CHL7DBSegmentDefItem* CHL7DBSegmentDef::Find(int nId) const
	{
	return m_pSegDefList->Find(nId);
	}

//--------------------------------------------------------------------------------
const CHL7DBSegmentDefItem* CHL7DBSegmentDef::Find(LPCTSTR pName) const
	{
	return m_pSegDefList->Find(pName);
	}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
const CHL7DBSegmentDefItem* CHL7DBSegmentDefItemList::Find(int nId) const
	{
	if(this == NULL)
		return NULL;

	for(POSITION pos = GetHeadPosition(); pos; GetNext(pos))
		{
		CHL7DBSegmentDefItem* pInfo = GetAt(pos);
		if(pInfo->m_nSegId == nId)
			return pInfo;
		}

	return NULL;
	}

//--------------------------------------------------------------------------------
const CHL7DBSegmentDefItem* CHL7DBSegmentDefItemList::Find(LPCTSTR pName) const
	{
	if(this == NULL)
		return NULL;

	for(POSITION pos = GetHeadPosition(); pos; GetNext(pos))
		{
		CHL7DBSegmentDefItem* pInfo = GetAt(pos);
		if(strcmp(pInfo->m_pSegName, pName) == 0)
			return pInfo;
		}

	return NULL;
	}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void CHL7DBSegmentDefItem::TraceDump()
	{
	GetIO()->FormatOutput(IOMASK_4, "HL7_SegmentDef segid=%d, segname=\"%s\", segdesc=\"%s\"", m_nSegId, m_pSegName, m_pSegDesc);
	}

