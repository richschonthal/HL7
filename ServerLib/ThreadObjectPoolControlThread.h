//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#if !defined(AFX_THREADOBJECTPOOLCONTROLTHREAD_H__499C63A4_DD70_11D3_AF12_005004A1C5F3__INCLUDED_)
#define AFX_THREADOBJECTPOOLCONTROLTHREAD_H__499C63A4_DD70_11D3_AF12_005004A1C5F3__INCLUDED_

//--------------------------------------------------------------------------------
#include "ThreadObject.h"

class CThreadObjectPool;

//--------------------------------------------------------------------------------
class CThreadObjectPoolControlThread : public CThreadObject  
	{
	private:
		CThreadObjectPool* m_pPool;
		CTime m_ctLastCheck;

	public:
		CThreadObjectPoolControlThread(CThreadObjectPool*);
		virtual ~CThreadObjectPoolControlThread();

		virtual bool MainLoop();

		CThreadObjectPool* GetPool() const;
	};

#endif // !defined(AFX_THREADOBJECTPOOLCONTROLTHREAD_H__499C63A4_DD70_11D3_AF12_005004A1C5F3__INCLUDED_)
