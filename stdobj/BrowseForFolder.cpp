//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////
//
// ShellBrowser.cpp: implementation of the CShellBrowser class.
//

#include "stdafx.h"
#include <memory>

typedef std::auto_ptr<TCHAR> AUTO_STR;
AUTO_STR g_pchTitle;

#include "BrowseForFolder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
//
// Construction/Destruction
//

//--------------------------------------------------------------------------------
CBrowseForFolder::CBrowseForFolder(const HWND hParent /*= NULL*/, const LPITEMIDLIST pidl /*= NULL*/, const int nTitleID /*= 0*/)
	{
	m_hwnd = NULL;
	SetOwner(hParent);
	SetRoot(pidl);
	SetTitle(nTitleID);
	m_bi.lpfn = BrowseCallbackProc;
	m_bi.lParam = reinterpret_cast<long>(this);
	m_bi.pszDisplayName = m_szSelected;
	}

//--------------------------------------------------------------------------------
CBrowseForFolder::CBrowseForFolder(const HWND hParent, const LPITEMIDLIST pidl, const CString& strTitle)
	{
	m_hwnd = NULL;
	SetOwner(hParent);
	SetRoot(pidl);
	SetTitle(strTitle);
	m_bi.lpfn = BrowseCallbackProc;
	m_bi.lParam = reinterpret_cast<long>(this);
	m_bi.pszDisplayName = m_szSelected;
	}

//--------------------------------------------------------------------------------
CBrowseForFolder::~CBrowseForFolder()
	{

	}

//////////////////////////////////////////////////////////////////////
//
// Implementation
//

//--------------------------------------------------------------------------------
void CBrowseForFolder::SetOwner(const HWND hwndOwner)
	{
	if (m_hwnd != NULL)
		return;

	m_bi.hwndOwner = hwndOwner;
	}

//--------------------------------------------------------------------------------
void CBrowseForFolder::SetRoot(const LPITEMIDLIST pidl)
	{
	if (m_hwnd != NULL)
		return;

	m_bi.pidlRoot = pidl;
	}

//--------------------------------------------------------------------------------
CString CBrowseForFolder::GetTitle() const
	{
	return m_bi.lpszTitle;
	}

//--------------------------------------------------------------------------------
void CBrowseForFolder::SetTitle(const CString& strTitle)
	{
	if(m_hwnd != NULL)
		return;

	g_pchTitle = std::auto_ptr<TCHAR>(new TCHAR[static_cast<size_t>(strTitle.GetLength()) + 1]);
	_tcscpy(g_pchTitle.get(), strTitle);
	m_bi.lpszTitle = g_pchTitle.get();
	}

//--------------------------------------------------------------------------------
bool CBrowseForFolder::SetTitle(const int nTitle)
	{
	if (nTitle <= 0)
		return false;

	CString strTitle;
	if(!strTitle.LoadString(static_cast<size_t>(nTitle)))
		{
		return false;
		}
	SetTitle(strTitle);
	return true;
	}

//--------------------------------------------------------------------------------
void CBrowseForFolder::SetFlags(const UINT ulFlags)
	{
	if (m_hwnd != NULL)
		return;

	m_bi.ulFlags = ulFlags;
	}

//--------------------------------------------------------------------------------
CString CBrowseForFolder::GetSelectedFolder() const
	{
	return m_strPath;
	}

//--------------------------------------------------------------------------------
bool CBrowseForFolder::SelectFolder()
	{
	m_hwnd = NULL;

	LPITEMIDLIST pidl;
	if ((pidl = ::SHBrowseForFolder(&m_bi)) != NULL)
		{
		m_strPath = m_bi.pszDisplayName;

		LPMALLOC pMalloc;
		//Retrieve a pointer to the shell's IMalloc interface
		if (::SHGetMalloc(&pMalloc))
			{
			// free the PIDL that SHBrowseForFolder returned to us.
			pMalloc->Free(pidl);
			// release the shell's IMalloc interface
			(void)pMalloc->Release();
			}

		return true;
		}

	return false;
	}

//--------------------------------------------------------------------------------
void CBrowseForFolder::OnInit() const
	{

	}

//--------------------------------------------------------------------------------
void CBrowseForFolder::OnSelChanged(const LPITEMIDLIST) const
	{
	}

//--------------------------------------------------------------------------------
void CBrowseForFolder::EnableOK(const bool bEnable) const
	{
	if (m_hwnd == NULL)
		return;

	(void)SendMessage(m_hwnd, BFFM_ENABLEOK, static_cast<WPARAM>(bEnable), NULL);
	}

//--------------------------------------------------------------------------------
void CBrowseForFolder::SetSelection(const LPITEMIDLIST pidl) const
	{
	if (m_hwnd == NULL)
		return;

	(void)SendMessage(m_hwnd, BFFM_SETSELECTION, FALSE, reinterpret_cast<long>(pidl));
	}

//--------------------------------------------------------------------------------
void CBrowseForFolder::SetSelection(const CString& strPath) const
	{
	if (m_hwnd == NULL)
		return;

	(void)SendMessage(m_hwnd, BFFM_SETSELECTION, TRUE, reinterpret_cast<long>(LPCTSTR(strPath)));
	}

//--------------------------------------------------------------------------------
void CBrowseForFolder::SetStatusText(const CString& strText) const
	{
	if (m_hwnd == NULL)
		return;

	(void)SendMessage(m_hwnd, BFFM_SETSTATUSTEXT, NULL, reinterpret_cast<long>(LPCTSTR(strText)));
	}

//--------------------------------------------------------------------------------
int __stdcall CBrowseForFolder::BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
	{
	CBrowseForFolder* pbff = reinterpret_cast<CBrowseForFolder*>(lpData);
	pbff->m_hwnd = hwnd;
	if (uMsg == BFFM_INITIALIZED)
		pbff->OnInit();
	else if (uMsg == BFFM_SELCHANGED)
		pbff->OnSelChanged(reinterpret_cast<LPITEMIDLIST>(lParam));
	
	return 0;
	}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
CBrowseForComputer::CBrowseForComputer()
		: CBrowseForFolder(NULL, NULL, CString(""))
		, m_pItems(NULL)
	{
	SetFlags(BIF_BROWSEFORCOMPUTER);

	::SHGetSpecialFolderLocation(NULL, CSIDL_NETWORK, &m_pItems);

	SetRoot(m_pItems);
	}

//--------------------------------------------------------------------------------
CBrowseForComputer::~CBrowseForComputer()
	{
	if(m_pItems)
		{
		LPMALLOC pMalloc;
		//Retrieve a pointer to the shell's IMalloc interface
		if (::SHGetMalloc(&pMalloc))
			{
			// free the PIDL that SHBrowseForFolder returned to us.
			pMalloc->Free(m_pItems);
			// release the shell's IMalloc interface
			(void)pMalloc->Release();
			}
		}		
	}
