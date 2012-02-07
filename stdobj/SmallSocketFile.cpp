//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "SmallSocket.h"
#include "SmallSocketFile.h"

// ripped from the CSocketFile MFC class

//--------------------------------------------------------------------------------
CSmallSocketFile::CSmallSocketFile(CSmallSocket* pSocket, BOOL bArchiveCompatible)
	{
	m_pSocket = pSocket;
	m_bArchiveCompatible = bArchiveCompatible;

#ifdef _DEBUG
	ASSERT(m_pSocket != NULL);
	ASSERT(m_pSocket->m_hSocket != INVALID_SOCKET);

	int nType = 0;
	int nTypeLen = sizeof(int);
	ASSERT(m_pSocket->GetSockOpt(SOL_SOCKET, SO_TYPE, &nType, &nTypeLen));
	ASSERT(nType == SOCK_STREAM);
#endif // _DEBUG
	}

//--------------------------------------------------------------------------------
CSmallSocketFile::~CSmallSocketFile()
	{
	}

//--------------------------------------------------------------------------------
UINT CSmallSocketFile::Read(void* lpBuf, UINT nCount)
	{
	ASSERT(m_pSocket != NULL);

	int nRead;

	if (!m_bArchiveCompatible)
		{
		int nLeft = nCount;
		PBYTE pBuf = (PBYTE)lpBuf;

		while(nLeft > 0)
			{
			nRead = m_pSocket->Receive(pBuf, nLeft);
			if (nRead == SOCKET_ERROR)
				{
				int nError = m_pSocket->GetLastError();
				AfxThrowFileException(CFileException::generic, nError);
				ASSERT(FALSE);
				}
			else if (nRead == 0)
				{
				return nCount - nLeft;
				}

			nLeft -= nRead;
			pBuf += nRead;
			}
		return nCount - nLeft;
		}

	nRead = m_pSocket->Receive(lpBuf, nCount, 0);
	if(nRead == SOCKET_ERROR)
		{
		int nError = m_pSocket->GetLastError();
		AfxThrowFileException(CFileException::generic, nError);
		ASSERT(FALSE);
		}
	return nRead;
	}

//--------------------------------------------------------------------------------
void CSmallSocketFile::Write(const void* lpBuf, UINT nCount)
	{
	ASSERT (m_pSocket!=NULL);

	int nWritten = m_pSocket->Send(lpBuf, nCount);
	if (nWritten == SOCKET_ERROR)
		{
		int nError = m_pSocket->GetLastError();
		AfxThrowFileException(CFileException::generic, nError);
		}
	}

//--------------------------------------------------------------------------------
void CSmallSocketFile::Close()
	{
	m_pSocket = NULL;
	}

//--------------------------------------------------------------------------------
BOOL CSmallSocketFile::Open(LPCTSTR, UINT, CFileException*)
	{
	AfxThrowNotSupportedException();
	return FALSE;
	}

//--------------------------------------------------------------------------------
CFile* CSmallSocketFile::Duplicate() const
	{
	AfxThrowNotSupportedException();
	return NULL;
	}

//--------------------------------------------------------------------------------
DWORD CSmallSocketFile::GetPosition() const
	{
	AfxThrowNotSupportedException();
	return 0;
	}

//--------------------------------------------------------------------------------
LONG CSmallSocketFile::Seek(LONG lOff, UINT nFrom)
	{
	if (lOff != 0L || nFrom != current)
		TRACE0("Warning - Attempt made to seek on a CSmallSocketFile\n");
	return 0;
	}

//--------------------------------------------------------------------------------
void CSmallSocketFile::SetLength(DWORD /*dwNewLen*/)
	{
	AfxThrowNotSupportedException();
	}

//--------------------------------------------------------------------------------
DWORD CSmallSocketFile::GetLength() const
	{
	AfxThrowNotSupportedException();
	return 0;
	}

//--------------------------------------------------------------------------------
void CSmallSocketFile::LockRange(DWORD /*dwPos*/, DWORD /*dwCount*/)
	{
	AfxThrowNotSupportedException();
	}

//--------------------------------------------------------------------------------
void CSmallSocketFile::UnlockRange(DWORD /*dwPos*/, DWORD /*dwCount*/)
	{
	AfxThrowNotSupportedException();
	}

//--------------------------------------------------------------------------------
void CSmallSocketFile::Flush()
	{
	}

//--------------------------------------------------------------------------------
void CSmallSocketFile::Abort()
	{
	AfxThrowNotSupportedException();
	}

//--------------------------------------------------------------------------------
#ifdef _DEBUG
void CSmallSocketFile::AssertValid() const
	{
	CFile::AssertValid();
	if (m_pSocket != NULL)
		ASSERT_VALID(m_pSocket);
	}

//--------------------------------------------------------------------------------
void CSmallSocketFile::Dump(CDumpContext& dc) const
	{
	CFile::Dump(dc);
	if (dc.GetDepth() > 0)
		{
		if (m_pSocket != NULL)
			dc << "with no socket\n";
		else
			dc << "with socket: " << m_pSocket;
		}
	}
#endif //_DEBUG
