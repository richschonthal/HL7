//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _IOSUBSYSTEM_H_
#define _IOSUBSYSTEM_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
#include "SubSystem.h"

//--------------------------------------------------------------------------------
class CNTService;
class CQuickMail;

//--------------------------------------------------------------------------------
enum
	{
	IOMASK_OFF = 0,
	IOMASK_1 =		0x00000001,
	IOMASK_2 =		0x00000002,
	IOMASK_3 =		0x00000004,
	IOMASK_4 =		0x00000008,
	IOMASK_5 =		0x00000010,
	IOMASK_6 =		0x00000020,
	IOMASK_7 =		0x00000040,
	IOMASK_8 =		0x00000080,
	IOMASK_9 =		0x00000100,
	IOMASK_10 =		0x00000200,
	IOMASK_11 =		0x00000400,
	IOMASK_12 =		0x00000800,
	IOMASK_13 =		0x00001000,
	IOMASK_14 =		0x00002000,
	IOMASK_15 =		0x00004000,
	IOMASK_16 =		0x00008000,
	IOMASK_CONST =	0x01000000,
	IOMASK_ERR =	0x1000FFFF,
	IOMASK_LOG =	0x2000FFFF,
	IOMASK_DEBUG =	0x40000000,
	IOMASK_ALWAYSDISPLAYMASK = 0xFFFF0000,
	};

//--------------------------------------------------------------------------------
class CIOSubSystem : public CThreadPoolSubSystem  
	{
	public:
		DECLARE_DYNAMIC(CIOSubSystem);

	friend class CIOThread;

	public:
		enum
			{
			MSG_OUTPUT = WM_USER,
			MSG_OUTPUT_CONST,
			MSG_QUICKMAIL
			};

	public:
		CIOSubSystem(CSystem*);
		virtual ~CIOSubSystem();

	protected:
		static DWORD m_nTraceLevel;
		static CMutex m_mutexLogFile;
		static CStdioFile* m_pLogFile;
		static CNTService* m_pService;

	public:
		virtual bool Init();

		virtual void SetTraceLevel(DWORD);
		virtual DWORD GetTraceLevel() const;

		static bool IsErrorMsg(DWORD);
		static bool IsLogMsg(DWORD);

		virtual bool IsDisplayed(DWORD) const;

		virtual void SetServicePtr(CNTService*);
		virtual CNTService* GetServicePtr() const;

		virtual bool CreateLogFile(LPCTSTR pFilename,  bool bTruncate = false);
		virtual bool CloseLogFile();

		virtual CStdioFile* GetLogFilePtr();
		virtual CMutex& GetLogFileLock() const;

		virtual bool Output(DWORD, LPCTSTR);
		virtual bool Output(DWORD, LPTSTR);
		virtual bool FormatOutput(DWORD, LPCTSTR, ...);

		virtual bool Debug(LPCTSTR);
		virtual bool Debug(LPTSTR);
		virtual bool DebugPrintf(LPCTSTR, ...);

		virtual bool SendQuickMail(CQuickMail*);

	protected:
		virtual void WriteLogFileHeader();

		virtual bool OutputToDebugger(LPCTSTR);
		virtual bool OutputToFile(LPCTSTR, CStdioFile*);

		virtual CThreadObject* CreateThread();

		virtual int GetObjectId();
	};

#endif // _IOSUBSYSTEM_H_
