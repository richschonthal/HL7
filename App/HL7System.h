//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _HL7SYSTEM_H_
#define _HL7SYSTEM_H_

//--------------------------------------------------------------------------------
#include <System.h>

class CHL7IOSubSystem;

//--------------------------------------------------------------------------------
class CHL7System : public CSystem
	{
	private:
		CHL7IOSubSystem* m_pIO;

#ifdef _DEBUG
	public:
		virtual LPCTSTR DebugGetSystemObjectName();
#endif

	public:
		CHL7System();
		virtual ~CHL7System();
		CHL7IOSubSystem* GetIO() { return m_pIO; }
	};

#endif // _HL7SYSTEM_H_
