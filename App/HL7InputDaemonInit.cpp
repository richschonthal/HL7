//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// HL7InputDaemonInit.cpp : implementation file
//

//--------------------------------------------------------------------------------
#include "stdafx.h"
#include "HL7InputDaemonInit.h"
#include "defs.h"
#include "globals.h"

#include <IPAddress.h>

//--------------------------------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//--------------------------------------------------------------------------------
CHL7InputDaemonInitList::CHL7InputDaemonInitList()
	{
	CRegKey rkRoot(g_rkRoot);
	if(! rkRoot.OpenKey())
		return;

	CRegValueSz rvKeyName(rkRoot, REG_OUTPUT_MONKEYNAME);
	CString sKeyName;
	rvKeyName.ReadValue(sKeyName);
	if(sKeyName.IsEmpty())
		sKeyName = REG_MONITOR_KEYNAME;
	CRegKey rkConfig(g_rkRoot, sKeyName);

	// create and/or open registry key
	if(! rkConfig.CreateKey())
		{
		(CResult) *this = (CResult) rkConfig;
		return;
		}

	rkConfig.StartEnumFirst();
	for(CRegKey rkEnum; rkConfig.IterateEnumKey(rkEnum); )
		{
		CHL7InputDaemonInit* pNew = new CHL7InputDaemonInit(rkEnum);
		if(! pNew->HasErrors())
			Add(pNew);
		else
			delete pNew;
		}

	return;
	}

//--------------------------------------------------------------------------------
bool CHL7InputDaemonInitList::WriteList(LPCTSTR pKey)
	{
	CString sKeyName;

	// no key name provided so read the one that's stored
	if(pKey == NULL)
		{
		CRegValueSz rvKeyName(g_rkRoot, REG_OUTPUT_MONKEYNAME);
		if(rvKeyName.OpenKey())
			rvKeyName.ReadValue(sKeyName);
		if(sKeyName.IsEmpty())
			sKeyName = REG_MONITOR_KEYNAME;
		}
	else
		sKeyName = pKey;

	// delete the key if it exists
	{
	CRegKey rkMon(g_rkRoot, sKeyName);
	if(rkMon.OpenKey())
		{
		CRegKey rkKey;
		rkMon.StartEnumLast();
		while(rkMon.IterateEnumKey(rkKey))
			rkKey.DeleteKey();
		rkMon.DeleteKey();
		}
	}

	// create the root key ie company/hl7/versionx/monitors
	CRegKey rkMon(g_rkRoot, sKeyName);
	if(! rkMon.CreateKey())
		return false;

	for(int i = 0; i < GetSize(); i++)
		{
		CString sTemp;
		sTemp.Format("%s\\%d", (LPCTSTR) sKeyName, i);
		GetAt(i)->WriteConfig(sTemp);
		}

	return true;
	}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
CHL7InputDaemonInit::CHL7InputDaemonInit()
	{
	InitMembers();
	}

//--------------------------------------------------------------------------------
CHL7InputDaemonInit::CHL7InputDaemonInit(DWORD nType)
	{
	InitMembers();

	m_nInputType = (eInputType) nType;
	}

//---------------------------------------------------------------------------------
CHL7InputDaemonInit::CHL7InputDaemonInit(long nPort, LPCTSTR pAddr, bool bIsClient)
	{
	InitMembers();

	m_nInputType = INPUT_SOCKET;
	m_config.socket.nPort = nPort;

	if(pAddr != NULL)
		{
		m_config.socket.pAddress = new char[strlen(pAddr)+1];
		strcpy(m_config.socket.pAddress, pAddr);
		}

	SetIsClient(bIsClient);
	}

//--------------------------------------------------------------------------------
CHL7InputDaemonInit::CHL7InputDaemonInit(int nPort, int nBaud)
	{
	InitMembers();

	m_nInputType = INPUT_SERIAL;
	m_config.serial.nPort = nPort;
	m_config.serial.nBaud = nBaud;
	}

//--------------------------------------------------------------------------------
CHL7InputDaemonInit::CHL7InputDaemonInit(LPCTSTR pInPath, LPCTSTR pOutPath)
	{
	InitMembers();

	m_nInputType = INPUT_FILE;
	if(pInPath != NULL && *pInPath)
		{
		m_config.file.pPath = new char[strlen(pInPath)+1];
		strcpy(m_config.file.pPath, pInPath);
		}

	if(pOutPath != NULL && *pOutPath)
		{
		m_config.file.pOutPath = new char[strlen(pOutPath)+1];
		strcpy(m_config.file.pOutPath, pOutPath);
		}
	}

