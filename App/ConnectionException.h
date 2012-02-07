//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#if !defined(AFX_CONNECTIONEXCEPTION_H__6B297545_1E93_11D3_87B9_00104B9E6286__INCLUDED_)
#define AFX_CONNECTIONEXCEPTION_H__6B297545_1E93_11D3_87B9_00104B9E6286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <commondefines.h>

//--------------------------------------------------------------------------------
class CConnectionException : public CException  
	{
	DECLARE_DYNAMIC(CConnectionException)

	private:
		CString m_sMsg;

	public:
		CConnectionException(LPCTSTR = NULL);
		virtual ~CConnectionException();

		virtual BOOL GetErrorMessage(LPTSTR lpszError, UINT nMaxError,
			PUINT pnHelpContext = NULL);
	};

#endif // !defined(AFX_CONNECTIONEXCEPTION_H__6B297545_1E93_11D3_87B9_00104B9E6286__INCLUDED_)
