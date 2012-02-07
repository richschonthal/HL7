//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// PlatformInfo.h: interface for the CPlatformInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLATFORMINFO_H__BDF13295_1838_11D3_87B1_00104B9E6286__INCLUDED_)
#define AFX_PLATFORMINFO_H__BDF13295_1838_11D3_87B1_00104B9E6286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "result.h"

//--------------------------------------------------------------------------------
class CPlatformInfo  : public OSVERSIONINFO, public CResult
	{
	private:
		// -1 when not yet looked for
		// 0 when not available (pre OS2R Win95)
		// > 1 when available
		static int m_bGetDiskFreeSpaceExFound;

		static CDWordArray m_nP3SerialNumbers;
		static UINT SerialNumberThread(void*);
	public:
		enum
			{
			VER_WIN31,
			VER_WIN95,
			VER_WIN98,
			VER_WINNT4,
			VER_WIN2000,
			VER_WINNEWER
			};

	public:
		CPlatformInfo();
		virtual ~CPlatformInfo();

		int GetVersion();
		void GetVersion(CString&);

		static void GetComputerName(CString&);
		static void GetUserName(CString&);
		static void GetSystemDirectory(CString&);
		static void GetWindowsDirectory(CString&);

		bool IsWin31();
		bool IsWin95();
		bool IsWin95OS2R();
		bool IsWin98();
		bool IsWinNT();
		bool IsWin2000();

		static void ExpandEnvironmentStrings(LPCTSTR, CString&);
		static void ParseEnvironmentStrings(LPCTSTR, CStringArray&);
		static void ParseEnvironmentStrings(const CString&, CStringArray&);

		// file/dir utilities
		static HANDLE GetDirHandle(LPCTSTR pPath, bool bLock = false);
		static bool FileExists(LPCTSTR, bool bExcludeDir = false);

		static void GetCurrentDirectory(CString&);
		static bool SetCurrentDirectory(const CString&);

		static DWORD GetDiskFreeSpace(LPCTSTR, ULARGE_INTEGER* = NULL);

		static bool CreatePath(LPCTSTR);

		static bool IsDirectory(LPCTSTR);

		static bool GetLoadLibrarySearchPath(CStringArray&);

		static bool GetProcessorSerialNumberOnCurrentThread(DWORD nId[3]);
		static bool GetProcessorSerialNumber(DWORD nId[3], int nWhich);
		static bool GetProcessorSerialNumber(CString&, int nWhich = 0);
		static void GetProcessorSerialNumber(CString&, const DWORD nId[3]);

		static CString GetTempPath();
		static UINT GetTempFileName(LPCTSTR, LPCTSTR, UINT, CString&);
		static CString GetTempFileName(LPCTSTR = NULL, LPCTSTR = "", UINT = 0);

		static bool GetLnkFileTarget(LPCTSTR pLnkPath, CString& sTarget);
	};

#endif // !defined(AFX_PLATFORMINFO_H__BDF13295_1838_11D3_87B1_00104B9E6286__INCLUDED_)