//--------------------------------------------------------------------------------
CHL7InputDaemonInit::CHL7InputDaemonInit(CRegKey& rkMain)
	{
	InitMembers();

	ReadConfig(rkMain);
	}

//--------------------------------------------------------------------------------
CHL7InputDaemonInit::CHL7InputDaemonInit(const CHL7InputDaemonInit& rval)
	{
	if(&rval == this)
		return;

	InitMembers();

	*this = rval;
	}

//--------------------------------------------------------------------------------
void CHL7InputDaemonInit::operator=(const CHL7InputDaemonInit& rval)
	{
	if(&rval == this)
		return;

	ALLOC_STRING(m_pKeyName, rval.m_pKeyName);
	ALLOC_STRING(m_pName, rval.m_pName);
	ALLOC_STRING(m_pLogFile, rval.m_pLogFile);
	ALLOC_STRING(m_pInternalName, rval.m_pInternalName);
	m_nFlags = rval.m_nFlags;
	m_nVersion = rval.m_nVersion;
	m_nInputType = rval.m_nInputType;
	m_nTokenId = rval.m_nTokenId;
	m_nMaxIdle = rval.m_nMaxIdle;

	m_outconfig.nPort = rval.m_outconfig.nPort;
	ALLOC_STRING(m_outconfig.pAddress, rval.m_outconfig.pAddress);

	switch(rval.m_nInputType)
		{
		case INPUT_SOCKET:
			m_config.socket.nPort = rval.m_config.socket.nPort;
			ALLOC_STRING(m_config.socket.pAddress, rval.m_config.socket.pAddress);
			break;
		
		case INPUT_SERIAL:
			m_config.serial.nPort = rval.m_config.serial.nPort;
			m_config.serial.nBaud = rval.m_config.serial.nBaud;
			break;

		case INPUT_FILE:
			ALLOC_STRING(m_config.file.pPath, rval.m_config.file.pPath);
			ALLOC_STRING(m_config.file.pOutPath, rval.m_config.file.pOutPath);
			break;

		case INPUT_DATABASE:
			ALLOC_STRING(m_config.database.pDB, rval.m_config.database.pDB);
			break;
		}
	}

//--------------------------------------------------------------------------------
bool CHL7InputDaemonInit::ReadConfig(CRegKey& rkMain)
	{
	if(! rkMain.OpenKey())
		{
		(CResult) *this = (CResult) rkMain;
		return false;
		}

	// read what type of monitor we are
	CRegValueDWord rvNum(rkMain, REG_VALNAME_TYPE);
	DWORD nVal;
	rvNum.ReadValue(&nVal);
	m_nInputType = (eInputType) nVal;

	CRegValueSz rvString(rkMain, "");
	CString sTemp;

	switch(m_nInputType)
		{
		case INPUT_SOCKET:
			{
			rvNum.SetValueName(REG_MONITOR_VALNAME_INPORT);
			rvNum.ReadValue(&m_config.socket.nPort);

			rvString.SetValueName(REG_MONITOR_VALNAME_INADDR);
			rvString.ReadValue(sTemp);
			if(! sTemp.IsEmpty())
				{
				m_config.socket.pAddress = new char[sTemp.GetLength() + 1];
				strcpy(m_config.socket.pAddress, sTemp);
				}
			}
			break;

		case INPUT_FILE:
			{
			rvString.SetValueName(REG_MONITOR_VALNAME_INPATH);
			rvString.ReadValue(sTemp);

			if(! sTemp.IsEmpty())
				{
				m_config.file.pPath = new char[sTemp.GetLength() + 1];
				strcpy(m_config.file.pPath, sTemp);
				}

			rvString.SetValueName(REG_MONITOR_VALNAME_OUTPATH);
			rvString.ReadValue(sTemp);
			if(! sTemp.IsEmpty())
				{
				m_config.file.pOutPath = new char[sTemp.GetLength()+1];
				strcpy(m_config.file.pOutPath, sTemp);
				}
			}
			break;

		default:
			return false;
		}

	rvString.SetValueName(REG_VALNAME_LOGFILE);
	rvString.ReadValue(sTemp);
	sTemp.TrimLeft();
	sTemp.TrimRight();
	ALLOC_STRING(m_pLogFile, sTemp);

	// read common config stuff
	rvNum.SetValueName(REG_VALNAME_FLAGS);
	rvNum.ReadValue(&m_nFlags);

	if(IsOutModule())
		{
		DWORD nVal;
		rvNum.SetValueName(REG_MONITOR_VALNAME_OUTADDR);
		rvNum.ReadValue(&nVal);

		SetOutAddr(CIPAddress::AddrToString(nVal));

		rvNum.SetValueName(REG_MONITOR_VALNAME_OUTPORT);
		rvNum.ReadValue(&m_outconfig.nPort);
		}

	ALLOC_STRING(m_pKeyName, rkMain.GetKeyName());

	rvNum.SetValueName(REG_MONITOR_VALNAME_VERSION);
	rvNum.ReadValue((DWORD*) &m_nVersion);

	rvNum.SetValueName(REG_MONITOR_VALNAME_TOKEN_ID);
	rvNum.ReadValue((DWORD*) &m_nTokenId);

	// one of the few settings to have a default value
	rvNum.SetValueName(REG_MONITOR_VALNAME_MAXIDLE);
	if(! rvNum.ReadValue(&m_nMaxIdle))
		m_nMaxIdle = 30000;

	rvString.SetValueName(REG_MONITOR_VALNAME_INTNAME);
	rvString.ReadValue(sTemp);
	ALLOC_STRING(m_pInternalName, sTemp);

	rvString.SetValueName(REG_VALNAME_NAME);
	rvString.ReadValue(sTemp);
	ALLOC_STRING(m_pName, sTemp);

	return true;
	}

