//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// HL7MessageConfig.h: interface for the CHL7MessageConfig class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HL7MESSAGECONFIG_H__E50FD358_F982_11D2_87A7_00104B9E6286__INCLUDED_)
#define AFX_HL7MESSAGECONFIG_H__E50FD358_F982_11D2_87A7_00104B9E6286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
class CHL7Segment;

//--------------------------------------------------------------------------------
class CHL7MessageConfig  
	{
	public:
		CHL7MessageConfig(const CHL7Segment* = NULL);

		char m_cSepComponent;
		char m_cSepSubComponent;
		char m_cSepRepetition;
		char m_cEscape;

		void SetConfiguration(const CHL7Segment*);

		void SerializeConfiguration(CArchive&);

	};

#endif // !defined(AFX_HL7MESSAGECONFIG_H__E50FD358_F982_11D2_87A7_00104B9E6286__INCLUDED_)
