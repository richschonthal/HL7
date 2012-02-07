//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// Handle.h: interface for the CHandle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEHANDLE_H__6619A261_5018_11D3_A6BB_005004A1C5F3__INCLUDED_)
#define AFX_FILEHANDLE_H__6619A261_5018_11D3_A6BB_005004A1C5F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
class CHandle  
	{
	protected:
		HANDLE m_handle;

	public:
		CHandle(HANDLE = INVALID_HANDLE_VALUE);
		CHandle(const CHandle&);
		~CHandle();

		void Close();

		operator HANDLE();
		operator=(HANDLE);

		bool IsValid();
	};

//--------------------------------------------------------------------------------
class CFindFileHandle : public CHandle
	{
	public:
		CFindFileHandle(HANDLE = INVALID_HANDLE_VALUE);
		~CFindFileHandle();
		void Close();

		operator HANDLE();
		operator=(HANDLE);
	};

#endif // !defined(AFX_FILEHANDLE_H__6619A261_5018_11D3_A6BB_005004A1C5F3__INCLUDED_)
