//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "DBDatabase.h"

#include <WriteLock.h>
#include <ReadLock.h>

//--------------------------------------------------------------------------------
CDBDatabase::CDBDatabase()
		: m_pMap(new Map)
		, m_pList(new List)
	{
	}

//--------------------------------------------------------------------------------
CDBDatabase::~CDBDatabase()
	{
	delete m_pMap;
	delete m_pList;
	}

//--------------------------------------------------------------------------------
bool CDBDatabase::Add(CDBDatabaseElement* pObj)
	{
	CWriteLock lock(this, false);
	if(! lock.Lock(10000))
		return false;

	if(m_pList->AddTail(pObj) == NULL)
		return false;

	m_pMap->SetAt(pObj->m_nElementId, pObj);

	return true;
	}

// returns the number of items removed
//--------------------------------------------------------------------------------
int CDBDatabase::Remove(CDBDatabaseElement* pObj, bool bDelete)
	{
	CWriteLock lock(this, false);
	if(! lock.Lock(10000))
		return 0;

	int nCount = 0;
	for(POSITION pos;;)
		{
		pos = m_pList->Find(pObj);

		if(pos == NULL)
			break;

		m_pList->RemoveAt(pos);
		nCount++;
		}

	m_pMap->RemoveKey(pObj->m_nElementId);

	if(bDelete)
		delete pObj;

	return nCount;
	}

//--------------------------------------------------------------------------------
bool CDBDatabase::ReQueue(CDBDatabaseElement* pObj)
	{
	CWriteLock lock(this, false);
	if(! lock.Lock(10000))
		return false;


	POSITION pos = m_pList->Find(pObj);

	if(pos != NULL)
		m_pList->RemoveAt(pos);

	return m_pList->AddTail(pObj) != NULL;
	}

//--------------------------------------------------------------------------------
bool CDBDatabase::Move(CDBDatabaseElement* pObj, CDBDatabase* pDB)
	{
	CWriteLock lock1(this);
	if(! lock1.Lock(10000))
		return false;

	CWriteLock lock2(this);
	if(! lock2.Lock(10000))
		return false;

	Remove(pObj);

	// an error?
	if(pDB->m_pList->AddTail(pObj) == NULL)
		return false;

	// insert into the new map
	pDB->m_pMap->SetAt(pObj->m_nElementId, pObj);

	return true;
	}

//--------------------------------------------------------------------------------
POSITION CDBDatabase::GetHeadPosition()
	{
	CReadLock lock(this, false);
	if(! lock.Lock(10000))
		return NULL;

	return m_pList->GetHeadPosition();
	}

//--------------------------------------------------------------------------------
CDBDatabaseElement* CDBDatabase::GetNext(POSITION& pos)
	{
	CReadLock lock(this, false);
	if(! lock.Lock(10000))
		return NULL;

	return m_pList->GetNext(pos);
	}

//--------------------------------------------------------------------------------
CDBDatabaseElement* CDBDatabase::GetNextAndReQueue()
	{
	CWriteLock lock(this, false);
	if(! lock.Lock(10000))
		return false;

	CDBDatabaseElement* pElement = NULL;
	POSITION pos = m_pList->GetHeadPosition();

	if(pos == NULL)
		return NULL;

	pElement = m_pList->GetAt(pos);
	m_pList->RemoveAt(pos);
	m_pList->AddTail(pElement);

	return pElement;
	}

//--------------------------------------------------------------------------------
CDBDatabaseElement* CDBDatabase::Find(DWORD nId)
	{
	CReadLock lock(this, false);
	if(! lock.Lock(10000))
		return false;

	CDBDatabaseElement* pElement = NULL;
	if(! m_pMap->Lookup(nId, pElement))
		return NULL;
	return pElement;
	}

//--------------------------------------------------------------------------------
int CDBDatabase::GetCount()
	{
	CReadLock lock(this, false);
	if(! lock.Lock(10000))
		return false;

	return m_pMap->GetCount();
	}
