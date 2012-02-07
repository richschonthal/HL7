//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "ProtocolObj.h"

#include <allocstring.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//--------------------------------------------------------------------------------
IMPLEMENT_SERIAL(CProtocolObj, CObject, 1)

//--------------------------------------------------------------------------------
CProtocolObjList::CProtocolObjList()
		: m_pLock(NULL)
	{
	}

//--------------------------------------------------------------------------------
CProtocolObjList::~CProtocolObjList()
	{
	////AFX_MANAGE_STATE(AfxGetStaticModuleState());

	Unlock();
	Lock();

	for(POSITION pos = GetTailPosition(); pos; GetPrev(pos))
		delete GetAt(pos);
	RemoveAll();

	Unlock();
	}

//--------------------------------------------------------------------------------
bool CProtocolObjList::IsLocked() const
	{
	if(m_pLock != NULL)
		return m_pLock->IsLocked() != 0;
	return false;
	}

//--------------------------------------------------------------------------------
bool CProtocolObjList::Lock()
	{
	if(m_pLock != NULL)
		return false;

	m_pLock = new CSingleLock(&m_mutex, false);
	if(! m_pLock->Lock())
		{
		delete m_pLock;
		m_pLock = NULL;
		}

	return m_pLock != NULL;
	}

//--------------------------------------------------------------------------------
bool CProtocolObjList::Unlock()
	{
	if(m_pLock == NULL)
		return false;

	if(! IsLocked())
		return false;

	m_pLock->Unlock();
	delete m_pLock;
	m_pLock = NULL;
	return true;
	}

//--------------------------------------------------------------------------------
void CProtocolObjList::RemoveAt(POSITION position)
	{
	if(! IsLocked())
		return;
	m_data.RemoveAt(position);
	}

//--------------------------------------------------------------------------------
void CProtocolObjList::RemoveAll()
	{
	if(! IsLocked())
		return;
	m_data.RemoveAll();
	}

//--------------------------------------------------------------------------------
POSITION CProtocolObjList::GetTailPosition() const
	{
	if(! IsLocked())
		return NULL;
	return m_data.GetTailPosition();
	}

//--------------------------------------------------------------------------------
POSITION CProtocolObjList::GetHeadPosition() const
	{
	if(! IsLocked())
		return NULL;
	return m_data.GetHeadPosition();
	}

//--------------------------------------------------------------------------------
CProtocolObj* CProtocolObjList::GetPrev(POSITION& rPosition) const
	{
	if(! IsLocked())
		return NULL;
	return m_data.GetPrev(rPosition);
	}

//--------------------------------------------------------------------------------
CProtocolObj* CProtocolObjList::GetAt(POSITION& rPosition) const
	{
	if(! IsLocked())
		return NULL;
	return m_data.GetAt(rPosition);
	}

//--------------------------------------------------------------------------------
CProtocolObj* CProtocolObjList::GetNext(POSITION& rPosition) const
	{
	if(! IsLocked())
		return NULL;
	return m_data.GetNext(rPosition);
	}

//--------------------------------------------------------------------------------
POSITION CProtocolObjList::AddHead(CProtocolObj* newElement)
	{
	if(! IsLocked())
		return NULL;
	return m_data.AddHead(newElement);
	}

//--------------------------------------------------------------------------------
POSITION CProtocolObjList::AddTail(CProtocolObj* newElement)
	{
	if(! IsLocked())
		return NULL;
	return m_data.AddTail(newElement);
	}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
CProtocolObj::CProtocolObj()
	{
	////AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_nCommand = UNKNOWN;
	for(int i = 0; i < MAX_PARAMS; i++)
		m_pParam[i] = NULL;
	}

//--------------------------------------------------------------------------------
CProtocolObj::CProtocolObj(const CProtocolObj& rVal)
	{
	////AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(&rVal == this)
		return;

	for(int i = 0; i < MAX_PARAMS; i++)
		m_pParam[i] = NULL;

	m_nCommand = rVal.m_nCommand;
	for(i = 0; i < MAX_PARAMS; i++)
		SetParam(i, rVal.m_pParam[i]);
	}

//--------------------------------------------------------------------------------
CProtocolObj::CProtocolObj(eCommand nCmd, LPCTSTR p1, LPCTSTR p2, LPCTSTR p3, LPCTSTR p4)
	{
	////AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_nCommand = nCmd;

	for(int i = 0; i < MAX_PARAMS; i++)
		m_pParam[i] = NULL;

	ALLOC_STRING(m_pParam[0], p1);
	ALLOC_STRING(m_pParam[1], p2);
	ALLOC_STRING(m_pParam[2], p3);
	ALLOC_STRING(m_pParam[3], p4);
	}

//--------------------------------------------------------------------------------
CProtocolObj::~CProtocolObj()
	{
	////AFX_MANAGE_STATE(AfxGetStaticModuleState());

	for(int i = 0; i < MAX_PARAMS; i++)
		delete m_pParam[i];
	}

//--------------------------------------------------------------------------------
CProtocolObj::operator=(const CProtocolObj& rVal)
	{
	////AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_nCommand = rVal.m_nCommand;
	for(int i = 0; i < MAX_PARAMS; i++)
		SetParam(i, rVal.m_pParam[i]);
	}

//--------------------------------------------------------------------------------
void CProtocolObj::SetCommand(eCommand cmd, bool bClearAll)
	{
	////AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_nCommand = cmd;

	if(! bClearAll)
		return;

	for(int i = 0; i < MAX_PARAMS; i++)
		{
		delete m_pParam[i];	
		m_pParam[i] = NULL;
		}
	}

//--------------------------------------------------------------------------------
int CProtocolObj::GetParamCount()
	{
	////AFX_MANAGE_STATE(AfxGetStaticModuleState());

	for(int i = 0; i < MAX_PARAMS; i++)
		{
		if(m_pParam[i] == NULL)
			break;
		}

	return i;
	}

//--------------------------------------------------------------------------------
LPCTSTR CProtocolObj::GetParam(int nIndex)
	{
	////AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(nIndex < 0 || nIndex >= MAX_PARAMS)
		return NULL;
	return m_pParam[nIndex];
	}

//--------------------------------------------------------------------------------
void CProtocolObj::SetParam(int nIndex, LPCTSTR pParam)
	{
	////AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(nIndex < 0 || nIndex >= MAX_PARAMS)
		return;
	if(pParam == NULL)
		{
		delete m_pParam[nIndex];
		m_pParam[nIndex] = NULL;
		return;
		}

	ALLOC_STRING(m_pParam[nIndex], pParam);
	}

//--------------------------------------------------------------------------------
void CProtocolObj::GetTraceString(CString& sOut)
	{
	////AFX_MANAGE_STATE(AfxGetStaticModuleState());

	char* pCmdNames[] =
		{
		"UNKNOWN",
		"HELLO",
		"GOODBYE",
		"STATUS",
		"PAUSE",
		"CONTINUE",
		"ACK",
		"NACK",
		"HL7MSG",
		"GO",
		"OBJECT"
		};

	sOut = "cmd=";
	int nIndex = (int) m_nCommand;
	if(nIndex >= 0 && nIndex < (sizeof(pCmdNames) / sizeof(char*)))
		sOut += pCmdNames[nIndex];
	else
		sOut.Format("cmd=%d", nIndex);

	sOut += " p(0)=\"";
	sOut += m_pParam[0];
	sOut += "\" p(1)=\"";
	sOut += m_pParam[1];
	sOut += "\" p(2)=\"";
	sOut += m_pParam[2];
	sOut += "\" p(3)=\"";
	sOut += m_pParam[3];
	sOut += '"';
	}

