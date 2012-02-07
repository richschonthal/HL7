//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "ControlPanel.h"
#include "MainPropSheet.h"

#include <Registry.h>

#include <defs.h>
#include <OutputConfig.h>
#include <HL7InputDaemonInit.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//--------------------------------------------------------------------------------
CRegKey g_rkRoot(HKEY_LOCAL_MACHINE, REG_VERSION_3_ROOT);

//--------------------------------------------------------------------------------
CControlPanelApp theApp;

//--------------------------------------------------------------------------------
CControlPanelApp* CControlPanelApp::m_pThis = &theApp;


//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

//--------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CControlPanelApp, CWinApp)
	//{{AFX_MSG_MAP(CControlPanelApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//--------------------------------------------------------------------------------
CControlPanelApp::CControlPanelApp()
	{
	}

//--------------------------------------------------------------------------------
BOOL CControlPanelApp::InitInstance() 
	{
	
	return CWinApp::InitInstance();
	}

//--------------------------------------------------------------------------------
LONG APIENTRY CControlPanelApp::CPlApplet(HWND hWnd, UINT nMsg, LONG lParam1, LONG lParam2)
	{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 

	ASSERT(m_pThis);

	switch(nMsg) 
		{
		case CPL_DBLCLK:
			return m_pThis->OnDblclk(hWnd, lParam1, lParam2);

		case CPL_EXIT:
			return m_pThis->OnExit();

		case CPL_GETCOUNT:
			return m_pThis->OnGetCount();

		case CPL_INIT:
			return m_pThis->OnInit();

		case CPL_INQUIRE:
			return m_pThis->OnInquire(lParam1, (CPLINFO*) lParam2);

		case CPL_NEWINQUIRE:
			return m_pThis->OnNewInquire(lParam1, (NEWCPLINFO*)lParam2);

		case CPL_STOP:
			return m_pThis->OnStop(lParam1, lParam2);

		case CPL_STARTWPARMS:
			return m_pThis->OnDblclk(hWnd, lParam1, lParam2);

		default:
			break;
		}

	return 1;
	}

//--------------------------------------------------------------------------------
LONG CControlPanelApp::OnNewInquire(UINT uAppNum, NEWCPLINFO* pInfo) 
{    
	// Fill in the data
	pInfo->dwSize = sizeof(NEWCPLINFO);
	pInfo->dwFlags = 0;
	pInfo->dwHelpContext = 0;
	pInfo->lData = 0;
	pInfo->hIcon = LoadIcon(IDR_MAINFRAME);

	CString sCplName;
	sCplName.LoadString(IDS_CPL_NAME);

	CString sCplInfo;
	sCplInfo.LoadString(IDS_CPL_INFO);

	_tcscpy(pInfo->szName, sCplName);
	_tcscpy(pInfo->szInfo, sCplInfo);
	_tcscpy(pInfo->szHelpFile, _T(""));

	return 0;
	}

//--------------------------------------------------------------------------------
LONG CControlPanelApp::OnInquire(UINT uAppNum, CPLINFO* pInfo)
	{
	pInfo->idIcon = IDR_MAINFRAME;
	pInfo->idName = IDS_CPL_NAME;
	pInfo->idInfo = IDS_CPL_INFO;
	pInfo->lData = 0;

	return 0;
	}

//--------------------------------------------------------------------------------
LONG CControlPanelApp::OnDblclk(HWND hWnd, UINT nAppNum, LONG nData)
	{
	COutputConfig config;
	CHL7InputDaemonInitList inputConfig;

	CMainPropSheet dlg(IDS_TITLE, &config, &inputConfig);

	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();

	return 0;
	}

//--------------------------------------------------------------------------------
LONG CControlPanelApp::OnExit()
	{
	return 0; // OK
	}

//--------------------------------------------------------------------------------
LONG CControlPanelApp::OnGetCount()
	{
	return 1; // only one applet
	}

//--------------------------------------------------------------------------------
LONG CControlPanelApp::OnInit()
	{
	return 1; // OK
	}

//--------------------------------------------------------------------------------
LONG CControlPanelApp::OnStop(UINT uAppNum, LONG lData) 
	{
	return 1; // not handled
	}
