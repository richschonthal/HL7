//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "HL7System.h"
#include <System.h>
#include "HL7IOSubSystem.h"

//--------------------------------------------------------------------------------
CHL7System::CHL7System()
	{
	SetState(STATE_INIT);
	AddSubSystem(m_pIO = new CHL7IOSubSystem(this));
	}

//--------------------------------------------------------------------------------
CHL7System::~CHL7System()
	{
	}

//--------------------------------------------------------------------------------
#ifdef _DEBUG
LPCTSTR CHL7System::DebugGetSystemObjectName()
	{
	return "CHL7System";
	}
#endif
