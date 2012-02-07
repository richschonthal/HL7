//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "Output.h"
#include "ThreadListAccess.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//--------------------------------------------------------------------------------
CThreadListAccess::CThreadListAccess(COutputApp* pOutput)
		: m_lock(&pOutput->m_threads.m_mutex)
	{
	if(! m_lock.Lock(60000))
		m_pThreads = NULL;
	else
		m_pThreads = &pOutput->m_threads;
	}

//--------------------------------------------------------------------------------
CThreadListAccess::~CThreadListAccess()
	{
	m_lock.Unlock();
	}
