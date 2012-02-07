//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// OutputConfig.h: interface for the COutputConfig class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OUTPUTCONFIG_H__4B03F5B8_69E9_11D3_AEDF_005004A1C5F3__INCLUDED_)
#define AFX_OUTPUTCONFIG_H__4B03F5B8_69E9_11D3_AEDF_005004A1C5F3__INCLUDED_

//--------------------------------------------------------------------------------
#include <result.h>
#include <commondefines.h>

//--------------------------------------------------------------------------------
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
class COutputConfig  : public CResult
	{
	public:
		enum eFlagAndMasks
			{
			FlagIsDailyLogfile = 1
			};

	private:
		static int m_nRefCount;

		static DWORD m_nTraceLevel;
		static LPTSTR m_pLogFile;
		static LPTSTR m_pDBKeyName;
		static LPTSTR m_pMonKeyName;
		static DWORD m_nFlags;

		static LPTSTR m_pODBCString;
		static LPTSTR m_pDBType;
		static LPTSTR m_pProcAccnt;
		static LPTSTR m_pTableAccnt;

		static LPTSTR m_pSecServIP;
		static DWORD m_nSecServPort;
		static LPTSTR m_pSecServBkupIP;
		static DWORD m_nSecServBkupPort;

	public:
		static const DWORD m_nTraceLevelDefault;

	public:
		COutputConfig();
		virtual ~COutputConfig();

		bool WriteConfig();

		DWORD GetTraceLevel() const		{ return m_nTraceLevel; }
		void SetTraceLevel(DWORD n)		{ m_nTraceLevel = n; }

		LPCTSTR GetLogFile() const		{ return m_pLogFile; }
		void SetLogFile(LPCTSTR p)		{ ALLOC_STRING(m_pLogFile,p); }

		CString GenerateLogfileName() const;

		bool IsDailyLog() const			{ return (m_nFlags & FlagIsDailyLogfile) == FlagIsDailyLogfile; }
		void SetIsDailyLog(bool b)		{ m_nFlags = b ? m_nFlags|FlagIsDailyLogfile : m_nFlags&~FlagIsDailyLogfile; }

		LPCTSTR GetDBKeyName() const	{ return m_pDBKeyName; }
		void SetDBKeyName(LPCTSTR p)	{ ALLOC_STRING(m_pDBKeyName, p); }

		DWORD GetFlags() const			{ return m_nFlags; }
		void SetFlags(DWORD n)			{ m_nFlags = n; }

		LPCTSTR GetMonKeyName() const	{ return m_pMonKeyName; }
		void SetMonKeyName(LPCTSTR p)	{ ALLOC_STRING(m_pMonKeyName, p); }

		LPCTSTR GetODBCString() const	{ return m_pODBCString; }
		void SetODBCString(LPCTSTR p)	{ ALLOC_STRING(m_pODBCString, p); }

		LPCTSTR GetDBType() const		{ return m_pDBType; }
		void SetDBType(LPCTSTR p)		{ ALLOC_STRING(m_pDBType, p); }

		LPCTSTR GetProcAccnt() const	{ return m_pProcAccnt; }
		void SetProcAccnt(LPCTSTR p)	{ ALLOC_STRING(m_pProcAccnt, p); }

		LPCTSTR GetTableAccnt() const	{ return m_pTableAccnt; }
		void SetTableAccnt(LPCTSTR p)	{ ALLOC_STRING(m_pTableAccnt, p); }

		LPCTSTR GetSecServIP() const	{ return m_pSecServIP; }
		void SetSecServIP(LPCTSTR p)	{ ALLOC_STRING(m_pSecServIP, p); }

		DWORD GetSecServPort() const	{ return m_nSecServPort; }
		void SetSecServPort(DWORD n)	{ m_nSecServPort = n; }

		LPCTSTR GetSecServBkupIP() const	{ return m_pSecServBkupIP; }
		void SetSecServBkupIP(LPCTSTR p)	{ ALLOC_STRING(m_pSecServBkupIP, p); }

		DWORD GetSecServBkupPort() const	{ return m_nSecServBkupPort; }
		void SetSecServBkupPort(DWORD n)	{ m_nSecServBkupPort = n; }
	};

#endif // !defined(AFX_OUTPUTCONFIG_H__4B03F5B8_69E9_11D3_AEDF_005004A1C5F3__INCLUDED_)
