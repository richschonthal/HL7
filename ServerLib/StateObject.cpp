//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "StateObject.h"
#include "SystemObject.h"

#include <ReadLock.h>
#include <WriteLock.h>

//--------------------------------------------------------------------------------
#include "waitlen.h"

//--------------------------------------------------------------------------------
CStateObject::CStateObject(int nValue)
		: m_nClients(0)
		, m_nClientsInSync(0)
		, m_nValue(nValue)
		, m_evtClientSync(false, true)
		, m_evtChanged(false, true)
	{
	}

//--------------------------------------------------------------------------------
CStateObject::~CStateObject()
	{
	}

//--------------------------------------------------------------------------------
bool CStateObject::SetValue(int nValue)
	{
	int nPrev = ::InterlockedExchange(&m_nValue, nValue);
	if(nPrev != m_nValue)
		{
		m_evtClientSync.ResetEvent();
		m_evtChanged.SetEvent();
		}

	return true;
	}

//--------------------------------------------------------------------------------
int CStateObject::GetValue()
	{
	return (int) m_nValue;
	}

//--------------------------------------------------------------------------------
void CStateObject::AddClient()
	{
	::InterlockedIncrement(&m_nClients);
	}

//--------------------------------------------------------------------------------
void CStateObject::RemoveClient()
	{
	::InterlockedDecrement(&m_nClients);
	}

//--------------------------------------------------------------------------------
void CStateObject::AcknowledgeChange()
	{
	if(m_nClients == 0)
		return;

	if(::InterlockedIncrement(&m_nClientsInSync) >= m_nClients)
		{
		m_nClientsInSync = 0;
		m_evtClientSync.SetEvent();
		m_evtChanged.ResetEvent();
		}
	}

#ifdef _DEBUG
//--------------------------------------------------------------------------------
static struct tagStateNames
	{
	int nState;
	LPCTSTR pName;
	} g_stateNames[] =
	{
	STATE_UNKNOWN, _T("STATE_UNKNOWN"),
	STATE_INIT, _T("STATE_INIT"),
	STATE_RUN, _T("STATE_RUN"),
	STATE_PAUSE, _T("STATE_PAUSE"),
	STATE_STOP, _T("STATE_STOP"),
	STATE_EXIT, _T("STATE_EXIT"),
	STATE_ERROR, _T("STATE_ERROR"),
	STATE_NOSTATE, _T("STATE_NOSTATE"),
	STATE_UNATTAINED, _T("STATE_UNATTAINED"),
	0, NULL
	};

//--------------------------------------------------------------------------------
LPCTSTR CStateObject::GetStateName(int nState) const
	{
	LPCTSTR sTemp = _T("");
	for(int i = 0; ; i++)
		{
		if(g_stateNames[i].pName == NULL)
			break;
		if(g_stateNames[i].nState == nState)
			{
			sTemp = g_stateNames[i].pName;
			break;
			}
		}

	return sTemp;
	}

#endif
