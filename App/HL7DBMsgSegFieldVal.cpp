//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "output.h"
#include "HL7DBMsgSegFieldVal.h"

#include "globals.h"

#include <WordWrap.h>

#include "PACSDatabase.h"
#include "HL7DBMessagedef.h"
#include "HL7DBFieldDef.h"
#include "HL7DBMsgSegDef.h"
#include "HL7DBRuleDef.h"
#include "HL7DBSegmentDef.h"
#include "HL7DBVersionDef.h"
#include "HL7DBNextMsgNum.h"

#include "HL7Message.h"
#include "HL7Segment.h"
#include "HL7MessageConfig.h"

#include "HL7DBProcMsg.h"

#include "HL7CallUID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//--------------------------------------------------------------------------------
CHL7DBMsgSegFieldValItemList* CHL7DBMsgSegFieldVal::m_pMsgSegFieldValList = NULL;
LPCTSTR CHL7DBMsgSegFieldVal::pTableName = _T("HL7_MsgSegFieldVal");

//-----------------------------------------------------------
CHL7DBMsgSegFieldVal::CHL7DBMsgSegFieldVal(CPACSDatabase& database, long nMsgNo)
		: m_posCurrent(NULL)
	{
	m_pMsgSegFieldValList = NULL;

	if(database.HasErrors())
		{
		(CResult) *this = (CResult) database;
		return;
		}

	CRecordset records(database.GetDBPtr());

	CString sStatement;
	
	sStatement.Format("select * from %s where msgno = %ld",
		(LPCTSTR) (database.GetTableAccountName() + pTableName), nMsgNo);

	GetIO()->FormatOutput(IOMASK_3, "CHL7DBMsgSegFieldVal: statement = %s", sStatement);

	database.GetDBPtr()->SetLoginTimeout(300);
	if(! records.Open(CRecordset::forwardOnly, sStatement))
		return;
		
	m_pMsgSegFieldValList = new CHL7DBMsgSegFieldValItemList;

	if(m_pMsgSegFieldValList == NULL)
		{
		records.Close();
		return;
		}

	while(! records.IsEOF())
		{
		// populate the list with data in the table
		CDBVariant vTemp;
		records.GetFieldValue((short)0, vTemp, SQL_C_SLONG);

		CHL7DBMsgSegFieldValItem* pItem = new CHL7DBMsgSegFieldValItem;
		pItem->m_nMsgNo = vTemp.m_lVal;

		records.GetFieldValue(1, vTemp, SQL_C_SLONG);
		pItem->m_nFieldNo = vTemp.m_lVal;

		records.GetFieldValue(2, vTemp, SQL_C_SLONG);
		pItem->m_nGroupNo = vTemp.m_lVal;

		records.GetFieldValue(3, vTemp, SQL_C_SLONG);
		pItem->m_nCalluid = vTemp.m_lVal;

		CString sTemp;

		records.GetFieldValue(4, sTemp);
		sTemp.TrimLeft();
		sTemp.TrimRight();
		ALLOC_STRING(pItem->m_pFieldValue, sTemp);

		m_pMsgSegFieldValList->AddTail(pItem);

		records.MoveNext();
		}

	records.Close();
	}

//--------------------------------------------------------------------------------
CHL7DBMsgSegFieldVal::~CHL7DBMsgSegFieldVal()
	{
	delete m_pMsgSegFieldValList;
	m_pMsgSegFieldValList = NULL;
	}

//--------------------------------------------------------------------------------
CHL7DBMsgSegFieldValItem* CHL7DBMsgSegFieldVal::GetHead()
	{
	if(m_pMsgSegFieldValList != NULL)
		{
		m_posCurrent = m_pMsgSegFieldValList->GetHeadPosition();
		return m_pMsgSegFieldValList->GetNext(m_posCurrent);
		}
	return NULL;
	}

//--------------------------------------------------------------------------------
CHL7DBMsgSegFieldValItem* CHL7DBMsgSegFieldVal::GetNext()
	{
	if(m_pMsgSegFieldValList != NULL && m_posCurrent != NULL)
		return m_pMsgSegFieldValList->GetNext(m_posCurrent);
	return NULL;
	}

//--------------------------------------------------------------------------------
void CHL7DBMsgSegFieldValItem::TraceDump()
	{
	GetIO()->FormatOutput(IOMASK_4, "HL7_MsgSegFieldVal msgno=%ld fieldno=%ld calluid=%ld value=%s***",
		m_nMsgNo, m_nFieldNo, m_nCalluid, (LPCTSTR)m_pFieldValue);
	}

