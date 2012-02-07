//--------------------------------------------------------------------------------
// Copyright (c) 2001 @COMPANY, Inc.
// Created...: 6/11/01
// Author....: Rich Schonthal
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "FileIndexFileObject.h"

//--------------------------------------------------------------------------------
CFileIndexFileObject::CFileIndexFileObject(CFileIndexDirObject* pDir, LPCTSTR pFilename)
		: CFileIndexObject(pFilename)
		, m_pDir(pDir)
	{
	}

//--------------------------------------------------------------------------------
CFileIndexFileObject::~CFileIndexFileObject()
	{
	}

//--------------------------------------------------------------------------------
CFileIndexDirObject* CFileIndexFileObject::GetDir()
	{
	return m_pDir;
	}

//--------------------------------------------------------------------------------
UINT CFileIndexFileObject::GetObjectId()
	{
	return 2;
	}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
CFileIndexFileMap::CFileIndexFileMap(CFileIndexDirObject* pOwnerObj)
		: m_pDirObj(pOwnerObj)
	{
	m_nHashTableSize = 2617;
	}

//--------------------------------------------------------------------------------
CFileIndexObject* CFileIndexFileMap::CreateNewIndexObject(LPCTSTR pName)
	{
	return (CFileIndexObject*) new CFileIndexFileObject(m_pDirObj, pName);
	}
