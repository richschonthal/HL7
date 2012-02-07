//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "SharedDB.h"

//--------------------------------------------------------------------------------
CPACSDatabase* CSharedDB::m_pDB = NULL;
CMutex CSharedDB::m_mutex;

//--------------------------------------------------------------------------------
CSharedDB::CSharedDB(bool bCreateIfNeeded)
		: m_lock(&m_mutex, false)
	{
	if(m_lock.Lock() && bCreateIfNeeded)
		{
		TRY
			{
			if(m_pDB == NULL)
				m_pDB = new CPACSDatabase;
			}
		CATCH_ALL(e)
			{
			delete m_pDB;
			m_pDB = NULL;
			}
		END_CATCH_ALL;
		}
	}

//--------------------------------------------------------------------------------
CSharedDB::~CSharedDB()
	{
	}

//--------------------------------------------------------------------------------
CPACSDatabase* CSharedDB::GetDB()
	{
	if(! m_lock.IsLocked())
		return NULL;
	return m_pDB;
	}

//--------------------------------------------------------------------------------
bool CSharedDB::DeleteConnection()
	{
	if(! m_lock.IsLocked())
		return false;

	delete m_pDB;
	m_pDB = NULL;
	return true;
	}
