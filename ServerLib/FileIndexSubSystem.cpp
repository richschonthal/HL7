//--------------------------------------------------------------------------------
// Copyright (c) 2001 @COMPANY, Inc.
// Created...: 6/11/01
// Author....: Rich Schonthal
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "FileIndexSubSystem.h"

#include <ReadLock.h>
#include <WriteLock.h>
#include <Handle.h>

//--------------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CFileIndexSubSystem, CChangeNotificationSubSystem);

//--------------------------------------------------------------------------------
BOOL AFXAPI CompareElements<LPCTSTR, LPCTSTR>(LPCTSTR* pElement1, LPCTSTR* pElement2)
	{
	return stricmp(*pElement1, *pElement2) == 0;
	}

//--------------------------------------------------------------------------------
CFileIndexSubSystem::CFileIndexSubSystem(CSystem* pParent)
		: CChangeNotificationSubSystem(pParent)
	{
	}

//--------------------------------------------------------------------------------
CFileIndexSubSystem::~CFileIndexSubSystem()
	{
	}

//--------------------------------------------------------------------------------
bool CFileIndexSubSystem::AddDirectory(LPCTSTR pDir, CFileIndexDirObject*& pObj)
	{
	pObj = NULL;
	CWriteLock lock(&m_tree, false);
	if(! lock.Lock(1000))
		return false;

	return AddDirectory(pDir, pObj, lock);
	}

//--------------------------------------------------------------------------------
bool CFileIndexSubSystem::AddDirectory(LPCTSTR pDir, CFileIndexDirObject*& pObj, CWriteLock& lock)
	{
	pObj = NULL;

	CFileIndexDirObject* pTemp;
	if(m_tree.Lookup(pDir, (CFileIndexObject*&) pTemp))
		return false;

	pObj = new CFileIndexDirObject(pDir);
	m_tree.SetAt(pObj->GetName(), pObj);
	return true;
	}

//--------------------------------------------------------------------------------
bool CFileIndexSubSystem::FindDirectory(LPCTSTR pPath, CFileIndexDirObject*& pObj)
	{
	CReadLock lock(&m_tree, false);
	if(! lock.Lock(1000))
		return false;

	return m_tree.Lookup(pPath, (CFileIndexObject*&) pObj) != 0;
	}

//--------------------------------------------------------------------------------
bool CFileIndexSubSystem::RemoveDirectory(LPCTSTR pPath)
	{
	CWriteLock lock(&m_tree, false);
	if(! lock.Lock(1000))
		return false;

	return m_tree.RemoveKey(pPath) != 0;
	}

//--------------------------------------------------------------------------------
bool CFileIndexSubSystem::RemoveDirectory(CFileIndexDirObject* pObj)
	{
	CWriteLock lock(&m_tree, false);
	if(! lock.Lock(1000))
		return false;

	return m_tree.RemoveKey(pObj->GetName()) != 0;
	}

//--------------------------------------------------------------------------------
bool CFileIndexSubSystem::AddFile(LPCTSTR pFullPathname, CFileIndexDirObject*& pDir, CFileIndexFileObject*& pFile)
	{
	if(pFullPathname == NULL || *pFullPathname == 0)
		return false;

	CString sPath;
	CString sFilename;
	if(! CDirectory::ParsePath(pFullPathname, sPath, sFilename))
		return false;

	return AddFile(sPath, sFilename, pDir, pFile);
	}

//--------------------------------------------------------------------------------
bool CFileIndexSubSystem::AddFile(LPCTSTR pFullPathname, CFileIndexDirObject*& pDir, CFileIndexFileObject*& pFile, CWriteLock& lock)
	{
	if(pFullPathname == NULL || *pFullPathname == 0)
		return false;

	CString sPath;
	CString sFilename;
	if(! CDirectory::ParsePath(pFullPathname, sPath, sFilename))
		return false;

	return AddFile(sPath, sFilename, pDir, pFile, lock);
	}

//--------------------------------------------------------------------------------
bool CFileIndexSubSystem::AddFile(LPCTSTR pDirname, LPCTSTR pFilename, CFileIndexDirObject*& pDir, CFileIndexFileObject*& pFile)
	{
	pDir = NULL;
	pFile = NULL;

	if(pDirname == NULL || *pDirname == 0 || pFilename == NULL || *pFilename == 0)
		return false;

	CWriteLock lock(&m_tree, false);
	if(! lock.Lock(1000))
		return false;

	return AddFile(pDirname, pFilename, pDir, pFile, lock);
	}

//--------------------------------------------------------------------------------
bool CFileIndexSubSystem::AddFile(LPCTSTR pDirname, LPCTSTR pFilename, CFileIndexDirObject*& pDir, CFileIndexFileObject*& pFile,
								  CWriteLock& lock)
	{
	pDir = NULL;
	pFile = NULL;

	if(pDirname == NULL || *pDirname == 0 || pFilename == NULL || *pFilename == 0)
		return false;

	bool bIsNewDir = false;
	bool bIsNewFile = false;
	bool bRv = false;

	if(! m_tree.Lookup(pDirname, (CFileIndexObject*&) pDir))
		{
		bIsNewDir = true;
		pDir = CreateFileIndexDirObject(pDirname);
		if(pDir == NULL)
			return false;
		m_tree.SetAt(pDir->GetName(), pDir);
		}

	for(; pDir != NULL; )
		{
		CSingleLock lockDir(&pDir->m_mutex, false);
		if(lockDir.Lock(1000))
			{
			if(! pDir->FindFile(pFilename, pFile))
				{
				pFile = CreateFileIndexFileObject(pDir, pFilename);
				if(pFile == NULL)
					break;
				bIsNewFile = true;
				if(! pDir->AddFile(pFile))
					break;
				}

			bRv = true;
			}

		break;
		}

	if(! bRv)
		{
		if(bIsNewDir)
			{
			if(m_tree.Lookup(pDir->GetName(), (CFileIndexObject*&) pDir))
				m_tree.RemoveKey(pDir->GetName());
			delete pDir;
			pDir = NULL;
			}

		if(bIsNewFile)
			{
			delete pFile;
			pFile = NULL;
			}
		}

	return bRv;
	}

