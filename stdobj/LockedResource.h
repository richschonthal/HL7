//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _LOCKEDRESOURCE_H_
#define _LOCKEDRESOURCE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
class CLockedResource  
	{
	public:
		CLockedResource();
		virtual ~CLockedResource();

	protected:
		CMutex m_mutex;
		long m_nCurrentUserCount;

	public:
		CSingleLock* CreateExclusiveLock(DWORD = INFINITE);
		bool GetExclusiveLock(CSingleLock&, DWORD = INFINITE);

		bool AddUser(DWORD = INFINITE);
		void RemoveUser();

		long GetCurrentUserCount() const;
		CMutex& GetMutex();
	};

#endif // _LOCKEDRESOURCE_H_
