//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _HL7FILEDAEMON_H_
#define _HL7FILEDAEMON_H_
#pragma once

//--------------------------------------------------------------------------------
#include "HL7InputDaemon.h"
#include "HL7Message.h"

//--------------------------------------------------------------------------------
class CHL7FileDaemon : public CHL7InputDaemon
	{
	public:
		CHL7FileDaemonThread();
		virtual ~CHL7FileDaemonThread();

	private:
		CStdioFile* m_pCurFile;
		CStdioFile* m_pAckFile;

	public:
		virtual void MainLoop();
		virtual bool SendString(LPCTSTR);
	};

#endif // _HL7FILEDAEMON_H_

