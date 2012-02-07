//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#include "stdafx.h"
#include "StateObject.h"
#include "SystemControlObject.h"

//--------------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CSystemControlObject, CSystemObject)

//--------------------------------------------------------------------------------
CSystemControlObject::CSystemControlObject(CSystemObject* pParent)
		: CSystemObject(pParent)
	{
	}

//--------------------------------------------------------------------------------
CSystemControlObject::~CSystemControlObject()
	{
	}

//--------------------------------------------------------------------------------
bool CSystemControlObject::SetState(DWORD nState)
	{
	if(m_pState == NULL)
		return false;

	return m_pState->SetValue(nState);
	}

//--------------------------------------------------------------------------------
bool CSystemControlObject::WaitForStateSync(DWORD nWait)
	{
	if(m_pState == NULL)
		return false;

	if(m_pState->m_nClients == 0)
		return true;

	return ::WaitForSingleObject(m_pState->m_evtClientSync, nWait) == WAIT_OBJECT_0;
	}

//--------------------------------------------------------------------------------
void CSystemControlObject::OnStateSync()
	{
	}
