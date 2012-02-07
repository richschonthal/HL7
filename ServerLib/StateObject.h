//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#if !defined(AFX_StateObject_H__42D4F267_830D_11D3_AEFF_005004A1C5F3__INCLUDED_)
#define AFX_StateObject_H__42D4F267_830D_11D3_AEFF_005004A1C5F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
#include "result.h"
#include "ReadWriteObject.h"

//--------------------------------------------------------------------------------
enum
	{
	STATE_UNKNOWN = 0,
	// standard states
	STATE_LOWEST = 1,
	STATE_INIT = STATE_LOWEST,
	STATE_RUN,
	STATE_PAUSE,
	STATE_STOP,
	STATE_EXIT,
	STATE_ERROR,
	HIGHEST_STANDARD_STATE = STATE_ERROR,
	// error condition states
	STATE_NOSTATE = -1,
	STATE_UNATTAINED = -2
	};

//--------------------------------------------------------------------------------
class CStateObject : public CResult, public CReadWriteObject
	{
	public:
		CStateObject(int = STATE_UNKNOWN);
		virtual ~CStateObject();

		CMutex m_mutex;
		CEvent m_evtChanged;

		// the # of threads responding to state changes
		long m_nClients;
		// the # of threads that have acknowledged the last state change
		long m_nClientsInSync;
		// event signaled when state sync is reached (ie m_nStateClients == m_nStateInSync)
		CEvent m_evtClientSync;

	private:
		// the state value
		long m_nValue;

	public:
		bool SetValue(int);
		int GetValue();

		void AddClient();
		void RemoveClient();
		void AcknowledgeChange();

#ifdef _DEBUG
		LPCTSTR GetStateName(int) const;
#endif
	};

#endif // !defined(AFX_StateObject_H__42D4F267_830D_11D3_AEFF_005004A1C5F3__INCLUDED_)
