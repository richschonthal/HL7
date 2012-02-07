//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// ProcessInfo.h: interface for the CProcessInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROCESSINFO_H__35CFA811_2FD4_11D3_87C5_00104B9E6286__INCLUDED_)
#define AFX_PROCESSINFO_H__35CFA811_2FD4_11D3_87C5_00104B9E6286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
class CProcessInfo  
	{
	private:
		CDWordArray m_nProcId;

	public:
		CProcessInfo(LPCTSTR);
		virtual ~CProcessInfo();

	public:
		static void GetProcessName(DWORD nProcId, CString& sName);

		int GetCount()				{ return m_nProcId.GetSize(); }
		DWORD GetProcId(int n = 0)	{ return m_nProcId[n]; }
	};

#endif // !defined(AFX_PROCESSINFO_H__35CFA811_2FD4_11D3_87C5_00104B9E6286__INCLUDED_)
