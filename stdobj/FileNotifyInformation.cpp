//--------------------------------------------------------------------------------
// Copyright (c) 2001 @COMPANY, Inc.
// Created...: 6/9/01
// Author....: Rich Schonthal
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "FileNotifyInformation.h"

//--------------------------------------------------------------------------------
CFileNotifyInformation::CFileNotifyInformation(const FILE_NOTIFY_INFORMATION* pInfo)
	{
	if(pInfo != NULL)
		*this = *pInfo;
	else
		{
		m_nAction = m_nFileNameLength = 0;
		m_pFilename = NULL;
		}
	}

//--------------------------------------------------------------------------------
CFileNotifyInformation::CFileNotifyInformation(const FILE_NOTIFY_INFORMATION& info)
	{
	*this = info;
	}

//--------------------------------------------------------------------------------
CFileNotifyInformation::CFileNotifyInformation(DWORD nAction, LPCTSTR pFilename)
		: m_nAction(nAction)
	{
	m_nFileNameLength = strlen(pFilename);
	m_pFilename = new char[m_nFileNameLength];
	memcpy(m_pFilename, pFilename, m_nFileNameLength);
	}

//--------------------------------------------------------------------------------
CFileNotifyInformation::~CFileNotifyInformation()
	{
	delete[] m_pFilename;
	}

//--------------------------------------------------------------------------------
CFileNotifyInformation::operator=(const FILE_NOTIFY_INFORMATION& info)
	{
    m_nAction = info.Action;

#ifdef _UNICODE
    m_nFileNameLength = info.FileNameLength;
	m_pFilename = new WCHAR[m_nFileNameLength];
	memcpy(m_pFilename, info.FileName, sizeof(WCHAR) * m_nFileNameLength);
#else
    m_nFileNameLength = info.FileNameLength / sizeof(WCHAR);
	m_pFilename = new CHAR[m_nFileNameLength + 1];
	LPCTSTR pBase = (LPCTSTR) info.FileName;
	for(DWORD i = 0; i < m_nFileNameLength; i++, pBase += 2)
		m_pFilename[i] = *pBase;
	m_pFilename[m_nFileNameLength] = 0;
#endif
	}

//--------------------------------------------------------------------------------
CFileNotifyInformation::operator=(const FILE_NOTIFY_INFORMATION* pInfo)
	{
	*this = *pInfo;
	}

#ifdef _DEBUG
/*
#define FILE_ACTION_ADDED                   0x00000001   
#define FILE_ACTION_REMOVED                 0x00000002   
#define FILE_ACTION_MODIFIED                0x00000003   
#define FILE_ACTION_RENAMED_OLD_NAME        0x00000004   
#define FILE_ACTION_RENAMED_NEW_NAME        0x00000005   
*/

LPCTSTR g_pFileActions[] =
	{
	"FILE_ACTION_ADDED",
	"FILE_ACTION_REMOVED",
	"FILE_ACTION_MODIFIED",
	"FILE_ACTION_RENAMED_OLD_NAME",
	"FILE_ACTION_RENAMED_NEW_NAME"
	};

//--------------------------------------------------------------------------------
LPCTSTR CFileNotifyInformation::GetActionName()
	{
	int i = (m_nAction & 0x0f) - 1;
	if(i < 0 || i > 4)
		return "";
	return g_pFileActions[i];
	}

#endif //_DEBUG
