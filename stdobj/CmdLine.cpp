//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

/*
m_cmdIndex is the index where the "commands" are in the m_params array

ie m_params contains:
[0] /param1
[1] arg1
[2] arg2
[3] /param2
[4] arg1
[5] arg2

m_cmdIndex will contain:
[0] 0
[1] 3

commands are any strings that start with any char contained in m_pTokens

*/


// CmdLine.cpp: implementation of the CCmdLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CmdLine.h"

//--------------------------------------------------------------------------------
HANDLE CCmdLine::m_hMapFile = NULL;
LPVOID CCmdLine::m_pMapView = NULL;

// the data these point to will be stored in the memory mapped file
LPTSTR CCmdLine::m_pCmdLine = NULL;
LPTSTR CCmdLine::m_pTokens = NULL;
LPWORD CCmdLine::m_pRefCount = NULL;

// these get inited all the time so they can be regular statics
CString CCmdLine::m_sExePath;
CStringArray CCmdLine::m_params;
CByteArray CCmdLine::m_cmdIndex;
	 
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

inline void CCmdLine::IncRefCount() { if(m_pRefCount) (*m_pRefCount)++; }
inline void CCmdLine::DecRefCount() { if(m_pRefCount) (*m_pRefCount)--; }
inline int CCmdLine::GetRefCount() { return m_pRefCount != NULL ? (*m_pRefCount) : 0; }

//--------------------------------------------------------------------------------
CCmdLine::CCmdLine(bool)
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(! AllocAll())
		return;

	IncRefCount();
	}

//--------------------------------------------------------------------------------
CCmdLine::CCmdLine(LPCTSTR pCmdLine, LPCTSTR pTokens)
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(! AllocAll())
		return;

	LPCTSTR p = pTokens == NULL ? _T("/,-") : pTokens;

	if(_tcscmp(m_pTokens, p) != 0)
		{
		_tcsncpy(m_pTokens, p, 15);
		m_pTokens[15] = 0;
		}

	if(pCmdLine != NULL)
		_tcscpy(m_pCmdLine, pCmdLine);
	else
		if(! *m_pCmdLine)
			_tcscpy(m_pCmdLine, GetCommandLine());

	*this = m_pCmdLine;

	IncRefCount();
	}

//--------------------------------------------------------------------------------
CCmdLine::CCmdLine(int nArgc, LPCTSTR* pArgv, LPCTSTR pTokens)
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(! AllocAll())
		return;

	LPCTSTR p = pTokens == NULL ? _T("/,-") : pTokens;

	if(_tcscmp(m_pTokens, p) != 0)
		{
		_tcsncpy(m_pTokens, p, 15);
		m_pTokens[15] = 0;
		}

	CString sCmdLine;

	for(int i = 0; i < nArgc; i++)
		{
		sCmdLine += pArgv[i];
		if(i != nArgc-1)
			sCmdLine += ' ';
		}

	_tcscpy(m_pCmdLine, (LPCTSTR) sCmdLine);

	*this = m_pCmdLine;

	IncRefCount();
	}

//--------------------------------------------------------------------------------
bool CCmdLine::AllocAll()
	{
	// create or open the mapped memory file
	if(m_hMapFile == NULL)
		{
		CString sName;
		sName.Format(_T("cmdlinememfile%ld"), ::GetCurrentProcessId());

		m_hMapFile = ::CreateFileMapping( 
			(HANDLE) 0xFFFFFFFF,	// use paging file
			NULL,					// no security attributes
			PAGE_READWRITE,			// read/write access
			0,						// size: high 32-bits
			4096,					// size: low 32-bits
			sName);		// name of map object

		if(m_hMapFile == NULL)
			return false;

		bool bIsNew = (GetLastError() != ERROR_ALREADY_EXISTS);

		m_pMapView = ::MapViewOfFile( 
			m_hMapFile,     // object to map view of
			FILE_MAP_WRITE, // read/write access
			0,              // high offset:  map from
			0,              // low offset:   beginning
			0);             // default: map entire file

		if(m_pMapView == NULL)
			{
			::CloseHandle(m_hMapFile);
			m_hMapFile = NULL;
			return false;
			}

		if(bIsNew)
			::ZeroMemory(m_pMapView, 4096);
		}

	// map the pointers into the mapped mem file
	m_pRefCount = (LPWORD) m_pMapView;
	m_pTokens = (LPTSTR) ((TCHAR*) m_pMapView + sizeof(*m_pRefCount));
	m_pCmdLine = (LPTSTR) (m_pTokens + 16);

	return true;
	}

//--------------------------------------------------------------------------------
void CCmdLine::DeallocAll()
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(m_pMapView != NULL)
		::UnmapViewOfFile(m_pMapView);

	if(m_hMapFile != NULL)
		::CloseHandle(m_hMapFile);

	m_hMapFile = NULL;
	m_pMapView = NULL;
	m_pCmdLine = NULL;
	m_pTokens = NULL;
	m_pRefCount = NULL;
	}

