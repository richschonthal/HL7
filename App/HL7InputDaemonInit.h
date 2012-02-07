//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------
// HL7InputDaemonInit.h: interface for the CHL7InputDaemon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HL7INPUTDAEMONINIT_H__A5625AAC_F0F9_11D2_87A6_00104B9E6286__INCLUDED_)
#define AFX_HL7INPUTDAEMONINIT_H__A5625AAC_F0F9_11D2_87A6_00104B9E6286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
#include <result.h>
#include <allocstring.h>
#include <registry.h>
#include <bit.h>
#include <commondefines.h>

//--------------------------------------------------------------------------------
class CHL7InputDaemonInit : public CObject, public CResult
	{
	public:
		enum eInputType
			{
			INPUT_OFF,
			INPUT_SOCKET,
			INPUT_SERIAL,
			INPUT_FILE,
			INPUT_DATABASE
			};

		enum
			{
			// bit flags
			FLAG_ISCLIENT = 0x01,
			FLAG_ISOUTMODULE = 0x02,
			FLAG_ISDEBUG = 0x04,
			FLAG_ALL = 0xFFFFFFFF
			};

	private:
		DWORD m_nProcessId;
		LPTSTR m_pKeyName;
		LPTSTR m_pName;
		LPTSTR m_pLogFile;
		LPTSTR m_pInternalName;
		DWORD m_nFlags;
		int m_nVersion;
		DWORD m_nTokenId;
		DWORD m_nMaxIdle;

		eInputType m_nInputType;

		union
			{
			struct
				{
				DWORD nPort;
				LPTSTR pAddress;
				} socket;

			struct
				{
				DWORD nPort;
				DWORD nBaud;
				} serial;

			struct
				{
				LPTSTR pPath;
				LPTSTR pOutPath;
				} file;

			struct
				{
				LPTSTR pDB;
				} database;
			} m_config;

		struct
			{
			DWORD nPort;
			LPTSTR pAddress;
			} m_outconfig;

	private:
		inline void InitMembers()
			{
			m_pName = m_pKeyName = m_pLogFile = m_pInternalName = NULL;
			::ZeroMemory(&m_config, sizeof(m_config));
			::ZeroMemory(&m_outconfig, sizeof(m_outconfig));
			m_nFlags = 0;
			m_nInputType = INPUT_OFF;
			m_nVersion = 0;
			m_nTokenId = 0;
			// max time a socket is idle before considered dead
			// 10000 = 10 seconds- the default value
			m_nMaxIdle = 10000;
			}

	public:
		CHL7InputDaemonInit();
		CHL7InputDaemonInit(DWORD);
		CHL7InputDaemonInit(long, LPCTSTR, bool = false);
		CHL7InputDaemonInit(int, int);
		CHL7InputDaemonInit(LPCTSTR, LPCTSTR);
		CHL7InputDaemonInit(CRegKey&);

		// copy stuff
		CHL7InputDaemonInit(const CHL7InputDaemonInit&);
		void operator=(const CHL7InputDaemonInit&);

		~CHL7InputDaemonInit();

		bool ReadConfig(CRegKey&);
		bool WriteConfig(LPCTSTR = NULL);

	public:
		void SetSocket(LPCTSTR pAddr, int nPort, bool bIsClient = false);
		void SetFile(LPCTSTR pPath, LPCTSTR pOutPath);

		// bool fields
		inline void SetFlag(DWORD nFlag, bool bSetIt)
			{ m_nFlags = BitSet(BitClr(m_nFlags, nFlag), nFlag * BoolToInt(bSetIt)); }

		void SetIsClient(bool b)	{ SetFlag(FLAG_ISCLIENT, b); }
		bool IsClient() const		{ return BoolFromInt(m_nFlags & FLAG_ISCLIENT);}

		void SetIsOutModule(bool b)	{ SetFlag(FLAG_ISOUTMODULE, b); }
		bool IsOutModule() const	{ return BoolFromInt(m_nFlags & FLAG_ISOUTMODULE);}

		void SetIsDebug(bool b)		{ SetFlag(FLAG_ISDEBUG, b); }
		bool IsDebug() const		{ return BoolFromInt(m_nFlags & FLAG_ISDEBUG); }

		eInputType GetType() const	{ return m_nInputType; }
		void SetType(eInputType t)	{ m_nInputType = t; }

		// socket stuff
		int GetPort() const			{ return m_config.socket.nPort; }
		void SetPort(int n)			{ m_config.socket.nPort = n; }

		LPCTSTR GetAddress() const	{ return m_config.socket.pAddress; }

		// serial stuff
		int GetBaud() const			{ return m_config.serial.nBaud; }

		LPCTSTR GetPath() const		{ return m_config.file.pPath; }
		void SetPath(LPCTSTR p)		{ ALLOC_STRING(m_config.file.pPath, p); }

		LPCTSTR GetOutPath() const	{ return m_config.file.pOutPath; }
		void SetOutPath(LPCTSTR p)	{ ALLOC_STRING(m_config.file.pOutPath, p); }

		void SetOutPort(int n)		{ m_outconfig.nPort = n; }
		int GetOutPort() const		{ return m_outconfig.nPort; }

		void SetOutAddr(LPCTSTR p)	{ ALLOC_STRING(m_outconfig.pAddress, p); }
		LPCTSTR GetOutAddr() const	{ return m_outconfig.pAddress; }

		void SetName(LPCTSTR p)		{ ALLOC_STRING(m_pName, p); }
		LPCTSTR GetName() const		{ return m_pName; }

		void SetInternalName(LPCTSTR p)	{ ALLOC_STRING(m_pInternalName, p); }
		LPCTSTR GetInternalName()	{ return m_pInternalName; }

		void SetLogFile(LPCTSTR p)	{ ALLOC_STRING(m_pLogFile, p); }
		LPCTSTR GetLogFile() const	{ return m_pLogFile; }

		void SetVersion(int n)		{ m_nVersion = n; }
		int GetVersion() const		{ return m_nVersion; }

		void SetProcessId(DWORD nId) { m_nProcessId = nId; }
		DWORD GetProcessId()		{ return m_nProcessId; }

		void SetTokenId(DWORD nId)	{ m_nTokenId = nId; }
		DWORD GetTokenId() const	{ return m_nTokenId; }

		void SetMaxIdle(DWORD n)	{ m_nMaxIdle = n; }
		DWORD GetMaxIdle() const	{ return m_nMaxIdle; }
	};

//--------------------------------------------------------------------------------
class CHL7InputDaemonInitList : public CTypedPtrArray<CObArray, CHL7InputDaemonInit*>, public CResult
	{
	public:
		CHL7InputDaemonInitList();
		~CHL7InputDaemonInitList()
			{
			for(int i = GetUpperBound(); i >= 0; i--)
				delete GetAt(i);
			};

		bool WriteList(LPCTSTR pKey = NULL);
	};

#endif // !defined(AFX_HL7INPUTDAEMONINIT_H__A5625AAC_F0F9_11D2_87A6_00104B9E6286__INCLUDED_)
