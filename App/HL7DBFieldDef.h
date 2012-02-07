//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#if !defined(AFX_HL7DBFieldDef_H__A5625ABF_F0F9_11D2_87A6_00104B9E6286__INCLUDED_)
#define AFX_HL7DBFieldDef_H__A5625ABF_F0F9_11D2_87A6_00104B9E6286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <commondefines.h>
#include <result.h>
//--------------------------------------------------------------------------------
class CPACSDatabase;

//--------------------------------------------------------------------------------
class CHL7DBFieldDefItem : public CObject
	{
	public:
		CHL7DBFieldDefItem() : m_pDesc(NULL), m_pDatatype(NULL) {}
		~CHL7DBFieldDefItem() {delete[] m_pDesc; delete[] m_pDatatype;}

		void TraceDump();

		long m_nSegId;
		long m_nFieldId;
		LPTSTR m_pDesc;
		long m_nRuleId;
		long m_nVersionId;
		long m_nFieldLen;
		LPTSTR m_pDatatype;
		bool m_bMandatory;
		long m_nCalluid;
		long m_nWrapLen;
	};

//--------------------------------------------------------------------------------
class CHL7DBFieldDefItemArray : public CTypedPtrArray<CObArray, CHL7DBFieldDefItem*>
	{
	public:
		CHL7DBFieldDefItem* FindById(long) const;
	};

//--------------------------------------------------------------------------------
class CHL7DBFieldDefItemList : public CTypedPtrList<CObList, CHL7DBFieldDefItem*>
	{
	public:
		~CHL7DBFieldDefItemList()
			{
			for(POSITION pos = GetHeadPosition(); pos; GetNext(pos))
				delete GetAt(pos);
			RemoveAll();
			}

		const CHL7DBFieldDefItem* Find(int nVersion, int nCalluid) const;

		const CHL7DBFieldDefItem* Find(int nVersion, int nSegId, int nFieldId) const;
		int Find(int nVersion, int nSegId, CHL7DBFieldDefItemArray&) const;
		CHL7DBFieldDefItem* FindFirst(int nVer, int nSegId) const;
		CHL7DBFieldDefItem* FindLast(int nVer, int nSegId) const;
	};

//--------------------------------------------------------------------------------
class CHL7DBFieldDef : public CResult
	{
	public:
		CHL7DBFieldDef(int nVersion);
		// the only constructor that creates an index
		CHL7DBFieldDef(int nVersion, int nSegId);

		virtual ~CHL7DBFieldDef();

	private:
		friend class CStaticHolder;
		CHL7DBFieldDef();
		bool Init(CPACSDatabase&);

	private:
		// const values
		static LPCTSTR pTableName;
		static CHL7DBFieldDefItemList* m_pFieldDefList;
		static int m_nRefCount;

	private:
		int m_nVersion;
		CHL7DBFieldDefItemArray m_index;

	public:
		int GetRecordCount() const;

		const CHL7DBFieldDefItem* Find(int nCalluid) const;
		const CHL7DBFieldDefItem* Find(int nSegId, int nFieldId) const;
		int Find(int nSegId, CHL7DBFieldDefItemArray&) const;

		CHL7DBFieldDefItem* FindFirst(int nSegId) const { return m_pFieldDefList->FindFirst(m_nVersion, nSegId); }
		CHL7DBFieldDefItem* FindLast(int nSegId) const { return m_pFieldDefList->FindLast(m_nVersion, nSegId); }

		const CHL7DBFieldDefItem* GetIndexedItem(int) const;
	};

#endif // !defined(AFX_HL7DBFieldDef_H__A5625ABF_F0F9_11D2_87A6_00104B9E6286__INCLUDED_)
