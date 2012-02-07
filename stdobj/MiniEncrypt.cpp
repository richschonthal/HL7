//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// MiniEncrypt.cpp: implementation of the CMiniEncrypt class.
//
//////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------
#include "stdafx.h"
#include "MiniEncrypt.h"
#include "EncryptTables.hpp"

//--------------------------------------------------------------------------------
static const TBYTE g_key[] = { 0x73, 0x0E, 0x3B, 0xA1, 0xB9, 0xD3, 0x1C, 0x68, 0x00};

//--------------------------------------------------------------------------------
CMiniEncrypt::CMiniEncrypt(const TBYTE* pData, int nLen, const TBYTE* pKey)
		: m_pKey(NULL)
		, m_nLen(0)
		, m_pData(NULL)
	{
	Encrypt(pData, nLen, pKey);
	}

//--------------------------------------------------------------------------------
CMiniEncrypt::~CMiniEncrypt()
	{
	delete[] m_pData;
	delete[] m_pKey;
	}

//--------------------------------------------------------------------------------
bool CMiniEncrypt::Encrypt(const TBYTE* pData, int nLen, const TBYTE* pKey)
	{
	delete[] m_pData;
	m_pData = NULL;
	m_nLen = 0;
	delete[] m_pKey;
	m_pKey = NULL;

	// must be at least 4 bytes long to encrypt
	if(nLen < 4)
		return false;

	if(pData == NULL)
		return false;

	const TBYTE* pSrcKey = (pKey == NULL) ? g_key : pKey;

	// keys are null terminated so they look like strings
	m_pKey = new TBYTE[_tcslen((LPCTSTR) pSrcKey) + 1];

	_tcscpy((LPTSTR) m_pKey, (LPCTSTR) pSrcKey);

	// copy the length
	m_nLen = nLen;
	// allocate buffer
	m_pData = new TBYTE[m_nLen];
	// always encrypt the 1st byte with 7F
	m_pData[0] = pData[0] ^ 0x7F;
	// get the "shift" table using the 1st byte to determine which
	// table to use
	const TBYTE* pTable = g_encryptTblTbl[(int) m_pData[0]];

	int nCurKey = 0;
	for(int i = 1; i < m_nLen; i++)
		{
		//encrypt with key and shift it
		m_pData[i] = pTable[(int) (pData[i] ^ m_pKey[nCurKey++])];
		// wrap the key index when null is reached
		if(m_pKey[nCurKey] == 0)
			nCurKey = 0;
		}

	return true;
	}
