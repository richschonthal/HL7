//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// HL7OutMsgFormat.h: interface for the CHL7OutMsgFormat class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _HL7DBOUTMSGFORMAT_H_
#define _HL7DBOUTMSGFORMAT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <result.h>

//--------------------------------------------------------------------------------
class CPACSDatabase;

//--------------------------------------------------------------------------------
class CHL7OutMsgFormatItem : public CObject
	{
	public:
		long m_nVersion;
		long m_nType;
		CString m_sValue;

		void TraceDump() const;
	};

//--------------------------------------------------------------------------------
class CHL7OutMsgFormatItemArray : public CTypedPtrArray<CObArray, CHL7OutMsgFormatItem*>
	{
	public:
		~CHL7OutMsgFormatItemArray()
			{
			for(int i = GetUpperBound(); i >= 0; i--)
				delete GetAt(i);
			RemoveAll();
			}
	};

//--------------------------------------------------------------------------------
class CHL7OutMsgFormatItemIndex : public CTypedPtrArray<CObArray, CHL7OutMsgFormatItem*>
	{
	public:
		~CHL7OutMsgFormatItemIndex()
			{
			RemoveAll();
			}
	};

//--------------------------------------------------------------------------------
class CHL7OutMsgFormat : public CResult
	{
	public:
		// datatypes - ie the values in the 'type' column
		enum
			{
			OMFMTDT_COMMENT,
			OMFMTDT_INSERT_MSH,
			OMFMTDT_INSERT_PAC,
			OMFMTDT_RESERVED3,
			OMFMTDT_STRING,
			OMFMTDT_RESERVED5,
			OMFMTDT_CHAR_CODE,
			};

	private:
		static int m_nRefCount;
		static CHL7OutMsgFormatItemArray* m_pItems;

	private:
		CHL7OutMsgFormatItemIndex m_index;

	public:
		CHL7OutMsgFormat(CPACSDatabase&);
		CHL7OutMsgFormat(int);
		virtual ~CHL7OutMsgFormat();

		const CHL7OutMsgFormatItem* GetItem(int) const;
		int GetItemCount() const;
	};

#endif // !defined(_HL7DBOUTMSGFORMAT_H_)
