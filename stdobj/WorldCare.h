// WorldCare.h: interface for the CWorldCare class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __MC_WORLDCARE_H__
#define __MC_WORLDCARE_H__

#if _MSC_VER>1000
#pragma once
#endif //_MSC_VER>1000

#include <wipb.h> // Required WorldCare header.

#ifndef ERR_NONE
#define ERR_NONE	(0L)
#endif

class CWorldCare : public CObject  
{
	DECLARE_DYNAMIC(CWorldCare);
public:
	CWorldCare();
	virtual ~CWorldCare();	
	bool WaveletToRaw(
			BYTE *pbIn,
			DWORD dwLenIn, 
			BYTE **ppbOut,
			DWORD *dwLenOut,
			WORD *pwWidth,
			WORD *pwHeight,
			BYTE *pbBpp, 
			LONG (*fpTick)()=NULL);
	bool LLJpegToRaw(
			BYTE *pbIn,
			DWORD dwLenIn, 
			BYTE **ppbOut,
			DWORD *pdwLenOut,
			WORD *pwWidth,
			WORD *pwHeight, 
			BYTE *pbBpp,
			LONG (*fpTick)());
	bool RawToWavelet(
			BYTE *pbIn,
			DWORD dwLenIn,
			WORD wWidth,
			WORD wHeight,
			BYTE bBpp, 
			BYTE **ppbOut,
			DWORD *pdwLenOut,
			WORD wQuantizationThreshold, 
			WORD *pwType,
			LONG (*fpTick)()=NULL);
	bool RawToLLJpeg(
			BYTE *pbIn,
  			DWORD dwLenIn,
			WORD wWidth,
			WORD wHeight,
			BYTE bBpp, 
			BYTE **ppbOut,
			DWORD *dwLenOut,
			LONG (*fpTick)()=NULL);
	void Free(BYTE **ppbOut); 
	LONG VersionMajor();
	LONG VersionMinor();
	LONG VersionBuild();
	LPCTSTR	Version();
	LPCTSTR	Copyright();
	INT	About(HWND hWnd=NULL);
public:
	LONG GetLastError();	
	// How many CWorldCare objects do we have.
	static USHORT m_nInstances;	
	// This is the standard value used for all WorldCare WIPB API's that require 
	// a "compression type" as a passed parameter. Specifiing this value tells
	// WorldCare that you desire WaveletII compression type. Currently this is what
	// we are using.
	static const WORD m_WaveletII;	
private:
	LONG m_lErr;
	bool SetLastError(LONG lErr=ERR_NONE);	
};

#endif // __MC_WORLDCARE_H__