//--------------------------------------------------------------------------------
bool CHL7InputDaemonInit::WriteConfig(LPCTSTR pKeyName)
	{
	CString sKeyName;

	// if we werent passed a key name then see if we have one already
	if(pKeyName == NULL)
		{
		if(m_pKeyName == NULL)
			return false;
		sKeyName = m_pKeyName;
		}
	else
		sKeyName = pKeyName;

	// open our sub-key
	CRegKey rkMain(g_rkRoot, sKeyName);

	if(! rkMain.CreateKey())
		{
		(CResult) *this = (CResult) rkMain;
		return false;
		}

	// write what type of monitor we are
	CRegValueDWord rvNum(rkMain, REG_VALNAME_TYPE);
	rvNum.WriteValue((DWORD) m_nInputType);

	CRegValueSz rvString(rkMain, "");

	switch(m_nInputType)
		{
		case INPUT_SOCKET:
			rvNum.SetValueName(REG_MONITOR_VALNAME_INPORT);
			rvNum.WriteValue((DWORD) m_config.socket.nPort);
			rvString.SetValueName(REG_MONITOR_VALNAME_INADDR);
			rvString.WriteValue(m_config.socket.pAddress);
			break;

		case INPUT_FILE:
			rvString.SetValueName(REG_MONITOR_VALNAME_INPATH);
			rvString.WriteValue(m_config.file.pPath);
			rvString.SetValueName(REG_MONITOR_VALNAME_OUTPATH);
			rvString.WriteValue(m_config.file.pOutPath);
			break;

		default:
			return false;
		}

	rvString.SetValueName(REG_VALNAME_LOGFILE);
	rvString.WriteValue(m_pLogFile);

	// write common config stuff
	rvNum.SetValueName(REG_VALNAME_FLAGS);
	rvNum.WriteValue(m_nFlags);

	if(IsOutModule())
		{
		rvNum.SetValueName(REG_MONITOR_VALNAME_OUTADDR);

		CIPAddress addr(GetOutAddr());
		rvNum.WriteValue((DWORD) addr);

		rvNum.SetValueName(REG_MONITOR_VALNAME_OUTPORT);
		rvNum.WriteValue(m_outconfig.nPort);
		}

	rvNum.SetValueName(REG_MONITOR_VALNAME_VERSION);
	rvNum.WriteValue(m_nVersion);

	rvNum.SetValueName(REG_MONITOR_VALNAME_TOKEN_ID);
	rvNum.WriteValue(m_nTokenId);

	rvNum.SetValueName(REG_MONITOR_VALNAME_MAXIDLE);
	rvNum.WriteValue(m_nMaxIdle);

	rvString.SetValueName(REG_MONITOR_VALNAME_INTNAME);
	rvString.WriteValue(m_pInternalName);

	rvString.SetValueName(REG_VALNAME_NAME);
	rvString.WriteValue(m_pName);

	return true;
	}

//--------------------------------------------------------------------------------
CHL7InputDaemonInit::~CHL7InputDaemonInit()
	{
	delete[] m_pName;
	delete[] m_outconfig.pAddress;
	delete[] m_pInternalName;
	delete[] m_pKeyName;
	delete[] m_pLogFile;

	switch(m_nInputType)
		{
		case INPUT_FILE:
			delete[] m_config.file.pPath;
			delete[] m_config.file.pOutPath;
			break;
		case INPUT_SOCKET:
			delete[] m_config.socket.pAddress;
			break;
		}
	}

