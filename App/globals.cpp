//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"

#include <commondefines.h>

#include "output.h"
#include "HL7System.h"
#include "HL7IOSubSystem.h"
#include "HL7NTService.h"

#define _GLOBALS_H_
#include "globals.h"

#include "defs.h"

CRegKey g_rkRoot(HKEY_LOCAL_MACHINE, REG_VERSION_3_ROOT);

COutputApp theApp;
CHL7IOSubSystem* g_pIO = NULL;
CHL7NTService* g_pNTService = new CHL7NTService;

//--------------------------------------------------------------------------------
CHL7IOSubSystem* GetIO()
	{
	if(g_pIO != NULL)
		return g_pIO;
	g_pIO = theApp.GetSystem()->GetIO();
	return g_pIO;
	}

//--------------------------------------------------------------------------------
COutputApp* GetApp()
	{
	return &theApp;
	}

//--------------------------------------------------------------------------------
CHL7System* GetSystem()
	{
	return theApp.GetSystem();
	}

//--------------------------------------------------------------------------------
CHL7NTService* GetNTService()
	{
	return g_pNTService;
	}
