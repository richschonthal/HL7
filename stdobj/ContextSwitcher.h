//--------------------------------------------------------------------------------
//
// Copyright (c) 2001 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _CONTEXTSWITCHER_H_
#define _CONTEXTSWITCHER_H_

//--------------------------------------------------------------------------------
#include <StdobjQueue.h>

//--------------------------------------------------------------------------------
template<class TYPE>
class CContextSwitcher
	{
	protected:
		CQueue<TYPE*, TYPE*> m_contexts;
		LONG m_nContextCount;

	public:
		TYPE* GetNextContext();
		void AddNewContext(TYPE*);
		bool QueueContext(TYPE*);
		void DeleteContext(TYPE*&);
		LONG GetContextCount() const;

	protected:
		void IncContextCount();
		void DecContextCount();
	};

//--------------------------------------------------------------------------------
#define INLINE template<class TYPE> inline
#define CCST CContextSwitcher<TYPE>

INLINE TYPE* CCST::GetNextContext() { return m_contexts.GetNext(1000); }
INLINE void CCST::AddNewContext(TYPE* pContext) { m_contexts.Add(pContext, INFINITE, false); IncContextCount(); }
INLINE bool CCST::QueueContext(TYPE* pContext) { return m_contexts.Add(pContext); }
INLINE void CCST::DeleteContext(TYPE*& pContext)
	{ ASSERT(! m_contexts.Find(pContext)); DecContextCount(); delete pContext; pContext = NULL; }
INLINE LONG CCST::GetContextCount() const { return m_nContextCount; }
INLINE void CCST::IncContextCount() { InterlockedIncrement(&m_nContextCount); }
INLINE void CCST::DecContextCount() { InterlockedDecrement(&m_nContextCount); }

#undef CCST
#undef INLINE

#endif //_CONTEXTSWITCHER_H_
