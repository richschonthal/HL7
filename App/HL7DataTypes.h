//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#if !defined(AFX_HL7DATATYPES_H__49F9FC29_FE3A_11D2_87A7_00104B9E6286__INCLUDED_)
#define AFX_HL7DATATYPES_H__49F9FC29_FE3A_11D2_87A7_00104B9E6286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <commondefines.h>

//--------------------------------------------------------------------------------
class CHL7DataTypes
	{
	public:
		enum eType
			{
			DT_UNKNOWN,
			DT_AD, DT_CD, DT_CE, DT_CF, DT_CK, DT_CM, DT_CN, DT_CP,
			DT_CQ, DT_CX, DT_DT, DT_ED, DT_EI, DT_FT, DT_HD, DT_ID, 
			DT_IS, DT_MA, DT_MO, DT_NA, DT_NM, DT_PL, DT_PN, DT_PT,
			DT_RP, DT_SI, DT_SN, DT_ST, DT_TM, DT_TN, DT_TQ, DT_TS,
			DT_TX, DT_XAD, DT_XCN, DT_XON, DT_XPN, DT_XTN,
			DT_NUM_TYPES = DT_XTN
			};

		static struct listTag
			{
			eType m_nType;
			LPCTSTR m_pName;
			} m_list[DT_NUM_TYPES];

	public:
		static CHL7DataTypes::eType Find(LPCTSTR);
		static LPCTSTR Find(eType);
	};

#endif // !defined(AFX_HL7DATATYPES_H__49F9FC29_FE3A_11D2_87A7_00104B9E6286__INCLUDED_)
