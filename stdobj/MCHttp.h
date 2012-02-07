// MCHttp.h: interface for the CMCHttp class.
//
// ATTENTION:	See implementation file MCHttp.cpp for notes regarding
//				this classes members and example code.
//
//////////////////////////////////////////////////////////////////////

#ifndef _MCHTTP_H_
#define _MCHTTP_H_

#include <afxinet.h>
#include <MCMemBlock.h>

//#define CMCHTTP_CALLBACK_MSG (WM_USER+0x7FFF)
#define CMCHTTP_CALLBACK_MSG (WM_USER+1)

#if _MSC_VER>1000
#pragma once
#endif //_MSC_VER>1000

class CMCHttp : public CInternetSession  
{
public:
	CMCHttp(
		CWnd* pParent			=NULL,
		LPCTSTR pstrAgent		=NULL,
		DWORD dwContext			=1,
		DWORD dwAccessType		=INTERNET_OPEN_TYPE_PRECONFIG,
		LPCTSTR pstrProxyName	=NULL,
		LPCTSTR pstrProxyBypass	=NULL,
		DWORD dwFlags			=0); //INTERNET_FLAG_ASYNC);
	virtual ~CMCHttp();
public:
	bool AddRequestHeaders(CString& str,DWORD dwFlags=HTTP_ADDREQ_FLAG_ADD_IF_NEW);
	bool CloseRequest();	
	bool FreeRequest(BYTE** ppBuf);
	static LPCTSTR GetHttpCallBackMsg(DWORD nInternetStatus);
	bool GetHttpConnection(
			LPCTSTR	pstrServer,
			INTERNET_PORT nPort		=INTERNET_INVALID_PORT_NUMBER,
			LPCTSTR	pstrUserName	=NULL,
			LPCTSTR	pstrPassword	=NULL);	
	virtual void OnStatusCallback(
			DWORD	dwContext,
			DWORD	dwInternetStatus,
			LPVOID	lpvStatusInformation,
			DWORD	dwStatusInformationLength);
	bool OpenRequest(		
			int nVerb,
			LPCTSTR pstrObjectName,
			LPCTSTR pstrReferer			=NULL,
			DWORD dwContext				=1,
			LPCTSTR* pstrAcceptTypes	=NULL,
			LPCTSTR pstrVersion			=NULL,
			DWORD dwFlags				=INTERNET_FLAG_EXISTING_CONNECT);
	bool ReadRequest(BYTE** ppBuf, DWORD& nLen);
	bool SendRequest(
			LPCTSTR pstrHeaders	=NULL,
			DWORD dwHeadersLen	=0,
			LPVOID lpOptional	=NULL,
			DWORD dwOptionalLen	=0);
	bool SetSendTimeout(DWORD nTime);
	bool WriteRequest(const BYTE** ppBuf,DWORD nLen,LPCTSTR pFile);
	bool GetLastErrorMsg(CString& strErr); // Retrieve the last internet error message.
private:
	DWORD	GetInternetExceptionErr(CInternetException* pe);
	DWORD	GetInternetExceptionContext(CInternetException* pe);
	LPCTSTR GetInternetExceptionMsg(CInternetException* pe);
private:
	CHttpConnection*	m_pHttpConnection;
	CHttpFile*			m_pHttpFile;	
	CWnd*				m_pParent;		// Parent window to send messages to, supplied by caller.
private:
	DWORD				m_nErr;
	DWORD				m_nContext;
	CString				m_strErr;
public:
	static UINT			m_nNumObjects;	// Reference counter.
	static UINT			m_nStdBlock;	// Buffering block size.
	static UINT			m_nCallBackMsg; // WM_USER+0x7FFF for callback.
};

#endif // #ifndef _MCHTTP_H_ WM_USER
