//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "InternetTime.h"
#include <afxinet.h>

#if 0
#ifdef TRACE
#undef TRACE
#endif
#include <debugPrintf.h>
#define TRACE DebugPrintf
#endif

//--------------------------------------------------------------------------------
#pragma comment(lib, "ws2_32.lib")

//--------------------------------------------------------------------------------
#define SERVERCOUNT 12

//--------------------------------------------------------------------------------
long CInternetTime::m_nLastTriedServer = -1;

//--------------------------------------------------------------------------------
struct CInternetTime::tagServers CInternetTime::m_servers[SERVERCOUNT] =
	{
	"time-a.nist.gov",				"129.6.15.28",		"NIST, Gaithersburg, Maryland", 13,
	"time-b.nist.gov",				"129.6.15.29",		"NIST, Gaithersburg, Maryland", 13,
	"time-a.timefreq.bldrdoc.gov",	"132.163.4.101",	"NIST, Boulder, Colorado", 13,
	"time-b.timefreq.bldrdoc.gov",	"132.163.4.102",	"NIST, Boulder, Colorado", 13,
	"time-c.timefreq.bldrdoc.gov",	"132.163.4.103",	"NIST, Boulder, Colorado", 13,
	"utcnist.colorado.edu",			"128.138.140.44",	"University of Colorado, Boulder", 13,
	"time.nist.gov",				"192.43.244.18",	"NCAR, Boulder, Colorado", 13,
	"time-nw.nist.gov",				"131.107.1.10",		"Microsoft, Redmond, Washington", 13,
	"nist1.datum.com",				"209.0.72.7",		"Datum, San Jose, California", 13,
	"nist1.dc.certifiedtime.com",	"216.200.93.8",		"Abovnet, Virginia", 13,
	"nist1.nyc.certifiedtime.com",	"208.184.49.9",		"Abovnet, New York City", 13,
	"nist1.sjc.certifiedtime.com",	"208.185.146.41",	"Abovnet, San Jose, Californ", 13
	};

//--------------------------------------------------------------------------------
CInternetTime::CInternetTime()
		: m_otTime((time_t)0)
	{
	// init winsock incase the app programmer forgot to
	AfxSocketInit();
	}

//--------------------------------------------------------------------------------
CInternetTime::CInternetTime(int nServer)
		: m_otTime((time_t)0)
	{
	// init winsock incase the app programmer forgot to
	AfxSocketInit();
	GetCurrentNetTime(FlagDef, nServer);
	}

//--------------------------------------------------------------------------------
CInternetTime::CInternetTime(LPCTSTR pIP, int nPort)
		: m_otTime((time_t)0)
	{
	// init winsock incase the app programmer forgot to
	AfxSocketInit();
	GetCurrentNetTime(m_otTime, FlagDef, pIP, nPort);
	}

//--------------------------------------------------------------------------------
CInternetTime::operator COleDateTime()
	{
	return m_otTime;
	}

//--------------------------------------------------------------------------------
bool CInternetTime::GetCurrentNetTime(BYTE nFlags, int nServer)
	{
	if(nServer == TryAllServers)
		{
		for(nServer = 0; nServer < SERVERCOUNT; nServer++)
			{
			if(GetCurrentNetTime(m_otTime, nFlags, m_servers[nServer].pIP, m_servers[nServer].nPort))
				return true;
			}
		return false;
		}

	if(nServer == NextAvailServer)
		{
		if(::InterlockedIncrement(&m_nLastTriedServer) == ServerCount)
			::InterlockedExchange(&m_nLastTriedServer, 0);
		nServer = m_nLastTriedServer;
		}

	if(nServer >= 0 && nServer < SERVERCOUNT)
		return GetCurrentNetTime(m_otTime, nFlags, m_servers[nServer].pIP, m_servers[nServer].nPort);

	return false;
	}

//--------------------------------------------------------------------------------
class CCustomInternetSession : public CInternetSession
	{
	public:
		CEvent m_evtComplete;
		bool m_bSent;

	public:
		CCustomInternetSession()
				: m_bSent(false)
			{
			TRY
				{
				//EnableStatusCallback();
				}
			CATCH_ALL(e)
				{
				}
			END_CATCH_ALL;
			}

		virtual ~CCustomInternetSession()
			{
			TRY
				{
			//	EnableStatusCallback(FALSE);
				}
			CATCH_ALL(e)
				{
				}
			END_CATCH_ALL;
			}

		virtual void OnStatusCallback(DWORD, DWORD nStatus, LPVOID, DWORD)
			{
			TRACE("status=%d\n", nStatus);
			if(m_bSent && nStatus == INTERNET_STATUS_RESPONSE_RECEIVED)
				m_evtComplete.SetEvent();
			if(nStatus == INTERNET_STATUS_REQUEST_SENT)
				m_bSent = true;
			}
	};

//--------------------------------------------------------------------------------
bool CInternetTime::GetCurrentNetTime(COleDateTime& otTime, BYTE nFlags, LPCTSTR pIP, int nPort)
	{
	ASSERT(pIP != NULL);

	if(pIP == NULL)
		return false;

	bool bRv = false;
	TRY
		{
		CCustomInternetSession session;
		CString sURL;
		sURL.Format("http://%s:%d", pIP, nPort);
		TRACE("opening %s\n", pIP);
		CHttpFile* pHttp = (CHttpFile*) session.OpenURL(sURL);
		if(pHttp->SendRequest())
			{
			DWORD nStart = GetTickCount();
			CString sTemp;
			for(;;)
				{
				pHttp->ReadString(sTemp);
				if(! sTemp.IsEmpty())
					break;
				::Sleep(10);
				DWORD nNow = GetTickCount();
				if(nNow < nStart)
					nStart = GetTickCount();
				else
					if(nNow > nStart + 10000)
						break;
				}

			pHttp->Close();
			delete pHttp;
			bRv = ParseNetworkTimeString(sTemp, otTime, nFlags);
			}
		}
	CATCH_ALL(e)
		{
		return false;
		}
	END_CATCH_ALL;

	return bRv;
	}

