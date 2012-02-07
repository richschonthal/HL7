//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// MCWavelet.h: interface for the CWavelet class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _MCWAVELET_H_
#define _MCWAVELET_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER>1000

#include <dcmtypes.h>

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
		bool IsMark0(LPCTSTR pFilename) const;
		bool PackFile(LPCTSTR pFilename, LPCTSTR pPath, DCM_HEADER* pHeader, int nQual, LPCTSTR pQualityFile);
		bool UnpackFile(LPCTSTR pFilename, LPCTSTR pPath);	
	};

#endif // #ifndef _MCWAVELET_H_
