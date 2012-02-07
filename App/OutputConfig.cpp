//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// OutputConfig.cpp: implementation of the COutputConfig class.
//
//////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------
#include "stdafx.h"
#include "OutputConfig.h"

#include <allocstring.h>
#include <registry.h>
#include "globals.h"
#include "defs.h"
#include <result.h>

//--------------------------------------------------------------------------------
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//--------------------------------------------------------------------------------
int COutputConfig::m_nRefCount = 0;
DWORD COutputConfig::m_nTraceLevel = 0;
LPTSTR COutputConfig::m_pLogFile = NULL;
LPTSTR COutputConfig::m_pDBKeyName = NULL;
LPTSTR COutputConfig::m_pMonKeyName = NULL;
DWORD COutputConfig::m_nFlags = 0;

LPTSTR COutputConfig::m_pODBCString = NULL;
LPTSTR COutputConfig::m_pDBType = NULL;
LPTSTR COutputConfig::m_pProcAccnt = NULL;
LPTSTR COutputConfig::m_pTableAccnt = NULL;

LPTSTR COutputConfig::m_pSecServIP = NULL;
DWORD COutputConfig::m_nSecServPort = 0;
LPTSTR COutputConfig::m_pSecServBkupIP = NULL;
DWORD COutputConfig::m_nSecServBkupPort = 0;

const DWORD COutputConfig::m_nTraceLevelDefault = 0x1A11;

// Construction/Destruction

//--------------------------------------------------------------------------------
COutputConfig::COutputConfig()
	{
	m_nRefCount++;

	if(m_pDBKeyName != NULL)
		return;

	CRegKey rkConfig(g_rkRoot, (LPCTSTR) NULL);

	if(! rkConfig.CreateKey())
		{
		SetResultCode(errOS);
		SetOSResult(::GetLastError());
		return;
		}

	CRegValueSz rvStr(rkConfig, REG_VALNAME_LOGFILE);
	CString sTemp;

	if(rvStr.ReadValue(sTemp))
		ALLOC_STRING(m_pLogFile, sTemp);

	rvStr.SetValueName(REG_OUTPUT_DBKEYNAME);

	if(rvStr.ReadValue(sTemp))
		ALLOC_STRING(m_pDBKeyName, sTemp);
	else
		ALLOC_STRING(m_pDBKeyName, "Database");

	CRegValueDWord rvVal(rkConfig, REG_VALNAME_TRACELEVEL_FLAGS);
	if(! rvVal.ReadValue(&m_nTraceLevel))
		m_nTraceLevel = m_nTraceLevelDefault;

	rvVal.SetValueName(REG_VALNAME_FLAGS);
	rvVal.ReadValue(&m_nFlags);

	rvStr.SetValueName(REG_OUTPUT_MONKEYNAME);
	if(rvStr.ReadValue(sTemp))
		ALLOC_STRING(m_pMonKeyName, sTemp);
	else
		ALLOC_STRING(m_pMonKeyName, "Monitors");

	// read the database config
	{
	CRegKey rkDB(rkConfig, m_pDBKeyName);
	if(! rkDB.OpenKey())
		return;
	CRegValueSz rvStr(rkDB, "");

	rvStr.SetValueName(REG_OUTPUT_VALNAME_DB_ODBC);
	if(rvStr.ReadValue(sTemp))
		ALLOC_STRING(m_pODBCString, sTemp);
	else
		ALLOC_STRING(m_pODBCString, "");

	rvStr.SetValueName(REG_OUTPUT_VALNAME_DB_TYPE);
	if(rvStr.ReadValue(sTemp))
		ALLOC_STRING(m_pDBType, sTemp);
	else
		ALLOC_STRING(m_pDBType, "");

	rvStr.SetValueName(REG_OUTPUT_VALNAME_DB_PROC);
	if(rvStr.ReadValue(sTemp))
		ALLOC_STRING(m_pProcAccnt, sTemp);
	else
		ALLOC_STRING(m_pProcAccnt, "");

	rvStr.SetValueName(REG_OUTPUT_VALNAME_DB_TABLE);
	if(rvStr.ReadValue(sTemp))
		ALLOC_STRING(m_pTableAccnt, sTemp);
	else
		ALLOC_STRING(m_pTableAccnt, "");

	}

	rvStr.SetValueName(REG_SECSERV_MAIN_IP);
	if(rvStr.ReadValue(sTemp))
		ALLOC_STRING(m_pSecServIP, sTemp);

	rvVal.SetValueName(REG_SECSERV_MAIN_PORT);
	rvVal.ReadValue(&m_nSecServPort);

	rvStr.SetValueName(REG_SECSERV_BKUP_IP);
	if(rvStr.ReadValue(sTemp))
		ALLOC_STRING(m_pSecServBkupIP, sTemp);

	rvVal.SetValueName(REG_SECSERV_BKUP_PORT);
	rvVal.ReadValue(&m_nSecServBkupPort);
	}

