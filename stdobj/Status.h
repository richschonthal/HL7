//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// Status.h: interface for the CStatus class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATUS_H__73565D67_F8B9_11D2_87A7_00104B9E6286__INCLUDED_)
#define AFX_STATUS_H__73565D67_F8B9_11D2_87A7_00104B9E6286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <result.h>

#include <commondefines.h>

//--------------------------------------------------------------------------------
class CNTService;
class CStatusOutThread;
class CStatusOutThreadArray;

//--------------------------------------------------------------------------------
class DllSpec CStatus  : public CResult
	{
	friend class CStatusOutThread;

	public:
		enum eLevel
			{
			S_NO_TRACE,
			S_TRACE_1,
			S_TRACE_2,
			S_TRACE_3,
			S_TRACE_4,
			S_TRACE_5,
			S_TRACE_6,
			S_TRACE_7,
			S_TRACE_8,
			S_TRACE_HIGHEST = S_TRACE_8,
			S_LOG,
			S_ERROR
			};

	public:
		CStatus();

		// must be called first to set the service ptr
		// to get the ref counter right you must declare
		// 1 CStatus object on the heap at the begining of
		// the services Run loop if you are using NT's event log
		// messages and/or trace to file
		CStatus(CNTService*);

		// only instantiate a cstatus after calling the above 
		// call this constructor to set trace levels
		CStatus(eLevel);

		// call this contructor to send output to a file
		CStatus(LPCTSTR);

		// use this constructor in place of AddToMessageLog calls
		CStatus(LPCTSTR lpszMsg, WORD wEventType, DWORD dwEventID = -1);

		// general/all purpose
		CStatus(eLevel, LPCTSTR pMsg, CResult* = NULL,
			long nThreadId = 0, LPCTSTR pObj = NULL, LPCTSTR pFunc = NULL);

		// you can also instantiate a CStatus and use these functions
		// they use printf-type parameters
		static int Trace(eLevel, LPCTSTR, ...);
		static int Log(WORD, LPCTSTR, ...);
		static int Error(LPCTSTR, ...);

		static bool SetTraceLevel(eLevel);
		static eLevel GetTraceLevel();

		static bool SetLogFile(LPCTSTR, CResult* = NULL);

		static bool IsDisplayed(eLevel);
		static bool IsDisplayed();

		virtual ~CStatus();

	protected:
		friend class CNTService;
		static CNTService* m_pService;
		static CStdioFile* m_pFile;
		static eLevel m_nTraceLevel;
		static int m_nRefCount;
		static CStatusOutThread* m_pOutThread;
		static CStatusOutThreadArray* m_pOutThreads;
	};

#endif // !defined(AFX_STATUS_H__73565D67_F8B9_11D2_87A7_00104B9E6286__INCLUDED_)
