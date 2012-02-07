//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _READLOCK_H_
#define _READLOCK_H_

//--------------------------------------------------------------------------------
#ifndef __AFXMT_H__
#include <afxmt.h>
#endif

//--------------------------------------------------------------------------------
#include "ReadWriteObject.h"
class CWriteLock;

//--------------------------------------------------------------------------------
class CReadLock : public CReadWriteLock
	{
	// Constructor
	public:
		CReadLock(CReadWriteObject* = NULL, bool bLockNow = true);
		virtual ~CReadLock();

	// Operations
		virtual bool Lock(DWORD dwTimeout = INFINITE);
		virtual bool Unlock();
		bool Lock(CWriteLock*);
	};


//--------------------------------------------------------------------------------
#include <RefCount.h>

//--------------------------------------------------------------------------------
class CRefCountReadLock : public CReadLock
	{
	public:
		CRefCountReadLock(CReadWriteObject* = NULL, bool = true);
		virtual ~CRefCountReadLock();

	// Operations
		virtual bool Lock(DWORD dwTimeout = INFINITE);
		virtual bool Unlock();
		bool Lock(CWriteLock*);
	};

//--------------------------------------------------------------------------------
inline CRefCountReadLock::CRefCountReadLock(CReadWriteObject* pObj, bool bLockNow)
		: CReadLock(pObj, bLockNow)
	{
	}
//--------------------------------------------------------------------------------
inline CRefCountReadLock::~CRefCountReadLock() {}

//--------------------------------------------------------------------------------
inline bool CRefCountReadLock::Lock(DWORD dwTimeout)
	{
	if(CReadLock::Lock(dwTimeout))
		{
		reinterpret_cast<CRefCountObj*>(m_pRW)->m_nRefCount++;
		return true;
		}
	return false;
	}

//--------------------------------------------------------------------------------
inline bool CRefCountReadLock::Unlock()
	{
	if(CReadLock::Unlock())
		{
		reinterpret_cast<CRefCountObj*>(m_pRW)->m_nRefCount--;
		return true;
		}
	return false;
	}

//--------------------------------------------------------------------------------
inline bool CRefCountReadLock::Lock(CWriteLock* pLock)
	{
	if(CReadLock::Lock(pLock))
		{
		reinterpret_cast<CRefCountObj*>(m_pRW)->m_nRefCount++;
		return true;
		}
	return false;
	}

#endif // _READLOCK_H_
