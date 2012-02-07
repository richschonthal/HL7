//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#include "stdafx.h"
#include "Parameter.h"

//--------------------------------------------------------------------------------
CParameter::CParameter(DWORD nId)
	{
	m_nId = nId;
	}

//--------------------------------------------------------------------------------
CParameter::~CParameter()
	{

	}

//--------------------------------------------------------------------------------
DWORD CParameter::GetId() const
	{
	return m_nId;
	}

//--------------------------------------------------------------------------------
void CParameter::SetId(DWORD nId)
	{
	m_nId = nId;
	}
