//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _SHARED_DB_H_
#define _SHARED_DB_H_

//--------------------------------------------------------------------------------
#ifdef DB_SHARE

//--------------------------------------------------------------------------------
#include <PACSDatabase.h>

//--------------------------------------------------------------------------------
class CSharedDB
	{
	protected:
		static CPACSDatabase* m_pDB;
		static CMutex m_mutex;

	protected:
		CSingleLock m_lock;

	public:
		CSharedDB(bool bCreateIfNeeded = true);
		~CSharedDB();

		CPACSDatabase* GetDB();

		bool DeleteConnection();
	};

#endif // DB_SHARE
#endif // _SHARED_DB_H_
