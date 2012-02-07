//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _ALLOCSTRING_H_
#define _ALLOCSTRING_H_

//--------------------------------------------------------------------------------
inline void AllocString(LPTSTR& pDest, LPCTSTR pSrc)
	{
	delete[] pDest;
	if(pSrc==NULL)
		{
		pDest=new TCHAR[sizeof(TCHAR)];
		*pDest=0;
		}
	else
		{
		pDest=new TCHAR[_tcslen(pSrc) + sizeof(TCHAR)];
		_tcscpy(pDest, pSrc);
		}
	}

//--------------------------------------------------------------------------------
inline LPTSTR AllocString(LPCTSTR pSrc)
	{
	if(pSrc == NULL)
		return NULL;

	LPTSTR pDest=new TCHAR[_tcslen(pSrc) + sizeof(TCHAR)];
	_tcscpy(pDest, pSrc);
	return pDest;
	}

#ifndef ALLOC_STRING
#define ALLOC_STRING AllocString
#endif

#endif // _ALLOCSTRING_H_
