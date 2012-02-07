//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// HL7DBRuleDef.h: interface for the CHL7DBRuleDef class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HL7DBRuleDef_H__A5625ABF_F0F9_11D2_87A6_00104B9E6286__INCLUDED_)
#define AFX_HL7DBRuleDef_H__A5625ABF_F0F9_11D2_87A6_00104B9E6286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <commondefines.h>
#include <result.h>

//--------------------------------------------------------------------------------
class CHL7DBRuleDefItem : public CObject
	{
	public:
		CHL7DBRuleDefItem() : m_nRuleId(0), m_pValue(NULL), m_pDesc(NULL), m_nVersionId(0) {}
		~CHL7DBRuleDefItem() {delete[] m_pValue; delete[] m_pDesc;}

		long m_nRuleId;
		LPTSTR m_pValue;
		LPTSTR m_pDesc;
		long m_nVersionId;

		void TraceDump();
	};

//--------------------------------------------------------------------------------
class CHL7DBRuleDefItemList : public CTypedPtrList<CObList, CHL7DBRuleDefItem*>
	{
	public:
		~CHL7DBRuleDefItemList()
			{
			for(POSITION pos = GetHeadPosition(); pos; GetNext(pos))
				delete GetAt(pos);
			RemoveAll();
			}

		const CHL7DBRuleDefItem* Find(int nVersion, int nRule) const;
	};

//--------------------------------------------------------------------------------
class CHL7DBRuleDef : public CResult  
	{
	public:
		CHL7DBRuleDef();
		virtual ~CHL7DBRuleDef();

	private:
		friend class CStaticHolder;
		bool Init(CPACSDatabase&);

	private:
		// const values
		static LPCTSTR pTableName;
		static CHL7DBRuleDefItemList* m_pMsgDefList;
		static int m_nRefCount;

	public:
		const CHL7DBRuleDefItem* Find(int nVersion, int nId) const;
		int GetRecordCount() const { return (m_pMsgDefList == NULL) ? 0 : m_pMsgDefList->GetCount(); }
	};

#endif // !defined(AFX_HL7DBRuleDef_H__A5625ABF_F0F9_11D2_87A6_00104B9E6286__INCLUDED_)
