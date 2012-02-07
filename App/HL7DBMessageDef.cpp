//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "PACSDatabase.h"
#include "HL7DBMessageDef.h"
#include <allocstring.h>

#include "globals.h"

//--------------------------------------------------------------------------------
CHL7DBMessageDefItemList* CHL7DBMessageDef::m_pMsgDefList = NULL;
LPCTSTR CHL7DBMessageDef::pTableName = _T("HL7_MessageDef");
int CHL7DBMessageDef::m_nRefCount = 0;

//--------------------------------------------------------------------------------
CHL7DBMessageDef::CHL7DBMessageDef()
	{
	m_nRefCount++;
	}

//--------------------------------------------------------------------------------
CHL7DBMessageDef::~CHL7DBMessageDef()
	{
	m_nRefCount--;

	if(m_pMsgDefList != (CHL7DBMessageDefItemList*) 0xFFFFFFFF && m_nRefCount < 1)
		{
		delete m_pMsgDefList;
		m_pMsgDefList = NULL;
		}
	}

//--------------------------------------------------------------------------------
bool CHL7DBMessageDef::Init(CPACSDatabase& database)
	{
	ASSERT(m_pMsgDefList == NULL && m_pMsgDefList != (CHL7DBMessageDefItemList*) 0xFFFFFFFF);

	// if its null then this is the first time
	// a CHL7DBMessageDef object is being instansiated so
	// we must populate the list
	// if its 0xFFFFFFFF then we tried this before and got errors

	// in case of an error we can just return
	m_pMsgDefList = (CHL7DBMessageDefItemList*) 0xFFFFFFFF;

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

	m_pMsgDefList = new CHL7DBMessageDefItemList;
	if(! m_pMsgDefList)
		{
		m_pMsgDefList = (CHL7DBMessageDefItemList*) 0xFFFFFFFF;
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

		CHL7DBMessageDefItem* pItem = new CHL7DBMessageDefItem;
		pItem->m_nMsgId = vTemp.m_lVal;

		CString sTemp;

		records.GetFieldValue(1, sTemp);
		sTemp.TrimLeft();
		sTemp.TrimRight();
		ALLOC_STRING(pItem->m_pMsgName, sTemp);

		records.GetFieldValue(2, sTemp);
		sTemp.TrimLeft();
		sTemp.TrimRight();
		ALLOC_STRING(pItem->m_pMsgDesc, sTemp);

		pItem->TraceDump();

		m_pMsgDefList->AddTail(pItem);

		records.MoveNext();
		}

	records.Close();
	return true;
	}

//--------------------------------------------------------------------------------
const CHL7DBMessageDefItem* CHL7DBMessageDef::Find(int nId) const
	{
	return m_pMsgDefList->Find(nId);
	}
	
//--------------------------------------------------------------------------------
const CHL7DBMessageDefItem* CHL7DBMessageDef::Find(LPCTSTR pName) const
	{
	return m_pMsgDefList->Find(pName);
	}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
const CHL7DBMessageDefItem* CHL7DBMessageDefItemList::Find(LPCTSTR pName) const
	{
	if(this == NULL)
		return NULL;

	for(POSITION pos = GetHeadPosition(); pos; GetNext(pos))
		{
		CHL7DBMessageDefItem* pInfo = GetAt(pos);
		if(strcmp(pInfo->m_pMsgName, pName) == 0)
			return pInfo;
		}

	return NULL;
	}

//--------------------------------------------------------------------------------
const CHL7DBMessageDefItem* CHL7DBMessageDefItemList::Find(int nId) const
	{
	if(this == NULL)
		return NULL;

	for(POSITION pos = GetHeadPosition(); pos; GetNext(pos))
		{
		CHL7DBMessageDefItem* pInfo = GetAt(pos);
		if(pInfo->m_nMsgId == nId)
			return pInfo;
		}

	return NULL;
	}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void CHL7DBMessageDefItem::TraceDump()
	{
	GetIO()->FormatOutput(IOMASK_4, "HL7_MessageDef msgid=%d, msgname=\"%s\", msgdesc=\"%s\"", m_nMsgId, m_pMsgName, m_pMsgDesc);
	}

