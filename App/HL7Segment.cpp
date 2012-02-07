//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "HL7Message.h"
#include "HL7Segment.h"

#include <AllocString.h>

//--------------------------------------------------------------------------------
CHL7Segment::CHL7Segment(int nId, int nGroup)
		: m_pArray(NULL)
		, m_nGroup(nGroup)
		, m_nId(nId)
	{
	}

//--------------------------------------------------------------------------------
CHL7Segment::CHL7Segment(CHL7SegmentArray* pArray, LPCTSTR pSeg)
		: m_pArray(pArray)
		, m_nGroup(0)
		, m_nId(0)
	{
	if(pSeg != NULL)
		Parse(pSeg);
	}

//--------------------------------------------------------------------------------
CHL7Segment::~CHL7Segment()
	{
	RemoveAllFields();
	}

//--------------------------------------------------------------------------------
CHL7Segment::eSegmentCode CHL7Segment::Parse(LPCTSTR pSegData, bool bRejectMSH)
	{
	RemoveAllFields();

	// skip any junk before start of msg
	while(*pSegData && *pSegData <= ' ')
		pSegData++;

	if(! *pSegData)
		return SEGMENT_BLANK;

	// kludge time
	// it seems the field delimiter may be changable so
	// we have to determine it by the first delim char
	// in the MSH record
	// i did notice somewhere in the spec that it mentioned
	// something about supporting messages without MSH records
	// so how you're supposed to know what the delim char is in
	// that case is beyond me... but this app doesnt currently
	// support msgs without MSH records anyway so...
	bool bIsMSH = strncmp(pSegData, "MSH", 3) == 0;
	if(bIsMSH)
		{
		m_pArray->m_pMsg->SetFieldDelim(pSegData[3]);
		m_pArray->m_pMsg->SetEncodingChars(&pSegData[4]);
		}

	char cDelim = m_pArray->m_pMsg->GetFieldDelim();
	char cEsc = m_pArray->m_pMsg->GetEscapeChar();

	CHL7Segment::eSegmentCode nRv = SEGMENT_ERROR;

	bool bDone = false;

	for(int i = 0; !bDone && *pSegData; i++)
		{
		// copy all chars up to but not including the delim char
		for(CString sTemp; *pSegData && *pSegData != cDelim; pSegData++)
			{
			// special case
			// when a char in the string is the same as the field delim (usually |)
			// it must be preceeded by the escape char (usually backslash)
			// but if this is the MSH segment then don't do escape processing until we've
			// passed the delim char field (the second field)
			if( (! bIsMSH || (bIsMSH && i > 1)) && *pSegData == cEsc)
				if(*(pSegData+1) == cDelim)
					{
					sTemp += cDelim;
					pSegData++;
					continue;
					}

			sTemp += *pSegData;
			}

		// no more chars - we're done
		if(! *pSegData)
			bDone = true;

		// skip the delim
		pSegData++;

		// NOTE - we dont do any trimming since
		// the msgs may require spaces for text data

		// check to make sure this is a valid segment
		if(i == 0)
			{
			if(sTemp.GetLength() != 3)
				return SEGMENT_ERROR;

			if(sTemp.Left(3) == "MSH" && bRejectMSH)
				return SEGMENT_IS_MSH;
			
			nRv = SEGMENT_OK;
			}

		if(nRv == SEGMENT_OK)
			AddField(sTemp);
		}

	return nRv;
	}

//--------------------------------------------------------------------------------
LPCTSTR CHL7Segment::GetField(int nFieldNo) const
	{
	if(nFieldNo >= 0 && nFieldNo < m_saData.GetSize())
		return (LPCTSTR) m_saData[nFieldNo];

	return "";
	}

//--------------------------------------------------------------------------------
void CHL7Segment::SetField(int nIndex, LPCTSTR pStr)
	{
	if(pStr == NULL)
		pStr = "";

	LPTSTR pTemp = new char[strlen(pStr)+1];
	strcpy(pTemp, pStr);
	m_saData.SetAtGrow(nIndex, pTemp);
	}

//--------------------------------------------------------------------------------
void CHL7Segment::AddField(LPCTSTR pStr)
	{
	if(pStr == NULL)
		pStr = "";

	LPTSTR pTemp = new char[strlen(pStr)+1];
	strcpy(pTemp, pStr);
	m_saData.Add(pTemp);
	}

//--------------------------------------------------------------------------------
void CHL7Segment::RemoveAllFields()
	{
	for(int i = 0; i < m_saData.GetSize(); i++)
		delete (LPTSTR) m_saData[i];

	m_saData.RemoveAll();
	}

//--------------------------------------------------------------------------------
void CHL7Segment::RemoveField(int nIndex)
	{
	if(nIndex < 0 || nIndex >= m_saData.GetSize())
		return;
	delete (LPTSTR) m_saData[nIndex];
	m_saData.RemoveAt(nIndex);
	}

//--------------------------------------------------------------------------------
void CHL7Segment::BuildString(CString& sOut)
	{
	if(m_pArray == NULL || m_pArray->m_pMsg == NULL)
		return;

	// This function can not be executed for a segment not connected to a message.
	// because a delim is needed.
	char cFieldDelim = m_pArray->m_pMsg->GetFieldDelim();
	long nCount = m_saData.GetSize();

	if(nCount > 0)
		for(int i = 0; i < nCount; i++)
			{
			sOut += (LPCTSTR) m_saData[i];
			sOut += cFieldDelim;
			}
	}

//--------------------------------------------------------------------------------
CHL7Message* CHL7Segment::GetMsgPtr() const
	{
	ASSERT(m_pArray != NULL);
	ASSERT(m_pArray->m_pMsg != NULL);

	return m_pArray->m_pMsg;
	}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
void CHL7SegmentArray::GetGroupSortArray(CDWordArray& nArray)
	{
	nArray.RemoveAll();
	int nSize = GetSize();
	if(nSize < 1)
		return;

	nArray.SetSize(nSize);

	//default is already sorted
	for(int i = 0; i < nSize; i++)
		nArray[i] = i;

	for(i = 1; i < nSize; i++)
		{
		int k = i;
		while(k > 0 && (GetAt(k)->GetGroup() < GetAt(k - 1)->GetGroup()) )
			{
			int n = nArray[k - 1];
			nArray[k - 1] = nArray[k];
			nArray[k] = n;
			k--;
			}
		}
	}

//--------------------------------------------------------------------------------
CHL7Segment* CHL7SegmentArray::FindByGroup(int nGroup) const
	{
	for(int i = GetUpperBound(); i >= 0; i--)
		{
		CHL7Segment* pCur = GetAt(i);
		if(pCur->GetGroup() == nGroup)
			return pCur;
		}

	return NULL;
	}
