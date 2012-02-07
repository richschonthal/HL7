//--------------------------------------------------------------------------------
// Copyright (c) 2001 @COMPANY, Inc.
// Created...: 6/11/01
// Author....: Rich Schonthal
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "FileIndexDirObject.h"

//--------------------------------------------------------------------------------
static BOOL AFXAPI CompareElements<LPCTSTR, LPCTSTR>(LPCTSTR* pElement1, LPCTSTR* pElement2)
	{
	return stricmp(*pElement1, *pElement2) == 0;
	}

// turn off the warning about using 'this' in the init list
#pragma warning(disable : 4355)
//--------------------------------------------------------------------------------
CFileIndexDirObject::CFileIndexDirObject(LPCTSTR pDir)
		: CFileIndexObject(pDir)
		, m_files(this)
	{
	}

#pragma warning(default : 4355)

//--------------------------------------------------------------------------------
UINT CFileIndexDirObject::GetObjectId()
	{
	return 1;
	}

//--------------------------------------------------------------------------------
bool CFileIndexDirObject::AddFile(CFileIndexFileObject* pFile)
	{
	CSingleLock lock(&m_mutex, false);
	if(! lock.Lock(1000))
		return false;

	CFileIndexFileObject* pTempFile;
	if(! m_files.Lookup(pFile->GetName(), (CFileIndexObject*&) pTempFile))
		m_files.SetAt(pFile->GetName(), pFile);
	return true;
	}

//--------------------------------------------------------------------------------
bool CFileIndexDirObject::FindFile(LPCTSTR pFilename, CFileIndexFileObject*& pFile)
	{
	CSingleLock lock(&m_mutex, false);
	if(! lock.Lock(1000))
		return false;

	return m_files.Lookup(pFilename, (CFileIndexObject*&) pFile) != 0;
	}

//--------------------------------------------------------------------------------
bool CFileIndexDirObject::RemoveFile(LPCTSTR pFilename)
	{
	CSingleLock lock(&m_mutex, false);
	if(! lock.Lock(1000))
		return false;

	CFileIndexFileObject* pFile;
	if(! m_files.Lookup(pFilename, (CFileIndexObject*&) pFile))
		return false;

	m_files.RemoveKey(pFilename);
	return true;
	}

//--------------------------------------------------------------------------------
bool CFileIndexDirObject::RemoveFile(CFileIndexFileObject* pWhich)
	{
	CSingleLock lock(&m_mutex, false);
	if(! lock.Lock(1000))
		return false;

	CFileIndexFileObject* pFile;
	if(! m_files.Lookup(pWhich->GetName(), (CFileIndexObject*&) pFile))
		return false;

	m_files.RemoveKey(pWhich->GetName());
	return true;
	}

//--------------------------------------------------------------------------------
bool CFileIndexDirObject::Write(CFile& file, bool bResetPosOnError)
	{
	if(CFileIndexObject::Write(file, bResetPosOnError))
		return m_files.Write(file);
	return false;
	}

//--------------------------------------------------------------------------------
bool CFileIndexDirObject::Read(CFile& file, bool bResetPosOnError)
	{
	if(CFileIndexObject::Read(file, bResetPosOnError))
		return m_files.Read(file);
	return false;
	}

#ifdef _DEBUG
//--------------------------------------------------------------------------------
void CFileIndexDirObject::Dump(CStdioFile& dumpFile)
	{
	POSITION pos = m_files.GetStartPosition();
	LPCTSTR pKey;
	CFileIndexFileObject* pFile;
	CString sTemp;
	while(pos != NULL)
		{
		m_files.GetNextAssoc(pos, pKey, (CFileIndexObject*&) pFile);
		sTemp.Format("FILE: %s\\%s\n", GetName(), pFile->GetName());
		dumpFile.WriteString(sTemp);
		}
	}
#endif

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
CFileIndexDirObjectMap::CFileIndexDirObjectMap()
	{
	m_nHashTableSize = 409;
	}

//--------------------------------------------------------------------------------
CFileIndexObject* CFileIndexDirObjectMap::CreateNewIndexObject(LPCTSTR pName)
	{
	return (CFileIndexObject*) new CFileIndexDirObject(pName);
	}
