//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _HL7IOSUBSYSTEM_H_
#define _HL7IOSUBSYSTEM_H_

#include <IOSubSystem.h>

class CHL7System;

//--------------------------------------------------------------------------------
class CThreadNameList : public CMap<DWORD, DWORD, LPTSTR, LPTSTR&>
	{
	};

//--------------------------------------------------------------------------------
class CHL7IOSubSystem : public CIOSubSystem
	{
	DECLARE_DYNAMIC(CHL7IOSubSystem);

	private:
		CThreadNameList m_threadNames;
		static LPCTSTR m_pDefaultName;

	public:
		CHL7IOSubSystem(CHL7System*);
		virtual ~CHL7IOSubSystem();

		virtual void WriteLogFileHeader();

		virtual bool OutputToFile(LPCTSTR, CStdioFile*);

		void AddThreadName(LPCTSTR);
		LPCTSTR GetThreadName();

		void SetTraceLevel(DWORD);

		bool Output(DWORD, LPCTSTR);
		bool Output(DWORD, LPTSTR);
		bool FormatOutput(DWORD, LPCTSTR, ...);
	};

#endif //_HL7IOSUBSYSTEM_H_
