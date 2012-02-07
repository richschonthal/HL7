//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// HL7DBProcMsg.h: interface for the CHL7DBProcMsg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HL7DBPROCESSMESSAGE_H__7CBF9036_FBDE_11D2_87A7_00104B9E6286__INCLUDED_)
#define AFX_HL7DBPROCESSMESSAGE_H__7CBF9036_FBDE_11D2_87A7_00104B9E6286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <result.h>
#include <commondefines.h>

#include "HL7DBMsgEvent.h"
#include "StoredProcedure.h"

//--------------------------------------------------------------------------------
class CPACSDatabase;

//--------------------------------------------------------------------------------
class CHL7DBProcMsg : public CResult
	{
	friend class CHL7DBProcMsgRecordset;

	private:
		long m_nMsgNum;
		long m_nMsgId;

	public:
		CHL7DBProcMsg(CPACSDatabase&, const CHL7DBMsgEvent&);
	};

//--------------------------------------------------------------------------------
class  CHL7DBProcMsgRecordset : public CStoredProcedure, public CResult
	{
	friend class CHL7DBProcMsg;

	private:
		long m_nResult;

		CHL7DBProcMsg* m_pWrapper;

		CHL7DBProcMsgRecordset(CPACSDatabase&, CHL7DBProcMsg*);

		DECLARE_DYNAMIC(CHL7DBProcMsgRecordset)
			
		// Field/Param Data
		//{{AFX_FIELD(CHL7DBProcMsgRecordset, CRecordset)
		//}}AFX_FIELD

			// Overrides
			// ClassWizard generated virtual function overrides
			//{{AFX_VIRTUAL(CHL7DBProcMsgRecordset)
		public:
			virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
			//}}AFX_VIRTUAL
	};

#endif //AFX_HL7DBPROCESSMESSAGE_H__7CBF9036_FBDE_11D2_87A7_00104B9E6286__INCLUDED_
