//--------------------------------------------------------------------------------
// Copyright (c) 2001 @COMPANY, Inc.
// Created...: 6/9/01
// Author....: Rich Schonthal
//--------------------------------------------------------------------------------

#ifndef _FILENOTIFYINFORMATION_H_
#define _FILENOTIFYINFORMATION_H_
 
//--------------------------------------------------------------------------------
class CFileNotifyInformation
	{
	public:
		DWORD m_nAction;
		DWORD m_nFileNameLength;
#ifdef _UNICODE
		WCHAR* m_pFilename;
#else
		CHAR* m_pFilename;
#endif

	public:
		CFileNotifyInformation(const FILE_NOTIFY_INFORMATION* = NULL);
		CFileNotifyInformation(DWORD nAction, LPCTSTR pFilename);
		CFileNotifyInformation(const FILE_NOTIFY_INFORMATION&);
		~CFileNotifyInformation();

		operator=(const FILE_NOTIFY_INFORMATION&);
		operator=(const FILE_NOTIFY_INFORMATION*);

#ifdef _DEBUG
		LPCTSTR GetActionName();
#endif
	};

#endif //_FILENOTIFYINFORMATION_H_
