//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// HL7DataTypes.cpp: implementation of the CHL7DataTypes class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HL7DataTypes.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//--------------------------------------------------------------------------------
CHL7DataTypes::listTag CHL7DataTypes::m_list[DT_NUM_TYPES] =
	{
	DT_AD,"AD",
	DT_CD,"CD",
	DT_CE,"CE",
	DT_CF,"CF",
	DT_CK,"CK",
	DT_CM,"CM",
	DT_CN,"CN",
	DT_CP,"CP",
	DT_CQ,"CQ",
	DT_CX,"CX",
	DT_DT,"DT",
	DT_ED,"ED",
	DT_EI,"EI",
	DT_FT,"FT",
	DT_HD,"HD",
	DT_ID,"ID",
	DT_IS,"IS",
	DT_MA,"MA",
	DT_MO,"MO",
	DT_NA,"NA",
	DT_NM,"NM",
	DT_PL,"PL",
	DT_PN,"PN",
	DT_PT,"PT",
	DT_RP,"RP",
	DT_SI,"SI",
	DT_SN,"SN",
	DT_ST,"ST",
	DT_TM,"TM",
	DT_TN,"TN",
	DT_TQ,"TQ",
	DT_TS,"TS",
	DT_TX,"TX",
	DT_XAD,"XAD",
	DT_XCN,"XCN",
	DT_XON,"XON",
	DT_XPN,"XPN",
	DT_XTN,"XTN"
	};

//--------------------------------------------------------------------------------
CHL7DataTypes::eType CHL7DataTypes::Find(LPCTSTR pType)
	{
	CString sTemp(pType);

	for(int i = 0; i < DT_NUM_TYPES; i++)
		{
		if(sTemp == m_list[i].m_pName)
			return m_list[i].m_nType;
		}

	return DT_UNKNOWN;
	}

//--------------------------------------------------------------------------------
LPCTSTR CHL7DataTypes::Find(CHL7DataTypes::eType nType)
	{
	for(int i = 0; i < DT_NUM_TYPES; i++)
		{
		if(nType == m_list[i].m_nType)
			return m_list[i].m_pName;
		}

	return "";
	}

