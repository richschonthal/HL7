//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _NETFILEINFO_H_
#define _NETFILEINFO_H_

//--------------------------------------------------------------------------------
#include "NetConnection.h"

//--------------------------------------------------------------------------------
class CNetFileInfo : public CNetConnection
	{
	public:
		CString m_sFilename;
		CString m_sPath;

	public:
		CString GetFullPathname() const;
	};

#endif //_NETFILEINFO_H_
