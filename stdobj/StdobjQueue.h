//--------------------------------------------------------------------------------
//
// Copyright (c) 2001 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _STDOBJQUEUE_H_
#define _STDOBJQUEUE_H_

//--------------------------------------------------------------------------------
#include "ReadLock.h"
#include "WriteLock.h"

//--------------------------------------------------------------------------------
template<class TYPE, class ARG_TYPE>
class CQueue : public CReadWriteObject
	{
	protected:
		CList<TYPE, ARG_TYPE> m_list;

	public:
		CQueue(int = 10);

		TYPE GetNext(DWORD nWait = INFINITE);
		TYPE GetNextAndRequeue(DWORD nWait = INFINITE);
		bool Add(ARG_TYPE, DWORD nWait = INFINITE, bool bAddTail = true);

		int GetCount(DWORD nWait = INFINITE) const;

		bool Find(ARG_TYPE, DWORD nWait = INFINITE);

		// NOTE: The remaining functions do not lock the list
		// you should lock the list before iterating thru it
		// iteration
		POSITION GetHeadPosition() const;
		POSITION GetTailPosition() const;
		TYPE& GetNext(POSITION& rPosition); // return *Position++
		TYPE GetNext(POSITION& rPosition) const; // return *Position++
		TYPE& GetPrev(POSITION& rPosition); // return *Position--
		TYPE GetPrev(POSITION& rPosition) const; // return *Position--

		// getting/modifying an element at a given position
		TYPE& GetAt(POSITION position);
		TYPE GetAt(POSITION position) const;
		void SetAt(POSITION pos, ARG_TYPE newElement);
		void RemoveAt(POSITION position);
	};

//--------------------------------------------------------------------------------
#define WL(nWait) CWriteLock lock(this, false); if(! lock.Lock(nWait)) return 
#define WLF(nWait) CWriteLock lock(this, false); if(! lock.Lock(nWait)) return false
// QCS = queue class spec
#define QCS template<class TYPE, class ARG_TYPE> inline
// CN = class name
#define CN CQueue<TYPE, ARG_TYPE>

QCS int CN::GetCount(DWORD nWait) const
	{ CReadLock lock(const_cast<CReadWriteObject*>((CReadWriteObject*)this), false); if(! lock.Lock(nWait)) return -1; return m_list.GetCount(); }

//--------------------------------------------------------------------------------
QCS CN::CQueue(int nSize) : m_list(nSize) {}

//--------------------------------------------------------------------------------
QCS TYPE CN::GetNext(DWORD nWait) { WL(nWait) NULL; return m_list.IsEmpty() ? NULL : m_list.RemoveHead(); }

//--------------------------------------------------------------------------------
QCS TYPE CN::GetNextAndRequeue(DWORD nWait)
	{ WL(nWait) NULL;
		return m_list.IsEmpty() ? NULL : m_list.GetAt(m_list.AddTail(m_list.RemoveHead())); }

//--------------------------------------------------------------------------------
QCS bool CN::Add(ARG_TYPE arg, DWORD nWait, bool bAddTail) { WLF(nWait); bAddTail ? m_list.AddTail(arg) : m_list.AddHead(arg); return true;}
QCS bool CN::Find(ARG_TYPE arg, DWORD nWait)
	{ CReadLock lock(const_cast<CReadWriteObject*>((CReadWriteObject*)this), false); if(! lock.Lock(nWait)) return false;
		for(POSITION pos = GetHeadPosition(); pos != NULL; ) if(arg == GetNext(pos)) return true; return false; }

QCS POSITION CN::GetHeadPosition() const { return m_list.GetHeadPosition(); }
QCS POSITION CN::GetTailPosition() const { return m_list.GetTailPosition(); }
QCS TYPE& CN::GetNext(POSITION& pos) { return m_list.GetNext(pos); }
QCS TYPE CN::GetNext(POSITION& pos) const { return m_list.GetNext(pos); }
QCS TYPE& CN::GetPrev(POSITION& pos) { return m_list.GetPrev(pos); }
QCS TYPE CN::GetPrev(POSITION& pos) const { return m_list.GetPrev(pos); }
QCS TYPE& CN::GetAt(POSITION pos) { return m_list.GetAt(pos); }
QCS TYPE CN::GetAt(POSITION pos) const { return m_list.GetAt(pos); }
QCS void CN::SetAt(POSITION pos, ARG_TYPE newElement) { m_list.SetAt(pos, newElement); }
QCS void CN::RemoveAt(POSITION pos) { m_list.RemoveAt(pos); }

#undef WL
#undef QCS
#undef CN

#endif // _STDOBJQUEUE_H_
