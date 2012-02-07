//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#if !defined(AFX_HL7SEGMENT_H__A5625AA4_F0F9_11D2_87A6_00104B9E6286__INCLUDED_)
#define AFX_HL7SEGMENT_H__A5625AA4_F0F9_11D2_87A6_00104B9E6286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HL7Object.h"

#include <commondefines.h>

//--------------------------------------------------------------------------------
class CHL7Message;

//--------------------------------------------------------------------------------
class CHL7Segment : public CHL7Object  
	{
	friend class CHL7Message;
	friend class CHL7MessageConfig;
	friend class CHL7SegmentArray;

	public:
		enum eSegmentCode
			{
			SEGMENT_OK,
			SEGMENT_ERROR,
			SEGMENT_BLANK,
			SEGMENT_UNSUPPORTED,
			SEGMENT_IS_MSH
			};

	public:
		CHL7Segment(CHL7SegmentArray* = NULL, LPCTSTR = NULL);
		CHL7Segment(int id, int group);
		virtual ~CHL7Segment();

	private:
		int m_nId;
		int m_nGroup;
		CString m_sName;
		CPtrArray m_saData;
		// pointer to the segments array of the HL7message parent of this segment
		CHL7SegmentArray* m_pArray;

	public:
		int GetId() const			{ return m_nId; }

		LPCTSTR GetField(int nFieldNo) const;
		void SetField(int, LPCTSTR);
		void AddField(LPCTSTR);
		void RemoveAllFields();
		void RemoveField(int);

		int GetGroup() const			{ return m_nGroup; }
		void SetGroup(int g)			{ m_nGroup = g; }

		int GetFieldCount() const		{ return m_saData.GetSize(); }
		void SetFieldCount(int nCount)	{ m_saData.SetSize(nCount); }

		CString GetName() const			{ return m_sName; }
		void SetName(LPCTSTR p)			{ m_sName = p; }

		eSegmentCode Parse(LPCTSTR, bool = false);

		CHL7Message* GetMsgPtr() const;

		void BuildString(CString&);
		
		operator CString()				{ CString sTemp; BuildString(sTemp); return sTemp; }
	};

//--------------------------------------------------------------------------------
class CHL7SegmentArray : public CArray<CHL7Segment*, CHL7Segment*>
	{
	friend class CHL7Message;
	friend class CHL7Segment;

	private:
		CHL7Message* m_pMsg;

	private:
		CHL7SegmentArray(CHL7Message* p) : m_pMsg(p) {}

		void GetGroupSortArray(CDWordArray&);
		CHL7Segment* FindByGroup(int) const;
	};

#endif // !defined(AFX_HL7SEGMENT_H__A5625AA4_F0F9_11D2_87A6_00104B9E6286__INCLUDED_)
