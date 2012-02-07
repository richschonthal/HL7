//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"

#include "output.h"
#include "monitor.h"
#include "globals.h"
#include "HL7SystemMsg.h"
#include "HL7InputDaemonInit.h"
#include "ServerThread.h"
#include "HL7PacOutThread.h"
#include "PACSDatabase.h"
#include "HL7DBPACOut.h"
#include "HL7DBOutMSH.h"
#include "HL7DBOutMsgFormat.h"
#include "HL7DBServerConfig.h"
#include "HL7Message.h"
#include "HL7Segment.h"
#include "HL7DBSegmentDef.h"
#include "HL7DBFieldDef.h"
#include "HL7DBMsgSegFieldVal.h"

#include <DebugPrintf.h>
#include <WriteLock.h>

//#include <SecurityConnection.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//--------------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(CHL7PacOutThread, CServerThread)

//--------------------------------------------------------------------------------
CHL7PacOutThread::CHL7PacOutThread()
#if defined(DB_PRIVATE)
		: m_pRS(NULL)
#elif defined(DB_SHARE)
		: m_pList(NULL)
#endif
		, m_nLastMsgNo(0)
		, m_nRetries(0)
		, m_nVersion(0)
	{
	}

//--------------------------------------------------------------------------------
CHL7PacOutThread::~CHL7PacOutThread()
	{
	}

//--------------------------------------------------------------------------------
void CHL7PacOutThread::DoInit(CServerThreadInfo* pInfo, CHL7InputDaemonInit* pInitInfo)
	{
	ASSERT_VALID(this);

	GetIO()->Output(IOMASK_7|IOMASK_CONST, "CHL7PacOutThread::InitInstance entry");
	m_pInitInfo = pInitInfo;
	if(m_pInitInfo != NULL)
		GetIO()->AddThreadName(m_pInitInfo->GetName());
	m_nVersion = m_pInitInfo->GetVersion();
	m_pInfo = pInfo;
	m_evtInit.PulseEvent();
	GetIO()->Output(IOMASK_8|IOMASK_CONST, "CHL7PacOutThread::InitInstance exit");
	PostThreadMessage(HL7_GO, 0, 0);
	}

//--------------------------------------------------------------------------------
BOOL CHL7PacOutThread::InitInstance()
	{
	ASSERT_VALID(this);
	return TRUE;
	}

//--------------------------------------------------------------------------------
int CHL7PacOutThread::ExitInstance()
	{
	return CServerThread::ExitInstance();
	}

//--------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CHL7PacOutThread, CServerThread)
	//{{AFX_MSG_MAP(CHL7PacOutThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
   ON_THREAD_MESSAGE(INIT, DoInit)
   ON_THREAD_MESSAGE(HL7_GO, Loop)
   ON_THREAD_MESSAGE(HL7_GOODBYE, DoGoodbye)
END_MESSAGE_MAP()

//--------------------------------------------------------------------------------
void CHL7PacOutThread::DoGoodbye(WPARAM, LPARAM)
	{
	PostThreadMessage(WM_QUIT, 0, 0);
	}