//--------------------------------------------------------------------------------
CCmdLine::~CCmdLine()
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(! AllocAll())
		return;

	DecRefCount();
	if(GetRefCount() < 1)
		DeallocAll();
	}

//--------------------------------------------------------------------------------
CCmdLine::operator=(LPCTSTR pCmdLine)
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_sExePath.Empty();
	m_params.RemoveAll();

	LPCTSTR pParam = ParseParam(pCmdLine, m_sExePath);
	
	for(BYTE i = 0; pParam; i++)
		{
		CString sTemp;

		pParam = ParseParam(pParam, sTemp);
		m_params.Add(sTemp);
		// see if the 1st char matches any of the chars
		// in the token list - ie is it / , or -
		if(sTemp.Left(1).FindOneOf(m_pTokens) != -1)
			m_cmdIndex.Add(i);
		}
	}

//--------------------------------------------------------------------------------
int CCmdLine::GetParamCount()
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return m_params.GetSize();
	}

//--------------------------------------------------------------------------------
LPCTSTR CCmdLine::ParseParam(LPCTSTR pParam, CString& sReturn)
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	while(*pParam == ' ' && *pParam != 0)
		pParam++;

	if(*pParam == 0)
		return NULL;

	// if the path is quoted then we want to copy
	// up to the last quote
	// if not then we copy up to the first space
	if(*pParam == '"')
		{
		// skip the quote
		pParam++;
		while(*pParam && *pParam != '"')
			sReturn += *pParam++;

		// skip the last quote
		if(*pParam)
			pParam++;
		}
	else
		{
		while(*pParam && *pParam != ' ')
			{
			sReturn += *pParam++;
			if(_tcschr(m_pTokens, *pParam) != NULL)
				break;
			}
		}

	if(*pParam)
		pParam++;


	if(*pParam)
		return pParam;
	else
		return NULL;
	}

//--------------------------------------------------------------------------------
bool CCmdLine::HasParam(char cParam, int& nIndex)
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	for(nIndex = 0; nIndex < m_cmdIndex.GetSize(); nIndex++)
		{
		if(m_params[m_cmdIndex[nIndex]].GetLength() == 2)
			if(isalpha(cParam))
				{
				if(tolower(m_params[m_cmdIndex[nIndex]][1]) == tolower(cParam))
					return true;
				}
			else
				{
				if(m_params[m_cmdIndex[nIndex]][1] == cParam)
					return true;
				}
		}

	return false;
	}

//--------------------------------------------------------------------------------
bool CCmdLine::HasParam(LPCTSTR pParam, int& nIndex)
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString sTemp(pParam);

	for(nIndex = 0; nIndex < m_cmdIndex.GetSize(); nIndex++)
		{
		if(sTemp.CompareNoCase(m_params[m_cmdIndex[nIndex]]) == 0)
			return true;
		}

	return false;
	}

//--------------------------------------------------------------------------------
int CCmdLine::GetCmdIndex(int nCmd)
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(nCmd < 0 || nCmd > m_cmdIndex.GetUpperBound())
		return -1;

	return m_cmdIndex[nCmd];
	}

//--------------------------------------------------------------------------------
bool CCmdLine::GetArgs(CStringArray& saArgs)
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	saArgs.RemoveAll();
	saArgs.Append(m_params);
	return saArgs.GetSize() > 0;
	}

//--------------------------------------------------------------------------------
bool CCmdLine::GetArgs(CString& saArgs, char cSep)
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	saArgs.Empty();
	const int nSize = m_params.GetSize();
	for(int i = 0; i < nSize; i++)
		{
		if(m_params[i].Find(' ') != -1)
			saArgs += '"' + m_params[i] + '"';
		else
			saArgs += m_params[i];

		if(i < nSize-1)
			{
			saArgs += cSep;
			if(cSep != ' ')
				saArgs += ' ';
			}
		}

	return ! saArgs.IsEmpty();
	}

//--------------------------------------------------------------------------------
bool CCmdLine::GetArgs(int nCmd, CStringArray& saArgs)
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// get the index of the first arg after command nCmd
	int nLower = GetCmdIndex(nCmd);

	if(nLower == -1)
		return false;

	nLower += 1;

	if(nLower >= m_params.GetSize())
		return false;

	// get the index of the last arg before the next command
	int nUpper = GetCmdIndex(nCmd+1);

	if(nUpper == -1)
		nUpper = m_params.GetUpperBound();
	else
		{
		if(nLower == nUpper)
			return false;
		nUpper -= 1;
		}

	for(int i = nLower; i <= nUpper; i++)
		saArgs.Add(m_params[i]);

	return saArgs.GetSize() > 0;
	}

//--------------------------------------------------------------------------------
bool CCmdLine::GetArgs(int nCmd, CString& sArg)
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// get the index of the first arg after command nCmd
	int nLower = GetCmdIndex(nCmd);

	if(nLower == -1)
		return false;

	nLower += 1;

	if(nLower >= m_params.GetSize() || nLower == GetCmdIndex(nCmd+1))
		return false;

	sArg = m_params[nLower];

	return true;
	}

