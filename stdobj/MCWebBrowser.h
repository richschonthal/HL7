//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY Solutions
//
// Programming by Gene Angelo
//
//--------------------------------------------------------------------------------

#ifndef _MCWEBBROWSER_H_
#define _MCWEBBROWSER_H_

//--------------------------------------------------------------------------------
#include <ddeml.h>

//--------------------------------------------------------------------------------
// Import the proper libraries to link to.
#pragma comment(lib,"shlwapi.lib")

//--------------------------------------------------------------------------------
#ifdef _MCWEBBROWSER_USE_REG_LIB_
#ifdef _DEBUG
#pragma comment(lib,"MCRegD.lib")
#else
#pragma comment(lib,"MCReg.lib")
#endif
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER>1000

//--------------------------------------------------------------------------------
typedef enum tagMCWebBrowser{
	MCUnknown	=0,	
	MCIExplore	=1,
	MCNetscape	=2
} MCWebBrowser;

//--------------------------------------------------------------------------------
class CMCWebBrowser : public CObject  
	{
	public:
		CMCWebBrowser();		
		virtual ~CMCWebBrowser();

	public:
		bool Create(MCWebBrowser=MCIExplore);		
		bool Close();
		bool HasErrors() const { return m_bError; }

	public:		
		bool OpenMultiple(CString&);	// Open a new instance of the browser.		
		bool OpenSingle(CString&);		// Open a URL in an existing browser instance.	

	private:		
		void SetBrowserDir(LPCTSTR);	// Set the browser dir of the browser .exe.
		void SetBrowserExe(LPCTSTR);	// Set the browser .exe file to execute.					

	private:
		bool StartDdeConversation();
		bool EndDdeConversation();
		bool FreeDdeHandle(HDDEDATA&);
		bool FreeDdeHandle(DWORD,HSZ&);
		bool CreateDdeStringHandle(const CString&,HSZ&);
		bool CreateDdeDataHandle(const CString&,const HSZ&,HDDEDATA&,bool=true);

	private:		
		MCWebBrowser m_WebBrowser;
		CString m_sIExploreReg;	// Registry key to intetnet explorer.
		CString m_sNetscapeReg;	// Registry key to netscape navigator.
		CString m_sServer;		// Holde the DDE server name for the browser.
		CString m_sDefaultBrowserDir;	// The directory that the browser .exe is located.
		CString m_sDefaultBrowserExe;	// The web browser .exe name.					

	private: // DDE conversation variables.
		DWORD m_nInst;		

	private:
		bool m_bCreated;

		bool m_bError;
		CString m_sError;
	};

#endif // #ifndef _MCWEBBROWSER_H_
