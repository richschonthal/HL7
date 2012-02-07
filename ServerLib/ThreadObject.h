//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#if !defined(AFX_THREADOBJECT_H__9F10BA23_835B_11D3_B0D3_00A0CC271D0D__INCLUDED_)
#define AFX_THREADOBJECT_H__9F10BA23_835B_11D3_B0D3_00A0CC271D0D__INCLUDED_

//--------------------------------------------------------------------------------
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
#include "result.h"
#include "SystemObject.h"

//--------------------------------------------------------------------------------
class CSystem;
class CSubSystem;

//--------------------------------------------------------------------------------
class CThreadObject : public CSystemObject
	{
	DECLARE_DYNCREATE(CThreadObject);

	friend class CThreadPoolSubSystem;

	protected:
		HANDLE m_hThread;
		DWORD m_nThreadId;
		MSG m_msgCur;

		CEvent m_evtThreadBusy;

	protected:
		CThreadObject(CSystemObject* = NULL);

		static UINT StaticMain(CThreadObject*);

		// override if you want a different or dynamic sleep interval
		virtual DWORD GetThreadLoopSleepInterval() const;
		virtual DWORD GetPauseWaitSleepInterval() const;

		// derived mainloops return true to continue looping
		// or false to exit - use CResult::SetResultCode for exit code
		// use MainLoop in your derived thread when you want to run in a continuous loop
		// (see StaticMain)
		virtual bool MainLoop();

		// use OnMessage when you want your thread to be message based
		// (see StaticMain)
		virtual void OnMessage(UINT, WPARAM, LPARAM);

		// called only when this thread is part of a thread pool
		virtual bool OnActivate();
		virtual bool OnDeactivate();

		// called when the state changes
		virtual void OnStateChange(DWORD);

		// if init returns false, the main loop is never called
		// no events are process, and Exit is called with param false
		virtual bool Init();
		virtual void Exit(bool bInitOk);

		// checks the current state
		// returns true if the thread should continue running
		// (ie the state != STATE_STOP and != STATE_ERROR
		virtual bool CheckState();

		// returns m_bActive
		bool IsActive() const;

		bool SetThreadPriority(int nPriority);

		void SetThreadBusy(bool bIsBusy);

		bool SetParent(CSubSystem*);

	public:
		virtual ~CThreadObject();

		// always call after allocation
		virtual bool StartThread();
		bool EndThread();

		CEvent& GetEvent();

		// posts a message to this thread
		virtual bool PostMessage(UINT, WPARAM = 0, LPARAM = 0);

		// waits until the thread handle signals
		// use to wait for the thread to exit
		bool WaitForThreadExit(DWORD = INFINITE);

		// some helpful functions
		HANDLE GetThreadHandle();
		DWORD GetThreadId();

		int GetThreadPriority();

		bool IsThreadBusy(DWORD nWait = 0);

	protected:
		CSystemObject* GetParentFromClass(const CRuntimeClass*);
	};

//--------------------------------------------------------------------------------
template<class T_SYSTEM, class T_SUBSYSTEM>
class CTypedThreadObject : public CThreadObject
	{
	private:
		// cache for GetSystem and GetSubSystem
		T_SYSTEM* m_pSystem;
		T_SUBSYSTEM* m_pSubSystem;

	public:
		CTypedThreadObject(T_SUBSYSTEM*);
		virtual ~CTypedThreadObject();

		T_SYSTEM* GetSystem();
		T_SUBSYSTEM* GetSubSystem();
	};

//--------------------------------------------------------------------------------
template<class T_SYSTEM, class T_SUBSYSTEM>
inline CTypedThreadObject<T_SYSTEM, T_SUBSYSTEM>::CTypedThreadObject(T_SUBSYSTEM* pSys)
	: CThreadObject((CSystemObject*) pSys), m_pSystem(NULL), m_pSubSystem(NULL)  {}

template<class T_SYSTEM, class T_SUBSYSTEM>
inline CTypedThreadObject<T_SYSTEM, T_SUBSYSTEM>::~CTypedThreadObject() {}

template<class T_SYSTEM, class T_SUBSYSTEM>
inline T_SYSTEM* CTypedThreadObject<T_SYSTEM, T_SUBSYSTEM>::GetSystem()
	{
	if(m_pSystem != NULL)
		return m_pSystem;
	m_pSystem = (T_SYSTEM*) GetParentFromClass(RUNTIME_CLASS(CSystem));
	return m_pSystem;
	}

template<class T_SYSTEM, class T_SUBSYSTEM>
inline T_SUBSYSTEM* CTypedThreadObject<T_SYSTEM, T_SUBSYSTEM>::GetSubSystem()
	{
	if(m_pSubSystem != NULL)
		return m_pSubSystem;
	m_pSubSystem = (T_SUBSYSTEM*) GetParentFromClass(RUNTIME_CLASS(CSubSystem));
	return m_pSubSystem;
	}

//--------------------------------------------------------------------------------
inline bool CThreadObject::PostMessage(UINT nMsg, WPARAM wParam, LPARAM lParam)
	{ return ::PostThreadMessage(m_nThreadId, nMsg, wParam, lParam) != FALSE; }

inline HANDLE CThreadObject::GetThreadHandle() { return m_hThread; }
inline DWORD CThreadObject::GetThreadId() {	return m_nThreadId; }
inline CEvent& CThreadObject::GetEvent() { return m_event; }
inline bool CThreadObject::WaitForThreadExit(DWORD nWait)
	{
	if(m_hThread == NULL || m_hThread == INVALID_HANDLE_VALUE)
		return true;
	return ::WaitForSingleObject(m_hThread, nWait) == WAIT_OBJECT_0;
	}

inline bool CThreadObject::SetThreadPriority(int nPriority)
	{ return ::SetThreadPriority(m_hThread, nPriority) != 0; }

inline int CThreadObject::GetThreadPriority() { return ::GetThreadPriority(m_hThread); }

inline bool CThreadObject::IsThreadBusy(DWORD nWait) { return ::WaitForSingleObject(m_evtThreadBusy, nWait) == WAIT_OBJECT_0; }

//--------------------------------------------------------------------------------
// helper macro
#define DECLARE_THREADCLASS(SubSystem, System) \
	public: \
		SubSystem* GetSubSystem() \
	{ \
			for(CSystemObject* pPar = GetParent(); pPar != NULL; pPar = pPar->GetParent()) \
				{ \
				if(pPar->GetRuntimeClass() == RUNTIME_CLASS(SubSystem)) \
					return (SubSystem*) pPar; \
		} \
			return NULL;\
			}\
		System* GetSystem() \
	{ \
			for(CSystemObject* pPar = GetParent(); pPar != NULL; pPar = pPar->GetParent()) \
				{ \
				if(pPar->GetRuntimeClass() == RUNTIME_CLASS(System)) \
					return (System*) pPar; \
				}\
			return NULL;\
			}

//--------------------------------------------------------------------------------
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THREADOBJECT_H__9F10BA23_835B_11D3_B0D3_00A0CC271D0D__INCLUDED_)
