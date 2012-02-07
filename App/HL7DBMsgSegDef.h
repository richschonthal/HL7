//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#if !defined(AFX_HL7DBMsgSegDef_H__A5625ABF_F0F9_11D2_87A6_00104B9E6286__INCLUDED_)
#define AFX_HL7DBMsgSegDef_H__A5625ABF_F0F9_11D2_87A6_00104B9E6286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <commondefines.h>
#include <result.h>
#include "HL7DBFieldDef.h"

//--------------------------------------------------------------------------------
class CPACSDatabase;

//--------------------------------------------------------------------------------
class CHL7DBMsgSegDefItem : public CObject
	{
	public:
		CHL7DBMsgSegDefItem() : m_nMsgId(0), m_nSegId(0) {}

		long m_nMsgId;
		long m_nSegId;

		void TraceDump();
	};

//--------------------------------------------------------------------------------
class CHL7DBMsgSegDefItemList : public CTypedPtrList<CObList, CHL7DBMsgSegDefItem*>
	{
	public:
		~CHL7DBMsgSegDefItemList()
			{
			for(POSITION pos = GetHeadPosition(); pos; GetNext(pos))
				delete GetAt(pos);
			RemoveAll();
			}

		// retrieves the first item with nMsgId
		const CHL7DBMsgSegDefItem* Find(int nMsgId) const;
	};

//--------------------------------------------------------------------------------
class CHL7DBMsgSegDef : public CResult  
	{
	public:
		CHL7DBMsgSegDef();
		virtual ~CHL7DBMsgSegDef();

	private:
		friend class CStaticHolder;
		bool Init(CPACSDatabase&);

	private:
		// const values
		static LPCTSTR pTableName;
		static CHL7DBMsgSegDefItemList* m_pMsgSegDefList;
		static int m_nRefCount;

	private:
		POSITION m_posCurrent;

	public:
		// retrieves the first item with nMsgId
		const CHL7DBMsgSegDefItem* Find(int nMsgId) const;

		int GetSegCount() const;
		CHL7DBMsgSegDefItem* CHL7DBMsgSegDef::GetHead();
		CHL7DBMsgSegDefItem* CHL7DBMsgSegDef::GetNext();
	};

#endif // !defined(AFX_HL7DBMsgSegDef_H__A5625ABF_F0F9_11D2_87A6_00104B9E6286__INCLUDED_)

