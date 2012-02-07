//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// ServiceStatus.h: interface for the CServiceStatus class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVICESTATUS_H__FFF6C771_6232_11D3_A6BE_005004A1C5F3__INCLUDED_)
#define AFX_SERVICESTATUS_H__FFF6C771_6232_11D3_A6BE_005004A1C5F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
#include "result.h"

//--------------------------------------------------------------------------------
class CServiceStatus : public CResult  
	{
	public:
		CServiceStatus(LPCTSTR pServiceName);
		virtual ~CServiceStatus();

		bool IsInstalled();
		bool IsRunning();
		bool IsAuto();
		bool IsSystem();
		bool IsManual();
	};

#endif // !defined(AFX_SERVICESTATUS_H__FFF6C771_6232_11D3_A6BE_005004A1C5F3__INCLUDED_)
