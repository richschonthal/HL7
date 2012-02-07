//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "PACSDatabase.h"
#include "HL7DBVersionDef.h"
#include <allocstring.h>

#include "globals.h"

//--------------------------------------------------------------------------------
CHL7DBVersionDefItemList* CHL7DBVersionDef::m_pMsgDefList = NULL;
LPCTSTR CHL7DBVersionDef::pTableName = _T("HL7_VersionDef");
int CHL7DBVersionDef::m_nRefCount = 0;

//--------------------------------------------------------------------------------
CHL7DBVersionDef::CHL7DBVersionDef()
	{
	m_nRefCount++;
	}

//--------------------------------------------------------------------------------
bool CHL7DBVersionDef::Init(CPACSDatabase& database)
	{
	ASSERT(m_pMsgDefList == NULL && m_pMsgDefList != (CHL7DBVersionDefItemList*) 0xFFFFFFFF);

	// if its null then this is the first time
	// a CHL7DBVersionDef object is being instansiated so
	// we must populate the list
	// if its 0xFFFFFFFF then we tried this before and got errors

	// in case of an error we can just return
	m_pMsgDefList = (CHL7DBVersionDefItemList*) 0xFFFFFFFF;

	if(database.HasErrors())
		{
		(CResult) *this = (CResult) database;
		return false;
		}

	CRecordset records(database.GetDBPtr());

	if(! records.Open(CRecordset::forwardOnly, "select * from " + database.GetTableAccountName() + pTableName))
		return false;

	m_pMsgDefList = new CHL7DBVersionDefItemList;
	if(m_pMsgDefList == NULL)
		{
		m_pMsgDefList = (CHL7DBVersionDefItemList*) 0xFFFFFFFF;
		records.Close();
		return false;
		}

	// now we've opened and read the database and have
	// created our list - now we move the data into the list
	// and we're done
	while(! records.IsEOF())
		{
		/*
		 versionid      int          NOT NULL,
		 appname        varchar(180) NOT NULL,
		 appfacility    varchar(180)     NULL,
		 hl7version     char(8)      NOT NULL,
		 verdescription varchar(64)      NULL
		*/

		CDBVariant vTemp;
		records.GetFieldValue((int) 0, vTemp, SQL_C_SLONG);

		CHL7DBVersionDefItem* pItem = new CHL7DBVersionDefItem;
		pItem->m_nVersionId = vTemp.m_lVal;

		CString sTemp;

		records.GetFieldValue(1, sTemp);
		sTemp.TrimLeft();
		sTemp.TrimRight();
		ALLOC_STRING(pItem->m_pAppName, sTemp);

		records.GetFieldValue(2, sTemp);
		sTemp.TrimLeft();
		sTemp.TrimRight();
		ALLOC_STRING(pItem->m_pAppFacility, sTemp);

		records.GetFieldValue(3, sTemp);
		sTemp.TrimLeft();
		sTemp.TrimRight();
		ALLOC_STRING(pItem->m_pHL7Version, sTemp);

		records.GetFieldValue(4, sTemp);
		sTemp.TrimLeft();
		sTemp.TrimRight();
		ALLOC_STRING(pItem->m_pDesc, sTemp);

		pItem->TraceDump();

		m_pMsgDefList->AddTail(pItem);

		records.MoveNext();
		}

	records.Close();
	return true;
	}

//--------------------------------------------------------------------------------
CHL7DBVersionDef::~CHL7DBVersionDef()
	{
	m_nRefCount--;
	if(m_pMsgDefList != (CHL7DBVersionDefItemList*) 0xFFFFFFFF && m_nRefCount < 1)
		{
		delete m_pMsgDefList;
		m_pMsgDefList = NULL;
		}
	}

//--------------------------------------------------------------------------------
const CHL7DBVersionDefItem* CHL7DBVersionDef::Find(int nId) const
	{
	return m_pMsgDefList->Find(nId);
	}

//--------------------------------------------------------------------------------
const CHL7DBVersionDefItem* CHL7DBVersionDef::Find(LPCTSTR pAppName, LPCTSTR pAppFacility, LPCTSTR pHL7Version) const
	{
	if(pAppName == NULL || pAppFacility == NULL || pHL7Version == NULL)
		return NULL;
	return m_pMsgDefList->Find(pAppName, pAppFacility, pHL7Version);
	}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
const CHL7DBVersionDefItem* CHL7DBVersionDefItemList::Find(int nId) const
	{
	if(this == NULL)
		return NULL;

	for(POSITION pos = GetHeadPosition(); pos; GetNext(pos))
		{
		CHL7DBVersionDefItem* pInfo = GetAt(pos);
		if(pInfo->m_nVersionId == nId)
			return pInfo;
		}

	return NULL;
	}

//--------------------------------------------------------------------------------
const CHL7DBVersionDefItem* CHL7DBVersionDefItemList::Find(LPCTSTR pAppName, LPCTSTR pAppFacility, LPCTSTR pHL7Version) const
	{
	if(this == NULL)
		return NULL;

	for(POSITION pos = GetHeadPosition(); pos; GetNext(pos))
		{
		CHL7DBVersionDefItem* pInfo = GetAt(pos);
		if( strcmp(pAppName, pInfo->m_pAppName) == 0 &&
				strcmp(pAppFacility, pInfo->m_pAppFacility) == 0 &&
				strcmp(pHL7Version, pInfo->m_pHL7Version) == 0)
			return pInfo;
		}

	return NULL;
	}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void CHL7DBVersionDefItem::TraceDump()
	{
	GetIO()->FormatOutput(IOMASK_4, "HL7_VersionDef versionid=%ld, appname=\"%s\", appfacility=\"%s\", hl7version=\"%s\", desc=\"%s\"",
		m_nVersionId, m_pAppName, m_pAppFacility, m_pHL7Version, m_pDesc);
	}

