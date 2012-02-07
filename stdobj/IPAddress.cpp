//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#include "stdafx.h"
#include "IPAddress.h"

//--------------------------------------------------------------------------------
CIPAddress::CIPAddress(DWORD nAddress)
	{
	m_nDWord = nAddress;
	}

//--------------------------------------------------------------------------------
CIPAddress::CIPAddress(LPCTSTR pAddr)
	{
	m_nDWord = 0;
	if(pAddr == NULL)
		return;
	StringToAddr(pAddr, m_nDWord);
	}

//--------------------------------------------------------------------------------
CIPAddress::CIPAddress(const CIPAddress& rVal)
	{
	m_nDWord = rVal.m_nDWord;
	}

//--------------------------------------------------------------------------------
CIPAddress::~CIPAddress()
	{
	}

//--------------------------------------------------------------------------------
bool CIPAddress::StringToAddr(LPCTSTR pAddr, DWORD& nAddr)
	{
	nAddr = 0;
	if(pAddr == NULL)
		return false;

	CString sTemp;

	while(*pAddr != 0 && *pAddr != '.')
		{
		if(isdigit(*pAddr))
			sTemp += *pAddr;
		pAddr++;
		}

	if(*pAddr == 0)
		return false;

	IPADDRESS ip;
	if(! sTemp.IsEmpty())
		ip.m_bits.n4 = _ttoi((LPCTSTR) sTemp);

	pAddr++;
	sTemp.Empty();

	while(*pAddr != 0 && *pAddr != '.')
		{
		if(isdigit(*pAddr))
			sTemp += *pAddr;
		pAddr++;
		}

	if(*pAddr == 0)
		return false;

	if(! sTemp.IsEmpty())
		ip.m_bits.n3 = _ttoi(sTemp);

	pAddr++;
	sTemp.Empty();

	while(*pAddr != 0 && *pAddr != '.')
		{
		if(isdigit(*pAddr))
			sTemp += *pAddr;
		pAddr++;
		}

	if(*pAddr == 0)
		return false;

	if(! sTemp.IsEmpty())
		ip.m_bits.n2 = _ttoi(sTemp);

	pAddr++;
	sTemp.Empty();

	while(*pAddr != 0)
		{
		if(isdigit(*pAddr))
			sTemp += *pAddr;
		pAddr++;
		}

	if(! sTemp.IsEmpty())
		ip.m_bits.n1 = _ttoi(sTemp);

	nAddr = ip.m_nDWord;

	return true;
	}

//--------------------------------------------------------------------------------
CString CIPAddress::AddrToString(DWORD nDWord)
	{
	CString sTemp;
	IPADDRESS ip;
	ip.m_nDWord = nDWord;
	sTemp.Format(_T("%d.%d.%d.%d"), ip.m_bits.n4, ip.m_bits.n3, ip.m_bits.n2, ip.m_bits.n1);
	return sTemp;
	}

//--------------------------------------------------------------------------------
CIPAddress::operator DWORD() const
	{
	return m_nDWord;
	}

//--------------------------------------------------------------------------------
CIPAddress::operator DWORD&()
	{
	return m_nDWord;
	}

//--------------------------------------------------------------------------------
CIPAddress::operator=(LPCTSTR pStr)
	{
	StringToAddr(pStr, m_nDWord);
	}

//--------------------------------------------------------------------------------
CIPAddress::operator=(DWORD nDWord)
	{
	m_nDWord = nDWord;
	}

//--------------------------------------------------------------------------------
CIPAddress::operator=(const CIPAddress& rVal)
	{
	m_nDWord = rVal.m_nDWord;
	}

//--------------------------------------------------------------------------------
DWORD CIPAddress::GetAddress() const
	{
	return m_nDWord;
	}

//--------------------------------------------------------------------------------
void CIPAddress::GetAddress(CString& sStr) const
	{
	sStr = AddrToString(m_nDWord);
	}

//--------------------------------------------------------------------------------
void CIPAddress::SetAddress(DWORD nDWord)
	{
	m_nDWord = nDWord;
	}

//--------------------------------------------------------------------------------
void CIPAddress::SetAddress(LPCTSTR pStr)
	{
	StringToAddr(pStr, m_nDWord);
	}

//--------------------------------------------------------------------------------
UINT AFXAPI HashKey<CIPAddress>(CIPAddress ip)
	{
	return (UINT) (DWORD) ip;
	}

//--------------------------------------------------------------------------------
UINT AFXAPI HashKey<CIPAddress*>(CIPAddress* pIP)
	{
	return (UINT) ((DWORD) *pIP);
	}
