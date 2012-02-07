//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// NamedPipeFile.h: interface for the CNamedPipeFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NAMEDPIPEFILE_H__7EFA6B7D_F33C_11D2_87A7_00104B9E6286__INCLUDED_)
#define AFX_NAMEDPIPEFILE_H__7EFA6B7D_F33C_11D2_87A7_00104B9E6286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <result.h>

#include <commondefines.h>

#ifdef _WINDLL
#define DLLSPEC DllSpec
#else
#define DLLSPEC
#endif

//--------------------------------------------------------------------------------
class DLLSPEC CNamedPipeFile : public CFile, public CResult
	{
	DECLARE_DYNAMIC(CNamedPipeFile)

	public:
		// CNamedPipeFile-specific open flags
		enum
			{
			modeWaitForOpen = 0x10000
			};

	public:
	// Constructors
		CNamedPipeFile();
		CNamedPipeFile(LPCTSTR lpszFileName, UINT nOpenFlags);
		virtual ~CNamedPipeFile();

		bool IsOpen();

	// Attributes
		virtual DWORD GetPosition() const;

		virtual CString GetFileName() const;
		virtual CString GetFileTitle() const;
		virtual CString GetFilePath() const;
		virtual void SetFilePath(LPCTSTR lpszNewName);

	// Operations
		virtual BOOL Open(LPCTSTR lpszFileName, DWORD nOpenFlags,
			CFileException* pError = NULL);

	// Overridables
		virtual CFile* Duplicate() const;

		virtual LONG Seek(LONG lOff, UINT nFrom);
		virtual void SetLength(DWORD dwNewLen);
		virtual DWORD GetLength() const;

		virtual UINT Read(void* lpBuf, UINT nCount);
		virtual void Write(const void* lpBuf, UINT nCount);

		virtual void LockRange(DWORD dwPos, DWORD dwCount);
		virtual void UnlockRange(DWORD dwPos, DWORD dwCount);

		virtual void Abort();
		virtual void Flush();
		virtual void Close();

	// Implementation
	#ifdef _DEBUG
		virtual void AssertValid() const;
		virtual void Dump(CDumpContext& dc) const;
	#endif
		virtual UINT GetBufferPtr(UINT nCommand, UINT nCount = 0,
			void** ppBufStart = NULL, void** ppBufMax = NULL);

		OVERLAPPED m_overlap;
		bool m_bConnectPending;
		CEvent m_evtComplete;
	};

#if 0 
// dont really need this since CArchive does this for you

// use this class to transend the 64k msg size limit
//--------------------------------------------------------------------------------
class DllSpec CLargeNamedPipeFile : public CNamedPipeFile
	{
	public:
		CLargeNamedPipeFile() {}

		CLargeNamedPipeFile(LPCTSTR lpszFileName, UINT nOpenFlags)
			: CNamedPipeFile(lpszFileName, nOpenFlags) {}

		virtual UINT Read(void* lpBuf, UINT nCount);
		virtual void Write(const void* lpBuf, UINT nCount);
	};
#endif

#endif // !defined(AFX_NAMEDPIPEFILE_H__7EFA6B7D_F33C_11D2_87A7_00104B9E6286__INCLUDED_)
