//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#include "stdafx.h"
#include "TextSocket.h"

//--------------------------------------------------------------------------------
CTextSocket::CTextSocket()
	{
	m_nTimeOut = 2000;
	}

//--------------------------------------------------------------------------------
CTextSocket::~CTextSocket()
	{
	}

//--------------------------------------------------------------------------------
void CTextSocket::SetTimeOut(DWORD nTimeOut)
	{
	m_nTimeOut = nTimeOut;
	}

//--------------------------------------------------------------------------------
DWORD CTextSocket::GetTimeOut()
	{
	return m_nTimeOut;
	}

//--------------------------------------------------------------------------------
int CTextSocket::Receive(CString& sText)
	{
	// if we have something queued then just return it
	if(m_buffer.GetCount() > 0)
		{
		POSITION pos = m_buffer.GetHeadPosition();
		if(pos == NULL)
			return 0;
		sText = m_buffer.GetAt(pos);
		m_buffer.RemoveAt(pos);
		return sText.GetLength();
		}

	sText.Empty();
	DWORD nStart = ::GetTickCount();

	for(;;)
		{
		int nLen = CSmallSocket::Receive(sText.GetBuffer(1024), 1024);
		sText.ReleaseBuffer(nLen == -1 ? 0 : nLen);

		if(nLen == 0)
			return 0;

		if(nLen == SOCKET_ERROR)
			{
			DWORD nNow = ::GetTickCount();
			// check for tickcount wrap around - happens every 49.7 days
			// of continuous running - see GetTickCount
			if(nNow < nStart)
				{
				nStart = ::GetTickCount();
				continue;
				}

			// see if we've timed out
			if((nStart + GetTimeOut()) <= nNow)
				return 0;

			if(::WSAGetLastError() == WSAEWOULDBLOCK)
				{
				::Sleep(100);
				continue;
				}
			else
				return SOCKET_ERROR;
			}

		for(int i = 0; i < sText.GetLength(); i++)
			{
			TCHAR cCur = sText[i];
			switch(cCur)
				{
				case '\r':
					m_buffer.AddTail(m_sCurLine);
					m_sCurLine.Empty();
					break;
				case '\n':
					break;
				default:
					m_sCurLine += cCur;
					break;
				}
			}

		sText.Empty();
		return 0;
		}
	}

//--------------------------------------------------------------------------------
int CTextSocket::Send(const CString& sText)
	{
	return CSmallSocket::Send(sText, sText.GetLength());
	}
