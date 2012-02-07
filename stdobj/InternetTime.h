//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _INTERNETTIME_H_
#define _INTERNETTIME_H_

//--------------------------------------------------------------------------------
class CInternetTime
	{
	private:
		COleDateTime m_otTime;

	public:
		static long m_nLastTriedServer;
		static struct tagServers
			{
			LPCTSTR pURL;
			LPCTSTR pIP;
			LPCTSTR pName;
			int nPort;
			} m_servers[12];

		enum
			{
			// flags getting time
			AdjustToLocalTime = 1,
			AdjustForDST = 2,
			FlagDef = 3,

			// misc
			ServerCount = 12,

			// flags for which server to use
			TryAllServers = -1,
			NextAvailServer = -2,
			};

	public:
		CInternetTime();
		CInternetTime(int nServer);
		CInternetTime(LPCTSTR pIP, int nPort = 13);

		operator COleDateTime();

		bool GetCurrentNetTime(BYTE nFlags = FlagDef, int nServer = -1);
		static bool GetCurrentNetTime(COleDateTime&, BYTE nFlags = FlagDef, LPCTSTR pIP = NULL, int nPort = 13);

		static bool ParseNetworkTimeString(const CString&, COleDateTime&, BYTE nFlags = FlagDef);
		static bool ParseNetworkTimeString(LPCTSTR, COleDateTime&, BYTE nFlags = FlagDef);
	};

#endif //_INTERNETTIME_H_
