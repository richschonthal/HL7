//--------------------------------------------------------------------------------
// Copyright (c) 2001 @COMPANY, Inc.
// Created...: 6/8/01
// Author....: Rich Schonthal
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "ChangeNotificationContextObj.h"
#include "ChangeNotificationObj.h"

#include <SystemObject.h>
#include <Directory.h>
#include <AllocString.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//--------------------------------------------------------------------------------
CFileChangeNotificationContextObj::CFileChangeNotificationContextObj()
		: m_pPath(NULL)
		, m_hCompletionPort(INVALID_HANDLE_VALUE)
		, m_nCurBufIndex(0)
		, m_pPrevRenameObj(NULL)
	{
	}

//--------------------------------------------------------------------------------
bool CFileChangeNotificationContextObj::Create(LPCTSTR pPath, bool bWatchSubDir, DWORD nFilter, CSystemObject* pTarget, int nMsgId)
	{
	if(m_pReadDirectoryChangesW == NULL)
		return false;

	CDirectory::CreatePath(pPath);
	m_hDir = ::CreateFile(pPath,
		  FILE_LIST_DIRECTORY,                // access (read-write) mode
		  FILE_SHARE_READ|FILE_SHARE_WRITE,//|FILE_SHARE_DELETE,  // share mode
		  NULL,                               // security descriptor
		  OPEN_EXISTING,                      // how to create
		  FILE_FLAG_BACKUP_SEMANTICS|FILE_FLAG_OVERLAPPED ,         // file attributes
		  NULL                                // file with attributes to copy
		  );

	if(! m_hDir.IsValid())
		return false;

	m_pPath = pPath;
	m_bWatchSubDir = bWatchSubDir;
	if(nFilter == 0xffffffff)
		nFilter = FILE_NOTIFY_CHANGE_FILE_NAME|FILE_NOTIFY_CHANGE_DIR_NAME|FILE_NOTIFY_CHANGE_ATTRIBUTES|
			FILE_NOTIFY_CHANGE_SIZE|FILE_NOTIFY_CHANGE_LAST_WRITE|FILE_NOTIFY_CHANGE_LAST_ACCESS|
			FILE_NOTIFY_CHANGE_CREATION|FILE_NOTIFY_CHANGE_SECURITY;
	m_nFilter = nFilter;
	m_pTarget = pTarget;
	m_nMessageId = nMsgId;

	memset(&m_overlapped, 0, sizeof(m_overlapped));
	m_overlapped.hEvent = m_event;
	m_event.ResetEvent();

	m_hCompletionPort = ::CreateIoCompletionPort(m_hDir, NULL, (DWORD) m_pBuffer, 0);

	DWORD nSize;
	nSize = GetLastError();
	(m_pReadDirectoryChangesW)(m_hDir, m_pBuffer, BUFFER_SIZE, m_bWatchSubDir, m_nFilter, &nSize, &m_overlapped, NULL);
	nSize = GetLastError();
	return true;
	}

//--------------------------------------------------------------------------------
CFileChangeNotificationContextObj::~CFileChangeNotificationContextObj()
	{
	delete m_pPrevRenameObj;
	}

//--------------------------------------------------------------------------------
LPCTSTR CFileChangeNotificationContextObj::GetPath() const
	{
	return m_pPath;
	}

//--------------------------------------------------------------------------------
bool CFileChangeNotificationContextObj::IsValid()
	{
	return m_hCompletionPort.IsValid();
	}

//--------------------------------------------------------------------------------
bool CFileChangeNotificationContextObj::IsChangeReady()
	{
	return ::GetQueuedCompletionStatus(m_hCompletionPort, &m_nSize, (DWORD*) &m_pBase, &m_pOverlap, 100) != 0;
	}

//--------------------------------------------------------------------------------
bool CFileChangeNotificationContextObj::PostChanges()
	{
	DWORD nOffset = 0;
	for(;;)
		{
		FILE_NOTIFY_INFORMATION* pOrigInfo = (FILE_NOTIFY_INFORMATION*) (m_pBase + nOffset);
		CFileChangeNotificationObj* pInfo = new CFileChangeNotificationObj(pOrigInfo, GetPath());
		nOffset += pOrigInfo->NextEntryOffset;

		if(pInfo->m_nAction == FILE_ACTION_RENAMED_OLD_NAME)
			{
			delete m_pPrevRenameObj;
			m_pPrevRenameObj = pInfo;
			}
		else
			{
			if(pInfo->m_nAction == FILE_ACTION_RENAMED_NEW_NAME)
				{
				if(m_pPrevRenameObj == NULL)
					{
					delete pInfo;
					return false;
					}
				pInfo->m_pOldFilename = AllocString(m_pPrevRenameObj->m_pFilename);
				delete m_pPrevRenameObj;
				m_pPrevRenameObj = NULL;
				}

			m_pTarget->PostMessage(GetMessageId(), (WPARAM) pInfo);
			}

		if(pOrigInfo->NextEntryOffset == 0)
			return true;
		}
	}

//--------------------------------------------------------------------------------
bool CFileChangeNotificationContextObj::GetNextChange()
	{
	m_event.ResetEvent();
	(m_pReadDirectoryChangesW)(m_hDir, m_pBuffer, BUFFER_BLOCK_SIZE, m_bWatchSubDir, m_nFilter, &m_nSize, &m_overlapped, NULL);
	return true;
	}
