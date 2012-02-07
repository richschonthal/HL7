//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "output.h"
#include "HL7DBMsgEvent.h"

#include "globals.h"

#include "PACSDatabase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//--------------------------------------------------------------------------------
CHL7DBMsgEvent::CHL7DBMsgEvent(CPACSDatabase& db, long nMsgNo, long nMsgId, long nStatus)
	{
	m_nMsgNum = nMsgNo;
	m_nMsgId = nMsgId;

	CString sTemp;
	sTemp.Format("insert into %sHL7_Msg_Event values (%ld, %ld, %ld)",
		(LPCTSTR) db.GetTableAccountName(), nMsgNo, nMsgId, nStatus);

	db.GetDBPtr()->BeginTrans();
	GetIO()->FormatOutput(IOMASK_3, "CHL7DBMsgEvent::CHL7DBMsgEvent\t%s", (LPCTSTR) sTemp);
	db.GetDBPtr()->ExecuteSQL(sTemp);
	db.GetDBPtr()->CommitTrans();
	}

//--------------------------------------------------------------------------------
CHL7DBMsgEvent::~CHL7DBMsgEvent()
	{

	}

//--------------------------------------------------------------------------------
long CHL7DBMsgEvent::QueryStatus(CPACSDatabase& db)
	{
	CRecordset rs(db.GetDBPtr());

	CString sTemp;

	sTemp.Format("select status from %sHL7_Msg_Event where msgno = %ld and msgid = %ld", 
		(LPCTSTR) db.GetTableAccountName(), m_nMsgNum, m_nMsgId);

	GetIO()->FormatOutput(IOMASK_3, "CHL7DBMsgEvent::QueryStatus\t%s", (LPCTSTR) sTemp);

	if(! rs.Open(CRecordset::forwardOnly, sTemp))
		{
		SetResultCode(CResult::errOS);
		return -32767;
		}

	CDBVariant vTemp;
	rs.GetFieldValue((int) 0, vTemp, SQL_C_SLONG);

	rs.Close();

	return vTemp.m_lVal;
	}
