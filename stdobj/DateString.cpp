//--------------------------------------------------------------------------------
//
// Copyright (c) 2001 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "DateString.h"
#include "DebugPrintf.h"

//--------------------------------------------------------------------------------
CDateString::CDateString(LPCTSTR pString)
		: m_nMonth(0)
		, m_nDay(0)
		, m_nYear(0)
	{
	if(pString == NULL || *pString == 0)
		{
		m_pString = NULL;
		return;
		}

	m_pString = new char[strlen(pString) + 1];
	strcpy(m_pString, pString);
	ParseString(m_pString);
	}

//--------------------------------------------------------------------------------
CDateString::~CDateString()
	{
	delete[] m_pString;
	}

//--------------------------------------------------------------------------------
void CDateString::ParseString(LPCTSTR pString)
	{
	if(pString == NULL)
		pString = m_pString;

	m_bParsed = ParseString(pString, m_nMonth, m_nDay, m_nYear);
	}


//--------------------------------------------------------------------------------
bool CDateString::ParseString(LPCTSTR pString, int& nMon, int& nDay, int& nYear, int nDateOrder, LPCTSTR pSeperators)
	{
	char sTemp[256];

	// determine the date order 0 Month-Day-Year, 1 Day-Month-Year, 2 Year-Month-Day 
	int nLen;
	if(nDateOrder == -1)
		{
		nLen = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_IDATE, sTemp, 256);
		if(nLen == 0)
			{
			DebugPrintf("ERROR from GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_IDATE, sTemp, 256)\n");
			return false;
			}

		sTemp[nLen] = 0;
		nDateOrder = atoi(sTemp);
		if(nDateOrder < 0 || nDateOrder > 2)
			{
			DebugPrintf("ERROR \n");
			return false;
			}
		}

	if(pSeperators == NULL)
		{
		// determine the seperator character(s) used in the date
		nLen = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDATE, sTemp, 256);
		if(nLen == 0)
			{
			DebugPrintf("ERROR from GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_IDATE, sTemp, 256)\n");
			return false;
			}

		sTemp[nLen] = 0;
		pSeperators = sTemp;
		}

	int* pElements[3];
	switch(nDateOrder)
		{
		//0 Month-Day-Year, 1 Day-Month-Year, 2 Year-Month-Day 
		case 0:
			pElements[0] = &nMon;
			pElements[1] = &nDay;
			pElements[2] = &nYear;
			break;
		case 1:
			pElements[0] = &nDay;
			pElements[1] = &nMon;
			pElements[2] = &nYear;
			break;
		case 2:
			pElements[0] = &nYear;
			pElements[1] = &nMon;
			pElements[2] = &nDay;
			break;

		}

	size_t nIndex = 0;
	for(int i = 0; i < 3; i++)
		{
		if(*pString == 0)
			break;
		// locate the first seperator char
		nIndex = strcspn(pString, pSeperators);
		if(nIndex < 0 || nIndex > 7)
			return false;
		char sNum[8];
		strncpy(sNum, pString, nIndex);
		sNum[nIndex] = 0;
		*pElements[i] = atoi(sNum);
		pString += (nIndex + 1);
		}

	return i == 3;
	}

//--------------------------------------------------------------------------------
int CDateString::Compare(const CDateString& rval)
	{
	return 0;
	}

