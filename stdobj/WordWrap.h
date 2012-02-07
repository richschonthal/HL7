//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _WORDWRAP_H_
#define _WORDWRAP_H_

//--------------------------------------------------------------------------------
class CWordWrap
	{
	public:
		static bool Wrap(CStringArray&, LPCTSTR, int, LPCTSTR = NULL);
	};


#endif //_WORDWRAP_H_
