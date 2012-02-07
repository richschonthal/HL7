//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _THREADPOOLOBJECT_H_
#define _THREADPOOLOBJECT_H_

//--------------------------------------------------------------------------------
#include <ReadWriteObject.h>

class CThreadObList;
class CThreadObject;
class CThreadObjectPoolControlThread;

//--------------------------------------------------------------------------------
class CThreadObjectPool
	{
	public:
		enum
			{
			// set to keep the waiting list clean
			// ie when setting a thread to waiting, the max waiting count is checked
			// if moving a thread to the waiting list will make waitingcount > maxwaiting
			// then the thread is terminated instead of being moved to the waiting list
			TP_KEEPCLEAN = 0x00000001
			};

	private:
		CThreadObjectPoolControlThread* m_pControlThread;
		CThreadObList* m_pActiveThreads;
		CThreadObList* m_pWaitingThreads;

		DWORD m_nFlags;
		// the total max size of the pool
		DWORD m_nMaxSize;
		// max allowed to be waiting
		DWORD m_nMaxWaiting;
		// # of miliseconds between size checks
		DWORD m_nCleanUpInterval;

	public:
		CThreadObjectPool(DWORD nFlags = 0, DWORD nMaxSize = 0, DWORD nMaxWaiting = 0, DWORD nCleanUpInterval = 0);
		virtual ~CThreadObjectPool();

		// thread pool stuff

		// returns ptr to active thread list
		CThreadObList* GetActiveThreads() const;
		// returns ptr to waiting thread list
		CThreadObList* GetWaitingThreads() const;

		DWORD GetFlags() const;
		void SetFlags(DWORD);

		DWORD GetMaxSize() const;
		void SetMaxSize(DWORD);

		DWORD GetMaxWaiting() const;
		void SetMaxWaiting(DWORD);

		DWORD GetCleanUpInterval() const;
		void SetCleanUpInterval(DWORD);

		// called when there are no waiting threads
		// and one is requested
		virtual bool IncreasePool();

		// called to determine if more threads can be allocated
		virtual bool CanIncreasePool() const;

	public:
		// adds to the active thread list
		virtual bool SetThreadActive(CThreadObject*);
		// adds to the waiting thread list
		virtual bool SetThreadWaiting(CThreadObject*);

		// removes a thread from the waiting list
		// adds it to the active list
		// and returns a pointer to it
		// calls IncreasePool if the active pool is empty
		// returns NULL if there are no available waiting threads
		// and IncreasePool failed to add some
		virtual CThreadObject* GetNextWaiting(DWORD = INFINITE);

		// removes a thread from the active list
		// puts it back into the waiting list
		virtual bool ReleaseActive(CThreadObject*);

		// adds a thread to the thread list
		virtual bool AddThread(CThreadObject*, bool bActive = false);
	};

//--------------------------------------------------------------------------------
class CThreadLoadBalancePool : public CThreadObjectPool
	{
	public:
		virtual CThreadObject* GetNextWaiting(DWORD = INFINITE);
		virtual bool ReleaseActive(CThreadObject*);
		virtual bool SetThreadActive(CThreadObject*);
		virtual bool SetThreadWaiting(CThreadObject*);
	};

#endif // _THREADPOOLOBJECT_H_
