//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "ServerThread.h"
#include "ServerThreadInfo.h"
#include "Monitor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//--------------------------------------------------------------------------------
CServerThreadInfoArray::~CServerThreadInfoArray()
	{
	for(int i = GetUpperBound(); i >= 0; i--)
		delete GetAt(i);
	RemoveAll();
	}

//--------------------------------------------------------------------------------
bool CServerThreadInfoArray::AddMsgToAll(CProtocolObj obj)
	{
	for(int i = 0; i < GetSize(); i++)
		{
	//	CProtocolObj* pObj = new CProtocolObj;
	//	*pObj = obj;
	//	GetAt(i)->GetMsgQueue().AddTail(pObj);
		}

	return true;
	}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
CServerThreadInfo::CServerThreadInfo()
		: m_pThread(NULL)
		, m_pMonitor(NULL)
		, m_ctLastPulse(CTime::GetCurrentTime())
	{

	}

//--------------------------------------------------------------------------------
CServerThreadInfo::~CServerThreadInfo()
	{
	//delete m_pThread;
	}

//--------------------------------------------------------------------------------
CProtocolObj* CServerThreadInfo::GetNextMsg()
	{
	if(! GetMsgQueue().Lock())
		return NULL;
	POSITION pos = GetMsgQueue().GetHeadPosition();
	if(pos == NULL)
		{
		GetMsgQueue().Unlock();
		return NULL;
		}
	CProtocolObj* p = GetMsgQueue().GetAt(pos);
	GetMsgQueue().RemoveAt(pos);
	GetMsgQueue().Unlock();
	return p;
	}
