//--------------------------------------------------------------------------------
//
// Copyright (c) 2001 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "PackDirectory.h"

#include "AllocString.h"
#include "PlatformInfo.h"

#define FILEPACK_VERSION 1

//--------------------------------------------------------------------------------
bool CPackDirectory::Pack(LPCTSTR pFilename, LPCTSTR pPath, bool bRecursive)
	{
	CFile file;
	if(! file.Open(pFilename, CFile::modeCreate|CFile::modeReadWrite|CFile::shareExclusive))
		return false;
	bool bRv = Pack(file, pPath, bRecursive);
	file.Close();
	return bRv;
	}

//--------------------------------------------------------------------------------
bool CPackDirectory::Pack(CFile& file, LPCTSTR pPath, bool bRecursive)
	{
	// this is the initial call so write file header etc...
	if(file.GetLength() == 0)
		{
		BYTE nVer = 1;
		file.Write(&nVer, sizeof(BYTE));
		DWORD nPathLen = strlen(pPath) + 1;
		file.Write(&nPathLen, sizeof(UINT));
		file.Write(pPath, nPathLen);
		file.Flush();
		bool bRv = Pack(file, pPath, bRecursive);
		return bRv;
		}

	TRACE("packing dir %s\n", pPath);

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
		if(find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
			if(find.cFileName[0] != '.')
				{
				if(bRecursive)
					{
					// write a begin dir entry
					Entry entry;
					entry.m_nType = 1;
					AllocString(entry.m_pName, find.cFileName);
					entry.Write(file);

					// recurse thru the dir
					Pack(file, sPath + find.cFileName, true);

					// write end of dir
					delete[] entry.m_pName;
					entry.m_pName = NULL;
					entry.m_nType = 2;
					entry.Write(file);
					}
				}
			}
		else
			{
			// packing a file
			CFile in;
			if(! in.Open(sPath + find.cFileName, CFile::modeRead|CFile::shareExclusive))
				return false;
			TRACE("   packing file %s\n", find.cFileName);
			Entry entry;
			AllocString(entry.m_pName, find.cFileName);
			entry.m_nLen = in.GetLength();
			entry.Write(file, &in);
			BYTE temp[1024];
			// write the file data
			for(;;)
				{
				int nCount = in.Read(temp, 1024);
				if(nCount == 0)
					break;
				file.Write(temp, nCount);
				}
			}

		if(! ::FindNextFile(hand, &find))
			break;
		}

	::FindClose(hand);
	return true;
	}

//--------------------------------------------------------------------------------
bool CPackDirectory::Unpack(LPCTSTR pFilename, LPCTSTR pPath, bool bOverwrite)
	{
	CFile file;
	if(! file.Open(pFilename, CFile::modeReadWrite|CFile::shareExclusive))
		return false;
	bool bRv = Unpack(file, pPath, bOverwrite);
	file.Close();
	return bRv;
	}

//--------------------------------------------------------------------------------
CString MakePathWithStack(const CString& sRoot, const CStringList& sStack)
	{
	CString sPath(sRoot);
	if(sPath.Right(1) != '\\')
		sPath += '\\';

	for(POSITION pos = sStack.GetHeadPosition(); pos != NULL; )
		sPath += sStack.GetNext(pos) + '\\';
	return sPath;
	}

