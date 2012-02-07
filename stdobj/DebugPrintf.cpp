//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"

//--------------------------------------------------------------------------------
int DebugPrintf(LPCTSTR pFormat, ...)
	{
	va_list marker;

	va_start(marker, pFormat);
	TCHAR temp[2048];

	int nCount = _vstprintf(temp, pFormat, marker);
	OutputDebugString(temp);

	return nCount;
	}

