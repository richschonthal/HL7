//--------------------------------------------------------------------------------
// Copyright (c) 2001 @COMPANY, Inc.
// Created...: 6/26/01
// Author....: Rich Schonthal
//--------------------------------------------------------------------------------

#ifndef _SOCKETSERVERSUBSYSTEM_H_
#define _SOCKETSERVERSUBSYSTEM_H_

//--------------------------------------------------------------------------------
#include "SubSystem.h"
#include "SocketServerQueue.h"

#include <AllocString.h>
#include <StdobjQueue.h>

//--------------------------------------------------------------------------------
class CSocketServerMainThread;
class CSocketServerQueueId;

//--------------------------------------------------------------------------------
class CSocketServerSubSystem : public CThreadPoolSubSystem
	{
	public:
		// msgs sent only within the subsytem and worker threads
		// for msgs sent to target subsystems, see below
		enum
			{
			MsgConnect = WM_USER,
			};

	protected:
//		DECLARE_DYNAMIC(CSocketServerSubSystem);

		long m_nPort;
		LPTSTR m_pIP;
		int m_nMaxThreads;

		CSocketServerMainThread* m_pMainThread;

		// maps the queue id to the thread that owns the queue
		CMapWordToPtr m_mapQueueId;

	public:
		CSocketServerSubSystem(CSystem*);
		virtual ~CSocketServerSubSystem();

		LPCTSTR GetIP() const;
		void SetIP(LPCTSTR);
		
		int GetPort() const;
		void SetPort(int);

		int GetMaxThreads() const;
		void SetMaxThread(int);

		virtual void StartSubSystem();

		// override this to close ports when the sub system is paused
		// (the default returns false)
		virtual bool IsClosePortsOnPause();

		CSocketServerQueueId* FindQueue(int nId);
		bool WriteQueue(CSocketServerQueueId*&, const CSocketServerQueueObj*);
		bool ReadQueue(CSocketServerQueueId*&, CSocketServerQueueObj*&);

	protected:
		friend class CSocketServerMainThread;
		friend class CSocketServerSocketThread;

		void RegisterQueue(int, CThreadObject*);
		void UnregisterQueue(CThreadObject*);
	};

//--------------------------------------------------------------------------------
class CSocketServerQueueId
	{
	protected:
		friend class CSocketServerSubSystem;

		int m_nId;
		CThreadObject* m_pThread;
		POSITION m_position;
	};

#endif // _SOCKETSERVERSUBSYSTEM_H_