//--------------------------------------------------------------------------------
COutputConfig::~COutputConfig()
	{
	m_nRefCount--;

	if(m_nRefCount <= 0)
		{
		delete[] m_pLogFile;
		m_pLogFile = NULL;
		delete[] m_pDBKeyName;
		m_pDBKeyName = NULL;
		delete[] m_pMonKeyName;
		m_pMonKeyName = NULL;
		delete[] m_pODBCString;
		m_pODBCString = NULL;
		delete[] m_pDBType;
		m_pDBType = NULL;
		delete[] m_pProcAccnt;
		m_pProcAccnt = NULL;
		delete[] m_pTableAccnt;
		m_pTableAccnt = NULL;
		delete[] m_pSecServIP;
		m_pSecServIP = NULL;
		delete[] m_pSecServBkupIP;
		m_pSecServBkupIP = NULL;
		m_nTraceLevel = 0;
		m_nFlags = 0;
		m_nRefCount = 0;
		}
	}

//--------------------------------------------------------------------------------
bool COutputConfig::WriteConfig()
	{
	CRegKey rkConfig(g_rkRoot, (LPCTSTR) NULL);

	if(! rkConfig.OpenKey())
		{
		SetResultCode(errOS);
		SetOSResult(::GetLastError());
		return false;
		}

	CRegValueSz rvStr(rkConfig, REG_VALNAME_LOGFILE);
	CString sTemp;

	if(m_pLogFile != NULL)
		rvStr.WriteValue(m_pLogFile);

	rvStr.SetValueName(REG_OUTPUT_DBKEYNAME);

	if(m_pDBKeyName != NULL)
		rvStr.WriteValue(m_pDBKeyName);

	CRegValueDWord rvVal(rkConfig, REG_VALNAME_TRACELEVEL_FLAGS);
	rvVal.WriteValue(m_nTraceLevel);

	rvVal.SetValueName(REG_VALNAME_FLAGS);
	rvVal.WriteValue(m_nFlags);

	rvStr.SetValueName(REG_OUTPUT_MONKEYNAME);
	if(m_pMonKeyName != NULL)
		rvStr.WriteValue(m_pMonKeyName);

	// write the database config
	{
	CRegKey rkDB(rkConfig, m_pDBKeyName);
	if(! rkDB.CreateKey())
		return true;
	CRegValueSz rvStr(rkDB, "");

	rvStr.SetValueName(REG_OUTPUT_VALNAME_DB_ODBC);
	if(m_pODBCString != NULL)
		rvStr.WriteValue(m_pODBCString);

	rvStr.SetValueName(REG_OUTPUT_VALNAME_DB_TYPE);
	if(m_pDBType != NULL)
		rvStr.WriteValue(m_pDBType);

	rvStr.SetValueName(REG_OUTPUT_VALNAME_DB_PROC);
	if(m_pProcAccnt != NULL)
		rvStr.WriteValue(m_pProcAccnt);

	rvStr.SetValueName(REG_OUTPUT_VALNAME_DB_TABLE);
	if(m_pTableAccnt != NULL)
		rvStr.WriteValue(m_pTableAccnt);
	}

	rvStr.SetValueName(REG_SECSERV_MAIN_IP);
	if(m_pSecServIP != NULL)
		rvStr.WriteValue(m_pSecServIP);

	rvVal.SetValueName(REG_SECSERV_MAIN_PORT);
	rvVal.WriteValue(m_nSecServPort);

	rvStr.SetValueName(REG_SECSERV_BKUP_IP);
	if(m_pSecServBkupIP != NULL)
		rvStr.WriteValue(m_pSecServBkupIP);

	rvVal.SetValueName(REG_SECSERV_BKUP_PORT);
	rvVal.WriteValue(m_nSecServBkupPort);

	return true;
	}

//--------------------------------------------------------------------------------
CString COutputConfig::GenerateLogfileName() const
	{
	CString sName(CTime::GetCurrentTime().Format("HL7%Y%b%d.txt"));
	CString sPath(m_pLogFile);
	int i = sPath.ReverseFind('\\');
	if(i == -1)
		return sName;
	sPath.GetBufferSetLength(i + 1);
	sPath.ReleaseBuffer();
	return sPath + sName;
	}
