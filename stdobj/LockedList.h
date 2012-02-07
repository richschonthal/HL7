//--------------------------------------------------------------------------------
//
// Copyright (c) 2001 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _LOCKEDLIST_H_
#define _LOCKEDLIST_H_

//--------------------------------------------------------------------------------
#include "result.h"
#include "ReadLock.h"
#include "WriteLock.h"

// an exact replacement for CList except for the CObject stuff
//--------------------------------------------------------------------------------
template<class TYPE, class ARG_TYPE>
class CLockedList : public CReadWriteObject
	{
	protected:
		CList<TYPE, ARG_TYPE> m_list;

	public:
		// Construction
		CLockedList(int nBlockSize = 10);

		// Attributes (head and tail)
		// count of elements
		int GetCount() const;
		BOOL IsEmpty() const;

		// peek at head or tail
		TYPE& GetHead();
		TYPE GetHead() const;
		TYPE& GetTail();
		TYPE GetTail() const;

		// Operations
		// get head or tail (and remove it) - don't call on empty list !
		TYPE RemoveHead();
		TYPE RemoveTail();

		// add before head or after tail
		POSITION AddHead(ARG_TYPE newElement);
		POSITION AddTail(ARG_TYPE newElement);

		// add another list of elements before head or after tail
		void AddHead(CLockedList* pNewList);
		void AddTail(CLockedList* pNewList);

		// remove all elements
		void RemoveAll();

		// inserting before or after a given position
		POSITION InsertBefore(POSITION position, ARG_TYPE newElement);
		POSITION InsertAfter(POSITION position, ARG_TYPE newElement);

		// helper functions (note: O(n) speed)
		POSITION Find(ARG_TYPE searchValue, POSITION startAfter = NULL) const;
			// defaults to starting at the HEAD, return NULL if not found
		POSITION FindIndex(int nIndex) const;
			// get the 'nIndex'th element (may return NULL)

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
// LLCS = locked list class spec
#define LLCS template<class TYPE, class ARG_TYPE> inline
// CN = class name
#define CN CLockedList<TYPE, ARG_TYPE>
// RL = read lock
#define RL CReadLock lock(const_cast<CReadWriteObject*>((CReadWriteObject*)this))
#define RT2(p) CReadLock lock2(p)
// CE = check empty
#define CE if(m_list.IsEmpty()) return NULL

// WL = write lock
#define WL CWriteLock lock(const_cast<CReadWriteObject*>((CReadWriteObject*)this))

LLCS CN::CLockedList(int nBlockSize) : m_list(nBlockSize) {}

LLCS int CN::GetCount() const { RL; return m_list.GetCount(); }
LLCS BOOL CN::IsEmpty() const { RL; return m_list.IsEmpty(); }

LLCS TYPE& CN::GetHead() { RL; CE; return m_list.GetHead(); }
LLCS TYPE CN::GetHead() const { RL; CE; return m_list.GetHead(); }
LLCS TYPE& CN::GetTail() { RL; CE; return m_list.GetTail(); }
LLCS TYPE CN::GetTail() const { RL; CE; return m_list.GetTail(); }

LLCS TYPE CN::RemoveHead() { WL; CE; return m_list.RemoveHead(); }
LLCS TYPE CN::RemoveTail() { WL; CE; return m_list.RemoveTail(); }

LLCS POSITION CN::AddHead(ARG_TYPE newElement) { WL; return m_list.AddHead(newElement); }
LLCS POSITION CN::AddTail(ARG_TYPE newElement) { WL; return m_list.AddTail(newElement); }

LLCS void CN::AddHead(CLockedList* pNewList) { WL; RL2(pNewList); m_list.AddHead(&pNewList->m_list); }
LLCS void CN::AddTail(CLockedList* pNewList) { WL; RL2(pNewList); m_list.AddTail(&pNewList->m_list); }

LLCS void CN::RemoveAll() { WL; m_list.RemoveAll(); }

LLCS POSITION CN::GetHeadPosition() const { return m_list.GetHeadPosition(); }
LLCS POSITION CN::GetTailPosition() const { return m_list.GetTailPosition(); }
LLCS TYPE& CN::GetNext(POSITION& pos) { return m_list.GetNext(pos); }
LLCS TYPE CN::GetNext(POSITION& pos) const { return m_list.GetNext(pos); }
LLCS TYPE& CN::GetPrev(POSITION& pos) { return m_list.GetPrev(pos); }
LLCS TYPE CN::GetPrev(POSITION& pos) const { return m_list.GetPrev(pos); }
LLCS TYPE& CN::GetAt(POSITION pos) { return m_list.GetAt(pos); }
LLCS TYPE CN::GetAt(POSITION pos) const { return m_list.GetAt(pos); }
LLCS void CN::SetAt(POSITION pos, ARG_TYPE newElement) { m_list.SetAt(pos, newElement); }
LLCS void CN::RemoveAt(POSITION pos) { m_list.RemoveAt(pos); }

// inserting before or after a given position
LLCS POSITION CN::InsertBefore(POSITION pos, ARG_TYPE newElement)
	{ WL; return m_list.InsertBefore(pos, newElement); }

LLCS POSITION CN::InsertAfter(POSITION pos, ARG_TYPE newElement)
	{ WL; return m_list.InsertAfter(pos, newElement); }

LLCS POSITION CN::Find(ARG_TYPE searchValue, POSITION startAfter) const
	{ RL; return m_list.Find(searchValue, startAfter); }

LLCS POSITION CN::FindIndex(int nIndex) const
	{ RL; return m_list.FindIndex(nIndex); }

#undef LLCS
#undef CN
#undef RL
#undef RL2
#undef WL
#undef CE

#endif // _LOCKEDLIST_H_
