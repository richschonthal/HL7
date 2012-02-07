//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------


// HL7Object.cpp: implementation of the CHL7Object class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HL7Object.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------
CHL7Object::CHL7Object()
	{
	////AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_nHL7Version = VERSION_UNKNOWN;
	}

//--------------------------------------------------------------------------------
CHL7Object::~CHL7Object()
	{
	////AFX_MANAGE_STATE(AfxGetStaticModuleState());


	}

//--------------------------------------------------------------------------------
int CHL7Object::GetMsgId(char sSegmentName[4])
	{
	////AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return 0;
	}

//--------------------------------------------------------------------------------
LPCTSTR CHL7Object::GetFieldDesc(int nMsgId, int nFieldNo)
	{
	////AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return NULL;
	}

//--------------------------------------------------------------------------------
LPCTSTR CHL7Object::GetFieldName(int nMsgId, int nFieldNo)
	{
	////AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return NULL;
	}