//--------------------------------------------------------------------------------
bool CFileIndexSubSystem::FindFile(LPCTSTR pFullPathname, CFileIndexDirObject*& pDir, CFileIndexFileObject*& pFile)
	{
	if(pFullPathname == NULL || *pFullPathname == 0)
		return false;

	CString sPath;
	CString sFilename;
	if(! CDirectory::ParsePath(pFullPathname, sPath, sFilename))
		return false;

	return FindFile(sPath, sFilename, pDir, pFile);
	}

//--------------------------------------------------------------------------------
bool CFileIndexSubSystem::FindFile(LPCTSTR pFullPathname, CFileIndexDirObject*& pDir, CFileIndexFileObject*& pFile, CWriteLock& lock)
	{
	if(pFullPathname == NULL || *pFullPathname == 0)
		return false;

	CString sPath;
	CString sFilename;
	if(! CDirectory::ParsePath(pFullPathname, sPath, sFilename))
		return false;

	return FindFile(sPath, sFilename, pDir, pFile, lock);
	}

//--------------------------------------------------------------------------------
bool CFileIndexSubSystem::FindFile(LPCTSTR pDirname, LPCTSTR pFilename, CFileIndexDirObject*& pDir, CFileIndexFileObject*& pFile)
	{
	pDir = NULL;
	pFile = NULL;

	if(pDirname == NULL || *pDirname == 0 || pFilename == NULL || *pFilename == 0)
		return false;

	CWriteLock lock(&m_tree, false);
	if(! lock.Lock(1000))
		return false;

	return FindFile(pDirname, pFilename, pDir, pFile, lock);
	}
	
//--------------------------------------------------------------------------------
bool CFileIndexSubSystem::FindFile(LPCTSTR pDirname, LPCTSTR pFilename, CFileIndexDirObject*& pDir, CFileIndexFileObject*& pFile,
								   CWriteLock& lock)
	{
	pDir = NULL;
	pFile = NULL;

	if(pDirname == NULL || *pDirname == 0 || pFilename == NULL || *pFilename == 0)
		return false;

	if(! m_tree.Lookup(pDirname, (CFileIndexObject*&) pDir))
		return false;

	return pDir->FindFile(pFilename, pFile);
	}

//--------------------------------------------------------------------------------
bool CFileIndexSubSystem::ParseDirectory(CString sPath, CWriteLock& lock)
	{
	WIN32_FIND_DATA data;

	CFindFileHandle hand = ::FindFirstFile(sPath + "\\*.*", &data);
	if(! hand.IsValid())
		return false;

	CFileIndexDirObject* pDir;
	CFileIndexFileObject* pFile;

	for(;;)
		{
		if(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
			if(data.cFileName[0] != '.')
				ParseDirectory(sPath + '\\' + data.cFileName, lock);
			}
		else
			AddFile(sPath, data.cFileName, pDir, pFile, lock);

		if(! ::FindNextFile(hand, &data))
			break;
		}

	return true;
	}

//--------------------------------------------------------------------------------
bool CFileIndexSubSystem::ParseNewWatchPath(LPCTSTR pPath)
	{
	CWriteLock lock(&m_tree, false);
	if(! lock.Lock(10000))
		return false;

	ParseDirectory(pPath, lock);
	return true;
	}

//--------------------------------------------------------------------------------
bool CFileIndexSubSystem::WriteIndex(CFile& file)
	{
	CReadLock lock(&m_tree, false);
	if(! lock.Lock(10000))
		return false;

	return m_tree.Write(file);
	}

//--------------------------------------------------------------------------------
bool CFileIndexSubSystem::ReadIndex(CFile& file)
	{
	CWriteLock lock(&m_tree, false);
	if(! lock.Lock(10000))
		return false;

	return m_tree.Read(file);
	}

//--------------------------------------------------------------------------------
CFileIndexDirObject* CFileIndexSubSystem::CreateFileIndexDirObject(LPCTSTR pDir)
	{
	return new CFileIndexDirObject(pDir);
	}

//--------------------------------------------------------------------------------
CFileIndexFileObject* CFileIndexSubSystem::CreateFileIndexFileObject(CFileIndexDirObject* pDir, LPCTSTR pFilename)
	{
	return new CFileIndexFileObject(pDir, pFilename);
	}

#ifdef _DEBUG
//--------------------------------------------------------------------------------
void CFileIndexSubSystem::Dump(LPCTSTR pDumpFilename)
	{
	CStdioFile dumpFile;
	dumpFile.Open(pDumpFilename, CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive);

	POSITION pos = m_tree.GetStartPosition();
	LPCTSTR pKey;
	CFileIndexDirObject* pDir;
	CString sTemp;
	while(pos != NULL)
		{
		m_tree.GetNextAssoc(pos, pKey, (CFileIndexObject*&) pDir);
		sTemp.Format(" DIR: %s\n", pDir->GetName());
		dumpFile.WriteString(sTemp);
		pDir->Dump(dumpFile);
		}

	}
#endif
