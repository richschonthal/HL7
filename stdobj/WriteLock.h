//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _WRITELOCK_H_
#define _WRITELOCK_H_

//--------------------------------------------------------------------------------
#ifndef __AFXMT_H__
#include <afxmt.h>
#endif

//--------------------------------------------------------------------------------
#include "ReadWriteObject.h"

//--------------------------------------------------------------------------------
class CReadWriteObject;
class CReadLock;

//--------------------------------------------------------------------------------
class CWriteLock : public CReadWriteLock
	{
	public:
		CWriteLock(CReadWriteObject* = NULL, bool = true);
		virtual ~CWriteLock();

	public:
		virtual bool Lock(DWORD dwTimeout = INFINITE);
		virtual bool Unlock();
	};

//--------------------------------------------------------------------------------
class CMultiWriteLock
	{
	public:
		CMultiWriteLock();
		virtual ~CMultiWriteLock();
		CArray<CReadWriteObject*, CReadWriteObject*> m_objects;

	public:
		bool m_bIsLocked;

	public:
		virtual void SetTargets(int nCount, ...);
		virtual void AddTarget(CReadWriteObject*);

		virtual bool Lock(DWORD dwTimeout = INFINITE);
		virtual bool Unlock();

	private:
		int GetTotalReaderCount();
	};

//--------------------------------------------------------------------------------
inline void CMultiWriteLock::AddTarget(CReadWriteObject* pObj) { m_objects.Add(pObj); }

//--------------------------------------------------------------------------------
#include <RefCount.h>

//--------------------------------------------------------------------------------
class CRefCountWriteLock : public CWriteLock
	{
	public:
		CRefCountWriteLock(CReadWriteObject* = NULL, bool = true);
		virtual ~CRefCountWriteLock();

	// Operations
		virtual bool Lock(DWORD dwTimeout = INFINITE);
		virtual bool Unlock();
	};

//--------------------------------------------------------------------------------
inline CRefCountWriteLock::CRefCountWriteLock(CReadWriteObject* pObj, bool bLockNow)
		: CWriteLock(pObj, bLockNow)
	{
	}
//--------------------------------------------------------------------------------
inline CRefCountWriteLock::~CRefCountWriteLock() {}

//--------------------------------------------------------------------------------
inline bool CRefCountWriteLock::Lock(DWORD dwTimeout)
	{
	if(CWriteLock::Lock(dwTimeout))
		{
		reinterpret_cast<CRefCountObj*>(m_pRW)->m_nRefCount++;
		return true;
		}
	return false;
	}

//--------------------------------------------------------------------------------
inline bool CRefCountWriteLock::Unlock()
	{
	if(CWriteLock::Unlock())
		{
		reinterpret_cast<CRefCountObj*>(m_pRW)->m_nRefCount--;
		return true;
		}
	return false;
	}

#endif // _WRITELOCK_H_
