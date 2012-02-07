//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"

// this class is not used
#if 0

#include "PACSDatabase.h"
#include "HL7DBMsgSegDef.h"
#include <allocstring.h>

#include "globals.h"

//--------------------------------------------------------------------------------
CHL7DBMsgSegDefItemList* CHL7DBMsgSegDef::m_pMsgSegDefList = NULL;
LPCTSTR CHL7DBMsgSegDef::pTableName = _T("HL7_MsgSegDef");
int CHL7DBMsgSegDef::m_nRefCount = 0;

//--------------------------------------------------------------------------------
CHL7DBMsgSegDef::CHL7DBMsgSegDef()
		: m_posCurrent(NULL)
	{
	}

//--------------------------------------------------------------------------------
bool CHL7DBMsgSegDef::Init(CPACSDatabase& database)
	{
	// this class is not currently used
	ASSERT(FALSE);

	// if its null then this is the first time
	// a CHL7DBMsgSegDef object is being instanciated so
	// we must populate the list
	// if its 0xFFFFFFFF then we tried this before and got errors
	ASSERT(m_pMsgSegDefList == NULL && m_pMsgSegDefList != (CHL7DBMsgSegDefItemList*) 0xFFFFFFFF);
	
	// in case of an error we can just return
	m_pMsgSegDefList = (CHL7DBMsgSegDefItemList*) 0xFFFFFFFF;

	if(database.HasErrors())
		{
		(CResult) *this = (CResult) database;
		return false;
		}

	CRecordset records(database.GetDBPtr());

	if(! records.Open(CRecordset::forwardOnly,
			"select * from " + database.GetTableAccountName() + pTableName + " order by msgid, segid"))
		return false;

	m_pMsgSegDefList = new CHL7DBMsgSegDefItemList;
	if(! m_pMsgSegDefList)
		{
		m_pMsgSegDefList = (CHL7DBMsgSegDefItemList*) 0xFFFFFFFF;
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

		CHL7DBMsgSegDefItem* pItem = new CHL7DBMsgSegDefItem;
		pItem->m_nMsgId = vTemp.m_lVal;

		records.GetFieldValue(1, vTemp, SQL_C_SLONG);
		pItem->m_nSegId = vTemp.m_lVal;

		pItem->TraceDump();

		m_pMsgSegDefList->AddTail(pItem);

		records.MoveNext();
		}

	records.Close();
	return true;
	}

//--------------------------------------------------------------------------------
CHL7DBMsgSegDef::~CHL7DBMsgSegDef()
	{
	m_nRefCount --;
	if(m_pMsgSegDefList != (CHL7DBMsgSegDefItemList*) 0xFFFFFFFF && m_nRefCount < 1)
		{
		delete m_pMsgSegDefList;
		m_pMsgSegDefList = NULL;
		}
	}

//--------------------------------------------------------------------------------
const CHL7DBMsgSegDefItem* CHL7DBMsgSegDef::Find(int nMsgId) const
	{
	return m_pMsgSegDefList->Find(nMsgId);
	}

//--------------------------------------------------------------------------------
int CHL7DBMsgSegDef::GetSegCount() const
	{
	return m_nRefCount;
	}

//--------------------------------------------------------------------------------
CHL7DBMsgSegDefItem* CHL7DBMsgSegDef::GetHead()
	{
	if(m_pMsgSegDefList != NULL)
		{
		m_posCurrent = m_pMsgSegDefList->GetHeadPosition();
		return m_pMsgSegDefList->GetNext(m_posCurrent);
		}

	return NULL;
	}

//--------------------------------------------------------------------------------
CHL7DBMsgSegDefItem* CHL7DBMsgSegDef::GetNext()
	{
	if( m_pMsgSegDefList != NULL && m_posCurrent != NULL )
		return m_pMsgSegDefList->GetNext(m_posCurrent);
	return NULL;
	}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
const CHL7DBMsgSegDefItem* CHL7DBMsgSegDefItemList::Find(int nMsgId) const
	{
	if(this == NULL)
		return NULL;

	for(POSITION pos = GetHeadPosition(); pos; GetNext(pos))
		{
		CHL7DBMsgSegDefItem* pInfo = GetAt(pos);
		if(pInfo->m_nMsgId == nMsgId)
			return pInfo;
		}

	return NULL;
	}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void CHL7DBMsgSegDefItem::TraceDump()
	{
	GetIO()->FormatOutput(IOMASK_4, "HL7_MsgSegDef msgid=%ld, segid=%ld", m_nMsgId, m_nSegId);
	}

#endif // 0
