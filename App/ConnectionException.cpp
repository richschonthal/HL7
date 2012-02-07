//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "ConnectionException.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//--------------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CConnectionException, CException)

//--------------------------------------------------------------------------------
CConnectionException::CConnectionException(LPCTSTR pMsg)
	{
	if(pMsg)
		m_sMsg = pMsg;
	}

//--------------------------------------------------------------------------------
CConnectionException::~CConnectionException()
	{

	}

//--------------------------------------------------------------------------------
BOOL CConnectionException::GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext)
	{
	strcpy(lpszError, m_sMsg.Left(nMaxError));
	return TRUE;
	}

