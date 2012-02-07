//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#include "stdafx.h"
#include "SystemObject.h"

#ifdef _DEBUG
#include "IOThread.h"
#endif

//--------------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CSystemObject, CObject);

//--------------------------------------------------------------------------------
CSystemObject::CSystemObject(CSystemObject* pParent)
		: m_pParent(pParent)
		, m_pState(NULL)
	{
	}

//--------------------------------------------------------------------------------
CSystemObject::~CSystemObject()
	{
	delete m_pState;
	}

//--------------------------------------------------------------------------------
bool CSystemObject::PostMessage(UINT, WPARAM, LPARAM)
	{
	return false;
	}

//--------------------------------------------------------------------------------
CSystemObject* CSystemObject::GetParent()
	{
	return m_pParent;
	}

//--------------------------------------------------------------------------------
DWORD CSystemObject::GetState()
	{
	if(m_pParent == NULL && m_pState == NULL)
		return STATE_RUN;
	if(m_pState == NULL)
		return STATE_UNKNOWN;
	return m_pState->GetValue();
	}

//--------------------------------------------------------------------------------
CStateObject* CSystemObject::GetStatePtr()
	{
	return m_pState;
	}

//--------------------------------------------------------------------------------
void CSystemObject::AddStateClient()
	{
	if(m_pState != NULL)
		m_pState->AddClient();
	}

//--------------------------------------------------------------------------------
void CSystemObject::RemoveStateClient()
	{
	if(m_pState != NULL)
		m_pState->RemoveClient();
	}

//--------------------------------------------------------------------------------
void CSystemObject::AcknowledgeStateChange()
	{
	if(m_pState != NULL)
		m_pState->AcknowledgeChange();
	}

//--------------------------------------------------------------------------------
int CSystemObject::GetObjectId()
	{
	return -1;
	}
