//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// HL7MessageVerify.h: interface for the CHL7MessageVerify class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HL7MESSAGEVERIFY_H__8AF62AA8_FD70_11D2_87A7_00104B9E6286__INCLUDED_)
#define AFX_HL7MESSAGEVERIFY_H__8AF62AA8_FD70_11D2_87A7_00104B9E6286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <result.h>
#include "HL7Message.h"

//--------------------------------------------------------------------------------
class CMsgVerifyResult : public CResult
	{
	public:
		DECLARE_RESULTCLASS(CMsgVerifyResult)

		DECLARE_RESULTMAP(m_map)

		enum
			{
			errNoMsh = BeginErrorRange,
			errNoSeg,
			errNoSegId,
			errInvalidString,
			errInvalidTime,
			errMandFieldMissing,
			errFieldTooLarge,
			errFieldItemMissing
			};
	};

//--------------------------------------------------------------------------------
class CHL7MessageVerify : public CMsgVerifyResult  
	{
	public:
		CHL7MessageVerify(CHL7Message&);

		bool VerifySegment(const CHL7Segment*);

		static bool IsTimeOk(LPCTSTR);
		static bool IsDateOk(LPCTSTR);
		static bool SplitDateTime(char*, char*, LPCTSTR);
		static bool VerifyDateTime(LPCTSTR);
		static bool IsSequenceOk(LPCTSTR);
		static bool IsIdOk(LPCTSTR);
		static bool IsNumericOk(LPCTSTR);
		static bool IsStringOk(LPCTSTR);
	};

#endif // !defined(AFX_HL7MESSAGEVERIFY_H__8AF62AA8_FD70_11D2_87A7_00104B9E6286__INCLUDED_)
