//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// HL7Object.h: interface for the CHL7Object class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HL7OBJECT_H__A5625AA0_F0F9_11D2_87A6_00104B9E6286__INCLUDED_)
#define AFX_HL7OBJECT_H__A5625AA0_F0F9_11D2_87A6_00104B9E6286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <result.h>

#include <commondefines.h>

//--------------------------------------------------------------------------------
class CHL7Object : public CObject, public CResult
	{
	public:
		enum eHL7Version
			{ VERSION_UNKNOWN, VER_2_2, VER_2_3, VER_PRINCETON };

	public:
		CHL7Object();
		virtual ~CHL7Object();

	private:
		eHL7Version m_nHL7Version;		

	public:
		//--------------------------------------------------------------------------------
		void SetVersion(eHL7Version nVer) { m_nHL7Version = nVer; }
		eHL7Version GetVersion() { return m_nHL7Version;}

		//--------------------------------------------------------------------------------
		// static functions
		static int GetMsgId(char sSegmentName[4]);
		static LPCTSTR GetFieldDesc(int nMsgId, int nFieldNo);
		static LPCTSTR GetFieldName(int nMsgId, int nFieldNo);
	};

#endif // !defined(AFX_HL7OBJECT_H__A5625AA0_F0F9_11D2_87A6_00104B9E6286__INCLUDED_)
