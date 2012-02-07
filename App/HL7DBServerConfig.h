//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#if !defined(AFX_HL7DBSERVERCONFIG_H__9DCFC9C7_2802_11D3_87BA_00104B9E6286__INCLUDED_)
#define AFX_HL7DBSERVERCONFIG_H__9DCFC9C7_2802_11D3_87BA_00104B9E6286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <result.h>
#include "PACSDatabase.h"

//--------------------------------------------------------------------------------
class CHL7DBServerConfig : public CObject, public CResult
	{
	public:
		enum
			{
			E_UNKNOWN,
			E_APPLICATION,
			E_HL7_VERSION,
			E_ENCODING_CHARS,
			E_PULSE_WATCH_SLEEP_DIR,
			E_MAX_MONITOR_PULSE_TIME,
			E_PROCESS_MONITOR_INTERVAL,
			E_MAX_RETRIES,
			E_PACOUT_SLEEP_DUR
			};

	private:
		static int m_nRefCount;

		static LPCTSTR m_pEmptyString;

		static LPTSTR m_pApp;
		static LPTSTR m_pHL7Ver;
		static LPTSTR m_pEncodeChars;
		static long m_nPulseWatchSleepDur;
		static long m_nMaxMonitorPulseTime;
		static long m_nProcessMonitorInterval;
		static long m_nMaxRetries;
		static long m_nPacOutSleepDur;

	private:
		friend class CStaticHolder;
		bool Init(CPACSDatabase&);

	public:
		CHL7DBServerConfig();
		virtual ~CHL7DBServerConfig();

		LPCTSTR GetApplication()			{ return (m_pApp)?m_pApp:m_pEmptyString; }
		LPCTSTR GetHL7Verison()				{ return (m_pHL7Ver)?m_pHL7Ver:m_pEmptyString; }
		LPCTSTR GetEncodingChars()			{ return (m_pEncodeChars)?m_pEncodeChars:m_pEmptyString; }
		long GetPulseWatchSleepDur()		{ return m_nPulseWatchSleepDur; }
		long GetMaxMonitorPulseTime()		{ return m_nMaxMonitorPulseTime; }
		long GetProcessMonitorInterval()	{ return m_nProcessMonitorInterval; }
		long GetMaxRetries()				{ return m_nMaxRetries; }
		long GetPacOutSleepDur()			{ return m_nPacOutSleepDur; }
	};

#endif // !defined(AFX_HL7DBSERVERCONFIG_H__9DCFC9C7_2802_11D3_87BA_00104B9E6286__INCLUDED_)
