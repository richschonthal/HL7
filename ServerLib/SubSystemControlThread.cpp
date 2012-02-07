//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// purpose:
// to syncronize the state of the parent subsystem with the controlling system

//--------------------------------------------------------------------------------
#include "stdafx.h"
#include "SubSystemControlThread.h"

//--------------------------------------------------------------------------------
CSubSystemControlThread::CSubSystemControlThread(CSubSystem* pParent)
		: CThreadObject(pParent->GetParent())
	{
	ASSERT(pParent != NULL);

	m_pSubSystem = pParent;

	ASSERT(m_pSubSystem->GetRuntimeClass() == RUNTIME_CLASS(CSubSystem));

	m_nLastState = 0xffffffff;

	m_bRunning = true;
	}

//--------------------------------------------------------------------------------
CSubSystemControlThread::~CSubSystemControlThread()
	{
	}

//--------------------------------------------------------------------------------
bool CSubSystemControlThread::MainLoop()
	{
	return m_bRunning;
	}

//--------------------------------------------------------------------------------
void CSubSystemControlThread::OnStateChange(DWORD nState)
	{
	GetSystem()->AcknowledgeStateChange();
	m_bRunning = CheckState();
	}
