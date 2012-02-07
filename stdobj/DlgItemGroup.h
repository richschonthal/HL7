//--------------------------------------------------------------------------------
//
// Copyright (c) 2001 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _DLGITEMGROUP_H_
#define _DLGITEMGROUP_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
class CControlList;

//--------------------------------------------------------------------------------
class CDlgItemGroup
	{
	private:
		CControlList* m_pControls;
		CWnd* m_pDlg;
		int m_nAnchorCtrlId;

	public:
		enum
			{
			RelXToRight = 1, // set if X param is relative to the right of the list
			RelYToBottom = 2,  // set if y param is relative to the bottom of the list
			RelXToLeft = 4,
			RelYToTop = 8,
			RelSameWidth = 16
			};

	public:
		CDlgItemGroup(CWnd* pDlg, int = 0);
		virtual ~CDlgItemGroup();

		static CWnd* GetDlgItemRect(const CWnd* pDlg, int nId, CRect&);
		CWnd* GetDlgItemRect(int nId, CRect&) const;
		void UpdatePositions() const;

		void AddItem(int nId, int x, int y, int nFlags);
		void RemoveItem(int nId);
	};

#endif // _DLGITEMGROUP_H_
