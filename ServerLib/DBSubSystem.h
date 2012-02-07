//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#if !defined(AFX_DBSUBSYSTEM_H__B66F74A9_E5CF_11D3_B10F_00A0CC271D0D__INCLUDED_)
#define AFX_DBSUBSYSTEM_H__B66F74A9_E5CF_11D3_B10F_00A0CC271D0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
#include <SubSystem.h>

//--------------------------------------------------------------------------------
class CDBSubSystem : public CSubSystem  
	{
	public:
		CDBSubSystem(CSystem*);
		virtual ~CDBSubSystem();

		virtual int GetObjectId();
	};

#endif // !defined(AFX_DBSUBSYSTEM_H__B66F74A9_E5CF_11D3_B10F_00A0CC271D0D__INCLUDED_)
