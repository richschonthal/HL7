//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "IOSubSystem.h"
#include "IOThread.h"

#include <QuickMail.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//--------------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CIOSubSystem, CThreadPoolSubSystem);

//--------------------------------------------------------------------------------
#ifdef _DEBUG
DWORD CIOSubSystem::m_nTraceLevel = IOMASK_DEBUG;
#else
DWORD CIOSubSystem::m_nTraceLevel = 0;
#endif
CStdioFile* CIOSubSystem::m_pLogFile = NULL;
CNTService* CIOSubSystem::m_pService = NULL;
CMutex CIOSubSystem::m_mutexLogFile;

//--------------------------------------------------------------------------------
CIOSubSystem::CIOSubSystem(CSystem* pParent)
		: CThreadPoolSubSystem(pParent)
	{
	m_pLogFile = NULL;
	}

//--------------------------------------------------------------------------------
bool CIOSubSystem::Init()
	{
	SYSTEM_INFO info;

	::GetSystemInfo(&info);
	for(UINT i = 0; i < info.dwNumberOfProcessors; i++)
		AddThread(CreateThread());

	return true;
	}

//--------------------------------------------------------------------------------
CIOSubSystem::~CIOSubSystem()
	{
	delete m_pLogFile;
	}

//--------------------------------------------------------------------------------
CMutex& CIOSubSystem::GetLogFileLock() const
	{
	return m_mutexLogFile;
	}

//--------------------------------------------------------------------------------
CStdioFile* CIOSubSystem::GetLogFilePtr()
	{
	return m_pLogFile;
	}

//--------------------------------------------------------------------------------
void CIOSubSystem::SetServicePtr(CNTService* pService)
	{
	m_pService = pService;
	}

//--------------------------------------------------------------------------------
CNTService* CIOSubSystem::GetServicePtr() const
	{
	return m_pService;
	}

//--------------------------------------------------------------------------------
void CIOSubSystem::SetTraceLevel(DWORD nLevel)
	{
	m_nTraceLevel = nLevel;
	}

//--------------------------------------------------------------------------------
DWORD CIOSubSystem::GetTraceLevel() const
	{
	return m_nTraceLevel;
	}

//--------------------------------------------------------------------------------
bool CIOSubSystem::IsErrorMsg(DWORD nLevel)
	{
	return (nLevel & IOMASK_ERR) == IOMASK_ERR;
	} 

//--------------------------------------------------------------------------------
bool CIOSubSystem::IsLogMsg(DWORD nLevel)
	{
	return (nLevel & IOMASK_LOG) == IOMASK_LOG;
	}

//--------------------------------------------------------------------------------
bool CIOSubSystem::IsDisplayed(DWORD nLevel) const
	{
	if(m_nTraceLevel == IOMASK_OFF)
		return false;

	if( (nLevel & IOMASK_ALWAYSDISPLAYMASK) != 0)
		return true;

	return (nLevel & m_nTraceLevel) != 0;
	}

//--------------------------------------------------------------------------------
bool CIOSubSystem::CloseLogFile()
	{
	CSingleLock lock(&m_mutexLogFile, false);

	if(! lock.Lock(10000))
		return false;

	delete m_pLogFile;
	m_pLogFile = NULL;
	return true;
	}

//--------------------------------------------------------------------------------
bool CIOSubSystem::CreateLogFile(LPCTSTR pFilename,  bool bTruncate)
	{
	if(pFilename == NULL)
		return false;

	CSingleLock lock(&m_mutexLogFile, false);

	if(! lock.Lock(10000))
		return false;

	delete m_pLogFile;
	m_pLogFile = new CStdioFile;

	CFileException exception;
	UINT nFlags = CFile::modeCreate|CFile::modeReadWrite|(bTruncate?0:CFile::modeNoTruncate)|CFile::shareDenyWrite;

	if(! m_pLogFile->Open(pFilename, nFlags, &exception))
		{
		SetResultCode(CResult::errOS);
		SetOSResult(exception.m_lOsError);
		return false;
		}

	if(! bTruncate)
		m_pLogFile->SeekToEnd();

	WriteLogFileHeader();
	return true;
	}

