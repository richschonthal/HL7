//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// HL7DBNextMsgNum.h: interface for the CHL7DBNextMsgNum class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HL7DBNEXTMSGNUM_H__7CBF9036_FBDE_11D2_87A7_00104B9E6286__INCLUDED_)
#define AFX_HL7DBNEXTMSGNUM_H__7CBF9036_FBDE_11D2_87A7_00104B9E6286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <result.h>

#include <commondefines.h>
#include "StoredProcedure.h"

//--------------------------------------------------------------------------------
class CPACSDatabase;

//--------------------------------------------------------------------------------
class CHL7DBNextMsgNum : public CResult
	{
	public:
		CHL7DBNextMsgNum(CPACSDatabase&);

		long m_nMsgNum;

		operator long() const;
	};

//--------------------------------------------------------------------------------
class CHL7DBNextMsgNumRecordset : public CStoredProcedure, public CResult
	{
	friend class CHL7DBNextMsgNum;

	private:
		static CString m_sCounterName;

		CHL7DBNextMsgNum* m_pWrapper;

		CHL7DBNextMsgNumRecordset(CPACSDatabase&, CHL7DBNextMsgNum*);

		DECLARE_DYNAMIC(CHL7DBNextMsgNumRecordset)
			
		// Field/Param Data
		//{{AFX_FIELD(CHL7DBNextMsgNumRecordset, CRecordset)
		//}}AFX_FIELD

			// Overrides
			// ClassWizard generated virtual function overrides
			//{{AFX_VIRTUAL(CHL7DBNextMsgNumRecordset)
		public:
			virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
			//}}AFX_VIRTUAL
	};

#endif //AFX_HL7DBNEXTMSGNUM_H__7CBF9036_FBDE_11D2_87A7_00104B9E6286__INCLUDED_
