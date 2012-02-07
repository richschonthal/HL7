//--------------------------------------------------------------------------------
// Copyright (c) 2001 @COMPANY, Inc.
// Created...: 6/11/01
// Author....: Rich Schonthal
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "FileIndexObject.h"

//--------------------------------------------------------------------------------
CFileIndexObject::CFileIndexObject(LPCTSTR pName)
		: m_sName(pName)
		, m_nRefCount(0)
	{
	}

//--------------------------------------------------------------------------------
CFileIndexObject::~CFileIndexObject()
	{
	}

//--------------------------------------------------------------------------------
LPCTSTR CFileIndexObject::GetName()
	{
	return m_sName;
	}

//--------------------------------------------------------------------------------
void CFileIndexObject::SetName(LPCTSTR pName)
	{
	m_sName = pName;
	}

//--------------------------------------------------------------------------------
UINT CFileIndexObject::GetObjectId()
	{
	return 0;
	}

//--------------------------------------------------------------------------------
bool CFileIndexObject::Write(CFile& file, bool bResetPosOnError)
	{
	DWORD nPos;
	bool bPosOk = false;
	bool bRv = true;
	TRY
		{
		if(bResetPosOnError)
			{
			nPos = file.GetPosition();
			bPosOk = true;
			}

		DWORD nNum = GetObjectId();
		file.Write(&nNum, sizeof(nNum));
		nNum = m_sName.GetLength();
		file.Write(&nNum, sizeof(nNum));
		file.Write((LPCTSTR) m_sName, nNum);
	char temp[1024];
	file.Write(temp, 1024);
		}
	CATCH_ALL(e)
		{
		bRv = false;		
		}
	END_CATCH_ALL;

	if(bPosOk && bResetPosOnError && ! bRv)
		TRY
			{
			file.Seek(nPos, CFile::begin);
			}
		CATCH_ALL(e)
			{
			}
		END_CATCH_ALL;

	return bRv;
	}

//--------------------------------------------------------------------------------
bool CFileIndexObject::Read(CFile& file, bool bResetPosOnError)
	{
	DWORD nPos;
	bool bPosOk = false;
	DWORD nNum;
	char* pBuf = NULL;
	bool bRv = true;

	TRY
		{
		if(bResetPosOnError)
			{
			nPos = file.GetPosition();
			bPosOk = true;
			}

		// read the object id
		if(file.Read(&nNum, sizeof(nNum)) == sizeof(nNum))
			// be sure we're reading the object we want
			if(nNum == GetObjectId())
				// read the name length
				if(file.Read(&nNum, sizeof(nNum)) == sizeof(nNum))
					{
					pBuf = m_sName.GetBuffer(nNum);
					bRv = (file.Read(pBuf, nNum) == nNum);
					}
	char temp[1024];
	file.Read(temp, 1024);
		}
	CATCH_ALL(e)
		{
		bRv = false;
		}
	END_CATCH_ALL;

	if(pBuf != NULL)
		m_sName.ReleaseBuffer();

	if(bResetPosOnError && bPosOk && ! bRv)
		TRY
			{
			file.Seek(nPos, CFile::begin);
			}
		CATCH_ALL(e)
			{
			}
		END_CATCH_ALL;

	return bRv;
	}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
CFileIndexObjMap::CFileIndexObjMap()
	{
	m_nHashTableSize = 257;
	}

//--------------------------------------------------------------------------------
bool CFileIndexObjMap::Write(CFile& file)
	{
	DWORD nPos;
	bool bPosOk = false;
	bool bRv = true;
	POSITION pos = GetStartPosition();

	TRY
		{
		nPos = file.GetPosition();
		bPosOk = true;
		DWORD nNum = 'CMAP';
		file.Write(&nNum, sizeof(nNum));
		nNum = GetCount();
		file.Write(&nNum, sizeof(nNum));
		}
	CATCH_ALL(e)
		{
		bRv = false;
		pos = NULL;
		}
	END_CATCH_ALL;

	while(pos != NULL)
		TRY
			{
			LPCTSTR pKey;
			CFileIndexObject* pObj;
			GetNextAssoc(pos, pKey, pObj);
			if(pObj == NULL)
				THROW(new CException);

			pObj->Write(file, false);
			}
		CATCH_ALL(e)
			{
			bRv = false;
			break;
			}
		END_CATCH_ALL;

	if(bPosOk && ! bRv)
		TRY
			{
			file.Seek(nPos, CFile::begin);
			}
		CATCH_ALL(e)
			{
			}
		END_CATCH_ALL;

	return bRv;
	}

//--------------------------------------------------------------------------------
bool CFileIndexObjMap::Read(CFile& file)
	{
	DWORD nPos;
	bool bPosOk = false;
	bool bRv = true;
	POSITION pos = GetStartPosition();
	int nCount;

	TRY
		{
		nPos = file.GetPosition();
		bPosOk = true;
		DWORD nNum;
		if(file.Read(&nNum, sizeof(nNum)) != sizeof(nNum) || nNum != 'CMAP')
			bRv = false;
		if(file.Read(&nNum, sizeof(nNum)) != sizeof(nNum))
			bRv = false;
		nCount = nNum;
		}
	CATCH_ALL(e)
		{
		bRv = false;
		}
	END_CATCH_ALL;

	for(int i = 0; i < nCount; i++)
		{
		CFileIndexObject* pObj = CreateNewIndexObject();
		bRv = pObj->Read(file, false);
		if(! bRv)
			{
			delete pObj;
			break;
			}
		SetAt(pObj->GetName(), pObj);
		}

	if(bPosOk && ! bRv)
		TRY
			{
			file.Seek(nPos, CFile::begin);
			}
		CATCH_ALL(e)
			{
			}
		END_CATCH_ALL;

	return bRv;
	}

//--------------------------------------------------------------------------------
CFileIndexObject* CFileIndexObjMap::CreateNewIndexObject(LPCTSTR pName)
	{
	return new CFileIndexObject(pName);
	}
