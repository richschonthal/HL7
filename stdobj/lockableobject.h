//************************************************************************************
//
// general-purpose MFC-based classes
//
// Copyright (c) 1997 Rich Schonthal
//
//
//************************************************************************************
// module: lockable CObjects

#ifndef _LOCKABLEOBJECT_H_
#define _LOCKABLEOBJECT_H_

#include <afxmt.h>

class CMutexBase
   {
   private:
      CMutex m_mutex;
      CSingleLock m_singleLock;

   public:
      CMutexBase() : m_singleLock(&m_mutex) {};

      BOOL IsLocked()
            { return m_singleLock.IsLocked(); }

      BOOL Lock(DWORD dwTimeOut = INFINITE)
            { return m_singleLock.Lock(dwTimeOut); }

      BOOL Unlock()
            { return m_singleLock.Unlock(); }
  
      BOOL Unlock( LONG lCount, LPLONG lPrevCount = NULL )
            { return m_singleLock.Unlock(lCount, lPrevCount); }  
   };

class CSemaphoreBase
   {
   private:
      CSemaphore m_semaphore;
      CSingleLock m_singleLock;

   public:
      CSemaphoreBase(int nCount) : m_semaphore(0, nCount), m_singleLock(&m_semaphore) {};

      BOOL IsLocked()
            { return m_singleLock.IsLocked(); }

      BOOL Lock(DWORD dwTimeOut = INFINITE)
            { return m_singleLock.Lock(dwTimeOut); }

      BOOL Unlock()
            { return m_singleLock.Unlock(); }
  
      BOOL Unlock( LONG lCount, LPLONG lPrevCount = NULL )
            { return m_singleLock.Unlock(lCount, lPrevCount); }  
   };


#endif // _LOCKABLEOBJECT_H_