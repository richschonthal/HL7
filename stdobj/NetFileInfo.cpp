//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "NetFileInfo.h"

//--------------------------------------------------------------------------------
CString CNetFileInfo::GetFullPathname() const
	{
	CString sRv(m_sPath);
	if(! sRv.IsEmpty())
		if(sRv.Right(1) != '\\')
			sRv += '\\';
	if(! m_sFilename.IsEmpty())
		sRv += m_sFilename;
	if(! m_sServer.IsEmpty())
		sRv = "\\\\" + m_sServer + "\\" + sRv;
	TRACE(_T("%s\n"), (LPCTSTR) sRv);
	return sRv;
	}
