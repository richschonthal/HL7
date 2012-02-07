//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// MiniEncrypt.h: interface for the CMiniEncrypt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MINIENCRYPT_H__7951B9A4_74F9_11D3_AEE4_005004A1C5F3__INCLUDED_)
#define AFX_MINIENCRYPT_H__7951B9A4_74F9_11D3_AEE4_005004A1C5F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
class CMiniEncrypt  
	{
	private:
		TBYTE* m_pData;
		TBYTE* m_pKey;
		int m_nLen;

	public:
		CMiniEncrypt(const TBYTE* = NULL, int = 0, const TBYTE* = NULL);
		virtual ~CMiniEncrypt();

		int GetLength()			{ return m_nLen; }
		TBYTE* GetData()			{ return m_pData; }

		bool Encrypt(const TBYTE*, int, const TBYTE* = NULL);
	};

#endif // !defined(AFX_MINIENCRYPT_H__7951B9A4_74F9_11D3_AEE4_005004A1C5F3__INCLUDED_)
