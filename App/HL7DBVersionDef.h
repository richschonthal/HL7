//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// HL7DBVersionDef.h: interface for the CHL7DBVersionDef class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HL7DBVersionDef_H__A5625ABF_F0F9_11D2_87A6_00104B9E6286__INCLUDED_)
#define AFX_HL7DBVersionDef_H__A5625ABF_F0F9_11D2_87A6_00104B9E6286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <commondefines.h>
#include <result.h>

//--------------------------------------------------------------------------------
class CHL7DBVersionDefItem : public CObject
	{
	public:
		CHL7DBVersionDefItem() : m_nVersionId(0), m_pAppName(NULL), m_pAppFacility(NULL), m_pHL7Version(NULL), m_pDesc(NULL) {}
		~CHL7DBVersionDefItem() {delete[] m_pAppName;delete[] m_pAppFacility;delete[] m_pHL7Version;delete[] m_pDesc;}

		long m_nVersionId;
		LPTSTR m_pAppName;
		LPTSTR m_pAppFacility;
		LPTSTR m_pHL7Version;
		LPTSTR m_pDesc;

		void TraceDump();
	};

//--------------------------------------------------------------------------------
class CHL7DBVersionDefItemList : public CTypedPtrList<CObList, CHL7DBVersionDefItem*>
	{
	public:
		~CHL7DBVersionDefItemList()
			{
			for(POSITION pos = GetHeadPosition(); pos; GetNext(pos))
				delete GetAt(pos);
			RemoveAll();
			}

		const CHL7DBVersionDefItem* Find(int) const;
		const CHL7DBVersionDefItem* Find(LPCTSTR, LPCTSTR, LPCTSTR) const;
	};

//--------------------------------------------------------------------------------
class CHL7DBVersionDef : public CResult
	{
	public:
		CHL7DBVersionDef();
		virtual ~CHL7DBVersionDef();

	private:
		friend class CStaticHolder;
		bool Init(CPACSDatabase&);

	private:
		// const values
		static LPCTSTR pTableName;
		static CHL7DBVersionDefItemList* m_pMsgDefList;
		static int m_nRefCount;

	public:
		const CHL7DBVersionDefItem* Find(int nId) const;
		const CHL7DBVersionDefItem* Find(LPCTSTR, LPCTSTR, LPCTSTR) const;

		int GetRecordCount() const { return (m_pMsgDefList == NULL) ? 0 : m_pMsgDefList->GetCount(); }
	};

#endif // !defined(AFX_HL7DBVersionDef_H__A5625ABF_F0F9_11D2_87A6_00104B9E6286__INCLUDED_)
