//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "PACSDatabase.h"
#include "HL7DBRuleDef.h"
#include <allocstring.h>

#include "globals.h"

//--------------------------------------------------------------------------------
CHL7DBRuleDefItemList* CHL7DBRuleDef::m_pMsgDefList = NULL;
LPCTSTR CHL7DBRuleDef::pTableName = _T("HL7_RuleDef");
int CHL7DBRuleDef::m_nRefCount = 0;

//--------------------------------------------------------------------------------
CHL7DBRuleDef::CHL7DBRuleDef()
	{
	m_nRefCount++;
	}

//--------------------------------------------------------------------------------
bool CHL7DBRuleDef::Init(CPACSDatabase& database)
	{
	ASSERT(m_pMsgDefList == NULL && m_pMsgDefList != (CHL7DBRuleDefItemList*) 0xFFFFFFFF);

	// if its null then this is the first time
	// a CHL7DBRuleDef object is being instansiated so
	// we must populate the list
	// if its 0xFFFFFFFF then we tried this before and got errors

	// in case of an error we can just return
	m_pMsgDefList = (CHL7DBRuleDefItemList*) 0xFFFFFFFF;

	if(database.HasErrors())
		{
		(CResult) *this = (CResult) database;
		return false;
		}

	CRecordset records(database.GetDBPtr());

	if(! records.Open(CRecordset::forwardOnly,
			"select * from " + database.GetTableAccountName() + pTableName))
		return false;

	m_pMsgDefList = new CHL7DBRuleDefItemList;
	if(! m_pMsgDefList)
		{
		m_pMsgDefList = (CHL7DBRuleDefItemList*) 0xFFFFFFFF;
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

		CHL7DBRuleDefItem* pItem = new CHL7DBRuleDefItem;
		pItem->m_nRuleId = vTemp.m_lVal;

		CString sTemp;

		records.GetFieldValue(1, sTemp);
		sTemp.TrimLeft();
		sTemp.TrimRight();
		ALLOC_STRING(pItem->m_pValue, sTemp);

		records.GetFieldValue(2, sTemp);
		sTemp.TrimLeft();
		sTemp.TrimRight();
		ALLOC_STRING(pItem->m_pDesc, sTemp);

		records.GetFieldValue(3, vTemp, SQL_C_SLONG);
		pItem->m_nVersionId = vTemp.m_lVal;

		pItem->TraceDump();

		m_pMsgDefList->AddTail(pItem);

		records.MoveNext();
		}

	records.Close();
	return true;
	}

//--------------------------------------------------------------------------------
CHL7DBRuleDef::~CHL7DBRuleDef()
	{
	m_nRefCount--;
	if(m_pMsgDefList != (CHL7DBRuleDefItemList*) 0xFFFFFFFF && m_nRefCount < 1)
		{
		delete m_pMsgDefList;
		m_pMsgDefList = NULL;
		}
	}

//--------------------------------------------------------------------------------
const CHL7DBRuleDefItem* CHL7DBRuleDef::Find(int nVer, int nId) const
	{
	return m_pMsgDefList->Find(nVer, nId);
	}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
const CHL7DBRuleDefItem* CHL7DBRuleDefItemList::Find(int nVer, int nId) const
	{
	if(this == NULL)
		return NULL;

	for(POSITION pos = GetHeadPosition(); pos; GetNext(pos))
		{
		CHL7DBRuleDefItem* pInfo = GetAt(pos);
		if(pInfo->m_nRuleId == nId && pInfo->m_nVersionId == nVer)
			return pInfo;
		}

	return NULL;
	}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void CHL7DBRuleDefItem::TraceDump()
	{
	GetIO()->FormatOutput(IOMASK_4, "HL7_RuleDef ruleid=%ld, value=\"%s\", desc=\"%s\", versionid=%ld",
		m_nRuleId, m_pValue, m_pDesc, m_nVersionId);
	}

