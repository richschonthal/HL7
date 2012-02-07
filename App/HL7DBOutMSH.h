//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// HL7OutMSH.h: interface for the CHL7OutMSH class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _HL7DBOUTMSH_H_
#define _HL7DBOUTMSH_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <result.h>

//--------------------------------------------------------------------------------
class CPACSDatabase;

//--------------------------------------------------------------------------------
class CHL7OutMSHItem : public CObject
	{
	public:
		int m_nSegment;
		int m_nType;
		int m_nVersion;
		CString m_sValue;

		void TraceDump() const;
	};

//--------------------------------------------------------------------------------
class CHL7OutMSHItemArray : public CTypedPtrArray<CObArray, CHL7OutMSHItem*>
	{
	public:
		~CHL7OutMSHItemArray()
			{
			for(int i = GetUpperBound(); i >= 0; i--)
				delete GetAt(i);
			RemoveAll();
			}
	};

//--------------------------------------------------------------------------------
class CHL7OutMSH : public CResult
	{
	public:
		// datatypes - ie the values in the 'type' column
		enum
			{
			MSHDT_EMPTY_FIELD,
			MSHDT_SEGMENT_NAME,
			MSHDT_MESSAGE_CONTROL_ID,
			MSHDT_DATE,
			MSHDT_STRING,
			MSHDT_FIELD_DELIM,
			MSHDT_CHAR_CODE,
			MSHDT_RESERVED2,
			};

	private:
		static int m_nRefCount;
		static CHL7OutMSHItemArray* m_pItems;

	public:
		CHL7OutMSH(CPACSDatabase&);
		CHL7OutMSH();
		virtual ~CHL7OutMSH();

		const CHL7OutMSHItem* GetItem(int) const;
		int GetItemCount() const;
	};

#endif // !defined(_HL7DBOUTMSH_H_)
