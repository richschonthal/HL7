//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#if !defined(AFX_HL7DBMSGEVENT_H__8AD79FBA_018C_11D3_87A7_00104B9E6286__INCLUDED_)
#define AFX_HL7DBMSGEVENT_H__8AD79FBA_018C_11D3_87A7_00104B9E6286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <result.h>

//--------------------------------------------------------------------------------
class CPACSDatabase;

//--------------------------------------------------------------------------------
class CHL7DBMsgEvent  : public CResult
	{
	public:
		CHL7DBMsgEvent(CPACSDatabase&, long, long, long);
		virtual ~CHL7DBMsgEvent();

	private:
		long m_nMsgNum;
		long m_nMsgId;

	public:
		long GetMsgNo() const	{return m_nMsgNum;}
		long GetMsgId() const	{return m_nMsgId;}

		long QueryStatus(CPACSDatabase&);
	};

#endif // !defined(AFX_HL7DBMSGEVENT_H__8AD79FBA_018C_11D3_87A7_00104B9E6286__INCLUDED_)
