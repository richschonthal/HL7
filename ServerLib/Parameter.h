//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#if !defined(AFX_PARAMETER_H__5019D40A_84B9_11D3_B0D4_00A0CC271D0D__INCLUDED_)
#define AFX_PARAMETER_H__5019D40A_84B9_11D3_B0D4_00A0CC271D0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
#include "result.h"

//--------------------------------------------------------------------------------
class CParameter : public CResult  
	{
	public:
		CParameter(DWORD = 0);
		virtual ~CParameter();

	protected:
		DWORD m_nId;

	public:
		DWORD GetId() const;
		void SetId(DWORD);

	virtual void* GetValue() = 0;
	virtual bool SetValue(void*) = 0;

	};

#endif // !defined(AFX_PARAMETER_H__5019D40A_84B9_11D3_B0D4_00A0CC271D0D__INCLUDED_)
