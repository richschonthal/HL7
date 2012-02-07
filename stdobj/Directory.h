//--------------------------------------------------------------------------------
// Copyright (c) 2001 @COMPANY, Inc.
// Created...: 6/8/01
// Author....: Rich Schonthal
//--------------------------------------------------------------------------------

#if !defined(AFX_DIRECTORY_H__1A4AB953_5C82_11D5_B191_00A0CC271D0D__INCLUDED_)
#define AFX_DIRECTORY_H__1A4AB953_5C82_11D5_B191_00A0CC271D0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
class CDirectory  
	{
	private:
		CString m_sPath;

	public:
		CDirectory(LPCTSTR pPath);
		virtual ~CDirectory();

		LPCTSTR GetPath() const;

		bool Exists();

		HANDLE GetDirHandle(bool bLock = false, DWORD nFlags = 0);
		static HANDLE GetDirHandle(LPCTSTR pPath, bool bLock = false, DWORD nFlags = 0);

		bool CreatePath();
		static bool CreatePath(LPCTSTR);

		static bool IsDirectory(LPCTSTR);

		bool ParsePath(CString& sDir, CString& sFilename);
		static bool ParsePath(LPCTSTR pFullpath, CString& sDir, CString& sFilename);

		static bool Remove(LPCTSTR pPath, bool bRecursive = true);

		static int GetMatchingFileCount(LPCTSTR pSpec);
	};

#endif // !defined(AFX_DIRECTORY_H__1A4AB953_5C82_11D5_B191_00A0CC271D0D__INCLUDED_)
