#if !defined(AFX_X_H__F1BF7B4F_F926_11D3_AF1A_005004A1C5F3__INCLUDED_)
#define AFX_X_H__F1BF7B4F_F926_11D3_AF1A_005004A1C5F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// x.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Cx window

class Cx : public CAnimateCtrl
{
// Construction
public:
	Cx();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Cx)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~Cx();

	// Generated message map functions
protected:
	//{{AFX_MSG(Cx)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_X_H__F1BF7B4F_F926_11D3_AF1A_005004A1C5F3__INCLUDED_)
