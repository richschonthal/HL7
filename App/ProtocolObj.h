//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
// ProtocolObj.h: interface for the CProtocolObj class.
//
//--------------------------------------------------------------------------------

#if !defined(AFX_PROTOCOLOBJ_H__7424D852_F7F8_11D2_87A7_00104B9E6286__INCLUDED_)
#define AFX_PROTOCOLOBJ_H__7424D852_F7F8_11D2_87A7_00104B9E6286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <archiveobj.h>

#include <commondefines.h>

//--------------------------------------------------------------------------------
class CProtocolObj : public CObject, public CResult
	{
	DECLARE_SERIAL(CProtocolObj)

	public:
		enum eCommand
			{
			UNKNOWN,
			HELLO,
			GOODBYE,
			STATUS,
			PAUSE,
			CONTINUE,
			ACK,
			NACK,
			HL7MSG,
			GO,
			OBJECT
			};

	public:
		// does nothing
		CProtocolObj();
		CProtocolObj(const CProtocolObj&);
		CProtocolObj(eCommand, LPCTSTR = NULL, LPCTSTR = NULL, LPCTSTR = NULL, LPCTSTR = NULL);
		virtual ~CProtocolObj();

		enum { MAX_PARAMS = 4 };

	private:
		eCommand m_nCommand;

	public:
		LPTSTR m_pParam[MAX_PARAMS];

	public:
		eCommand GetCommand() const	{ return m_nCommand; }
		void SetCommand(eCommand, bool = true);

		int GetParamCount();

		LPCTSTR GetParam(int);
		void SetParam(int, LPCTSTR);

		operator=(const CProtocolObj&);

		void GetTraceString(CString&);
	};

//--------------------------------------------------------------------------------
class CProtocolObjList
	{
	private:
		CMutex m_mutex;
		CTypedPtrList<CObList, CProtocolObj*> m_data;
		CSingleLock* m_pLock;

	public:
		CProtocolObjList();
		~CProtocolObjList();

		bool IsLocked() const;
		bool Lock();
		bool Unlock();

		POSITION GetHeadPosition( ) const;
		POSITION GetTailPosition( ) const;
		CProtocolObj* GetPrev( POSITION& rPosition ) const;
		CProtocolObj* GetAt( POSITION& rPosition ) const;
		CProtocolObj* GetNext( POSITION& rPosition ) const;

		// add before head or after tail
		POSITION AddHead(CProtocolObj* newElement);
		POSITION AddTail(CProtocolObj* newElement);

		void RemoveAt(POSITION position);
		void RemoveAll();
	};

#endif // !defined(AFX_PROTOCOLOBJ_H__7424D852_F7F8_11D2_87A7_00104B9E6286__INCLUDED_)
