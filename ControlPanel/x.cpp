// x.cpp : implementation file
//

#include "stdafx.h"
#include "ControlPanel.h"
#include "x.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Cx

Cx::Cx()
{
	x = 0;

}

Cx::~Cx()
{
}


BEGIN_MESSAGE_MAP(Cx, CAnimateCtrl)
	//{{AFX_MSG_MAP(Cx)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Cx message handlers

BOOL Cx::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CAnimateCtrl::OnCommand(wParam, lParam);
}
