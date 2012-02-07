//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "HL7Segment.h"
#include "HL7Message.h"

#include "globals.h"

#include <DebugPrintf.h>

//--------------------------------------------------------------------------------
CHL7Message::CHL7Message() : m_nSourceId(0), m_nVersion(0), m_cFieldDelim(0)
	{
	m_pSegments = new CHL7SegmentArray(this);
	// set the default encoding characters
	int x = sizeof(m_cEncodingChars);
	memset(m_cEncodingChars, 0, sizeof(m_cEncodingChars));
	}

//--------------------------------------------------------------------------------
CHL7Message::~CHL7Message()
	{
	delete m_pSegments;
	}

//--------------------------------------------------------------------------------
CHL7Message::eMsgResult CHL7Message::AddSegment(LPCTSTR pMsg)
	{
	// allocate a CHL7Segment
	CHL7Segment* pSeg = new CHL7Segment(m_pSegments);

	ASSERT(pSeg != NULL);

	if(pSeg == NULL)
		DebugPrintf("pSeg NULL!!\n");

	// put each field into an element in an array
	bool bDontAddMSH = GetSegmentsPtr()->GetSize() > 1;
	CHL7Segment::eSegmentCode nCode = pSeg->Parse(pMsg, bDontAddMSH);

	CHL7Message::eMsgResult nRv = MSG_ERROR;

	switch(nCode)
		{
		// this will only be returned if bDontAddMSH is true
		case CHL7Segment::SEGMENT_IS_MSH:
			nRv = MSG_START_NEW;
			break;

		case CHL7Segment::SEGMENT_ERROR:
			nRv = MSG_ERROR;
			break;

		case CHL7Segment::SEGMENT_BLANK:
			nRv = MSG_BLANK;
			break;

		default:
			GetSegmentsPtr()->Add(pSeg);
			return MSG_OK;
		}

	// only come here if you havent added the segment
	delete pSeg;
	return nRv;
	}

//-----------------------------------------------------------------------
CHL7Message::eMsgResult CHL7Message::AddSegment(CHL7Segment* pNewSegment)
	{
	if(pNewSegment == NULL)
		return MSG_ERROR;
	m_pSegments->Add(pNewSegment);
	pNewSegment->m_pArray = m_pSegments;
	return MSG_OK;
	}

//--------------------------------------------------------------------------------
CHL7Segment* CHL7Message::GetSegment(int nIndex) const
	{
	return GetSegmentsPtr()->GetAt(nIndex);
	}

//--------------------------------------------------------------------------------
int CHL7Message::GetSegmentCount() const
	{
	return GetSegmentsPtr()->GetSize();
	}

//--------------------------------------------------------------------------------
void CHL7Message::SetEncodingChars(LPCTSTR pChars)
	{
	if(pChars == NULL)
		return;

	// make sure the str len is at least 4 chars
	// no need to get an entire strlen cuz the string may be pretty long
	LPCTSTR p = pChars;
	for(int i = 0; i < 4; i++, p++)
		if(! *p)
			return;

	strncpy(m_cEncodingChars, pChars, 4);
	}

//--------------------------------------------------------------------------------
void CHL7Message::BuildString(CString& sOut)
	{
	sOut += '\x0b';
	long nUpper = m_pSegments->GetUpperBound();
	if(nUpper > -1)
		{
		CDWordArray nIndexArray;
		m_pSegments->GetGroupSortArray(nIndexArray);
		CString sTemp;
		for(int i = 0; i <= nUpper; i++)
			{
			m_pSegments->GetAt(nIndexArray[i])->BuildString(sTemp);
			if(! sTemp.IsEmpty())
				{
				sOut += sTemp;
				if(i < nUpper)
					sOut += '\n';
				}
			}
		}

	sOut += '\x0d';
	sOut += '\x1c';
	sOut += '\x0d';
	}

//--------------------------------------------------------------------------------
void CHL7Message::TraceDump()
	{
	if(! GetIO()->IsDisplayed(IOMASK_9))
		return;
	for(int i = 0; i < GetSegmentCount(); i++)
		GetIO()->Output(IOMASK_9|IOMASK_CONST, (CString) *GetSegment(i));
	}
