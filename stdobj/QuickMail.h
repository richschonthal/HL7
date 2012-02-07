//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _QUICKMAIL_H_
#define _QUICKMAIL_H_

//--------------------------------------------------------------------------------
#include "result.h"

//--------------------------------------------------------------------------------
class CQuickMail : public CResult
	{
	public:
		enum
			{
			//ErrCantOpenMAPI = HighestResultCode,
			};

	public:
		// port defaults to 25 - to use another port
		// simply set it after construction but before Send
		int m_nPort;
		CString m_sIP;
		CString m_sUser;
		CString m_sFrom;
		CString m_sTo;
		CString m_sSubject;
		CString m_sMsg;

	public:
		CQuickMail(LPCTSTR pIP = NULL, LPCTSTR pUser = NULL, LPCTSTR pFrom = NULL, 
			LPCTSTR pTo = NULL, LPCTSTR pSubject = NULL, LPCTSTR pMsg = NULL);

		virtual bool Send();
		virtual CString GetFormattedMsg();
	};

#endif // _QUICKMAIL_H_
