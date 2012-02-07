//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// PACSDatabase.h: interface for the CPACSDatabase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACSDATABASE_H__7CBF9037_FBDE_11D2_87A7_00104B9E6286__INCLUDED_)
#define AFX_PACSDATABASE_H__7CBF9037_FBDE_11D2_87A7_00104B9E6286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "result.h"
#include "commondefines.h"

#ifndef __AFXDB_H__
#include <afxdb.h>
#endif //__AFXDB_H__

//--------------------------------------------------------------------------------
class CRegKey;

//--------------------------------------------------------------------------------
class CPACSDatabase : public CResult
	{
	public:
		enum eDBType
			{
			UNKNOWN,
			ORACLE,
			SYBASE,
			DB2,
			MSSQL
			};

	private:
		CDatabase m_db;

	private:
		static int m_nRefCount;
		static eDBType m_nType;

		static CString m_sTableAccountName;
		static CString m_sProcAccountName;
		static CString m_sDBConnectString;

	public:
		CPACSDatabase(bool bOpenNow = true);
		CPACSDatabase(const CRegKey*);
		virtual ~CPACSDatabase();

		static void Init(LPCTSTR pConnect, 
			LPCTSTR pDBType=NULL, LPCTSTR pProc=NULL, LPCTSTR pTable=NULL);

		CDatabase* GetDBPtr();

		bool Open();
		void Close();

	public:
		static CString GetTableAccountName();
		static CString GetProcAccountName();

		char GetQuoteChar() const;
	};

#endif // !defined(AFX_PACSDATABASE_H__7CBF9037_FBDE_11D2_87A7_00104B9E6286__INCLUDED_)
