//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _HL7SYSTEMMSG_H_
#define _HL7SYSTEMMSG_H_

enum eMessages
	{
	INIT = WM_USER,
	HL7_HELLO,
	HL7_HL7MSG,
	HL7_HL7MSG_IN,
	HL7_GOODBYE,
	HL7_PROTOCOL,
	HL7_GO,
	HL7_SECURITY_SERVER
	};

#endif //_HL7SYSTEMMSG_H_