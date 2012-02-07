//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// HL7DBSegmentDef.h: interface for the CHL7DBSegmentDef class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HL7DBSegmentDef_H__A5625ABF_F0F9_11D2_87A6_00104B9E6286__INCLUDED_)
#define AFX_HL7DBSegmentDef_H__A5625ABF_F0F9_11D2_87A6_00104B9E6286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <commondefines.h>
#include <result.h>

//--------------------------------------------------------------------------------
class CHL7DBSegmentDefItem : public CObject
	{
	public:
		CHL7DBSegmentDefItem() : m_pSegName(NULL), m_pSegDesc(NULL) {}
		~CHL7DBSegmentDefItem() {delete[] m_pSegName; delete[] m_pSegDesc;}

		int m_nSegId;
		LPTSTR m_pSegName;
		LPTSTR m_pSegDesc;

		void TraceDump();
	};

//--------------------------------------------------------------------------------
class CHL7DBSegmentDefItemList : public CTypedPtrList<CObList, CHL7DBSegmentDefItem*>
	{
	public:
		~CHL7DBSegmentDefItemList()
			{
			for(POSITION pos = GetHeadPosition(); pos; GetNext(pos))
				delete GetAt(pos);
			RemoveAll();
			}

		const CHL7DBSegmentDefItem* Find(int) const;
		const CHL7DBSegmentDefItem* Find(LPCTSTR) const;
	};

//--------------------------------------------------------------------------------
class CHL7DBSegmentDef : public CResult
	{
	public:
		CHL7DBSegmentDef();
		virtual ~CHL7DBSegmentDef();

	private:
		friend class CStaticHolder;
		bool Init(CPACSDatabase&);

	private:
		// const values
		static LPCTSTR pTableName;
		static CHL7DBSegmentDefItemList* m_pSegDefList;
		static int m_nRefCount;

	public:
		const CHL7DBSegmentDefItem* Find(int nId) const;
		const CHL7DBSegmentDefItem* Find(LPCTSTR pName) const;

		int GetRecordCount() const { return (m_pSegDefList == NULL) ? 0 : m_pSegDefList->GetCount(); }
	};

#endif // !defined(AFX_HL7DBSegmentDef_H__A5625ABF_F0F9_11D2_87A6_00104B9E6286__INCLUDED_)
