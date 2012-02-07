//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// NamedPipeFile.cpp: implementation of the CNamedPipeFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#define NO_DLL
#include <NamedPipeFile.h>
#include <result.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CNamedPipeFile, CFile)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------
CNamedPipeFile::CNamedPipeFile() : m_evtComplete(false, true), m_bConnectPending(false)
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_hFile = (UINT) INVALID_HANDLE_VALUE;
	::ZeroMemory(&m_overlap, sizeof(m_overlap));
	}

//--------------------------------------------------------------------------------
CNamedPipeFile::CNamedPipeFile(LPCTSTR lpszFileName, UINT nOpenFlags)
		: m_evtComplete(false, true),
		m_bConnectPending(false)
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_hFile = (UINT) INVALID_HANDLE_VALUE;
	::ZeroMemory(&m_overlap, sizeof(m_overlap));
	}

//--------------------------------------------------------------------------------
CNamedPipeFile::~CNamedPipeFile()
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(IsOpen())
		Close();
	}

//--------------------------------------------------------------------------------
bool CNamedPipeFile::IsOpen()
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return (m_hFile != (UINT) INVALID_HANDLE_VALUE);
	}

//--------------------------------------------------------------------------------
DWORD CNamedPipeFile::GetPosition() const
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return 0;
	}

//--------------------------------------------------------------------------------
CString CNamedPipeFile::GetFileName() const
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return "";
	}

//--------------------------------------------------------------------------------
CString CNamedPipeFile::GetFileTitle() const
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return "";
	}

//--------------------------------------------------------------------------------
CString CNamedPipeFile::GetFilePath() const
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return "";
	}

//--------------------------------------------------------------------------------
void CNamedPipeFile::SetFilePath(LPCTSTR lpszNewName)
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	}

// Operations
//--------------------------------------------------------------------------------
BOOL CNamedPipeFile::Open(LPCTSTR lpszFileName, DWORD nOpenFlags, CFileException* pError)
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// are we a server
	if(nOpenFlags & modeCreate)
		{
		// we've already created a pipe so
		// we just want to look for connections
		if(m_hFile != (UINT) INVALID_HANDLE_VALUE)
			{
			// if we've already made this call then
			// see if the event has signaled
			if(m_bConnectPending)
				{
				DWORD dwBytes;
				if(::GetOverlappedResult((HANDLE) m_hFile, &m_overlap, &dwBytes,
						((nOpenFlags&modeWaitForOpen)==modeWaitForOpen) ))
					{
					m_bConnectPending = false;
					m_evtComplete.ResetEvent();
					return true;
					}
				else
					{
					DWORD nErr = GetLastError();
					nErr = nErr;
					}

				return false;
				}
			else
				{
				m_overlap.hEvent = (HANDLE) m_evtComplete;
				m_bConnectPending = true;
				// got a connection right away?
				if(::ConnectNamedPipe((HANDLE) m_hFile, &m_overlap) != 0)
					{
					m_bConnectPending = false;
					return true;
					}
				else
					return false;
				}
			}

		DWORD nDirection;
 
		// look at the first 2 bits
		switch(nOpenFlags & 0x03)
			{
			case modeRead:
				nDirection = PIPE_ACCESS_INBOUND;
				break;
			case modeWrite:
				nDirection = PIPE_ACCESS_OUTBOUND;
				break;
			case modeReadWrite:
			default:
				nDirection = PIPE_ACCESS_DUPLEX;
				break;
			}

		DWORD nPipeType = (nOpenFlags & typeText) ? PIPE_TYPE_MESSAGE : PIPE_TYPE_BYTE;

		m_hFile = (UINT) ::CreateNamedPipe(
			lpszFileName,
			nDirection|FILE_FLAG_OVERLAPPED,
			nPipeType,
			PIPE_UNLIMITED_INSTANCES,
			1024,
			1024,
			10,
			NULL
			);

		return (HANDLE) m_hFile != INVALID_HANDLE_VALUE;
		}
	else
		// we are opening an exsisting pipe
		// so we are a client
		{
		CString sName(lpszFileName);
		BOOL b;
		for(int i = 0; i < 10; i++)
			if(b = ::WaitNamedPipe(sName, 1000))
				break;
		
		if(! b)
			{
			SetResultCode(CResult::errOS);
			SetOSResult(GetLastError());
			return false;
			}

		// we must be a client so open the pipe
		m_hFile = (UINT) ::CreateFile(
			(LPCTSTR) sName,		// pointer to name of the file
			GENERIC_WRITE|GENERIC_READ,			// access (read-write) mode
			FILE_SHARE_WRITE|FILE_SHARE_READ,		// share mode
			NULL,					// pointer to security attributes
			OPEN_EXISTING,			// how to create
			FILE_ATTRIBUTE_NORMAL,	// file attributes
			NULL
			);

		if((HANDLE) m_hFile == INVALID_HANDLE_VALUE)
			{
			SetResultCode(CResult::errOS);
			SetOSResult(GetLastError());
			return false;
			}
		else
			return true;
		}
	}

// Overridables
//--------------------------------------------------------------------------------
CFile* CNamedPipeFile::Duplicate() const
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return NULL;
	}