//--------------------------------------------------------------------------------
void CHL7PacOutThread::Loop(WPARAM, LPARAM)
	{
	for(;;)
		TRY
			{
			CString sTemp;

			if(m_pList == NULL)
				{
				CSharedDB db;
				CPACSDatabase* pDB = db.GetDB();
				if(pDB == NULL)
					break;

				CRecordset rs(pDB->GetDBPtr());
				sTemp.Format("select msgno, msgid from %sHL7_Msg_Event where status = 6", 
					(LPCTSTR) CPACSDatabase::GetTableAccountName());
				if(rs.Open(CRecordset::forwardOnly, sTemp) == 0)
					break;

				if(rs.IsEOF())
					break;

				m_pList = new CList<__int64, __int64>;
				while(! rs.IsEOF())
					{
					LARGE_INTEGER li;
					CDBVariant vTemp;
					rs.GetFieldValue((short) 0, vTemp, SQL_C_SLONG);
					li.LowPart = vTemp.m_lVal; // msgno

					rs.GetFieldValue(1, vTemp, SQL_C_SLONG);
					li.HighPart = vTemp.m_lVal; // msgid

					m_pList->AddTail(li.QuadPart);
					rs.MoveNext();
					}
				}

			if(m_pList != NULL)
				{
				CSharedDB db;
				CPACSDatabase* pDB = db.GetDB();
				if(pDB == NULL)
					break;

				POSITION pos = m_pList->GetHeadPosition();
				if(pos == NULL)
					{
					delete m_pList;
					m_pList = NULL;
					break;
					}

				LARGE_INTEGER li;
				li.QuadPart = m_pList->GetAt(pos);
				sTemp.Format("update %sHL7_Msg_Event set status = %ld where msgno = %ld and msgid = %ld",
					(LPCTSTR) CPACSDatabase::GetTableAccountName(),
					STATUS_MSG_PROCESSING, li.LowPart, li.HighPart
					);

				pDB->GetDBPtr()->ExecuteSQL(sTemp);

				long nStatus = DoPACMessage(pDB, li.LowPart, li.HighPart);
				GetIO()->FormatOutput(IOMASK_6, "PAC Msg msgno=%ld msgid=%ld status=%ld\n", 
					li.LowPart, li.HighPart, nStatus);

				sTemp.Format("update %sHL7_Msg_Event set status = %ld where msgno = %ld and msgid = %ld",
					(LPCTSTR) CPACSDatabase::GetTableAccountName(),
					nStatus, li.LowPart, li.HighPart
					);

				pDB->GetDBPtr()->ExecuteSQL(sTemp);

				m_pList->RemoveAt(pos);
				}

			break;
			}
		CATCH_ALL(e)
			{
			TRACE("CATCH 1\n");
			CString sTemp;
			e->GetErrorMessage(sTemp.GetBuffer(1024), 1024);
			sTemp.ReleaseBuffer();
			GetIO()->FormatOutput(IOMASK_ERR, "Exception occured during init - exiting %s", (LPCTSTR) sTemp);
			delete m_pList;
			m_pList = NULL;
			CSharedDB db(false);
			db.DeleteConnection();
			break;
			}
		END_CATCH_ALL

	MSG msg;
	if(! PeekMessage(&msg, NULL, WM_QUIT, WM_QUIT, PM_NOREMOVE))
		{
		CHL7DBServerConfig config;
		::Sleep(config.GetPacOutSleepDur());
		PostThreadMessage(HL7_GO, 0, 0);
		}
	}
	
//--------------------------------------------------------------------------------
int CHL7PacOutThread::DoPACMessage(CPACSDatabase* pDB, long nMsgNo, long nMsgId)
	{
	GetIO()->Output(IOMASK_7|IOMASK_CONST, "CHL7PacOutThread::DoPACMessage entry");

	CString sOut;
	if(! BuildString(pDB, nMsgNo, nMsgId, sOut))
		{
		CString sTemp;
		sTemp.Format("Message %ld is empty", nMsgNo );
		GetIO()->Output(IOMASK_ERR|IOMASK_CONST, sTemp);
		return STATUS_MSG_ERROR;
		}

	LPTSTR pData = new char[sOut.GetLength() + 1];
	strcpy(pData, sOut);

	GetMonitorPtr()->PostThreadMessage(HL7_HL7MSG, (WPARAM) pData, 0);

	// get the response from the monitor
	MSG msg;
	::GetMessage(&msg, NULL, HL7_HL7MSG, HL7_HL7MSG);

	// check how many times the message has been sent
	if(nMsgNo != m_nLastMsgNo)
		{
		// no - so reset the counter
		m_nLastMsgNo = nMsgNo;
		m_nRetries = 0;
		}
	else
		{
		CHL7DBServerConfig config;
		m_nRetries++;

		if(m_nRetries >= config.GetMaxRetries())
			{
			// too many tries so just move on
			GetIO()->FormatOutput(IOMASK_ERR, "Message %ld sent %ld times without ACK", nMsgNo, m_nRetries );
			m_nRetries = 0;
			return STATUS_MSG_ERROR;
			}
		}

	if(msg.wParam == NULL )
		{
		GetIO()->FormatOutput(IOMASK_ERR, "wParam==NULL (Must be a socket error) lParam=%ld", msg.lParam);
		return STATUS_MSG_ERROR_RESEND;
		}

	CHL7Message* pMsg = (CHL7Message*) msg.wParam;

	// get the MSA segment
	CHL7Segment* pSeg = pMsg->GetSegment(1);

	if(pSeg == NULL)
		{
		GetIO()->Output(IOMASK_ERR|IOMASK_CONST, "pSeg == NULL");
		delete pMsg;
		return STATUS_MSG_ERROR_RESEND;
		}

	// look at the ACK/NACK character
	LPCTSTR pAck = pSeg->GetField(1);

	if(strlen(pAck) < 2)
		{
		GetIO()->Output(IOMASK_ERR|IOMASK_CONST, "strlen(pAck) < 2");
		delete pMsg;
		return STATUS_MSG_ERROR_RESEND;
		}

	if(pAck[1] != 'A')
		{
		GetIO()->Output(IOMASK_ERR|IOMASK_CONST, "pAck[1] != 'A'");
		delete pMsg;
		return STATUS_MSG_ERROR;
		}

	delete pMsg;
	GetIO()->Output(IOMASK_8|IOMASK_CONST, "CHL7PacOutThread::DoPACMessage ok exit");
	return STATUS_MSG_OK;
	}

