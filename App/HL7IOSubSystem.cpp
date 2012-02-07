//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "HL7IOSubSystem.h"
#include "HL7IOThread.h"
#include "HL7System.h"
#include <IOThread.h>

//--------------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CHL7IOSubSystem, CIOSubSystem);

//--------------------------------------------------------------------------------
LPCTSTR CHL7IOSubSystem::m_pDefaultName = "<main>";

//--------------------------------------------------------------------------------
CHL7IOSubSystem::CHL7IOSubSystem(CHL7System* pSystem)
		: CIOSubSystem(pSystem)
	{
	for(int i = 0; i < 2; i++)
		AddThread(new CHL7IOThread(this));
	}

//--------------------------------------------------------------------------------
CHL7IOSubSystem::~CHL7IOSubSystem()
	{
	for(POSITION pos = m_threadNames.GetStartPosition(); pos != NULL;)
		{
		LPTSTR pText;
		DWORD nKey;
		m_threadNames.GetNextAssoc(pos, nKey, pText);
		delete[] pText;
		}	
	}

//--------------------------------------------------------------------------------
void CHL7IOSubSystem::SetTraceLevel(DWORD nFlags)
	{
	nFlags |= IOMASK_DEBUG;
	CIOSubSystem::SetTraceLevel(nFlags);
	}

//--------------------------------------------------------------------------------
void CHL7IOSubSystem::WriteLogFileHeader()
	{
	if(m_pLogFile == NULL)
		return;

	CString sLine('-', 160);
	m_pLogFile->WriteString(sLine + "\r\n");
	m_pLogFile->WriteString("@COMPANY, Inc. HL7 Log File\r\nStarted: ");
	m_pLogFile->WriteString(CTime::GetCurrentTime().Format("%c"));
	m_pLogFile->WriteString("\r\n");
	m_pLogFile->WriteString(sLine + "\r\n");
	}

//--------------------------------------------------------------------------------
bool CHL7IOSubSystem::OutputToFile(LPCTSTR pText, CStdioFile* pFile)
	{
	if(pText == NULL || pFile == NULL)
		return false;

	CString sTemp;
	sTemp.Format("%s\t%s", (LPCTSTR) CTime::GetCurrentTime().Format("%c"), pText);

	if(! CIOSubSystem::OutputToFile(sTemp, pFile))
		return false;

	pFile->Flush();
	return true;
	}

//--------------------------------------------------------------------------------
void CHL7IOSubSystem::AddThreadName(LPCTSTR pData)
	{
	LPTSTR pName = NULL;
	ALLOC_STRING(pName, pData);
	m_threadNames.SetAt(::GetCurrentThreadId(), pName);
	}

//--------------------------------------------------------------------------------
LPCTSTR CHL7IOSubSystem::GetThreadName()
	{
	LPTSTR pName;
	if(! m_threadNames.Lookup(::GetCurrentThreadId(), pName))
		return m_pDefaultName;
	else
		return pName;
	}

//--------------------------------------------------------------------------------
bool CHL7IOSubSystem::Output(DWORD nFlags, LPCTSTR pText)
	{
	if(! IsDisplayed(nFlags))
		return true;

	CString sTemp;
	sTemp.Format("%s\t%s", GetThreadName(), pText);
	LPTSTR pNewText = NULL;
	ALLOC_STRING(pNewText, sTemp);

	return CIOSubSystem::Output(nFlags & ~IOMASK_CONST, (LPTSTR) pNewText);
	}

//--------------------------------------------------------------------------------
bool CHL7IOSubSystem::Output(DWORD nFlags, LPTSTR pText)
	{
	if(! IsDisplayed(nFlags))
		return true;

	bool bRv = CHL7IOSubSystem::Output(nFlags, (LPCTSTR) pText);

	if((nFlags & IOMASK_CONST) == 0)
		delete[] pText;
	return bRv;
	}

//--------------------------------------------------------------------------------
bool CHL7IOSubSystem::FormatOutput(DWORD nFlags, LPCTSTR pFormat, ...)
	{
	if(! IsDisplayed(nFlags))
		return true;

	va_list marker;

	va_start(marker, pFormat);
	char* pTemp = new char[4096];

	vsprintf(pTemp, pFormat, marker);

	return CHL7IOSubSystem::Output(nFlags & ~IOMASK_CONST, pTemp);
	}
