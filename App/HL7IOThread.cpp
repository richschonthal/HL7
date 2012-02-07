//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "HL7IOThread.h"
#include "HL7IOSubSystem.h"

//--------------------------------------------------------------------------------
CHL7IOThread::CHL7IOThread(CHL7IOSubSystem* pParent)
		: CIOThread(pParent)
	{
	}

//--------------------------------------------------------------------------------
void CHL7IOThread::OnMessage(UINT nMsg, WPARAM param, LPARAM)
	{
	LPTSTR pText = (LPTSTR) param;

	if(pText != NULL)
		{
		// make a copy and strip out non-esential characters
		LPTSTR pNewText = new char[strlen(pText) + 1];

		char c;
		int j = 0;

		for(int i = 0; c = pText[i]; i++)
			{
			if(c >= ' ' || c == '\t')
				{
				pNewText[j] = c;
				j++;
				}
			}

		pNewText[j] = 0;

		switch(nMsg)
			{
			case CIOSubSystem::MSG_OUTPUT:
				delete[] pText;
				Output(pNewText);
				break;

			case CIOSubSystem::MSG_OUTPUT_CONST:
				Output(pNewText);
				break;
			}

		delete pNewText;
		}
	}
