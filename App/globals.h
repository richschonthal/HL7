//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// file to be included when needing globals
// this is not a header file for globals.cpp

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <registry.h>
#include <CommonDefines.h>

#include "Output.h"
#include "HL7System.h"
#include "HL7IOSubSystem.h"

#include "HL7NTService.h"

extern CRegKey g_rkRoot;

CHL7IOSubSystem* GetIO();
COutputApp* GetApp();
CHL7System* GetSystem();
CHL7NTService* GetNTService();

#endif
