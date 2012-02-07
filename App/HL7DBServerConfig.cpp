//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "output.h"
#include "HL7DBServerConfig.h"

#include "globals.h"

#include <allocstring.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//--------------------------------------------------------------------------------
static struct
		{
		int nToken;
		LPCTSTR pStr;
		} g_keywordlist[] =
	{
	CHL7DBServerConfig::E_APPLICATION,					"Application",
	CHL7DBServerConfig::E_HL7_VERSION,					"HL7Version",
	CHL7DBServerConfig::E_ENCODING_CHARS,				"EncodingChars",
	CHL7DBServerConfig::E_PULSE_WATCH_SLEEP_DIR,		"PulseWatchSleepDur",
	CHL7DBServerConfig::E_MAX_MONITOR_PULSE_TIME,		"MaxMonitorPulseTime",
	CHL7DBServerConfig::E_PROCESS_MONITOR_INTERVAL,		"ProcessMonitorInterval",
	CHL7DBServerConfig::E_MAX_RETRIES,					"MaxRetries",
	CHL7DBServerConfig::E_PACOUT_SLEEP_DUR,				"PacOutMsgEventPollSleepDur",
	0, NULL
	};

//--------------------------------------------------------------------------------
int CHL7DBServerConfig::m_nRefCount = 0;
LPCTSTR CHL7DBServerConfig::m_pEmptyString = "";
LPTSTR CHL7DBServerConfig::m_pApp = NULL;
LPTSTR CHL7DBServerConfig::m_pHL7Ver = NULL;
LPTSTR CHL7DBServerConfig::m_pEncodeChars = NULL;
long CHL7DBServerConfig::m_nPulseWatchSleepDur = 1200;
long CHL7DBServerConfig::m_nMaxMonitorPulseTime = 60;
long CHL7DBServerConfig::m_nProcessMonitorInterval = 10000;
long CHL7DBServerConfig::m_nMaxRetries = 3;
long CHL7DBServerConfig::m_nPacOutSleepDur = 1000;

//--------------------------------------------------------------------------------
CHL7DBServerConfig::CHL7DBServerConfig()
	{
	m_nRefCount++;
	}

//--------------------------------------------------------------------------------
bool CHL7DBServerConfig::Init(CPACSDatabase& db)
	{
	CRecordset rs(db.GetDBPtr());

	if(! rs.Open(CRecordset::forwardOnly, "select * from " + db.GetTableAccountName() + "HL7_ServerConfig"))
		{
		SetResultCode(CResult::errOS);
		return false;
		}

	while(! rs.IsEOF())
		{
		CString sTemp;
		rs.GetFieldValue((short) 0, sTemp);

		sTemp.TrimRight();

		CString sValue;
		rs.GetFieldValue(1, sValue);

		for(int i = 0; g_keywordlist[i].pStr != NULL; i++)
			if(sTemp.CompareNoCase(g_keywordlist[i].pStr) == 0)
				switch(g_keywordlist[i].nToken)
					{
					case E_APPLICATION:
						ALLOC_STRING(m_pApp, sValue);
						break;

					case E_HL7_VERSION:
						ALLOC_STRING(m_pHL7Ver, sValue);
						break;

					case E_ENCODING_CHARS:
						ALLOC_STRING(m_pEncodeChars, sValue);
						break;

					case E_PULSE_WATCH_SLEEP_DIR:
						m_nPulseWatchSleepDur = atol(sValue);
						break;

					case E_MAX_MONITOR_PULSE_TIME:
						m_nMaxMonitorPulseTime = atol(sValue);
						break;

					case E_PROCESS_MONITOR_INTERVAL:
						m_nProcessMonitorInterval = atol(sValue);
						break;

					case E_MAX_RETRIES:
						m_nMaxRetries = atol(sValue);
						break;

					case E_PACOUT_SLEEP_DUR:
						m_nPacOutSleepDur = atol(sValue);
						break;
					}

		if(GetIO()->IsDisplayed(IOMASK_4))
			{
			CString sComment;
			rs.GetFieldValue(2, sComment);

			GetIO()->FormatOutput(IOMASK_4, "entryname=%s, entryvalue=%s, entrycomment=%s", (LPCTSTR) sTemp, (LPCTSTR) sValue,
				(LPCTSTR) sComment);
			}

		rs.MoveNext();
		}

	return true;
	}

//--------------------------------------------------------------------------------
CHL7DBServerConfig::~CHL7DBServerConfig()
	{
	m_nRefCount--;
	if(m_nRefCount < 1)
		{
		delete[] m_pApp;
		m_pApp = NULL;
		delete[] m_pHL7Ver;
		m_pHL7Ver = NULL;
		delete[] m_pEncodeChars;
		m_pEncodeChars = NULL;
		}
	}
