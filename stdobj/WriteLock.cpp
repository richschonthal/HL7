//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "WriteLock.h"
#include "Timer.h"

#include "DebugPrintf.h"

//--------------------------------------------------------------------------------
CWriteLock::CWriteLock(CReadWriteObject* pRW, bool bInitialLock)
		: CReadWriteLock(pRW)
	{
	if(bInitialLock)
		Lock(INFINITE);
	}

//--------------------------------------------------------------------------------
CWriteLock::~CWriteLock()
	{
	Unlock();
	}

//--------------------------------------------------------------------------------
bool CWriteLock::Lock(DWORD nWait)
	{
	if(m_pRW == NULL)
		return false;

	if(m_bIsLocked)
		return true;

	HANDLE hands[2] = { (HANDLE) CReadWriteObject::m_hReaderMapMutex , m_pRW->m_mutexWriter };

	if(::WaitForMultipleObjects(2, hands, TRUE, nWait) < 2)
		{
		// lookup or add our thread to the reader map for this CReadWriteObject
		LARGE_INTEGER nId = { GetCurrentThreadId(), (DWORD) m_pRW };
		LONG nCount = 0;
		bool bFound = CReadWriteObject::m_pReaderMap->Lookup(nId.QuadPart, nCount) != 0;
		::ReleaseMutex(CReadWriteObject::m_hReaderMapMutex);

		// there's still readers so wait for them to release or timeout
		if(m_pRW->m_nReaderCount > nCount)
			{
			switch(nWait)
				{
				case INFINITE:
					while(m_pRW->m_nReaderCount > nCount)
						::Sleep(100);
					m_bIsLocked = true;
					break;

				case 0:
					m_bIsLocked = (m_pRW->m_nReaderCount <= nCount);
					break;

				default:
					{
					CPulseTimer timer;
					if(! timer.Set(nWait))
						return false;
					while(! timer.Wait(100))
						if(m_pRW->m_nReaderCount <= nCount)
							{
							m_bIsLocked = true;
							break;
							}

					break;
					}
				}
			}
		else
			m_bIsLocked = true;

#ifdef _DEBUG
//		TRACE("writelock nCount = %d nReaderCount = %d islocked %c\n",
//			nCount, m_pRW->m_nReaderCount, m_bIsLocked ? 'Y' : 'N');
#endif

		if(! m_bIsLocked)
			m_pRW->m_mutexWriter.Unlock();
		}

	return m_bIsLocked;
	}

//--------------------------------------------------------------------------------
bool CWriteLock::Unlock()
	{
	if(m_bIsLocked)
		{
		m_pRW->m_mutexWriter.Unlock();
		m_bIsLocked = false;
		}

	return true;
	}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
CMultiWriteLock::CMultiWriteLock()
	{
	}

//--------------------------------------------------------------------------------
CMultiWriteLock::~CMultiWriteLock()
	{
	}

//--------------------------------------------------------------------------------
void CMultiWriteLock::SetTargets(int nCount, ...)
	{
	ASSERT(nCount < 64 && nCount > 0);
	m_objects.SetSize(nCount);

	va_list marker;
	va_start(marker, nCount);

	for(int i = 0; i < nCount; i++)
		AddTarget(va_arg(marker, CReadWriteObject*));
	}

//--------------------------------------------------------------------------------
inline int CMultiWriteLock::GetTotalReaderCount()
	{
	int nTotal = 0;
	for(int i = 0; i < m_objects.GetSize(); i++)
		nTotal += m_objects[i]->m_nReaderCount;
	return nTotal;
	}

//--------------------------------------------------------------------------------
bool CMultiWriteLock::Lock(DWORD nWait)
	{
	if(m_bIsLocked)
		return true;

	CArray<HANDLE, HANDLE> hands;
	hands.SetSize(m_objects.GetSize() + 1);
	hands.Add((HANDLE) CReadWriteObject::m_hReaderMapMutex);

	for(int i = 0; i < m_objects.GetSize(); i++)
		hands.Add(m_objects[i]->m_mutexWriter);

	if(::WaitForMultipleObjects(hands.GetSize(), hands.GetData(), TRUE, nWait) < (DWORD) hands.GetSize())
		{
		LONG nCount = 0;
		// get total reader count for this thread for all objects in array
		for(i = 0; i < m_objects.GetSize(); i++)
			{
			// lookup or add our thread to the reader map for this CReadWriteObject
			LARGE_INTEGER nId = { GetCurrentThreadId(), (DWORD) m_objects[i] };
			LONG nCurCount = 0;
			CReadWriteObject::m_pReaderMap->Lookup(nId.QuadPart, nCurCount);
			nCount += nCurCount;
			}

		::ReleaseMutex(CReadWriteObject::m_hReaderMapMutex);

		// there's still readers so wait for them to release or timeout
		if(GetTotalReaderCount() > nCount)
			{
			switch(nWait)
				{
				case INFINITE:
					while(GetTotalReaderCount() > nCount)
						::Sleep(100);
					m_bIsLocked = true;
					break;

				case 0:
					m_bIsLocked = (GetTotalReaderCount() <= nCount);
					break;

				default:
					{
					CPulseTimer timer;
					if(! timer.Set(nWait))
						return false;
					while(! timer.Wait(100))
						if(GetTotalReaderCount() <= nCount)
							{
							m_bIsLocked = true;
							break;
							}

					break;
					}
				}
			}
		else
			m_bIsLocked = true;

		if(! m_bIsLocked)
			for(i = 0; i < m_objects.GetSize(); i++)
				m_objects[i]->m_mutexWriter.Unlock();
		}

	return m_bIsLocked;
	}

//--------------------------------------------------------------------------------
bool CMultiWriteLock::Unlock()
	{
	for(int i = 0; i < m_objects.GetSize(); i++)
		m_objects[i]->m_mutexWriter.Unlock();

	m_bIsLocked = false;
	return true;
	}

