//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// SettingsFile.h: interface for the CSettingsFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SETTINGSFILE_H__A4FC90E3_5AF6_11D3_A6BC_005004A1C5F3__INCLUDED_)
#define AFX_SETTINGSFILE_H__A4FC90E3_5AF6_11D3_A6BC_005004A1C5F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
#include "DelimFile.h"

//--------------------------------------------------------------------------------
class CSettingsInfo
	{
	public:
		virtual operator=(const CStringArray&) {};
	};

//--------------------------------------------------------------------------------
class CSettingsFile : public CDelimFile  
	{
	public:
		CSettingsFile(char cDelim = ',', char cComment = ';', bool bStrip = true);

		virtual ~CSettingsFile();

	protected:
		int m_nCurLine;

	public:
		bool Find(LPCTSTR, CSettingsInfo&);
		bool GetLine(int, CSettingsInfo&);
		bool GetLine(int, CStringArray&);

		virtual int GetMinSize() = 0;
	};

#endif // !defined(AFX_SETTINGSFILE_H__A4FC90E3_5AF6_11D3_A6BC_005004A1C5F3__INCLUDED_)
