//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _DEFS_H_
#define _DEFS_H_

// registry defs
#define REG_VERSION_3_ROOT				"SOFTWARE\\@COMPANY\\HL7\\Version 3"

#define REG_VALNAME_LOGFILE				"Log"
#define REG_VALNAME_TRACELEVEL			"TraceLevel"
#define REG_VALNAME_TRACELEVEL_FLAGS	"TraceLevelFlags"
#define REG_VALNAME_FLAGS				"Flags"
#define REG_VALNAME_NAME				"Name"
#define REG_VALNAME_TYPE				"Type"

#define REG_OUTPUT_KEYNAME				"HL7OutputDaemon"
#define REG_MONITOR_KEYNAME				"Monitors"

#define REG_OUTPUT_DBKEYNAME			"DBKeyName"
#define REG_OUTPUT_MONKEYNAME			"MonKeyName"

#define REG_OUTPUT_VALNAME_DB_ODBC		"ConnectString"
#define REG_OUTPUT_VALNAME_DB_TYPE		"dbtype"
#define REG_OUTPUT_VALNAME_DB_PROC		"ProcedureAccount"
#define REG_OUTPUT_VALNAME_DB_TABLE		"TableAccount"

#define REG_MONITOR_VALNAME_INPORT		"InPort"
#define REG_MONITOR_VALNAME_OUTPORT		"OutPort"
#define REG_MONITOR_VALNAME_INADDR		"InAddr"
#define REG_MONITOR_VALNAME_OUTADDR		"OutAddr"
#define REG_MONITOR_VALNAME_INPATH		"InPath"
#define REG_MONITOR_VALNAME_OUTPATH		"OutPath"
#define REG_MONITOR_VALNAME_VERSION		"Version"
#define REG_MONITOR_VALNAME_INTNAME		"InternalName"
#define REG_MONITOR_VALNAME_TOKEN_ID	"SecServTokenId"
#define REG_MONITOR_VALNAME_MAXIDLE		"MaxIdle"

#define REG_SECSERV_MAIN_IP				"SecServMainIP"
#define REG_SECSERV_MAIN_PORT			"SecServMainPort"
#define REG_SECSERV_BKUP_IP				"SecServBkupIP"
#define REG_SECSERV_BKUP_PORT			"SecServBkupPort"

#endif
