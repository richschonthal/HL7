//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#if !defined(AFX_HL7DBMessageDef_H__A5625ABF_F0F9_11D2_87A6_00104B9E6286__INCLUDED_)
#define AFX_HL7DBMessageDef_H__A5625ABF_F0F9_11D2_87A6_00104B9E6286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <commondefines.h>
#include <result.h>

//--------------------------------------------------------------------------------
class CHL7DBMessageDefItem : public CObject
	{
	public:
		CHL7DBMessageDefItem() : m_pMsgName(NULL), m_pMsgDesc(NULL) {}
		~CHL7DBMessageDefItem() {delete[] m_pMsgName; delete[] m_pMsgDesc;}

		int m_nMsgId;
		LPTSTR m_pMsgName;
		LPTSTR m_pMsgDesc;

		void TraceDump();
	};

//--------------------------------------------------------------------------------
class CHL7DBMessageDefItemList : public CTypedPtrList<CObList, CHL7DBMessageDefItem*>
	{
	public:
		~CHL7DBMessageDefItemList()
			{
			//AFX_MANAGE_STATE(AfxGetStaticModuleState());

			for(POSITION pos = GetHeadPosition(); pos; GetNext(pos))
				delete GetAt(pos);
			RemoveAll();
			}

		const CHL7DBMessageDefItem* Find(int) const;
		const CHL7DBMessageDefItem* Find(LPCTSTR) const;
	};

//--------------------------------------------------------------------------------
class CHL7DBMessageDef : public CResult
	{
	public:
		CHL7DBMessageDef();
		virtual ~CHL7DBMessageDef();

	private:
		friend class CStaticHolder;
		bool Init(CPACSDatabase&);

	private:
		// const values
		static LPCTSTR pTableName;

		static CHL7DBMessageDefItemList* m_pMsgDefList;
		static int m_nRefCount;

	public:
		const CHL7DBMessageDefItem* Find(int nId) const;
		const CHL7DBMessageDefItem* Find(LPCTSTR pName) const;

		int GetRecordCount() const { return (m_pMsgDefList == NULL) ? 0 : m_pMsgDefList->GetCount(); }
	};

#endif // !defined(AFX_HL7DBMessageDef_H__A5625ABF_F0F9_11D2_87A6_00104B9E6286__INCLUDED_)
