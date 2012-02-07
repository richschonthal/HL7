//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// SettingsFile.cpp: implementation of the CSettingsFile class.
//
//////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------
#include "stdafx.h"
#include "DelimFile.h"
#include "SettingsFile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------
CSettingsFile::CSettingsFile(char cDelim, char cComment, bool bStrip)
		: CDelimFile(cDelim, cComment, bStrip)
		, m_nCurLine(-1)
	{

	}

//--------------------------------------------------------------------------------
CSettingsFile::~CSettingsFile()
	{

	}

//--------------------------------------------------------------------------------
bool CSettingsFile::Find(LPCTSTR pKey, CSettingsInfo& info)
	{
	CStringArray sTemp;
	CString sKey(pKey);
	sKey.MakeLower();
	for(int i = 0; GetLine(i, sTemp); i++)
		{
		sTemp[0].MakeLower();
		if(sTemp[0] == sKey)
			{
			info = sTemp;
			return true;
			}
		}

	return false;
	}

//--------------------------------------------------------------------------------
bool CSettingsFile::GetLine(int nIndex, CStringArray& sInfo)
	{
	sInfo.RemoveAll();
	if(m_nCurLine == -1 || nIndex < m_nCurLine)
		{
		SeekToBegin();
		m_nCurLine = 0;
		}

	while(ReadLine(sInfo))
		{
		if(sInfo.GetSize() < GetMinSize())
			{
			sInfo.RemoveAll();
			continue;
			}

		if(m_nCurLine == nIndex)
			{
			m_nCurLine++;
			return true;
			}
		
		sInfo.RemoveAll();
		m_nCurLine++;
		}

	return false;
	}

//--------------------------------------------------------------------------------
bool CSettingsFile::GetLine(int nIndex, CSettingsInfo& info)
	{
	CStringArray sTemp;
	if(GetLine(nIndex, sTemp))
		{
		info = sTemp;
		return true;
		}

	return false;
	}
