//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// Wavelet.h: interface for the CWavelet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAVELET_H__F40BB5D0_347B_11D3_87C7_00104B9E6286__INCLUDED_)
#define AFX_WAVELET_H__F40BB5D0_347B_11D3_87C7_00104B9E6286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dcmtypes.h"

//--------------------------------------------------------------------------------
class CWavelet  
	{
	public:
		CWavelet();
		virtual ~CWavelet();

	public:
		enum {PACK, UNPACK};
		static LPCTSTR	m_pTempExt;		// File extension used for temporary compressed files.
		static int		m_nStdQuality;	// Set this to the @COMPANY standard quality setting.

	private:
		static LPCTSTR	m_pSignature;
		static WORD		m_nSigLen;
		
	public:
		bool PackFile(LPCTSTR pFilename, LPCTSTR pPath, DCM_HEADER* pHeader, int nQual, LPCTSTR pQualityFile);
		bool UnpackFile(LPCTSTR pFilename, LPCTSTR pPath);
		bool CallWaveProg(int, LPCTSTR pParams, LPCTSTR pPath);
	};

#endif // !defined(AFX_WAVELET_H__F40BB5D0_347B_11D3_87C7_00104B9E6286__INCLUDED_)
