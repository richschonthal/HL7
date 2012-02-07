//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// DelimFile.cpp: implementation of the CDelimFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DelimFile.h"
#include "PlatformInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------
CDelimFile::CDelimFile(char cDelim, char cComment, bool bStrip)
		: m_cFieldDelim(cDelim)
		, m_cComment(cComment)
		, m_bStripQuotes(bStrip)
		, m_nCurLine(-1)
	{

	}

//--------------------------------------------------------------------------------
CDelimFile::~CDelimFile()
	{

	}

//--------------------------------------------------------------------------------
void CDelimFile::SetParams(char cDelim, char cComment, bool bStrip)
	{
	m_cFieldDelim = cDelim;
	m_cComment = cComment;
	m_bStripQuotes = bStrip;
	}

//--------------------------------------------------------------------------------
void CDelimFile::ParseLine(char cDelim, char cComment, bool bStripQuotes, LPCTSTR pText, CStringArray& sOut)
	{
	CString sIn(pText);

	sIn.TrimLeft();
	sIn.TrimRight();
	if(sIn.IsEmpty())
		return;

	// if this is a comment line then
	// add the whole thing and return
	if(sIn[0] == cComment)
		{
		sOut.Add(sIn);
		return;
		}

	CString sTemp;
	for(int i = 0; i < sIn.GetLength(); i++)
		{
		// is this a quoted string
		if(sIn[i] == '"')
			{
			// find the closing quote
			int nIndex = sIn.Find('"', i + 1);
			// no closing quote
			if(nIndex == -1)
				{
				// add the remainder of the string and return
				sTemp = sIn.Mid(i + 1);
				sTemp.TrimLeft();
				sTemp.TrimRight();
				sOut.Add(sTemp);
				return;
				}
			else
				{
				// start at the quote if quotes are included
				// otherwise start at the next char
				int nOffset = bStripQuotes ? 1 : 0;
				int nBackset = bStripQuotes ? -1 : 1;
				sTemp = sIn.Mid(i + nOffset, nIndex - i + nBackset);
				sTemp.TrimLeft();
				sTemp.TrimRight();
				sOut.Add(sTemp);
				sTemp.Empty();
				i = nIndex + 1;
				}
			}
		else
			if(sIn[i] == cDelim)
				{
				sTemp.TrimLeft();
				sTemp.TrimRight();
				sOut.Add(sTemp);
				sTemp.Empty();
				}
			else
				sTemp += sIn[i];
		}

	if(! sTemp.IsEmpty())
		{
		sTemp.TrimLeft();
		sTemp.TrimRight();
		sOut.Add(sTemp);
		}
	}

//--------------------------------------------------------------------------------
void CDelimFile::ParseLine(LPCTSTR pText, CStringArray& sOut)
	{
	ParseLine(m_cFieldDelim, m_cComment, m_bStripQuotes, pText, sOut);
	}

//--------------------------------------------------------------------------------
bool CDelimFile::ReadLine(CStringArray& sOut)
	{
	CString sTemp;
	if(! ReadString(sTemp))
		return false;

	ParseLine(sTemp, sOut);
	return true;
	}