//--------------------------------------------------------------------------------
bool CInternetTime::ParseNetworkTimeString(LPCTSTR pString, COleDateTime& otTime, BYTE nFlags)
	{
	CString sString(pString);
	return ParseNetworkTimeString(sString, otTime);
	}

//--------------------------------------------------------------------------------
bool CInternetTime::ParseNetworkTimeString(const CString& sString, COleDateTime& otTime, BYTE nFlags)
	{
	TRACE("%s\n", (LPCTSTR) sString);

	// parses a time string in the format:
	// JJJJJ YR-MO-DA HH:MM:SS TT L H msADV UTC(NIST) OTM
	/*
	where:
		JJJJJ is the Modified Julian Date (MJD). The MJD is the last five digits of
		the Julian Date, which is simply a count of the number of days 
		since January 1, 4713 B.C. To get the Julian Date, add 2.4 million to the MJD.

		YR-MO-DA is the date. It shows the last two digits of the year,
		the month, and the current day of month.

		HH:MM:SS is the time in hours, minutes, and seconds. The time is always 
		sent as Coordinated Universal Time (UTC). An offset needs to be applied 
		to UTC to obtain local time. For example, Mountain Time in the U. S. 
		is 7 hours behind UTC during Standard Time, and 6 hours behind UTC 
		during Daylight Saving Time.

		TT is a two digit code (00 to 99) that indicates whether the United States is 
		on Standard Time (ST) or Daylight Saving Time (DST). It also indicates when
		ST or DST is approaching. This code is set to 00 when ST is in effect, or to 
		50 when DST is in effect. During the month in which the time change actually occurs,
		this number will decrement every day until the change occurs. For example, during 
		the month of October, the U.S. changes from DST to ST. On October 1, the number 
		will change from 50 to the actual number of days until the time change. It will
		decrement by 1 every day until the change occurs at 2 a.m. local time when the 
		value is 1. Likewise, the spring change is at 2 a.m. local time when the value reaches 51.

		L is a one-digit code that indicates whether a leap second will be added or subtracted 
		at midnight on the last day of the current month.
		If the code is 0, no leap second will occur this month. If the code is 1, a positive 
		leap second will be added at the end of the month. This means that the last minute 
		of the month will contain 61 seconds instead of 60. If the code is 2, a second will 
		be deleted on the last day of the month. Leap seconds occur at a rate of about one 
		per year. They are used to correct for irregularity in the earth's rotation. The 
		correction is made just before midnight UTC (not local time).

		H is a health digit that indicates the health of the server. If H=0, the server 
		is healthly. If H=1, then the server is operating properly but its time may be 
		in error by up to 5 seconds. This state should change to fully healthy within 
		10 minutes. If H=2, then the server is operating properly but its time is known 
		to be wrong by more than 5 seconds. If H=4, then a hardware or software failure 
		has occurred and the amount of the time error is unknown.

		msADV displays the number of milliseconds that NIST advances the time code to 
		partially compensate for network delays. The advance is currently set to 50.0 milliseconds.

		The label UTC(NIST) is contained in every time code. It indicates that you are 
		receiving Coordinated Universal Time (UTC) from the 
		National Institute of Standards and Technology (NIST).

		OTM (on-time marker) is an asterisk (*). The time values sent by the time 
		code refer to the arrival time of the OTM. In other words, if the time code 
		says it is 12:45:45, this means it is 12:45:45 when the OTM arrives
		*/

	if(sString.GetLength() < 34)
		return false;

	// skip over the julian date
	int nIndex = sString.Find(' ');
	if(nIndex == -1 || nIndex > 8)
		return false;

	int nYear = atoi(sString.Mid(nIndex + 1, 2)) + 2000;
	int nMon  = atoi(sString.Mid(nIndex + 4, 2));
	int nDay  = atoi(sString.Mid(nIndex + 7, 2));

	nIndex = sString.Find(' ', nIndex+1);
	if(nIndex == -1 || nIndex > 16)
		return false;

	int nHour = atoi(sString.Mid(nIndex + 1, 2));
	int nMin  = atoi(sString.Mid(nIndex + 4, 2));
	int nSec  = atoi(sString.Mid(nIndex + 7, 2));

	otTime.SetDateTime(nYear, nMon, nDay, nHour, nMin, nSec);

	if((nFlags & AdjustToLocalTime) == 0)
		return true;

	TIME_ZONE_INFORMATION tzi;
	if(::GetTimeZoneInformation(&tzi) == TIME_ZONE_ID_UNKNOWN)
		tzi.Bias = 0;

	bool bNeg = false;
	if(tzi.Bias > 0)
		{
		bNeg = true;
		tzi.Bias *= -1;
		}

	COleDateTimeSpan span(0, 0, tzi.Bias, 0);

	if(bNeg)
		otTime += span;
	else
		otTime -= span;

	if((nFlags & AdjustForDST) == 0)
		return true;

	return true;
	}
