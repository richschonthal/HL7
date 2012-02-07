//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// HL7MSAConfig.h: interface for the CHL7MSAConfig class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HL7MSACONFIG_H__2E9B8FD1_0EF9_11D3_87AF_00104B9E6286__INCLUDED_)
#define AFX_HL7MSACONFIG_H__2E9B8FD1_0EF9_11D3_87AF_00104B9E6286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <result.h>

//--------------------------------------------------------------------------------
class CPACSDatabase;

//--------------------------------------------------------------------------------
class CHL7MSAConfigItem : public CObject
	{
	public:
		int m_nSegment;
		int m_nType;
		int m_nVersion;
		CString m_sValue;

		void TraceDump() const;
	};

//--------------------------------------------------------------------------------
class CHL7MSAConfigItemArray : public CTypedPtrArray<CObArray, CHL7MSAConfigItem*>
	{
	public:
		~CHL7MSAConfigItemArray()
			{
			for(int i = GetUpperBound(); i >= 0; i--)
				delete GetAt(i);
			RemoveAll();
			}
	};

//--------------------------------------------------------------------------------
class CHL7MSAConfigItemIndex : public CTypedPtrArray<CObArray, CHL7MSAConfigItem*>
	{
	public:
		~CHL7MSAConfigItemIndex()
			{
			RemoveAll();
			}
	};

//--------------------------------------------------------------------------------
class CHL7MSAConfig : public CResult
	{
	public:
		// datatypes - ie the values in the 'type' column
		enum
			{
			MSADT_EMPTY_FIELD,
			MSADT_SEGMENT_NAME,
			MSADT_MSH_FIELD_NUM,
			MSADT_DATE,
			MSADT_STRING,
			MSADT_FIELD_DELIM,
			MSADT_CHAR_CODE,
			MSADT_OK_ERR
			};

	private:
		static int m_nRefCount;
		static CHL7MSAConfigItemArray* m_pItems;

	private:
		CHL7MSAConfigItemIndex m_index;

	private:
		friend class CStaticHolder;
		bool Init(CPACSDatabase&);

	public:
		CHL7MSAConfig();
		CHL7MSAConfig(int);
		virtual ~CHL7MSAConfig();

		const CHL7MSAConfigItem* GetItem(int) const;
		int GetItemCount() const;
	};

#endif // !defined(AFX_HL7MSACONFIG_H__2E9B8FD1_0EF9_11D3_87AF_00104B9E6286__INCLUDED_)
