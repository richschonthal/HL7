//--------------------------------------------------------------------------------
// Copyright (c) 2001 @COMPANY, Inc.
// Created...: 6/8/01
// Author....: Rich Schonthal
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "Directory.h"
#include "Handle.h"

//--------------------------------------------------------------------------------
CDirectory::CDirectory(LPCTSTR pPath)
		: m_sPath(pPath)
	{
	}

//--------------------------------------------------------------------------------
CDirectory::~CDirectory()
	{
	}

//--------------------------------------------------------------------------------
LPCTSTR CDirectory::GetPath() const
	{
	return m_sPath;
	}

//--------------------------------------------------------------------------------
bool CDirectory::Exists()
	{
	return IsDirectory(GetPath());
	}

//--------------------------------------------------------------------------------
bool CDirectory::IsDirectory(LPCTSTR pPath)
	{
	DWORD nAttr = ::GetFileAttributes(pPath);
	if(nAttr == 0xFFFFFFFF)
		return false;

	return (nAttr & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY;
	}

//--------------------------------------------------------------------------------
HANDLE CDirectory::GetDirHandle(bool bLock, DWORD nFlags)
	{
	return GetDirHandle(m_sPath, bLock, nFlags);
	}

//--------------------------------------------------------------------------------
HANDLE CDirectory::GetDirHandle(LPCTSTR pPath, bool bLock, DWORD nFlags)
	{
	return ::CreateFile(
		pPath,
		GENERIC_READ|GENERIC_WRITE|FILE_LIST_DIRECTORY,
		bLock ? 0 : FILE_SHARE_DELETE|FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL|FILE_FLAG_BACKUP_SEMANTICS|nFlags,
		NULL
		);
	}

//--------------------------------------------------------------------------------
bool CDirectory::CreatePath()
	{
	return CreatePath(m_sPath);
	}

//--------------------------------------------------------------------------------
bool CDirectory::CreatePath(LPCTSTR pPath)
	{
	if(pPath == NULL || *pPath == 0)
		return false;

	CString sTemp(pPath);
	int nIndex = -1;

	bool bIsWide = false;
	const CString sWide("\\\\?\\");

	// first see if the caller is trying to call the wide char version
	if(sTemp.GetLength() > 4)
		if(sTemp[2] == '?')
			{
			sTemp = sTemp.Mid(4);
			bIsWide = true;
			}

	int nColon = sTemp.Find(':', nIndex + 1);

	if(nColon != -1)
		nIndex = nColon + 1;
	else
		if(sTemp.GetLength() > 2)
			{
			// see if its a UNC name
			if(sTemp[nIndex + 1] == '\\' && sTemp[nIndex + 2] == '\\')
				{
				nIndex = sTemp.Find('\\', nIndex + 3);
				if(nIndex == -1)
					return false;
				// now skip over the share name
				nIndex = sTemp.Find('\\', nIndex + 1);
				if(nIndex == -1)
					return false;
				}
			}

	if(nIndex >= sTemp.GetLength())
		return false;

	for(bool bDone = false;! bDone;)
		{
		nIndex = sTemp.Find('\\', nIndex + 1);
		if(nIndex == -1)
			{
			bDone = true;
			nIndex = sTemp.GetLength();
			}

		if(nIndex == 2)
			continue;

		if(bIsWide)
			::CreateDirectory(sWide + sTemp.Left(nIndex), NULL);
		else
			::CreateDirectory(sTemp.Left(nIndex), NULL);
		int nErr = ::GetLastError();
		}

	return true;
	}

//--------------------------------------------------------------------------------
bool CDirectory::ParsePath(CString& sDir, CString& sFilename)
	{
	return ParsePath(m_sPath, sDir, sFilename);
	}

//--------------------------------------------------------------------------------
bool CDirectory::ParsePath(LPCTSTR pOrigFullpath, CString& sDir, CString& sFilename)
	{
	if(pOrigFullpath == NULL || *pOrigFullpath == 0)
		return false;

	char* pFullpath = new char[strlen(pOrigFullpath) + 1];
	strcpy(pFullpath, pOrigFullpath);
	char* pBackslash = strrchr(pFullpath, '\\');
	if(pBackslash == NULL)
		{
		sDir.Empty();
		sFilename = pFullpath;
		delete[] pFullpath;
		return true;
		}

	*pBackslash = 0;
	sDir = pFullpath;
	sFilename = pBackslash + 1;
	delete[] pFullpath;
	return true;
	}

//--------------------------------------------------------------------------------
bool CDirectory::Remove(LPCTSTR pPath, bool bRecursive)
	{
	// make life easier with a CString
	CString sPath(pPath);
	if(sPath.Right(1) != '\\')
		sPath += '\\';

	WIN32_FIND_DATA find;
	HANDLE hand = ::FindFirstFile(sPath + "*.*", &find);
	if(hand == INVALID_HANDLE_VALUE || hand == NULL)
		return false;

	for(;;)
		{
		CString sName(sPath + find.cFileName);
		if(find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
			if(find.cFileName[0] != '.')
				{
				if(bRecursive)
					// recurse thru the dir
					if(! Remove(sName, true))
						return false;
				}
			}
		else
			{
			::SetFileAttributes(sName, FILE_ATTRIBUTE_NORMAL);
			if(! ::DeleteFile(sName))
				return false;
			}

		if(! ::FindNextFile(hand, &find))
			break;
		}

	::FindClose(hand);

	::SetFileAttributes(sPath, FILE_ATTRIBUTE_NORMAL);
	return ::RemoveDirectory(sPath) == TRUE;
	}

//--------------------------------------------------------------------------------
int CDirectory::GetMatchingFileCount(LPCTSTR pSpec)
	{
	WIN32_FIND_DATA data;
	CFindFileHandle hand = ::FindFirstFile(pSpec, &data);
	if(hand != INVALID_HANDLE_VALUE)
		{
		int nCount = 0;
		for(;;)
			{
			if((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
				nCount++;
			if(! ::FindNextFile(hand, &data))
				return nCount;
			}
		}
	return 0;
	}
