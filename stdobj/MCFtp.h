//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY Solutions
//
// Programming by Gene Angelo
//
// MCFtp.h: interface for the CMCFtp class.
//
// ATTENTION:	See implementation file MCFtp.cpp for notes regarding
//				this classes members and example code.
//
//--------------------------------------------------------------------------------

#ifndef _MCFTP_H_
#define _MCFTP_H_

//--------------------------------------------------------------------------------
#include <afxinet.h>
#include <MCMemBlock.h>

//--------------------------------------------------------------------------------
#define CMCFTP_CALLBACK_MSG (WM_USER+1)

//--------------------------------------------------------------------------------
#pragma comment (lib,"Wininet.lib")

#if _MSC_VER>1000
#pragma once
#endif //_MSC_VER>1000

//--------------------------------------------------------------------------------
class CMCFtp : public CInternetSession  
	{
	public:
		CMCFtp(
			CWnd* pParent			=NULL,
			LPCTSTR pstrAgent		=NULL,
			DWORD dwContext			=1,
			DWORD dwAccessType		=INTERNET_OPEN_TYPE_PRECONFIG,
			LPCTSTR pstrProxyName	=NULL,
			LPCTSTR pstrProxyBypass	=NULL,
			DWORD dwFlags			=0); //INTERNET_FLAG_ASYNC);
		virtual ~CMCFtp();

	public:		
		// Use this member to override the port used in the FTP operations.
		// This must be called before any FTP operations take place.
		void SetFTPPort(INTERNET_PORT=21);
		// Return the current FTP port used in FTP operations for this object.
		INTERNET_PORT GetFTPPort();
		// Note: Transfer type should be one of the valid ftp transfer types defined in wininet.h
		// i.e. FTP_TRANSFER_TYPE_UNKNOWN, FTP_TRANSFER_TYPE_ASCII, FTP_TRANSFER_TYPE_BINARY.
		bool DownloadFtpFile(		
			LPCTSTR lpszRemoteFile,
			LPCTSTR lpszLocalFile,
			LPCTSTR lpszUserid,
			LPCTSTR lpszPassword,
			DWORD nTransferType=FTP_TRANSFER_TYPE_BINARY,
			bool bEcrypt=false);	
		static LPCTSTR GetFtpCallBackMsg(DWORD nInternetStatus);	
		virtual void OnStatusCallback(
				DWORD	dwContext,
				DWORD	dwInternetStatus,
				LPVOID	lpvStatusInformation,
				DWORD	dwStatusInformationLength);	
		// Note: Transfer type should be one of the valid ftp transfer types defined in wininet.h
		// i.e. FTP_TRANSFER_TYPE_UNKNOWN, FTP_TRANSFER_TYPE_ASCII, FTP_TRANSFER_TYPE_BINARY.
		bool UploadFtpFile(		
				LPCTSTR lpszRemoteFile,
				LPCTSTR lpszLocalFile,
				LPCTSTR lpszUserid,
				LPCTSTR lpszPassword,
				DWORD nTransferType=FTP_TRANSFER_TYPE_BINARY,
				bool bEcrypt=false);	
		bool GetLastErrorMsg(CString& strErr);

	private:
		bool	ChangeCurrentFtpDirectory(LPCTSTR lpszDir);
		bool	CheckFtpFileExists(LPCTSTR lpszRemoteFile);
		bool	CloseFtpConnection();		
		DWORD	GetContext() const { return CInternetSession::GetContext(); }
		bool	GetFtpConnection(
					LPCTSTR	pstrServer,
					INTERNET_PORT nPort=INTERNET_INVALID_PORT_NUMBER,
					LPCTSTR	pstrUserName=NULL,
					LPCTSTR	pstrPassword=NULL);	
		DWORD	GetInternetExceptionErr(CInternetException* pe);
		DWORD	GetInternetExceptionContext(CInternetException* pe);
		LPCTSTR GetInternetExceptionMsg(CInternetException* pe);
		bool	ParseFtpFile(LPCTSTR lpszRemoteFile);
		// Note: Transfer type should be one of the valid ftp transfer types defined in wininet.h
		// i.e. FTP_TRANSFER_TYPE_UNKNOWN, FTP_TRANSFER_TYPE_ASCII, FTP_TRANSFER_TYPE_BINARY.
		bool	ProcessFtpRequest(
				LPCTSTR lpszRemoteFile,
				LPCTSTR lpszLocalFile,
				LPCTSTR lpszUserid,
				LPCTSTR lpszPassword,
				DWORD nTransferType,
				bool bEcrypt,
				bool bDownload);	
		// Note: Transfer type should be one of the valid ftp transfer types defined in wininet.h
		// i.e. FTP_TRANSFER_TYPE_UNKNOWN, FTP_TRANSFER_TYPE_ASCII, FTP_TRANSFER_TYPE_BINARY.
		bool	ReadWriteFtpFile(
					LPCTSTR lpszInFile,
					LPCTSTR lpszLocalFile,
					DWORD nTransferType=FTP_TRANSFER_TYPE_BINARY,
					bool bDownload=true);	

	private:
		CFtpConnection*		m_pFtpConnection;	
		CWnd*				m_pParent;				// Parent window to send messages to, supplied by caller.
		CString				m_strRemoteServerIP;	// Ftp server IP to connect to.
		CString				m_strRemoteFile;		// Ftp file to download/upload.
		CString				m_strRemoteDir;			// Directory Ftp file is located.
		CString				m_strErr;				// Last ftp error encountered.
		DWORD				m_nContext;				// Context of the last ftp error encountered.
		DWORD				m_nErr;					// Return code of the last ftp error encountered.
		UINT				m_nPort;				// used in place of m_nDefPort it > 0;

	public:
		static UINT			m_nNumObjects;	// Reference counter.
		static UINT			m_nStdBlock;	// Buffering block size.
		static UINT			m_nCallBackMsg; // WM_USER+0x7FFF for callback.	
		static UINT			m_nDefPort;		// Default ftp port.	
	};

#endif // #ifndef _MCFTP_H_
