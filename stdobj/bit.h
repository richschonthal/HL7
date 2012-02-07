//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _BIT_H_
#define _BIT_H_

//--------------------------------------------------------------------------------
inline DWORD BitSet(DWORD nData, DWORD nBit)
	{ return (nData | nBit); }

//--------------------------------------------------------------------------------
inline DWORD BitClr(DWORD nData, DWORD nBit)
	{ return (nData & ~nBit); }

//--------------------------------------------------------------------------------
inline DWORD BitToggle(DWORD nData, DWORD nBit)
	{ return (nData ^ nBit); }

//--------------------------------------------------------------------------------
inline DWORD BoolToInt(DWORD nBool)
	{ return nBool == 0 ? 0:1; }

//--------------------------------------------------------------------------------
inline bool BoolFromInt(DWORD nBool)
	{ return nBool == 0 ? false:true; }

#endif //_BIT_H_
