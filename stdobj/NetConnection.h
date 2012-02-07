//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _NETCONNECTION_H_
#define _NETCONNECTION_H_

//--------------------------------------------------------------------------------
#include "result.h"

//--------------------------------------------------------------------------------
class CNetConnection : public CResult
	{
	public:
		CString m_sServer;
		LPTSTR m_pUser;
		LPTSTR m_pPass;

		CNetConnection();
		virtual ~CNetConnection();

		void SetUser(LPCTSTR);
		void SetPass(LPCTSTR);

		bool ReadSettings(CRegKey&);
		bool WriteSettings(CRegKey&);

		bool GetNetworkConnection() const;
	};

#endif//_NETCONNECTION_H_
