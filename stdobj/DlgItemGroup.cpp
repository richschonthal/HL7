//--------------------------------------------------------------------------------
//
// Copyright (c) 2001 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#include "stdafx.h"
#include "DlgItemGroup.h"

//--------------------------------------------------------------------------------
class CControlInfo
	{
	public:
		CPoint m_ptRelative;
		int m_nControlId;
		UCHAR m_nFlags;
	public:
		CControlInfo(int nId, int x, int y, UCHAR nFlags) : m_nControlId(nId) , m_ptRelative(x,y), m_nFlags(nFlags) {}
	};

//--------------------------------------------------------------------------------
class CControlList : public CList<CControlInfo*, CControlInfo*>
	{
	public:
		~CControlList()
			{
			for(POSITION pos = GetTailPosition(); pos != NULL; GetPrev(pos))
				delete GetAt(pos);
			}
	};

//--------------------------------------------------------------------------------
CDlgItemGroup::CDlgItemGroup(CWnd* pDlg, int nAnchor)
		: m_pDlg(pDlg)
		, m_nAnchorCtrlId(nAnchor)
		, m_pControls(new CControlList)
	{
	}

//--------------------------------------------------------------------------------
CDlgItemGroup::~CDlgItemGroup()
	{
	delete m_pControls;
	}

//--------------------------------------------------------------------------------
CWnd* CDlgItemGroup::GetDlgItemRect(const CWnd* pDlg, int nId, CRect& rect)
	{
	CWnd* pWnd = pDlg->GetDlgItem(nId);
	if(pWnd == NULL)
		return NULL;
	pWnd->GetWindowRect(rect);	
	pDlg->ScreenToClient(rect);
	return pWnd;
	}

//--------------------------------------------------------------------------------
CWnd* CDlgItemGroup::GetDlgItemRect(int nId, CRect& rect) const
	{
	return GetDlgItemRect(m_pDlg, nId, rect);
	}

//--------------------------------------------------------------------------------
void CDlgItemGroup::UpdatePositions() const
	{
	if(m_pDlg == NULL)
		return;

	long nBase = ::GetDialogBaseUnits();
	int nBaseX = LOWORD(nBase);
	int nBaseY = HIWORD(nBase);
	CRect rect;
	CRect rctAnchor;

	if(m_nAnchorCtrlId != 0)
		GetDlgItemRect(m_nAnchorCtrlId, rctAnchor);
	else
		m_pDlg->GetClientRect(&rctAnchor);

	for(POSITION pos = m_pControls->GetHeadPosition(); pos != NULL; m_pControls->GetNext(pos))
		{
		CControlInfo* pInfo = m_pControls->GetAt(pos);
		CWnd* pWnd = GetDlgItemRect(pInfo->m_nControlId, rect);
		if(pWnd == NULL)
			continue;
		int nTop;
		
		if(pInfo->m_nFlags & CDlgItemGroup::RelYToBottom)
			nTop = rctAnchor.bottom + ::MulDiv(pInfo->m_ptRelative.y, nBaseY, 8);
		else
			if(pInfo->m_nFlags & CDlgItemGroup::RelYToTop)
				nTop = rctAnchor.top + ::MulDiv(pInfo->m_ptRelative.y, nBaseY, 8);
			else
				nTop = rect.top;

		int nLeft;
		
		if(pInfo->m_nFlags & CDlgItemGroup::RelXToRight)
			nLeft = rctAnchor.right + ::MulDiv(pInfo->m_ptRelative.x, nBaseX, 4);
		else
			if(pInfo->m_nFlags & CDlgItemGroup::RelXToLeft)
				nLeft = rctAnchor.left + ::MulDiv(pInfo->m_ptRelative.x, nBaseX, 4);
			else
				nLeft = rect.left;

		if(pInfo->m_nFlags & CDlgItemGroup::RelSameWidth)
			rect.right = rctAnchor.right;

		pWnd->MoveWindow(nLeft, nTop, rect.Width(), rect.Height(), FALSE);
		}
	}

//--------------------------------------------------------------------------------
void CDlgItemGroup::AddItem(int nId, int x, int y, int nFlags)
	{
	m_pControls->AddTail(new CControlInfo(nId, x, y, nFlags));
	}

//--------------------------------------------------------------------------------
void CDlgItemGroup::RemoveItem(int nId)
	{
	for(POSITION pos = m_pControls->GetHeadPosition(); pos != NULL; m_pControls->GetNext(pos))
		if(m_pControls->GetAt(pos)->m_nControlId == nId)
			{
			m_pControls->RemoveAt(pos);
			return;
			}
	}