//--------------------------------------------------------------------------------
bool CHL7PacOutThread::BuildString(CPACSDatabase* pDB, long nMsgNo, long nMsgId, CString& sOut)
	{
	sOut.Empty();

	CHL7Message msgCurrent;

	msgCurrent.SetFieldDelim('|');
	msgCurrent.SetVersion(m_nVersion);

	// get the description of the segments names sorted by segid
	CHL7DBSegmentDef defSegment;

	// get the description of fields sorted by fieldid ( for the current versionid )
	CHL7DBFieldDef defField(m_nVersion);

	// retrieve the output values from HL7_MsgSegFieldVal sorted by fieldno
	CHL7DBMsgSegFieldVal values(*pDB, nMsgNo);

	for(CHL7DBMsgSegFieldValItem* pDBVal = values.GetHead(); pDBVal != NULL; pDBVal = values.GetNext())
		{
		// get segid e fieldid for these calluid and versionid
		// versionid already set during construction
		const CHL7DBFieldDefItem* pCurFieldDefItem = defField.Find(pDBVal->m_nCalluid);
		if(pCurFieldDefItem == NULL)
			{
			GetIO()->FormatOutput(IOMASK_ERR, "msgno (%ld) : Calluid %ld not found in HL7_FieldDef",
				nMsgNo, pDBVal->m_nCalluid);
			continue;
			}

		CHL7Segment* pSeg = msgCurrent.Find(pDBVal->m_nGroupNo);

		if(pSeg == NULL)
			{
			pSeg = new CHL7Segment(m_nVersion, pCurFieldDefItem->m_nSegId);
			pSeg->SetGroup(pDBVal->m_nGroupNo);

			pSeg->SetName((defSegment.Find(pCurFieldDefItem->m_nSegId))->m_pSegName);

			if(defField.FindFirst(pSeg->GetId()) == NULL)
				{
				GetIO()->FormatOutput(IOMASK_ERR, "msgno (%ld) : first field of segment %s not found in HL7_FieldDef",
					nMsgNo, (LPCTSTR) pSeg->GetName());
				delete pSeg;
				continue;
				}

			CHL7DBFieldDefItem* pLast = defField.FindLast(pSeg->GetId());
			if(pLast == NULL)
				{
				GetIO()->FormatOutput(IOMASK_ERR, "msgno (%ld) : last field of segment %s not found in HL7_FieldDef",
					nMsgNo, (LPCTSTR) pSeg->GetName());
				delete pSeg;
				continue;
				}

			// allocates the necessary number of empty fields
			pSeg->SetFieldCount(pLast->m_nFieldId);

			// add the segment to the message
			switch(msgCurrent.AddSegment(pSeg))
				{
				case CHL7Message::MSG_OK:
					break;
				case CHL7Message::MSG_ERROR:
					GetIO()->FormatOutput(IOMASK_ERR, "msgno (%ld) : segment %s not added",
						nMsgNo, (LPCTSTR) pSeg->GetName());
					delete pSeg;
					continue;
				}
			}

		pSeg->SetField(pCurFieldDefItem->m_nFieldId, pDBVal->m_pFieldValue);
		}

	msgCurrent.BuildString(sOut);
	return ! sOut.IsEmpty();
	}
