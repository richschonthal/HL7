//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#include "stdafx.h"
#include "ServerThread.h"
#include "ServerThreadInfo.h"
#include "Monitor.h"

#include "globals.h"

#include "HL7InputDaemonInit.h"

#include "HL7SystemMsg.h"

#include "ProtocolObj.h"
#include "HL7Message.h"
#include "HL7Segment.h"

#include "HL7MessageVerify.h"

#include "HL7DBMsgSegFieldVal.h"
#include "HL7DBVersionDef.h"

#include "HL7MSAConfig.h"
#include "HL7DBServerConfig.h"

#include "HL7DBFieldDef.h"

#include "HL7CallUID.h"

#include <WriteLock.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//--------------------------------------------------------------------------------
#if defined(DB_PRIVATE)
#pragma message("*** using seperate DB connections for each thread")
#elif defined(DB_SHARE)
#pragma message("*** using one shared DB connection for all threads")
#else
#error "must define DB_PRIVATE or DB_SHARE"
#endif

//--------------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(CServerThread, CWinThread)

//--------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CServerThread, CWinThread)
	//{{AFX_MSG_MAP(CServerThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
   ON_THREAD_MESSAGE(INIT, DoInit)
   ON_THREAD_MESSAGE(HL7_HL7MSG, DoHL7Message)
   ON_THREAD_MESSAGE(HL7_GOODBYE, DoGoodbye)

END_MESSAGE_MAP()

//--------------------------------------------------------------------------------
CServerThread::CServerThread()
		: m_nRetries(0)
	{
	}

//--------------------------------------------------------------------------------
CServerThread::~CServerThread()
	{
	}

//--------------------------------------------------------------------------------
BOOL CServerThread::InitInstance()
	{
	ASSERT_VALID(this);
	m_evtInit.PulseEvent();
	return TRUE;
	}

//--------------------------------------------------------------------------------
int CServerThread::ExitInstance()
	{
	return CWinThread::ExitInstance();
	}

//--------------------------------------------------------------------------------
void CServerThread::DoGoodbye(WPARAM wParam, LPARAM)
	{
	PostThreadMessage(WM_QUIT, 0, 0);
	}

//--------------------------------------------------------------------------------
void CServerThread::DoPause()
	{
	}

//--------------------------------------------------------------------------------
void CServerThread::DoContinue()
	{
	}

//--------------------------------------------------------------------------------
void CServerThread::DoInit(CServerThreadInfo* pInfo, CHL7InputDaemonInit* pInitInfo)
	{
	ASSERT_VALID(this);

	GetIO()->Output(IOMASK_7|IOMASK_CONST, "CServerThread::DoInit entry");

	m_pInitInfo = pInitInfo;
	if(m_pInitInfo != NULL)
		GetIO()->AddThreadName(m_pInitInfo->GetName());
	m_pInfo = pInfo;
	m_evtInit.PulseEvent();
#ifndef _NO_SECURITY_SERVER
	CSecurityConnection* pSec = ((COutputApp*) AfxGetApp())->m_pSecurity;
	if(pSec == NULL)
		return;
	CWriteLock lock(pSec, false);
	if(! lock.Lock(30000))
		return;
	if(! pSec->AquireToken(m_pInitInfo->GetTokenId()))
		GetIO()->FormatOutput(IOMASK_ERR, "Did not get token %d", m_pInitInfo->GetTokenId());
#endif

	GetIO()->Output(IOMASK_8|IOMASK_CONST, "CServerThread::DoInit exit");
	}

