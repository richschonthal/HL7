//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#if !defined(AFX_SYSTEM_H__9F10BA2C_835B_11D3_B0D3_00A0CC271D0D__INCLUDED_)
#define AFX_SYSTEM_H__9F10BA2C_835B_11D3_B0D3_00A0CC271D0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
#include "SystemControlObject.h"
#include "SubSystem.h"

//--------------------------------------------------------------------------------
typedef CStateObject CSystemState;

//--------------------------------------------------------------------------------
class CSubSystem;
class CSystemConfig;

//--------------------------------------------------------------------------------
class CSystem : public CSystemControlObject
	{
	friend class CSubSystem;

	DECLARE_DYNAMIC(CSystem)

	public:
		CSystem(CSystemControlObject* = NULL);
		virtual ~CSystem();

		enum
			{
			MSG_QUIT = WM_QUIT,
			MSG_INIT = WM_USER,
			MSG_RELOAD,
			MSG_CONFIG_CHANGED,
			MSG_STATE_CHANGED,
			// use MSG_DERIVED_START as the 1st msg num in derived classes
			MSG_DERIVED_START = WM_USER + 200
			};

	protected:
		// the subsystem list - a system can have any number of subsystems
		CList<CSystemControlObject*, CSystemControlObject*> m_subSystemList;

	public:
		// adds a sub system to the sub system list
		bool AddSubSystem(CSubSystem*);
		// posts a message to all sub-systems
		virtual bool PostMessage(UINT, WPARAM = 0, LPARAM = 0);

		CSubSystem* FindSubSystem(int nId);
	};

//--------------------------------------------------------------------------------
#define SUBSYSTEMID_IO			OIDTM_SUBSYSTEM + SLOID_IO
#define SUBSYSTEMID_DB			OIDTM_SUBSYSTEM + SLOID_DB
#define SUBSYSTEMID_FTP			OIDTM_SUBSYSTEM + SLOID_FTP

#endif // !defined(AFX_SYSTEM_H__9F10BA2C_835B_11D3_B0D3_00A0CC271D0D__INCLUDED_)
