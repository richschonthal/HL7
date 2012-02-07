//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#if !defined(AFX_SYSTEMOBJECT_H__9F10BA26_835B_11D3_B0D3_00A0CC271D0D__INCLUDED_)
#define AFX_SYSTEMOBJECT_H__9F10BA26_835B_11D3_B0D3_00A0CC271D0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
#include "result.h"
#include "StateObject.h"

#include <LockedList.h>

//--------------------------------------------------------------------------------
enum
	{
	SO_DEFAULT_PULSE_INTERVAL = 2000,
	// error codes
	SO_NULL_PARENT = -1
	};

//--------------------------------------------------------------------------------
class CSystemConfig;
class CSystemControlObject;

//--------------------------------------------------------------------------------
class CSystemObject : public CObject, public CResult
	{
	friend class CThreadObject;
	friend class CSubSystemBase;

	DECLARE_DYNAMIC(CSystemObject);

	public:
		// general purpose object lock
		CMutex m_mutex;
		// general purpose signal
		CEvent m_event;
		// signaled when this system object
		// throws an exception
		CEvent m_evtException;

	public:
		CSystemObject(CSystemObject* pParent);
		virtual ~CSystemObject();

	// attributes
	protected:
		// the parent of this object or null if this is a root object
		CSystemObject* m_pParent;
		// the state - a non systemcontrol object will be given a local state
		// by a systemcontrol object
		CStateObject* m_pState;

	public:
		// returns the parent of this object or null if this is a root object
		CSystemObject* GetParent();

		// state
		//--------------------------------------------------------------------------------
		// returns the state of the controlling parent
		virtual DWORD GetState();

		virtual CStateObject* GetStatePtr();

		virtual void AddStateClient();
		virtual void RemoveStateClient();
		virtual void AcknowledgeStateChange();

		// messaging
		//--------------------------------------------------------------------------------
		virtual bool PostMessage(UINT, WPARAM = 0, LPARAM = 0);

		// object id type masks
		enum
			{
			OIDTM_THREAD =    0x10000000,
			OIDTM_SUBSYSTEM = 0x20000000,
			OIDTM_SYSTEM =    0x40000000
			};

		// serverlib object ids
		enum
			{
			SLOID_IO	= 0x00010000,
			SLOID_DB	= 0x00020000,
			SLOID_CN	= 0x00040000,
			SLOID_FTP	= 0x00080000
			};

		// ID format:
		// bit 31 - default (invalid) id is -1
		// bit 30 - object "type" (see above enum)
		// bit 29-16 - ServerLib default object id range
		// bit 15-0 - application id range
		// identification
		virtual int GetObjectId();
	};

//--------------------------------------------------------------------------------
class CSystemObjectList : public CLockedList<CSystemObject*, CSystemObject*>
	{
	public:
		CSystemObjectList(int nBlock = 10);
		virtual ~CSystemObjectList();

		POSITION Find(CSystemObject*, POSITION = NULL) const;
	};

#endif // !defined(AFX_SYSTEMOBJECT_H__9F10BA26_835B_11D3_B0D3_00A0CC271D0D__INCLUDED_)
