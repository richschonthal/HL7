//--------------------------------------------------------------------------------
// Copyright (c) 2001 @COMPANY, Inc.
// Created...: 6/11/01
// Author....: Rich Schonthal
//--------------------------------------------------------------------------------

#if !defined(AFX_FILEINDEXFILEOBJECT_H__B66F74A9_E5CF_11D3_B10F_00A0CC271D0D__INCLUDED_)
#define AFX_FILEINDEXFILEOBJECT_H__B66F74A9_E5CF_11D3_B10F_00A0CC271D0D__INCLUDED_

//--------------------------------------------------------------------------------
#include "FileIndexObject.h"
#include <ReadWriteObject.h>

#ifndef __AFXTEMPL_H__
#include <afxtempl.h>
#endif

//--------------------------------------------------------------------------------
class CFileIndexDirObject;

//--------------------------------------------------------------------------------
class CFileIndexFileObject : public CFileIndexObject
	{
	protected:
		CFileIndexDirObject* m_pDir;

	public:
		CFileIndexFileObject(LPCTSTR pName = NULL);
		CFileIndexFileObject(CFileIndexDirObject* pDir, LPCTSTR pFilename);

		virtual ~CFileIndexFileObject();

		CFileIndexDirObject* GetDir();
		virtual UINT GetObjectId();
	};

//--------------------------------------------------------------------------------
class CFileIndexFileMap : public CFileIndexObjMap
	{
	protected:
		CFileIndexDirObject* m_pDirObj;

	public:
		CFileIndexFileMap(CFileIndexDirObject*);

		virtual CFileIndexObject* CreateNewIndexObject(LPCTSTR pName = NULL);
	};

#endif//AFX_FILEINDEXFILEOBJECT_H__B66F74A9_E5CF_11D3_B10F_00A0CC271D0D__INCLUDED_
