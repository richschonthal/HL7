//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"

#include "WordWrap.h"

//--------------------------------------------------------------------------------
bool CWordWrap::Wrap(CStringArray& sReturn, LPCTSTR pText, int nWidth, LPCTSTR pBreakChars)
	{
	// dont do anything if we dont have to
	if(pText == NULL || nWidth < 1 || *pText == 0)
		return false;

	CString sText(pText);

	if(sText.GetLength() < nWidth)
		return false;

	CString sBreak;
	if(pBreakChars == NULL)
		sBreak = " \t";
	else
		sBreak = pBreakChars;

	int nBegin = 0;
	int nEnd = nWidth;

	if(sText[nEnd] == 0)
		return false;

	for(;;)
		{
		// find a place to break
		while(sBreak.Find(sText[nEnd]) == -1 && nEnd > nBegin)
			nEnd--;
		// no place to break nicely so force a break
		if(nBegin == nEnd)
			nEnd = nBegin + nWidth;
		// copy the string
		sReturn.Add(sText.Mid(nBegin, nEnd - nBegin));
		// advance indexes
		nBegin = nEnd;
		nEnd += nWidth;
		// are we at the end?
		if(nEnd >= sText.GetLength())
			{
			if(nBegin < sText.GetLength())
				sReturn.Add(sText.Mid(nBegin));
			break;
			}
		}

	return true;
	}

