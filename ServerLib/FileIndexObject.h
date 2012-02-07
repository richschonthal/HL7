//--------------------------------------------------------------------------------
// Copyright (c) 2001 @COMPANY, Inc.
// Created...: 6/11/01
// Author....: Rich Schonthal
//--------------------------------------------------------------------------------

#if !defined(AFX_FILEINDEXOBJECT_H__B66F74A9_E5CF_11D3_B10F_00A0CC271D0D__INCLUDED_)
#define AFX_FILEINDEXOBJECT_H__B66F74A9_E5CF_11D3_B10F_00A0CC271D0D__INCLUDED_

//--------------------------------------------------------------------------------
#include <ReadWriteObject.h>

#ifndef __AFXTEMPL_H__
#include <afxtempl.h>
#endif

//--------------------------------------------------------------------------------
class CFileIndexObject
	{
	public:
		CMutex m_mutex;
		LONG m_nRefCount;

	protected:
		CString m_sName;

	public:
		CFileIndexObject(LPCTSTR pName);

		virtual ~CFileIndexObject();

		LPCTSTR GetName();
		void SetName(LPCTSTR pName);

		virtual UINT GetObjectId();

		virtual bool Write(CFile&, bool bResetPosOnError = true);
		virtual bool Read(CFile&, bool bResetPosOnError = true);
	};

//--------------------------------------------------------------------------------
class CFileIndexObjMap : public CReadWriteObject, public CMap<LPCTSTR, LPCTSTR, CFileIndexObject*, CFileIndexObject*>
	{
	public:
		CFileIndexObjMap();

		virtual bool Write(CFile&);
		virtual bool Read(CFile&);

		virtual CFileIndexObject* CreateNewIndexObject(LPCTSTR pName = NULL);
	};

#endif//AFX_FILEINDEXOBJECT_H__B66F74A9_E5CF_11D3_B10F_00A0CC271D0D__INCLUDED_
