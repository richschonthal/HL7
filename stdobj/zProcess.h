//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// Process.h: interface for the CProcess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROCESS_H__FA9A9163_6B7B_11D3_AEE0_005004A1C5F3__INCLUDED_)
#define AFX_PROCESS_H__FA9A9163_6B7B_11D3_AEE0_005004A1C5F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
#include "Handle.h"
#include "result.h"

//--------------------------------------------------------------------------------
class CProcess : public CObject, public CResult
	{
	public:
		PROCESS_INFORMATION m_procInfo;
		CHandle m_openHandle;

	public:
		CProcess();
		~CProcess();

		bool Create(LPCTSTR pName, LPTSTR pCmdLine = NULL, LPCTSTR pCurDir = NULL,
			DWORD dwCreationFlags = 0,
			bool bInheritHandles = false,
			LPSTARTUPINFO lpStartupInfo = NULL,
			LPSECURITY_ATTRIBUTES lpProcessAttributes = NULL,
			LPSECURITY_ATTRIBUTES lpThreadAttributes = NULL,
			LPVOID lpEnvironment = NULL
			);
	};

//--------------------------------------------------------------------------------
class CProcessArray : public CTypedPtrArray<CObArray, CProcess*>
	{
	public:
		~CProcessArray()
			{
			for(int i = GetUpperBound(); i >= 0; i--)
				delete GetAt(i);
			};
	};

#endif // !defined(AFX_PROCESS_H__FA9A9163_6B7B_11D3_AEE0_005004A1C5F3__INCLUDED_)