//--------------------------------------------------------------------------------
void CServerThread::DoHL7Message(CHL7Message* pMsg, LPARAM)
	{
	GetIO()->Output(IOMASK_7|IOMASK_CONST, "CServerThread::DoHL7Message entry");

	CTime ctNow(CTime::GetCurrentTime());

	CHL7Segment* pMSHSeg = pMsg->GetSegment(0);
	if(pMSHSeg == NULL)
		{
		GetIO()->FormatOutput(IOMASK_ERR, "Received msg @ %s but the MSH segment is missing", (LPCTSTR) ctNow.Format("%H:%M:%S"));
		CProtocolObj* pObj = new CProtocolObj(CProtocolObj::NACK);
		GetMonitorPtr()->PostThreadMessage(HL7_PROTOCOL, (WPARAM) pObj, 0);
		delete pMsg;
		return;
		}

	GetIO()->FormatOutput(IOMASK_14, "Processing msg %s %s", pMSHSeg->GetField(8), pMSHSeg->GetField(9));

	// if the Monitor hasnt attached a verion number then we must
	// determine it
	if(pMsg->GetVersion() == 0)
		{
		// determine the HL7 version
		CHL7DBVersionDef verDef;
		// sending app = field 2, sending facility = 3, version = 11
		const CHL7DBVersionDefItem* pVerItem = verDef.Find(pMSHSeg->GetField(2), pMSHSeg->GetField(3), pMSHSeg->GetField(11));
		if(pVerItem == NULL)
			{
			GetIO()->FormatOutput(IOMASK_ERR, "version not found (%s, %s, %s)", pMSHSeg->GetField(2), pMSHSeg->GetField(3), pMSHSeg->GetField(11));
			CProtocolObj* pObj = new CProtocolObj(CProtocolObj::NACK);
			BuildResponse(pObj, pMsg);
			GetMonitorPtr()->PostThreadMessage(HL7_PROTOCOL, (WPARAM) pObj, 0);
			delete pMsg;
			return;
			}

		pMsg->SetVersion(pVerItem->m_nVersionId);
		}

	// verify it
	CHL7MessageVerify verify(*pMsg);
	if(verify.HasErrors())
		{
		CString sMsg;
		verify.GetErrorMessage(sMsg);
		GetIO()->FormatOutput(IOMASK_ERR, "data validation error (%s)", (LPCTSTR) sMsg);
		CProtocolObj* pObj = new CProtocolObj(CProtocolObj::NACK);
		BuildResponse(pObj, pMsg);
		GetMonitorPtr()->PostThreadMessage(HL7_PROTOCOL, (WPARAM) pObj, 0);
		delete pMsg;
		return;
		}

	CProtocolObj* pObj = new CProtocolObj(CProtocolObj::NACK);

	long nStatus = 0;

	TRY
		{
		// the msg is ok so put it into the database
		CHL7DBMsgSegFieldVal tblInsert;

			{
		CSharedDB db;
		CPACSDatabase* pDB = db.GetDB();
		if(pDB == NULL)
			{
			CDBException* pEx = new CDBException;
			pEx->m_strError = "Internal error - can't lock global DB pointer";
			throw pEx;
			}
		nStatus = tblInsert.AddMessage(*pDB, pMsg);
			}

		GetIO()->FormatOutput(IOMASK_14, "Message (%s|%s) processed status = %ld",
			pMSHSeg->GetField(8), pMSHSeg->GetField(9), nStatus);

		if(! tblInsert.HasErrors())
			pObj->SetCommand(CProtocolObj::ACK);
		else
			{
			// check to see if this message is being re-sent
			// if it is then we have to see how many times it's been re-sent
			// if it's the third time then we're just gonna ACK it and move on
			CHL7DBFieldDef defFld(pMsg->GetVersion());
			const CHL7DBFieldDefItem* pItem = defFld.Find(HL7FLD_MSH_MESSAGE_CONTROL_ID);

			if(pItem != NULL)
				{
				// get the contents of the processing id field
				CString sTemp(pMSHSeg->GetField(pItem->m_nFieldId));

				// is it the same as the last message?
				if(sTemp != m_sLastMsgNumber)
					{
					// no - so reset the counter
					m_sLastMsgNumber = sTemp;
					m_nRetries = 0;
					}
				else
					{
					CHL7DBServerConfig config;

					m_nRetries++;

					if(m_nRetries >= config.GetMaxRetries())
						{
						// too many tries so just ACK it and move on
						pObj->SetCommand(CProtocolObj::ACK);
						m_nRetries = 0;
						}
					}
				}
			}
		}
	CATCH(CDBException, e)
		{
		GetIO()->FormatOutput(IOMASK_ERR, "Database exception (%s)", (LPCTSTR) e->m_strError);
		m_nRetries = 0;
		CSharedDB db(false);
		db.DeleteConnection();
		}
	CATCH_ALL(e)
		{
		}
	END_CATCH_ALL

	BuildResponse(pObj, pMsg);
	delete pMsg;

	GetMonitorPtr()->PostThreadMessage(HL7_PROTOCOL, (WPARAM) pObj, 0);
	GetIO()->Output(IOMASK_8|IOMASK_CONST, "CServerThread::DoHL7Message exit");
	}

//--------------------------------------------------------------------------------
void CServerThread::BuildResponse(CProtocolObj* pObj, const CHL7Message* pMsg)
	{
	CHL7Segment* pSeg = pMsg->GetSegment(0);
	VERIFY(pSeg != NULL);

	// delim should always be set but just in case...
	char cDelim = pMsg->GetFieldDelim() == 0 ? '|' : pMsg->GetFieldDelim();

	// get the config table selected for the right version
	CHL7MSAConfig config(pMsg->GetVersion());

	// there should only be an MSH and an MSA for the ACK msg
	CString sOut[2];

	int nSeg = -1;

	for(int i = 0; i < config.GetItemCount(); i++)
		{
		const CHL7MSAConfigItem* pItem = config.GetItem(i);

		ASSERT(pItem != NULL);

		if(nSeg == -1)
			nSeg = pItem->m_nSegment - 1;
		else
			if(nSeg != pItem->m_nSegment - 1)
				nSeg = pItem->m_nSegment - 1;

		// there should only be an MSH and an MSA for the ACK msg
		ASSERT(nSeg > -1 && nSeg < 2);

		switch(pItem->m_nType)
			{
			case CHL7MSAConfig::MSADT_EMPTY_FIELD:
			case CHL7MSAConfig::MSADT_FIELD_DELIM:
				sOut[nSeg] += cDelim;
				break;

			case CHL7MSAConfig::MSADT_SEGMENT_NAME:
				sOut[nSeg] += pItem->m_sValue;
				break;

			case CHL7MSAConfig::MSADT_STRING:
				sOut[nSeg] += cDelim;
				sOut[nSeg] += pItem->m_sValue;
				break;

			case CHL7MSAConfig::MSADT_MSH_FIELD_NUM:
				sOut[nSeg] += cDelim;
				sOut[nSeg] += pSeg->GetField(atoi(pItem->m_sValue));
				break;

			case CHL7MSAConfig::MSADT_DATE:
				sOut[nSeg] += cDelim;
				sOut[nSeg] += CTime::GetCurrentTime().Format(pItem->m_sValue);
				break;

			case CHL7MSAConfig::MSADT_CHAR_CODE:
				sOut[nSeg] += (char) atoi(pItem->m_sValue);
				break;

			case CHL7MSAConfig::MSADT_OK_ERR:
				sOut[nSeg] += cDelim;
				sOut[nSeg] += (pObj->GetCommand()==CProtocolObj::ACK) ? pItem->m_sValue.Left(2):pItem->m_sValue.Right(2);
				break;
			}
		}

	pObj->SetParam(0, sOut[0]);
	pObj->SetParam(1, sOut[1]);
	}

//--------------------------------------------------------------------------------
CWinThread* CServerThread::GetMonitorPtr()
	{
	return m_pInfo->GetMonitorPtr();
	}
