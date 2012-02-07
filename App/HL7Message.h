//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#if !defined(AFX_HL7MESSAGE_H__A5625AA5_F0F9_11D2_87A6_00104B9E6286__INCLUDED_)
#define AFX_HL7MESSAGE_H__A5625AA5_F0F9_11D2_87A6_00104B9E6286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HL7Object.h"
#include "HL7Segment.h"
#include <archiveobj.h>
#include <commondefines.h>

//--------------------------------------------------------------------------------
#define MSH_ID 1

//--------------------------------------------------------------------------------
class CHL7InputDaemon;

//--------------------------------------------------------------------------------
class CHL7Message : public CHL7Object
	{
	public:
		enum eMsgResult
			{
			MSG_OK,
			// returned when trying to add an MSH segment
			// to a msg that already has an MSH
			MSG_START_NEW,
			MSG_ERROR,
			MSG_BLANK
			};

	public:
		CHL7Message();
		virtual ~CHL7Message();

	private:
		// time/date of msg receive
		CTime m_ctTimeReceived;

		// time/date of message being handled and released
		CTime m_ctTimeAcknowledged;

		// the "place" this msg came from (socket, file, or serial)
		DWORD m_nSourceId;

		// all the segments for this msg
		CHL7SegmentArray* m_pSegments;

		char m_cFieldDelim;
		int m_nVersion;
		char m_cEncodingChars[4];

	public:
		eMsgResult AddSegment(LPCTSTR);
		eMsgResult AddSegment(CHL7Segment*);

		CHL7SegmentArray* GetSegmentsPtr() const	{ return m_pSegments; }

		CHL7Segment* GetSegment(int) const;

		int GetSegmentCount() const;

		char GetFieldDelim() const		{ return m_cFieldDelim; }
		void SetFieldDelim(char c)		{ m_cFieldDelim = c; }

		void SetEncodingChars(LPCTSTR);

		char GetComponentSeparator()	{ return m_cEncodingChars[0]; }
		char GetRepetitionSeparator()	{ return m_cEncodingChars[1]; }
		char GetEscapeChar()			{ return m_cEncodingChars[2]; }
		char GetSubcomponentSeparator()	{ return m_cEncodingChars[3]; }

		int GetVersion() const			{ return m_nVersion; }
		void SetVersion(int v)			{ m_nVersion = v; }

		int GetSourceId() const			{ return m_nSourceId; }
		void SetSourceId(int si)		{ m_nSourceId = si; }

		void TraceDump();

		void BuildString(CString&);

		CHL7Segment* Find(int nGroupNo) const { return m_pSegments->FindByGroup(nGroupNo); }
	};

//--------------------------------------------------------------------------------
class CHL7MessageList : public CTypedPtrList<CObList, CHL7Message*>
	{
	};

#endif // !defined(AFX_HL7MESSAGE_H__A5625AA5_F0F9_11D2_87A6_00104B9E6286__INCLUDED_)
