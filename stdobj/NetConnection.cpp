//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "NetConnection.h"
#include "allocstring.h"
#include "registry.h"
#include "MiniEncrypt.h"

//--------------------------------------------------------------------------------
CNetConnection::CNetConnection()
	: m_pUser(NULL)
	, m_pPass(NULL)
	{
	}

//--------------------------------------------------------------------------------
CNetConnection::~CNetConnection()
	{
	delete[] m_pUser;
	delete[] m_pPass;
	}

//--------------------------------------------------------------------------------
void CNetConnection::SetUser(LPCTSTR pUser)
	{
	AllocString(m_pUser, pUser);
	}

//--------------------------------------------------------------------------------
void CNetConnection::SetPass(LPCTSTR pPass)
	{
	AllocString(m_pPass, pPass);
	}

//--------------------------------------------------------------------------------
bool CNetConnection::ReadSettings(CRegKey& rk)
	{
	ASSERT(0);
/*
	CRegValueSz rv(rk, _T("NetConnection"));
	if(! rv.CreateKey())
		return false;

	rv.SetValueName(_T("Server"));
	rv.WriteValue(m_sServer);

	rv.SetValueName(_T("User"));	
	if(m_pUser != NULL)
		rv.WriteValue(m_pUser);
	else
		rv.DeleteValue();

	if(m_pPass != NULL)
		{
		if(*m_pPass == 0)
			rv.WriteValue(m_pPass);
		else
			{
			CMiniEncrypt encrypt(m_pPass, _tcslen(m_pPass));
			}
		}
	else
		rv.DeleteValue();
*/
	return false;
	}

//--------------------------------------------------------------------------------
bool CNetConnection::WriteSettings(CRegKey& rk)
	{
	ASSERT(0);
	return false;
	}

//--------------------------------------------------------------------------------
bool CNetConnection::GetNetworkConnection() const
	{
	NETRESOURCE net;
	memset(&net, 0, sizeof(NETRESOURCE));
	net.dwType = RESOURCETYPE_DISK;

	DWORD nResult = WNetAddConnection2(&net, m_pUser, m_pPass, 0);

	// HA HA I'm un-const-ing
	if(nResult != NO_ERROR)
		{
		const_cast<CNetConnection*>(this)->SetResultCode(errOS);
		const_cast<CNetConnection*>(this)->SetOSResult(nResult);
		return false;
		}

	const_cast<CNetConnection*>(this)->SetResultCode(0);
	return true;
	}
