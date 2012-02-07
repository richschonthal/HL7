//--------------------------------------------------------------------------------
// Copyright (c) 2001 @COMPANY, Inc.
// Created...: 6/11/01
// Author....: Rich Schonthal
//--------------------------------------------------------------------------------

#if !defined(AFX_FILEINDEXDIROBJECT_H__B66F74A9_E5CF_11D3_B10F_00A0CC271D0D__INCLUDED_)
#define AFX_FILEINDEXDIROBJECT_H__B66F74A9_E5CF_11D3_B10F_00A0CC271D0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
#include "FileIndexObject.h"
#include "FileIndexFileObject.h"

#include <ReadWriteObject.h>

#ifndef __AFXTEMPL_H__
#include <afxtempl.h>
#endif

//--------------------------------------------------------------------------------
class CFileIndexDirObject : public CFileIndexObject
	{
	protected:
		CFileIndexFileMap m_files;

	public:
		CFileIndexDirObject(LPCTSTR pDir);

		bool AddFile(CFileIndexFileObject*);
		bool FindFile(LPCTSTR pPath, CFileIndexFileObject*&);
		bool RemoveFile(LPCTSTR pFilename);
		bool RemoveFile(CFileIndexFileObject*);

		virtual UINT GetObjectId();

		virtual bool Write(CFile&, bool bResetPosOnError = true);
		virtual bool Read(CFile&, bool bResetPosOnError = true);

#ifdef _DEBUG
		void Dump(CStdioFile&);
#endif
	};

//--------------------------------------------------------------------------------
class CFileIndexDirObjectMap :	public CFileIndexObjMap
	{
	public:
		CFileIndexDirObjectMap();
		
		virtual CFileIndexObject* CreateNewIndexObject(LPCTSTR pName = NULL);
	};

#endif // !defined(AFX_FILEINDEXDIROBJECT_H__B66F74A9_E5CF_11D3_B10F_00A0CC271D0D__INCLUDED_)
