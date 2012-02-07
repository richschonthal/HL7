//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#if !defined(AFX_SYSTEMCONFIG_H__42D4F266_830D_11D3_AEFF_005004A1C5F3__INCLUDED_)
#define AFX_SYSTEMCONFIG_H__42D4F266_830D_11D3_AEFF_005004A1C5F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
class CSystem;

//--------------------------------------------------------------------------------
#include "SystemObject.h"

//--------------------------------------------------------------------------------
class CSystemConfig : public CSystemObject
	{
	public:
		CSystemConfig(CSystemControlObject*);
		virtual ~CSystemConfig();

	public:
		virtual bool Read();
		virtual bool Reload();
		virtual bool Write();
		virtual bool IsDirty();
	};

#endif // !defined(AFX_SYSTEMCONFIG_H__42D4F266_830D_11D3_AEFF_005004A1C5F3__INCLUDED_)
