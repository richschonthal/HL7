//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "IOThread.h"
#include "IOSubSystem.h"
#include "NTService.h"

#include <QuickMail.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CIOThread, CSystemObject)

//--------------------------------------------------------------------------------
CIOThread::CIOThread(CIOSubSystem* pParent)
		: CTypedThreadObject<CSystem, CIOSubSystem>(pParent)
	{
	}

//--------------------------------------------------------------------------------
CIOThread::~CIOThread()
	{
	}

//--------------------------------------------------------------------------------
void CIOThread::Exit(bool)
	{
	MSG msg;
	while(PeekMessage(&msg, NULL, CIOSubSystem::MSG_OUTPUT, 0, PM_REMOVE))
		delete[] (LPTSTR) msg.wParam;
	while(PeekMessage(&msg, NULL, CIOSubSystem::MSG_QUICKMAIL, 0, PM_REMOVE))
		delete reinterpret_cast<CQuickMail*>(msg.wParam);
	}

//--------------------------------------------------------------------------------
void CIOThread::OnMessage(UINT nMsg, WPARAM param, LPARAM)
	{
	LPTSTR pText = (LPTSTR) param;

	switch(nMsg)
		{
		case CIOSubSystem::MSG_OUTPUT:
			Output(pText);
			delete[] pText;
			break;

		case CIOSubSystem::MSG_OUTPUT_CONST:
			Output(pText);
			break;

		case CIOSubSystem::MSG_QUICKMAIL:
			{
			CQuickMail* pQm = reinterpret_cast<CQuickMail*>(param);
			if(pQm != NULL)
				{
				pQm->Send();
				delete pQm;
				}
			}
			break;
		}
	}

//--------------------------------------------------------------------------------
void CIOThread::Output(LPCTSTR pText)
	{
	CSingleLock* pLock = NULL;
	CStdioFile* pFile = NULL;
	
	if(GetSubSystem()->GetLogFilePtr() != NULL)
		{
		pLock = new CSingleLock(&GetSubSystem()->GetLogFileLock(), false);

		if(! pLock->Lock())
			{
			delete pLock;
			pLock = NULL;
			}
		else
			pFile = GetSubSystem()->GetLogFilePtr();
		}

	if(pFile != NULL)
		GetSubSystem()->OutputToFile(pText, pFile);
	delete pLock;

	if((GetSubSystem()->GetTraceLevel() & IOMASK_DEBUG) == IOMASK_DEBUG)
		GetSubSystem()->OutputToDebugger(pText);

	CNTService* pService = GetSubSystem()->GetServicePtr();
	if(pService != NULL)
		pService->AddToMessageLog(pText);
	}

//--------------------------------------------------------------------------------
void CIOThread::AcknowledgeStateChange()
	{
	CSystemObject::AcknowledgeStateChange();
	}

//--------------------------------------------------------------------------------
int CIOThread::GetObjectId()
	{
	return OIDTM_THREAD + SLOID_IO;
	}
