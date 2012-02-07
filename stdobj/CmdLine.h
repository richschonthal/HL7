//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// CmdLine.h: interface for the CCmdLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDLINE_H__6598E964_F73E_11D2_87A7_00104B9E6286__INCLUDED_)
#define AFX_CMDLINE_H__6598E964_F73E_11D2_87A7_00104B9E6286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
class CCmdLine : public CObject  
	{
	public:
		CCmdLine(bool);
		CCmdLine(LPCTSTR = NULL, LPCTSTR = NULL);
		CCmdLine(int nArgc, LPCTSTR* pArgv, LPCTSTR = NULL);
		virtual ~CCmdLine();

	private:
		static HANDLE m_hMapFile;
		static LPVOID m_pMapView;

		static LPTSTR m_pCmdLine;
		static LPTSTR m_pTokens;
		static LPWORD m_pRefCount;

		static CString m_sExePath;
		static CStringArray m_params;
		static CByteArray m_cmdIndex;

		bool AllocAll();
		void DeallocAll();

		void IncRefCount();
		void DecRefCount();
		int GetRefCount();
	public:
		operator=(LPCTSTR);

		CString GetCmdLine()	{ return m_pCmdLine; }
		operator LPCTSTR()		{ return m_pCmdLine; }

		bool HasParam(char, int&);
		bool HasParam(LPCTSTR, int&);

		bool GetArgs(int, CStringArray&);
		bool GetArgs(int, CString&);
		bool GetArgs(CStringArray&);
		bool GetArgs(CString&, char = ' ');

		int GetCmdIndex(int);

		int GetParamCount();

		CString GetParam(int);

		static LPCTSTR ParseParam(LPCTSTR, CString&);
	};

#endif // !defined(AFX_CMDLINE_H__6598E964_F73E_11D2_87A7_00104B9E6286__INCLUDED_)