//--------------------------------------------------------------------------------
bool CPackDirectory::Unpack(CFile& file, LPCTSTR pPath, bool bOverwrite)
	{
	BYTE nVersion;
	// read the header stuff
	if(file.Read(&nVersion, sizeof(BYTE)) != sizeof(BYTE))
		return false;
	if(nVersion > FILEPACK_VERSION)
		return false;
	DWORD nPathLen;
	if(file.Read(&nPathLen, sizeof(UINT)) != sizeof(UINT))
		return false;

	if(nPathLen > 1024)
		return false;

	CString sRoot;
	if(file.Read(sRoot.GetBuffer(nPathLen), nPathLen) != nPathLen)
		{
		sRoot.ReleaseBuffer();
		return false;
		}
	sRoot.ReleaseBuffer();

	CString sDest;

	if(pPath == NULL)
		sDest = sRoot;		
	else
		sDest = pPath;

	if(sDest.Right(1) != '\\')
		sDest += '\\';

	::CreateDirectory(sDest, NULL);

	TRACE("restoring to %s\n", (LPCTSTR) sDest);

	CStringList sPathStack;

	Entry entry;
	while(entry.Read(file))
		{
		switch(entry.m_nType)
			{
			case Entry::TypeFile:
				{
				CString sPath(MakePathWithStack(sDest, sPathStack));
				CFile out;
				if(! out.Open(sPath + entry.m_pName, CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive))
					return false;

				TRACE("   restoring file %s\n", entry.m_pName);
				char temp[1024];
				int nRemaining = entry.m_nLen;
				int nCount = nRemaining > 1024 ? 1024 : nRemaining;
				while(file.Read(temp, nCount))
					{
					out.Write(temp, nCount);
					nRemaining -= nCount;
					nCount = nRemaining > 1024 ? 1024 : nRemaining;
					}
				if(entry.m_nAttr != 0xffffffff)
					{
					::SetFileAttributes(sPath + entry.m_pName, entry.m_nAttr);
					::SetFileTime((HANDLE) (HFILE) out, &entry.m_ftCreate, &entry.m_ftAccess, &entry.m_ftWrite);
					}
				out.Close();
				}
				break;

			case Entry::TypeBeginDir:
				::CreateDirectory(sDest + entry.m_pName, NULL);
				sPathStack.AddTail(entry.m_pName);
				TRACE("created sub dir %s\n", entry.m_pName);
				break;

			case Entry::TypeEndDir:
				TRACE("moved up\n");
				sPathStack.RemoveAt(sPathStack.GetTailPosition());
				break;
			}
		}

	return true;
	}

//--------------------------------------------------------------------------------
void CPackDirectory::Entry::Write(CFile& file, CFile* pTarget)
	{
	file.Write(&m_nType, sizeof(BYTE));
	if(m_nType == TypeEndDir)
		return;
	file.Write(&m_nLen, sizeof(DWORD));
	UINT nLen = m_pName == NULL ? 0 : strlen(m_pName) + 1;
	file.Write(&nLen, sizeof(UINT));
	if(nLen > 0)
		file.Write(m_pName, nLen);
	if(m_pName != NULL)
		{
		m_nAttr = ::GetFileAttributes(m_pName);
		if(m_nAttr != -1)
			{
			HANDLE hand;
			if(pTarget == NULL)
				hand = ::CreateFile(m_pName, 0, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_BACKUP_SEMANTICS, NULL);
			else
				hand = (HANDLE) (HFILE) *pTarget;
			if(hand != INVALID_HANDLE_VALUE && ::GetFileTime(hand, &m_ftCreate, &m_ftAccess, &m_ftWrite))
				{
				if(pTarget == NULL)
					::CloseHandle(hand);
				file.Write(&m_nAttr, sizeof(DWORD));
				file.Write(&m_ftCreate, sizeof(FILETIME));
				file.Write(&m_ftAccess, sizeof(FILETIME));
				file.Write(&m_ftWrite, sizeof(FILETIME));
				return;
				}
			}
		}

	// write a -1 file attribute
	DWORD nDummy = -1;
	file.Write(&nDummy, sizeof(DWORD));
	}

//--------------------------------------------------------------------------------
bool CPackDirectory::Entry::Read(CFile& file)
	{
	if(file.Read(&m_nType, sizeof(BYTE)) != sizeof(BYTE))
		return false;
	if(m_nType == TypeEndDir)
		return true;
	if(file.Read(&m_nLen, sizeof(DWORD)) != sizeof(DWORD))
		return false;
	UINT nLen = 0;
	if(file.Read(&nLen, sizeof(UINT)) != sizeof(UINT))
		return false;
	if(nLen > 1024)
		return false;
	delete[] m_pName;
	m_pName = NULL;
	if(nLen == 0)
		return true;
	m_pName = new char[nLen];
	if(file.Read(m_pName, nLen) != nLen)
		return false;
	file.Read(&m_nAttr, sizeof(DWORD));
	if(m_nAttr != -1)
		{
		file.Read(&m_ftCreate, sizeof(FILETIME));
		file.Read(&m_ftAccess, sizeof(FILETIME));
		file.Read(&m_ftWrite, sizeof(FILETIME));
		}
	return true;
	}
