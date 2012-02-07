//--------------------------------------------------------------------------------
//
// Copyright (c) 2001 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _MULTISOCKETLOCK_H_
#define _MULTISOCKETLOCK_H_

//--------------------------------------------------------------------------------
class CSocketWrapper;

//--------------------------------------------------------------------------------
class CMultiSocketLock
	{
	private:
		CMultiLock* m_pLock;

	public:
		CMultiSocketLock(CSocketWrapper*, CSocketWrapper*);
		~CMultiSocketLock();

		bool IsLocked();
		void Unlock();
	};

#endif // _MULTISOCKETLOCK_H_
