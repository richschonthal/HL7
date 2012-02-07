// SmallSocketFile.h: interface for the CSmallSocketFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SMALLSOCKETFILE_H__FB136E37_D27C_11D3_AF12_005004A1C5F3__INCLUDED_)
#define AFX_SMALLSOCKETFILE_H__FB136E37_D27C_11D3_AF12_005004A1C5F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
class CSmallSocket;

//--------------------------------------------------------------------------------
class CSmallSocketFile : public CFile
	{
	DECLARE_DYNAMIC(CSmallSocketFile)
	public:
	//Constructors
		CSmallSocketFile(CSmallSocket* pSocket, BOOL bArchiveCompatible = TRUE);

	// Implementation
	public:
		CSmallSocket* m_pSocket;
		BOOL m_bArchiveCompatible;

		virtual ~CSmallSocketFile();

	#ifdef _DEBUG
		virtual void AssertValid() const;
		virtual void Dump(CDumpContext& dc) const;
	#endif
		virtual UINT Read(void* lpBuf, UINT nCount);
		virtual void Write(const void* lpBuf, UINT nCount);
		virtual void Close();

	// Unsupported APIs
		virtual BOOL Open(LPCTSTR lpszFileName, UINT nOpenFlags, CFileException* pError = NULL);
		virtual CFile* Duplicate() const;
		virtual DWORD GetPosition() const;
		virtual LONG Seek(LONG lOff, UINT nFrom);
		virtual void SetLength(DWORD dwNewLen);
		virtual DWORD GetLength() const;
		virtual void LockRange(DWORD dwPos, DWORD dwCount);
		virtual void UnlockRange(DWORD dwPos, DWORD dwCount);
		virtual void Flush();
		virtual void Abort();
	};

#endif // !defined(AFX_SMALLSOCKETFILE_H__FB136E37_D27C_11D3_AF12_005004A1C5F3__INCLUDED_)