//--------------------------------------------------------------------------------
long CHL7DBMsgSegFieldVal::AddMessage(CPACSDatabase& db, const CHL7Message* pMsg)
	{
	GetIO()->Output(IOMASK_7|IOMASK_CONST, "CHL7DBMsgSegFieldVal::AddMessage entry");

	SetResultCode(CResult::errOS);

	// call the stored proc that gets the next msg number
	CHL7DBNextMsgNum nNextMsgNum(db);

	if(nNextMsgNum == 0L)
		{
		(CResult) *this = (CResult) nNextMsgNum;
		return 0;
		}

	GetIO()->FormatOutput(IOMASK_5, "nNextMsgNum == %ld\n", (long)nNextMsgNum);

	ASSERT(db.GetDBPtr()->CanUpdate());

	// get the encoding characters from the MSH segment
	CHL7MessageConfig msgCfg(pMsg->GetSegment(0));

	// look up the msg type in the HL7_MessageDef table
	CHL7DBMessageDef defMsg;
	long nMsgId = 0;

	{
	CHL7DBFieldDef fieldDef(pMsg->GetVersion());
	const CHL7DBFieldDefItem* pFieldDefItem = fieldDef.Find(HL7FLD_MSH_MESSAGE_TYPE);
	if(pFieldDefItem == NULL)
		{
		GetIO()->FormatOutput(IOMASK_ERR, "Calluid (%d) not found. ", HL7FLD_MSH_MESSAGE_TYPE);
		return 0;
		}

	CString sMsgType(pMsg->GetSegment(0)->GetField(pFieldDefItem->m_nFieldId));
	const CHL7DBMessageDefItem* pDefItem = defMsg.Find(sMsgType);
	if(pDefItem == NULL)
		{
		GetIO()->Output(IOMASK_ERR|IOMASK_CONST, "msg id not found. ignoring \"" + sMsgType + "\"");
		return 0;
		}

	nMsgId = pDefItem->m_nMsgId;
	}

	// create a record in the HL7_MSG_EVENT table set the status to 0
	CHL7DBMsgEvent msgEvent(db, (long) nNextMsgNum, nMsgId, 0);

	// add records to the table the unique sequence number
	long nCurField = 1;
	// the "group" the field belongs to
	// this is used to group a field together when it's been split apart
	long nFieldGroup = 1;

	// determine what char to use for quotes
	char cQuoteChar = db.GetQuoteChar();

	for(int i = 0; i <= pMsg->GetSegmentsPtr()->GetUpperBound(); i++)
		{
		long nCurSeg = 0;

		//look up the segment id
		CHL7DBSegmentDef defSeg;
		CHL7Segment* pSeg = pMsg->GetSegment(i);
		ASSERT(pSeg != NULL);

		{
		const CHL7DBSegmentDefItem* pItem = defSeg.Find(pSeg->GetField(0));
		if(pItem == NULL)
			{
			GetIO()->FormatOutput(IOMASK_ERR, "segment id (%s) not found", pSeg->GetField(0));
			return 0;
			}

		nCurSeg = pItem->m_nSegId;
		}

		// determine which fields we need
		CHL7DBFieldDef defField(pMsg->GetVersion(), nCurSeg);

		const CHL7DBFieldDefItem* pItem = NULL;

		for(int nIndex = 0; ; nIndex++)
			{
			pItem = defField.GetIndexedItem(nIndex);

			if(pItem == NULL)
				break;

			if(! pItem->m_bMandatory)
				continue;

			CString sField(pMsg->GetSegment(i)->GetField(pItem->m_nFieldId));

			// change quotes contained within the data to non-token quotes
			// ie sybase uses double quotes so if there are any double quotes
			// within the data, they have to be changed to single quotes before
			// it can be inserted - capice?
			if(cQuoteChar == '\"')
				{
				// change double quotes to single
				int nQuote = sField.Find('"');
				while(nQuote != -1)
					{
					sField.SetAt(nQuote, '\'');
					nQuote = sField.Find('"', nQuote);
					}
				}
			else
				{
				// change single quotes to double
				int nQuote = sField.Find('\'');
				while(nQuote != -1)
					{
					sField.SetAt(nQuote, '\"');
					nQuote = sField.Find('\'', nQuote);
					}
				}

			if(sField.IsEmpty())
				sField = " ";

			if(pItem->m_nWrapLen > 0 && sField.GetLength() > pItem->m_nWrapLen)
				{
				CStringArray sData;

				CWordWrap::Wrap(sData, sField, pItem->m_nWrapLen);

				for(int i = 0; i < sData.GetSize(); i++)
					{
					// build the insert statement and execute it
					CString sTemp;
					sTemp.Format("insert into %sHL7_MsgSegFieldVal values(%ld, %ld, %ld, %ld, %c%s%c)",
						(LPCTSTR) db.GetTableAccountName(),
						(long) nNextMsgNum, nCurField++, nFieldGroup, pItem->m_nCalluid, 
						cQuoteChar, (LPCTSTR) sData[i], cQuoteChar);

					GetIO()->Output(IOMASK_3|IOMASK_CONST, sTemp);
					db.GetDBPtr()->ExecuteSQL(sTemp);
					}
				}
			else
				{
				// build the insert statement and execute it
				CString sTemp;
				sTemp.Format("insert into %sHL7_MsgSegFieldVal values(%ld, %ld, %ld, %ld, %c%s%c)",
					(LPCTSTR) db.GetTableAccountName(),
					(long) nNextMsgNum, nCurField++, nFieldGroup, pItem->m_nCalluid, 
					cQuoteChar, (LPCTSTR) sField, cQuoteChar);

				GetIO()->Output(IOMASK_3|IOMASK_CONST, sTemp);
				db.GetDBPtr()->ExecuteSQL(sTemp);
				}

			nFieldGroup++;
			}
		}

	GetIO()->Output(IOMASK_14|IOMASK_CONST, "About to call HL7_ProcessMessage...\n");
	CTime ctNow(CTime::GetCurrentTime());

	// call the HL7_ProcessMessage stored proc
	CHL7DBProcMsg procMsg(db, msgEvent);

	CTimeSpan ctSpan(CTime::GetCurrentTime() - ctNow);
	GetIO()->FormatOutput(IOMASK_2, "HL7_ProcessMessage %d:%d\n", ctSpan.GetTotalMinutes(), ctSpan.GetTotalSeconds());

	// look at the returned status
	long nStatus = msgEvent.QueryStatus(db);

	if(nStatus == -1)
		SetResultCode(CResult::errOS);
	else
		SetResultCode(0);

	GetIO()->Output(IOMASK_8|IOMASK_CONST, "CHL7DBMsgSegFieldVal::AddMessage exit");
	return nStatus;
	}