//--------------------------------------------------------------------------------
void CIOSubSystem::WriteLogFileHeader()
	{
	}

//--------------------------------------------------------------------------------
bool CIOSubSystem::Output(DWORD nFlags, LPCTSTR pText)
	{
	bool bRv = false;
	TCHAR* pTextCopy = NULL;

	if(IsDisplayed(nFlags))
		{
		if((nFlags & IOMASK_CONST) == IOMASK_CONST)
			return PostNextThreadMessage(MSG_OUTPUT_CONST, (WPARAM) pText, 0);
		else
			{
			// we must make a copy of the text because, even though its passed as const
			// the original may be deleted before the IO thread is thru with it
			int nLen = _tcslen(pText);
			pTextCopy = new TCHAR[nLen + 1];
			memcpy(pTextCopy, pText, (nLen + 1) * sizeof(TCHAR));
			bRv = PostNextThreadMessage(MSG_OUTPUT, (WPARAM) pTextCopy, 0);
			}
		}

	if(! bRv)
		delete[] pTextCopy;

	return bRv;
	}

//--------------------------------------------------------------------------------
bool CIOSubSystem::Output(DWORD nFlags, LPTSTR pText)
	{
	bool bRv = false;
	if(IsDisplayed(nFlags))
		{
		// non-const - pText will be deleted by pThread
		if((nFlags & IOMASK_CONST) == IOMASK_CONST)
			return PostNextThreadMessage(MSG_OUTPUT_CONST, (WPARAM) pText, 0);
		else
			bRv = PostNextThreadMessage(MSG_OUTPUT, (WPARAM) pText, 0);
		}

	if(! bRv)
		delete pText;

	return bRv;
	}

//--------------------------------------------------------------------------------
bool CIOSubSystem::FormatOutput(DWORD nFlags, LPCTSTR pFormat, ...)
	{
	if(! IsDisplayed(nFlags))
		return true;

	va_list marker;

	va_start(marker, pFormat);
	TCHAR* pTemp = new TCHAR[4096];

	_vstprintf(pTemp, pFormat, marker);
	PostNextThreadMessage(MSG_OUTPUT, (WPARAM) pTemp, 0);
	return true;
	}

//--------------------------------------------------------------------------------
bool CIOSubSystem::Debug(LPCTSTR pText)
	{
	return Output(IOMASK_DEBUG, pText);
	}

//--------------------------------------------------------------------------------
bool CIOSubSystem::Debug(LPTSTR pText)
	{
	return Output(IOMASK_DEBUG, pText);
	}

//--------------------------------------------------------------------------------
bool CIOSubSystem::DebugPrintf(LPCTSTR pFormat, ...)
	{
	if(! IsDisplayed(IOMASK_DEBUG))
		return true;

	va_list marker;

	va_start(marker, pFormat);
	TCHAR* pTemp = new TCHAR[4096];

	_vstprintf(pTemp, pFormat, marker);

	PostNextThreadMessage(MSG_OUTPUT, (WPARAM) pTemp, 0);
	return true;
	}

//--------------------------------------------------------------------------------
bool CIOSubSystem::OutputToDebugger(LPCTSTR pText)
	{
	OutputDebugString(pText);
	OutputDebugString(_T("\r\n"));
	return true;
	}

//--------------------------------------------------------------------------------
bool CIOSubSystem::OutputToFile(LPCTSTR pText, CStdioFile* pFile)
	{
	if(pFile == NULL || pText == NULL)
		return false;

	pFile->WriteString(pText);
	pFile->WriteString(_T("\r\n"));

	return true;
	}

//--------------------------------------------------------------------------------
bool CIOSubSystem::SendQuickMail(CQuickMail* pMail)
	{
	bool bRv = PostNextThreadMessage(MSG_QUICKMAIL, (WPARAM) pMail);
	if(! bRv)
		delete pMail;
	return bRv;
	}

//--------------------------------------------------------------------------------
CThreadObject* CIOSubSystem::CreateThread()
	{
	return (CThreadObject*) new CIOThread(this);
	}

//--------------------------------------------------------------------------------
int CIOSubSystem::GetObjectId()
	{
	return OIDTM_SUBSYSTEM + SLOID_IO;
	}
