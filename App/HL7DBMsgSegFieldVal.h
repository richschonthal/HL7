//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#if !defined(AFX_HL7DBMSGSEGFIELDVAL_H__417C7082_FE4A_11D2_87A7_00104B9E6286__INCLUDED_)
#define AFX_HL7DBMSGSEGFIELDVAL_H__417C7082_FE4A_11D2_87A7_00104B9E6286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HL7DBMsgSegFieldVal.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHL7DBMsgSegFieldVal recordset

#include <result.h>


//--------------------------------------------------------------------------------
class CHL7DBMsgSegFieldValResult : public CResult
	{
	public:
		DECLARE_RESULTCLASS(CHL7DBMsgSegFieldValResult)
		DECLARE_RESULTMAP(m_map)

		enum
			{
			errMsgIdNotFound = BeginErrorRange
			};
	};

//--------------------------------------------------------------------------------
class CHL7DBMsgSegFieldValItem : public CObject
	{
	public:
		CHL7DBMsgSegFieldValItem() : m_pFieldValue(NULL) {}
		~CHL7DBMsgSegFieldValItem() {delete[] m_pFieldValue;}

		long m_nMsgNo;
		long m_nFieldNo;
		long m_nGroupNo;
		long m_nCalluid;
		LPTSTR m_pFieldValue;

		void TraceDump();
	};

//--------------------------------------------------------------------------------
class CHL7DBMsgSegFieldValItemList : public CTypedPtrList<CObList, CHL7DBMsgSegFieldValItem*>
	{
	public:
		~CHL7DBMsgSegFieldValItemList()
			{
			for(POSITION pos = GetHeadPosition(); pos; GetNext(pos))
				delete GetAt(pos);
			RemoveAll();
			}
	};

//--------------------------------------------------------------------------------
class CHL7Message;
class CPACSDatabase;

//--------------------------------------------------------------------------------
class CHL7DBMsgSegFieldVal : public CResult
	{
	private:
		POSITION m_posCurrent;

	private:
		// constants
		static LPCTSTR pTableName;
		static CHL7DBMsgSegFieldValItemList* m_pMsgSegFieldValList;

	public:
		CHL7DBMsgSegFieldVal() : m_posCurrent(NULL) {};
		CHL7DBMsgSegFieldVal(CPACSDatabase&, long nMsgNo);

		virtual ~CHL7DBMsgSegFieldVal();

		long AddMessage(CPACSDatabase&, const CHL7Message*);

	public:
		CHL7DBMsgSegFieldValItem* GetHead();
		CHL7DBMsgSegFieldValItem* GetNext();
	};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HL7DBMSGSEGFIELDVAL_H__417C7082_FE4A_11D2_87A7_00104B9E6286__INCLUDED_)
