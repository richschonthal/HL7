//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// ExecuteApp.h: interface for the CExecuteApp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXECUTEAPP_H__851F5681_557A_11D3_A6BC_005004A1C5F3__INCLUDED_)
#define AFX_EXECUTEAPP_H__851F5681_557A_11D3_A6BC_005004A1C5F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "result.h"
#include "handle.h"

//--------------------------------------------------------------------------------
class CExecuteApp : public CResult  
	{
	public:
		CExecuteApp(LPCTSTR pExe, LPCTSTR pParams, LPCTSTR pPath, DWORD nWaitTime = 0);
		virtual ~CExecuteApp();

		CHandle m_hProcess;
		DWORD m_nExitCode;
	};

#endif // !defined(AFX_EXECUTEAPP_H__851F5681_557A_11D3_A6BC_005004A1C5F3__INCLUDED_)
