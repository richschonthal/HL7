//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _READWRITEOBJECT_H_
#define _READWRITEOBJECT_H_

#ifndef __AFXMT_H__
#include <afxmt.h>
#endif

#ifndef __AFXTEMPL_H__
#include<afxtempl.h>
#endif

//--------------------------------------------------------------------------------
class CReadWriteObject
	{
	friend class CReadLock;
	friend class CWriteLock;
	friend class CMultiWriteLock;

	protected:
		CMutex m_mutexWriter;
		LONG m_nReaderCount;

		static LONG m_nReaderMapRefCount;
		static HANDLE m_hReaderMapMutex;
		static CMap<__int64, __int64, LONG, LONG>* m_pReaderMap;

	public:
		CReadWriteObject();

		virtual ~CReadWriteObject();
	};

//--------------------------------------------------------------------------------
class CReadWriteLock
	{
	protected:
		CReadWriteObject* m_pRW;
#ifdef _DEBUG
		int m_nLockNumber;
#endif

	public:
		bool m_bIsLocked;

	public:
		CReadWriteLock(CReadWriteObject*);

		virtual bool Attach(CReadWriteObject*);

		virtual bool Lock(DWORD dwTimeout = INFINITE) = 0;
		virtual bool Unlock() = 0;
	};

#endif //_READWRITEOBJECT_H_
