//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#include "stdafx.h"

#include "PACSDatabase.h"

#include "registry.h"
#include "DebugPrintf.h"

#include <atlconv.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define DATABASE_COUNT 4

//--------------------------------------------------------------------------------
int CPACSDatabase::m_nRefCount = 0;
CPACSDatabase::eDBType CPACSDatabase::m_nType = CPACSDatabase::UNKNOWN;
CString CPACSDatabase::m_sTableAccountName = "";
CString CPACSDatabase::m_sProcAccountName = "";
CString CPACSDatabase::m_sDBConnectString = "";

//--------------------------------------------------------------------------------
CPACSDatabase::CPACSDatabase(const CRegKey* pRegKey)
	{
	ASSERT(pRegKey != NULL);

	CRegValueSz rvIn(*pRegKey, NULL);

	CString sType;
	rvIn.SetValueName("dbtype");
	rvIn.ReadValue(sType);
	sType.MakeUpper();
	sType.TrimLeft();
	sType.TrimRight();

	DebugPrintf("dbtype=%s", (LPCTSTR) sType);

	if(sType == "ORACLE")
		m_nType = ORACLE;
	else
		if(sType == "SYBASE")
			m_nType = SYBASE;
		else
			if(sType == "MSSQL")
				m_nType = MSSQL;
			else
				m_nType = SYBASE;

	rvIn.SetValueName("ConnectString");
	rvIn.ReadValue(m_sDBConnectString);
	m_sDBConnectString.TrimLeft();
	m_sDBConnectString.TrimRight();

	DebugPrintf("ConnectString=%s", (LPCTSTR) m_sDBConnectString);

	rvIn.SetValueName("TableAccount");
	rvIn.ReadValue(m_sTableAccountName);
	m_sTableAccountName.TrimLeft();
	m_sTableAccountName.TrimRight();

	DebugPrintf("TableAccount=%s", (LPCTSTR) m_sTableAccountName);

	rvIn.SetValueName("ProcedureAccount");
	rvIn.ReadValue(m_sProcAccountName);
	m_sProcAccountName.TrimLeft();
	m_sProcAccountName.TrimRight();

	DebugPrintf("ProcedureAccount=%s", (LPCTSTR) m_sProcAccountName);
	}

//--------------------------------------------------------------------------------
void CPACSDatabase::Init(LPCTSTR pConnect, LPCTSTR pDBType, LPCTSTR pProc, LPCTSTR pTable)
	{
	ASSERT(pConnect != NULL);
	ASSERT(pDBType != NULL);
	ASSERT(pProc != NULL);
	ASSERT(pTable != NULL);

	m_sDBConnectString = pConnect;
	m_sDBConnectString.TrimLeft();
	m_sDBConnectString.TrimRight();

	CString sType(pDBType);
	sType.MakeUpper();
	sType.TrimLeft();
	sType.TrimRight();

	if(sType == "ORACLE")
		m_nType = ORACLE;
	else
		if(sType == "SYBASE")
			m_nType = SYBASE;
		else
			if(sType == "MSSQL")
				m_nType = MSSQL;
			else
				m_nType = SYBASE;

	m_sTableAccountName = pTable;
	m_sTableAccountName.TrimLeft();
	m_sTableAccountName.TrimRight();

	m_sProcAccountName = pProc;
	m_sProcAccountName.TrimLeft();
	m_sProcAccountName.TrimRight();
	}

//--------------------------------------------------------------------------------
CPACSDatabase::CPACSDatabase(bool bOpenNow)
	{
	if(bOpenNow)
		Open();
	}

//--------------------------------------------------------------------------------
CPACSDatabase::~CPACSDatabase()
	{
	Close();
	}

//--------------------------------------------------------------------------------
bool CPACSDatabase::Open()
	{
	ASSERT(m_nType != UNKNOWN);

	TRY
		{
		if(! m_db.OpenEx(m_sDBConnectString, CDatabase::noOdbcDialog|CDatabase::useCursorLib))
			{
			SetResultCode(errOS);
			return false;
			}
		}
	CATCH(CMemoryException, e)
		{
		CString sTemp;
		e->GetErrorMessage(sTemp.GetBuffer(1024), 1023);
		DebugPrintf("open db memory error=%s", (LPCTSTR) sTemp);
		SetResultCode(errOS);
		return false;
		}
	END_CATCH
	return true;
	}

//--------------------------------------------------------------------------------
void CPACSDatabase::Close()
	{
	if(m_db.IsOpen())
		m_db.Close();
	}

//--------------------------------------------------------------------------------
CDatabase* CPACSDatabase::GetDBPtr()
	{
	return &m_db;
	}

//--------------------------------------------------------------------------------
CString CPACSDatabase::GetTableAccountName()
	{
	return m_sTableAccountName;
	}

//--------------------------------------------------------------------------------
CString CPACSDatabase::GetProcAccountName()
	{
	return m_sProcAccountName;
	}

//--------------------------------------------------------------------------------
char CPACSDatabase::GetQuoteChar() const
	{
	switch(m_nType)
		{
		case ORACLE:
			return '\'';
		case SYBASE:
			return '\"';
		case MSSQL:
			return '\'';
		default:
			throw(new CDBException);
		}
	}
