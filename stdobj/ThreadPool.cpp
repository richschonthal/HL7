//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "ThreadPool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//--------------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(CThreadPool, CWinThread)

//--------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CThreadPool, CWinThread)
	//{{AFX_MSG_MAP(CThreadPool)
	//}}AFX_MSG_MAP
	ON_THREAD_MESSAGE(TPMSG_INIT, DoInit)
	ON_THREAD_MESSAGE(TPMSG_MESSAGE, DoMsg)
END_MESSAGE_MAP()

//--------------------------------------------------------------------------------
CThreadPool::CThreadPool()
	{
	}

//--------------------------------------------------------------------------------
CThreadPool::~CThreadPool()
	{
	}

//--------------------------------------------------------------------------------
BOOL CThreadPool::InitInstance()
	{
	return TRUE;
	}

//--------------------------------------------------------------------------------
int CThreadPool::ExitInstance()
	{
	MSG msg;

	while(PeekMessage(&msg, (HWND) -1, TPMSG_MESSAGE, TPMSG_MESSAGE, PM_REMOVE))
		delete (CThreadPoolMsg*) msg.wParam;

	CSingleLock lock(&m_waiting.m_mutex, false);
	if(! lock.Lock(INFINITE))
		return -1;

	CSingleLock lock2(&m_active.m_mutex, false);
	if(! lock2.Lock(INFINITE))
		return -1;

	// stop all threads
	// wait for them to exit
	int nSizeA = m_active.GetCount();
	int nSizeW = m_waiting.GetCount();
	int nSize = nSizeA + nSizeW;
	if(nSize > 0)
		{
		POSITION pos = m_active.GetHeadPosition();
		HANDLE* pHands = new HANDLE[nSize];
		CWinThread* pThread = NULL;
		for(int i = 0; i < nSizeA; i++)
			{
			pThread = m_active.GetNext(pos);
			pHands[i] = pThread->m_hThread;
			pThread->PostThreadMessage(WM_QUIT, 0, 0);
			}

		pos = m_waiting.GetHeadPosition();
		for(; i < nSize; i++)
			{
			pThread = m_waiting.GetNext(pos);
			pHands[i] = pThread->m_hThread;
			pThread->PostThreadMessage(WM_QUIT, 0, 0);
			}

		::WaitForMultipleObjects(nSize, pHands, true, 60000);
		}

	return CWinThread::ExitInstance();
	}

//--------------------------------------------------------------------------------
int CThreadPool::GetActiveCount()
	{
	CSingleLock lock(&m_active.m_mutex, false);
	if(! lock.Lock(10000))
		return -1;

	return m_active.GetCount();
	}

//--------------------------------------------------------------------------------
int CThreadPool::GetWaitingCount()
	{
	CSingleLock lock(&m_waiting.m_mutex, false);
	if(! lock.Lock(10000))
		return -1;

	return m_waiting.GetCount();
	}

//--------------------------------------------------------------------------------
int CThreadPool::GetPoolSize()
	{
	CSingleLock lock(&m_waiting.m_mutex, false);
	if(! lock.Lock(10000))
		return -1;

	CSingleLock lock2(&m_active.m_mutex, false);
	if(! lock2.Lock(10000))
		return -1;

#ifdef _DEBUG
	TRACE(_T("waiting = %d active = %d\n"), m_waiting.GetCount(), m_active.GetCount());
#endif
	return m_waiting.GetCount() + m_active.GetCount();
	}

//--------------------------------------------------------------------------------
bool CThreadPool::SetThreadActive(CWinThread* pThread)
	{
	CSingleLock lock(&m_waiting.m_mutex, false);
	if(! lock.Lock(10000))
		return false;

	CSingleLock lock2(&m_active.m_mutex, false);
	if(! lock2.Lock(10000))
		return false;

	POSITION pos = m_waiting.Find(pThread);
	if(pos != NULL)
		m_waiting.RemoveAt(pos);

	pos = m_active.Find(pThread);
	if(pos != NULL)
		m_active.RemoveAt(pos);

	return m_active.AddHead(pThread) != NULL;
	}

//--------------------------------------------------------------------------------
bool CThreadPool::SetThreadWaiting(CWinThread* pThread)
	{
	CSingleLock lock(&m_waiting.m_mutex, false);
	if(! lock.Lock(10000))
		return false;

	CSingleLock lock2(&m_active.m_mutex, false);
	if(! lock2.Lock(10000))
		return false;

	POSITION pos = m_waiting.Find(pThread);
	if(pos != NULL)
		m_waiting.RemoveAt(pos);

	pos = m_active.Find(pThread);
	if(pos != NULL)
		m_active.RemoveAt(pos);

	return m_waiting.AddHead(pThread) != NULL;
	}

//--------------------------------------------------------------------------------
CWinThread* CThreadPool::GetNextWaiting()
	{
	CSingleLock lock(&m_waiting.m_mutex, false);
	if(! lock.Lock(10000))
		return NULL;

	if(m_waiting.GetCount() == 0)
		return NULL;

	POSITION pos = m_waiting.GetHeadPosition();
	if(pos == NULL)
		return NULL;

	CWinThread* pThread = m_waiting.GetAt(pos);
	m_waiting.RemoveAt(pos);
	return pThread;
	}

//--------------------------------------------------------------------------------
void CThreadPool::DoInit(DWORD nCount, CRuntimeClass* pClass)
	{
	CSingleLock lock(&m_waiting.m_mutex, false);
	if(! lock.Lock(10000))
		return;

	for(UINT i = 0; i < nCount; i++)
		m_waiting.AddTail(::AfxBeginThread(pClass));
	}

//--------------------------------------------------------------------------------
void CThreadPool::DoMsg(CThreadPoolMsg* pMsg, LPARAM)
	{
	// attempt to move a thread from waiting to active, then send it the msg
	// if there are no avail waiting threads, then keep sending ourselves the msg
	// until a thread becomes available
	CWinThread* pThread = GetNextWaiting();
	if(pThread == NULL)
		{
		::Sleep(20);
		PostThreadMessage(TPMSG_MESSAGE, (WPARAM) pMsg, 0);
		return;
		}

	SetThreadActive(pThread);
	pThread->PostThreadMessage(pMsg->m_nMsg, pMsg->m_wparam, pMsg->m_lparam);
	delete pMsg;
	}
