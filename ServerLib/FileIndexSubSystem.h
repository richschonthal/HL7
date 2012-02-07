//--------------------------------------------------------------------------------
// Copyright (c) 2001 @COMPANY, Inc.
// Created...: 6/11/01
// Author....: Rich Schonthal
//--------------------------------------------------------------------------------

#if !defined(AFX_FILEINDEXSUBSYSTEM_H__B66F74A9_E5CF_11D3_B10F_00A0CC271D0D__INCLUDED_)
#define AFX_FILEINDEXSUBSYSTEM_H__B66F74A9_E5CF_11D3_B10F_00A0CC271D0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
#include "SubSystem.h"
#include "System.h"
#include "ChangeNotificationSubSystem.h"
#include "FileIndexDirObject.h"

#include <Directory.h>
#include <WriteLock.h>

#ifndef __AFXTEMPL_H__
#include <afxtempl.h>
#endif

//--------------------------------------------------------------------------------
class CFileIndexSubSystem : public CChangeNotificationSubSystem  
	{
	friend class CFileIndexThread;

	public:
		DECLARE_DYNAMIC(CFileIndexSubSystem);

	protected:
		CFileIndexDirObjectMap m_tree;

	protected:
		bool ParseDirectory(CString sPath, CWriteLock&);

	public:
		CFileIndexSubSystem(CSystem*);
		virtual ~CFileIndexSubSystem();

		bool ParseNewWatchPath(LPCTSTR pPath);

		bool AddDirectory(LPCTSTR pDir, CFileIndexDirObject*&);
		bool AddDirectory(LPCTSTR pDir, CFileIndexDirObject*&, CWriteLock&);
		bool FindDirectory(LPCTSTR pDir, CFileIndexDirObject*&);
		bool RemoveDirectory(LPCTSTR pDir);
		bool RemoveDirectory(CFileIndexDirObject*);

		bool AddFile(LPCTSTR pFullPathname, CFileIndexDirObject*&, CFileIndexFileObject*&);
		bool AddFile(LPCTSTR pFullPathname, CFileIndexDirObject*&, CFileIndexFileObject*&, CWriteLock&);
		bool AddFile(LPCTSTR pDir, LPCTSTR pFilename, CFileIndexDirObject*&, CFileIndexFileObject*&);
		bool AddFile(LPCTSTR pDir, LPCTSTR pFilename, CFileIndexDirObject*&, CFileIndexFileObject*&, CWriteLock&);

		bool FindFile(LPCTSTR pFullPathname, CFileIndexDirObject*&, CFileIndexFileObject*&);
		bool FindFile(LPCTSTR pFullPathname, CFileIndexDirObject*&, CFileIndexFileObject*&, CWriteLock&);
		bool FindFile(LPCTSTR pDir, LPCTSTR pFilename, CFileIndexDirObject*&, CFileIndexFileObject*&);
		bool FindFile(LPCTSTR pDir, LPCTSTR pFilename, CFileIndexDirObject*&, CFileIndexFileObject*&, CWriteLock&);

		virtual CFileIndexDirObject* CreateFileIndexDirObject(LPCTSTR pDir);
		virtual CFileIndexFileObject* CreateFileIndexFileObject(CFileIndexDirObject*, LPCTSTR pFilename);

		virtual bool WriteIndex(CFile&);
		virtual bool ReadIndex(CFile&);

#ifdef _DEBUG
		void Dump(LPCTSTR pDumpFilename);
#endif
	};

#endif // !defined(AFX_FILEINDEXSUBSYSTEM_H__B66F74A9_E5CF_11D3_B10F_00A0CC271D0D__INCLUDED_)