//--------------------------------------------------------------------------------
LONG CNamedPipeFile::Seek(LONG lOff, UINT nFrom)
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return 0;
	}

//--------------------------------------------------------------------------------
void CNamedPipeFile::SetLength(DWORD dwNewLen)
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(m_hFile == (UINT) INVALID_HANDLE_VALUE)
		return;

	::DisconnectNamedPipe((HANDLE) m_hFile);
	}

//--------------------------------------------------------------------------------
DWORD CNamedPipeFile::GetLength() const
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	DWORD nAvail = 0;

	::PeekNamedPipe((HANDLE) m_hFile, NULL, 0, NULL, &nAvail, NULL);
	return nAvail;
	}

//--------------------------------------------------------------------------------
UINT CNamedPipeFile::Read(void* lpBuf, UINT nCount)
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	UINT nBytes = 0;
	TRY
		{
		nBytes = CFile::Read(lpBuf, nCount);
		}
	CATCH_ALL(e)
		{
		SetResultCode(CResult::errOS);
		SetOSResult(((CFileException*) e)->m_lOsError);
		}
	END_CATCH_ALL

	return nBytes;
	}

//--------------------------------------------------------------------------------
void CNamedPipeFile::Write(const void* lpBuf, UINT nCount)
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	TRY
		{
		CFile::Write(lpBuf, nCount);
		}
	CATCH_ALL(e)
		{
		SetResultCode(CResult::errOS);
		SetOSResult(((CFileException*) e)->m_lOsError);
		}
	END_CATCH_ALL
	}

//--------------------------------------------------------------------------------
void CNamedPipeFile::LockRange(DWORD dwPos, DWORD dwCount)
	{
	}

//--------------------------------------------------------------------------------
void CNamedPipeFile::UnlockRange(DWORD dwPos, DWORD dwCount)
	{
	}

//--------------------------------------------------------------------------------
void CNamedPipeFile::Abort()
	{
	}

//--------------------------------------------------------------------------------
void CNamedPipeFile::Flush()
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(m_hFile == (UINT) INVALID_HANDLE_VALUE)
		return;

	::FlushFileBuffers((HANDLE) m_hFile);
	}

//--------------------------------------------------------------------------------
void CNamedPipeFile::Close()
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(m_hFile == (UINT) INVALID_HANDLE_VALUE)
		return;

	::FlushFileBuffers((HANDLE) m_hFile);

	::DisconnectNamedPipe((HANDLE) m_hFile);

	CFile::Close();
	m_hFile = (UINT) INVALID_HANDLE_VALUE;
	}

#ifdef _DEBUG
//--------------------------------------------------------------------------------
void CNamedPipeFile::AssertValid() const
	{
	}

//--------------------------------------------------------------------------------
void CNamedPipeFile::Dump(CDumpContext& dc) const
	{
	}

#endif

//--------------------------------------------------------------------------------
UINT CNamedPipeFile::GetBufferPtr(UINT nCommand, UINT nCount,
		void** ppBufStart, void** ppBufMax)
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return 0;
	}


//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

#if 0
//--------------------------------------------------------------------------------
UINT CLargeNamedPipeFile::Read(void* lpBuf, UINT nCount)
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// and i quote (CFile) avoid win32 null read
	if(nCount == 0)
		return 0;

	// total msg size in bytes
	UINT nSize;

	ClearResult();

	if(CNamedPipeFile::Read(&nSize, sizeof(nSize)) != sizeof(nSize))
		return 0;

	if(HasErrors())
		return 0;

	if(nCount > 0 && nCount < nSize)
		{
		SetResultCode(CResult::errOS);
		SetOSResult(ERROR_INSUFFICIENT_BUFFER);
		return 0;
		}

	UINT nRemaining = nCount;
	UINT nRead = 0;
	UINT nTotal = 0;
	char* pBuf = (char*) lpBuf;

	UINT nPackets = (nSize / 65535) + 1;

	for(UINT i = 0; i < nPackets; i++)
		{
		nRead = CNamedPipeFile::Read(pBuf, nRemaining > 65535 ? 65535 : nRemaining);

	TRACE("read %d bytes\n", nRead);

		nRemaining -= nRead;
		nTotal += nRead;
		pBuf += nRead;
		if(HasErrors())
			break;
		}

	return nTotal;
	}

//--------------------------------------------------------------------------------
void CLargeNamedPipeFile::Write(const void* lpBuf, UINT nCount)
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(nCount == 0)
		return;

	ClearResult();

	CNamedPipeFile::Write(&nCount, sizeof(nCount));
	if(HasErrors())
		return;

	UINT nRemaining = nCount;
	UINT nTotal = 0;
	char* pBuf = (char*) lpBuf;

	UINT nPackets = (nCount / 65535) + 1;

	for(UINT i = 0; i < nPackets; i++)
		{
		UINT nWrite = nRemaining > 65535 ? 65535 : nRemaining;
	
	TRACE("writing %d bytes\n", nWrite);

		CNamedPipeFile::Write(pBuf, nWrite);
		nRemaining -= nWrite;
		pBuf += nWrite;
		if(HasErrors())
			break;
		}
	}

#endif