//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "HL7Segment.h"
#include "HL7MessageConfig.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//--------------------------------------------------------------------------------
CHL7MessageConfig::CHL7MessageConfig(const CHL7Segment* pSeg)
	{
	// set default values
	m_cSepComponent = '^';
	m_cSepSubComponent = '&';
	m_cSepRepetition = '~';
	m_cEscape = '\\';

	if(pSeg != NULL)
		SetConfiguration(pSeg);
	}

//--------------------------------------------------------------------------------
void CHL7MessageConfig::SetConfiguration(const CHL7Segment* pSeg)
	{
	CString sData((LPCTSTR) pSeg->m_saData[0]);

	if(sData.GetLength() != 4)
		return;

	// make sure we have 4 unique values
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
			if(i != j)
				if(sData[i] == sData[j])
					return;

	m_cSepComponent = sData[0];
	m_cSepRepetition = sData[1];
	m_cEscape = sData[2];
	m_cSepSubComponent = sData[3];
	}

//--------------------------------------------------------------------------------
void CHL7MessageConfig::SerializeConfiguration(CArchive& arch)
	{
	if(arch.IsLoading())
		{
		//arch >> data
		arch.Read(&m_cSepComponent, sizeof(char));
		arch.Read(&m_cSepRepetition, sizeof(char));
		arch.Read(&m_cEscape, sizeof(char));
		arch.Read(&m_cSepSubComponent, sizeof(char));
		}
	else
		{
		// arch << data
		arch.Write(&m_cSepComponent, sizeof(char));
		arch.Write(&m_cSepRepetition, sizeof(char));
		arch.Write(&m_cEscape, sizeof(char));
		arch.Write(&m_cSepSubComponent, sizeof(char));
		}
	}
