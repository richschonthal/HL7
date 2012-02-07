//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#if !defined(AFX_LOCKOBJECT_H__3DB04CE3_888A_11D3_AF00_005004A1C5F3__INCLUDED_)
#define AFX_LOCKOBJECT_H__3DB04CE3_888A_11D3_AF00_005004A1C5F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
class CLockObject  
	{
	public:
		enum
			{
			AUTO_DELETE = 1
			};

	protected:
		CSyncObject* m_pLock;
		DWORD m_nFlags;

	public:
		CLockObject(CSyncObject* = NULL, DWORD = 0);
		virtual ~CLockObject();

	public:
		CSyncObject* GetLockPtr();
		void SetLockPtr(CSyncObject*, DWORD = 0);

		HANDLE GetLockHandle();
	};

#endif // !defined(AFX_LOCKOBJECT_H__3DB04CE3_888A_11D3_AF00_005004A1C5F3__INCLUDED_)
