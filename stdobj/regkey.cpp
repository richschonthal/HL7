//-------------------------------------------------------------------------------
// Reg Lib
// Classes for reading and writing registry stuff
//
// designed and coded by Rich Schonthal
// Copyright (c) 1997 Rich Schonthal
//
// this code is made public domain by the author
// it may be used in products commercial or otherwise
// as long as all copyright notices remain in tact
//-------------------------------------------------------------------------------

// note: some of the function descriptions are (modified) from the Reg function group in the
// Win32 online documentation


// RegQueryMultipleValues and RegNotifyChangeKeyValue are not implemented
// you can use these function with a regkey though
// by using the GetKey() func

#include "stdafx.h"

//-------------------------------------------------------------------------------
// guess what this function does...
void CRegKey::SetDefaultMemberValues()
	{
	m_hParent = NULL;
	m_hKey = NULL;
	m_pKeyName = NULL;
	m_pRemoteMachineName = NULL;
	m_pSaveFileName = NULL;
	m_nEnum = 0;
	m_nEnumDirection = 0;
	m_bIsCopy = FALSE;
	m_bIsLoaded = FALSE;
	m_pSecurityDescriptor = NULL;
	m_bIsRemote = FALSE;

	OSVERSIONINFO osInfo;
	
	osInfo.dwOSVersionInfoSize = sizeof(osInfo);

	::GetVersionEx(&osInfo);

	m_nPlatform = osInfo.dwPlatformId;
	}

//-------------------------------------------------------------------------------
// constructors ***

#if 0
//-------------------------------------------------------------------------------
// create a cregkey from an open HKEY
CRegKey::CRegKey(const HKEY hParent, HKEY hKey)
	{
	SetDefaultMemberValues();
	m_bIsCopy = TRUE;
	m_hParent = hParent;
	m_hKey = hKey;

	}
#endif

//-------------------------------------------------------------------------------
// create a new opened regkey from hParent
// sets the parent and the key to hParent
// this is useful for creating regkeys for HKEY_xxx_xxx keys
CRegKey::CRegKey(const HKEY hParent)
	{
	SetDefaultMemberValues();
	m_hParent = hParent;
	m_hKey = hParent;
	}

//-------------------------------------------------------------------------------
// create a new, unopened regkey
// the savefile is optional
CRegKey::CRegKey(const HKEY hParent, LPCTSTR pKeyName, LPCTSTR pSaveFile)
	{
	SetDefaultMemberValues();
	m_hParent = hParent;
	SetKeyName(pKeyName);
	SetSaveFileName(pSaveFile);
	}

//-------------------------------------------------------------------------------
// create a new, unopened regkey with the name and savefilename from rk
CRegKey::CRegKey(const HKEY hParent, const CRegKey& rk)
	{
	SetDefaultMemberValues();
	m_hParent = hParent;
	SetKeyName(rk.GetKeyName());
	SetSaveFileName(rk.GetSaveFileName());
	}

//-------------------------------------------------------------------------------
// creates a new regkey that is a copy of rk
CRegKey::CRegKey(const CRegKey& rk)
	{
	SetDefaultMemberValues();
	m_hParent = rk.m_hParent;
	m_hKey = rk.m_hKey;
	SetKeyName(rk.m_pKeyName);
	SetSaveFileName(rk.GetSaveFileName());
	m_nEnum = rk.m_nEnum;
	m_nEnumDirection = rk.m_nEnumDirection;
	m_bIsCopy = TRUE;
	}

//-------------------------------------------------------------------------------
// creates a new, unopened regkey whose name is rk.GetKeyName() + pKeyName
CRegKey::CRegKey(const CRegKey& rk, LPCTSTR pKeyName)
	{
	SetDefaultMemberValues();
	m_hParent = rk.m_hParent;
	SetKeyName(rk.m_pKeyName, pKeyName);
	}

//-------------------------------------------------------------------------------
// creates a new, unopenable regkey whose name is pKeyName,
// machine name is pMachineName and save file name is pFileName
// this is useful for const regkeys used as name containers
CRegKey::CRegKey(LPCTSTR pKeyName, LPCTSTR pFileName)
	{
	SetDefaultMemberValues();
	SetKeyName(pKeyName);
	SetSaveFileName(pFileName);
	m_bIsCopy = TRUE;
	}

//-------------------------------------------------------------------------------
// connect to remote registry
// CRegKeyResult return values:
// for parent keys other than HKEY_LOCAL_MACHINE and HKEY_USERS - errUnsupportedConnection
// error codes from RegConnectRegistry
CRegKey::CRegKey(LPCTSTR pMachineName, const HKEY hkParent)
	{
	SetDefaultMemberValues();

	if(ConnectRemote(pMachineName, hkParent))
		m_bIsRemote = TRUE;
	}


//-------------------------------------------------------------------------------
// destructor ***


//-------------------------------------------------------------------------------
CRegKey::~CRegKey()
	{
	// only close keys that we opened directly
	if(IsOpen() && ! IsCopy())
		CloseKey();
	if(m_pKeyName != NULL)
		delete m_pKeyName;
	if(m_pRemoteMachineName != NULL)
		delete m_pRemoteMachineName;
	if(m_pSaveFileName != NULL)
		delete m_pSaveFileName;
	}


//-------------------------------------------------------------------------------
// member access ***

//-------------------------------------------------------------------------------
// deletes and NULLs *pMemberString if it's allocated
// if pNewString != NULL is copied into a newly allocated *pMemberString
void CRegKey::SetMemberString(LPTSTR& pMemberString, LPCTSTR pNewString)
	{
	if(pMemberString != NULL)
		{
		delete pMemberString;
		pMemberString = NULL;
		}
	if(pNewString == NULL)
		return;
	pMemberString = new TCHAR[_tcslen(pNewString) + 1];
	_tcscpy(pMemberString, pNewString);
	}

//-------------------------------------------------------------------------------
// deletes and NULLs m_pKeyName if it's allocated
// if pKeyRootName and pKeyName != NULL they're appended and
// copied into a newly allocated m_pKeyName
// it's safe to pass m_pKeyName as 1 of the parameters
void CRegKey::SetKeyName(LPCTSTR pKeyRootName, LPCTSTR pKeyName)
	{
	if(pKeyRootName == NULL || (pKeyRootName == NULL && pKeyName == NULL))
		return;

	CString sTemp(pKeyRootName);
	if(pKeyName != NULL)
		{
		sTemp += "\\";
		sTemp += pKeyName;
		}
	delete m_pKeyName;
	m_pKeyName = new TCHAR[sTemp.GetLength()+1];
	_tcscpy(m_pKeyName, sTemp);
	}

//-------------------------------------------------------------------------------
// deletes and NULLs m_pKeyName if it's allocated
// appeneds pKeyName to m_pKeyName
// does nothing if m_pKeyName is already null
void CRegKey::AppendToKeyName(LPCTSTR pKeyName)
	{
	if(m_pKeyName == NULL)
		return;
	SetKeyName(m_pKeyName, pKeyName);
	}


//-------------------------------------------------------------------------------
// operators


//-------------------------------------------------------------------------------
const CRegKey& CRegKey::operator=(const CRegKey& rk)
	{
	SetDefaultMemberValues();
	m_hParent = rk.m_hParent;
	m_hKey = rk.m_hKey;
	SetKeyName(rk.m_pKeyName);
	SetSaveFileName(rk.GetSaveFileName());
	SetRemoteMachineName(rk.GetRemoteMachineName());
	m_nEnum = rk.m_nEnum;
	m_nEnumDirection = rk.m_nEnumDirection;
	m_bIsCopy = TRUE;
	return *this;
	}

//-------------------------------------------------------------------------------
// returns a regkey that is a copy of this with pName appended to the keyname
CRegKey CRegKey::operator+(LPCTSTR pName)
	{
	CRegKey rkTemp(*this);
	rkTemp.AppendToKeyName(pName);
	rkTemp.SetSaveFileName(NULL);

	return rkTemp;
	}

//-------------------------------------------------------------------------------
// returns a regkey that is a copy of this with rk.GetKeyName() appended to the keyname
CRegKey CRegKey::operator+(const CRegKey& rk)
	{
	CRegKey rkTemp(*this);
	rkTemp.AppendToKeyName(rk.m_pKeyName);
	rkTemp.SetSaveFileName(NULL);
	return rkTemp;
	}

//-------------------------------------------------------------------------------
CRegKey CRegKey::operator+=(const CRegKey& rk)
	{
	m_hParent = rk.m_hParent;
	m_hKey = rk.m_hKey;
	AppendToKeyName(rk.m_pKeyName);
	SetSaveFileName(NULL);
	m_nEnum = 0;
	m_nEnumDirection = 0;
	m_bIsCopy = TRUE;
	return *this;
	}

//-------------------------------------------------------------------------------
// Win32 reg wrappers ***

//-------------------------------------------------------------------------------
BOOL CRegKey::ConnectRemote(LPCTSTR pMachineName, const HKEY hkParent)
	{
	if(hkParent != HKEY_LOCAL_MACHINE && hkParent != HKEY_USERS)
		{
		SetResultCode(errUnsupportedConnection);
		return FALSE;
		}

	if(pMachineName == NULL)
		SetOSResult(::RegConnectRegistry(NULL, hkParent, &m_hParent));
	else
		{
		CString sName(pMachineName);
		if(sName.GetLength() < 3)
			{
			SetResultCode(errRegistry);
			return FALSE;
			}
		if(sName.Left(2) != "\\\\")
			sName.Insert(0, "\\\\");

		SetOSResult(::RegConnectRegistry(sName, hkParent, &m_hParent));
		if(! HasErrors())
			SetRemoteMachineName(sName);
		}

	return HasErrors();
	}

//-------------------------------------------------------------------------------
// creates and opens a registry key
// CRegKeyResult return values:
// for an already open regkey - errKeyAlreadyOpen
// on Win32s - errRegistryNotSupported
// error codes from GetVersionEx
// error codes from RegCreateKeyEx
BOOL CRegKey::CreateKey(DWORD* pDisposition, DWORD dwOptions, REGSAM samDesired)
	{
	if(IsOpen())
		{
		SetResultCode(errKeyAlreadyOpen);
		return FALSE;
		}

	DWORD nDisposition;
	if(pDisposition == NULL)
		pDisposition = &nDisposition;

	m_bIsCopy = FALSE;

	*pDisposition = 0;

	SetOSResult(::RegCreateKeyEx(GetParentKey(), GetKeyName(), 0, GetValueClass(), 
		dwOptions, samDesired, NULL, GetKeyPtr(), pDisposition));

	return GetOSResult() == ERROR_SUCCESS;
	}

//-------------------------------------------------------------------------------
// opens a registry key
// CRegKeyResult return values:
// for an already open regkey - errKeyAlreadyOpen
// on Win32s - errRegistryNotSupported
// error codes from GetVersionEx
// error codes from RegCreateKeyEx
BOOL CRegKey::OpenKey(REGSAM samDesired)
	{
	// if it's open and it IS a copy then
	// we'll open our own
	// otherwise we've already opened our own
	// so let's not open it again
	if(IsOpen() && ! IsCopy())
		{
		SetResultCode(errKeyAlreadyOpen);
		return FALSE;
		}

	m_bIsCopy = FALSE;

	SetOSResult(::RegOpenKeyEx(GetParentKey(), GetKeyName(), 0, samDesired, GetKeyPtr()));

	return GetOSResult() == ERROR_SUCCESS;
	}

//-------------------------------------------------------------------------------
// closes a regkey
// CRegKeyResult return values:
// a reg key copy - errNotKeyOwner
// unopen regkey - errKeyNotOpen
// error codes from RegCloseKey
BOOL CRegKey::CloseKey()
	{
	if(IsCopy())
		{
		SetResultCode(errNotKeyOwner);
		return FALSE;
		}

	if(! IsOpen() && ! IsRemote())
		{
		SetResultCode(errKeyNotOpen);
		return FALSE;
		}

	CRegKeyResult err;

	if(IsOpen())
		{
#ifdef _DEBUG
		HKEY hkey = GetKey();
#endif
		err.SetOSResult(::RegCloseKey(GetKey()));
		if(! err.HasErrors())
			m_hKey = NULL;
		}

	if(IsRemote())
		SetOSResult(::RegCloseKey(GetParentKey()));

	if(! HasErrors() && err.HasErrors())
		(CRegKeyResult) *this = err;

	return ! HasErrors();
	}

//-------------------------------------------------------------------------------
// deletes a regkey (the registry key but not the class instance)
// CRegKeyResult return values:
// an open copy of a regkey - errNotKeyOwner
// error codes from RegDeleteKey
BOOL CRegKey::DeleteKey()
	{
	if(IsOpen() && IsCopy())
		{
		SetResultCode(errNotKeyOwner);
		return FALSE;
		}

	// in win95 RegDeleteKey will kill a sub-key and all its sub-keys
	// in NT RegDeleteKey won't delete a sub-key if it has sub-keys
	// maybe some code to account for this difference would be nice
	SetOSResult(::RegDeleteKey(GetParentKey(), GetKeyName()));

	return GetOSResult() == ERROR_SUCCESS;
	}

//-------------------------------------------------------------------------------
// flushes a regkey
// CRegKeyResult return values:
// an unopen regkey - errKeyNotOpen
// error codes from RegFlushKey
BOOL CRegKey::FlushKey()
	{
	if(! IsOpen())
		{
		SetResultCode(errKeyNotOpen);
		return FALSE;
		}
	
	SetOSResult(::RegFlushKey(GetKey()));

	return GetOSResult() == ERROR_SUCCESS;
	}

//-------------------------------------------------------------------------------
// saves the regkey and all of its subkeys and values to a new file
// CRegKeyResult return values:
// an unopen key - errKeyNotOpen
// a NULL pFileName and a NULL m_pFileName - errInvalidSaveFileName
// error codes from RegSaveKey
BOOL CRegKey::SaveKey(LPSECURITY_ATTRIBUTES pSa, LPCTSTR pFileName)
	{
	if(! IsOpen())
		{
		SetResultCode(errKeyNotOpen);
		return FALSE;
		}

	if(pSa != NULL)
		pSa->nLength = sizeof(SECURITY_ATTRIBUTES);
	if(pFileName == NULL)
		pFileName = GetSaveFileName();
	if(pFileName == NULL)
		{
		SetResultCode(errInvalidSaveFileName);
		return FALSE;
		}
	SetOSResult(::RegSaveKey(GetKey(), pFileName, pSa));

	return GetOSResult() == ERROR_SUCCESS;
	}

//-------------------------------------------------------------------------------
// creates a subkey under HKEY_USER or HKEY_LOCAL_MACHINE and stores registration
// information from a specified file into that subkey. This registration information
// is in the form of a hive. A hive is a discrete body of keys, subkeys, and values 
// that is rooted at the top of the registry hierarchy. A hive is backed by a single 
// file and .LOG file.
// CRegKeyResult return values:
// a NULL pFileName and a NULL m_pSaveFileName - errInvalidSaveFileName
// error codes from RegLoadKey
BOOL CRegKey::LoadKey(LPCTSTR pFileName)
	{
	if(! IsRemote() && (GetParentKey() != HKEY_LOCAL_MACHINE && GetParentKey() != HKEY_USERS))
		{
		SetResultCode(errUnsupportedConnection);
		return FALSE;
		}

	if(pFileName == NULL)
		{
		if(GetSaveFileName() == NULL)
			{
			SetResultCode(errInvalidSaveFileName);
			return FALSE;
			}

		pFileName = GetSaveFileName();
		}

	SetOSResult(::RegLoadKey(GetParentKey(), GetKeyName(), pFileName));
	if(! HasErrors())
		m_bIsLoaded = TRUE;
	return m_bIsLoaded;
	}

//-------------------------------------------------------------------------------
BOOL CRegKey::UnLoadKey()
	{
	if(! IsLoaded())
		{
		SetResultCode(errKeyNotLoaded);
		return FALSE;
		}

	SetOSResult(::RegUnLoadKey(GetParentKey(), GetKeyName()));
	return GetOSResult() == ERROR_SUCCESS;
	}

//-------------------------------------------------------------------------------
BOOL CRegKey::ReplaceKey(LPCTSTR pNewFile, LPCTSTR pOldFile)
	{
	if(pOldFile == NULL)
		pOldFile = GetSaveFileName();
	SetOSResult(::RegReplaceKey(GetKey(), GetKeyName(), pNewFile, pOldFile));
	return GetOSResult() == ERROR_SUCCESS;
	}

//-------------------------------------------------------------------------------
BOOL CRegKey::RestoreKey(DWORD dwFlags, LPCTSTR pFileName)
	{
	if(pFileName == NULL)
		pFileName = GetSaveFileName();
	SetOSResult(::RegRestoreKey(GetKey(), pFileName, dwFlags));
	return GetOSResult() == ERROR_SUCCESS;
	}

//--------------------------------------------------------------------------------
BOOL CRegKey::NotifyChangeKeyValue(BOOL bWatchSubtree, DWORD dwNotifyFilter, HANDLE hEvent, BOOL fAsynchronous)
	{
	if(! IsOpen())
		{
		SetResultCode(errKeyNotOpen);
		return FALSE;
		}

	SetOSResult(::RegNotifyChangeKeyValue(GetKey(), bWatchSubtree, dwNotifyFilter, hEvent, fAsynchronous));

	return GetOSResult() == ERROR_SUCCESS;
	}

//-------------------------------------------------------------------------------
BOOL CRegKey::GetKeySecurity(SECURITY_INFORMATION si)
	{
	DWORD nSize = 0;

	SetOSResult(::RegGetKeySecurity(GetKey(), si, NULL, &nSize));
	if(GetOSResult() != ERROR_INSUFFICIENT_BUFFER)
		return FALSE;
	if(m_pSecurityDescriptor != NULL)
		{
		delete m_pSecurityDescriptor;
		m_pSecurityDescriptor = NULL;
		}

	AllocateSecurityDescriptor(nSize);
	SetOSResult(::RegGetKeySecurity(GetKey(), si, m_pSecurityDescriptor, &nSize));
	return GetOSResult() == ERROR_SUCCESS;
	}

//-------------------------------------------------------------------------------
BOOL CRegKey::SetKeySecurity(SECURITY_INFORMATION si)
	{
	if(m_pSecurityDescriptor == NULL)
		{
		SetResultCode(errSecurityDescNotSet);
		return FALSE;
		}

	SetOSResult(::RegSetKeySecurity(GetKey(), si, m_pSecurityDescriptor));
	return GetOSResult() == ERROR_SUCCESS;
	}

//-------------------------------------------------------------------------------
// inits m_nEnum to the last key or value entry index
// sets m_nEnumDirection to -1 for moving backwards when iterating enums
// CRegKeyResult return values:
// error codes for RegQueryInfoKey (via CRegKeyInfo)
BOOL CRegKey::StartEnumLast(BOOL bIsKey)
	{
	CRegKeyInfo rkinfo(*this);

	if(rkinfo.HasErrors())
		return FALSE;

	if(bIsKey)
		m_nEnum = rkinfo.GetSubKeyCount() - 1;
	else
		m_nEnum = rkinfo.GetValueCount() - 1;
	m_nEnumDirection = -1;

	return TRUE;
	}

//-------------------------------------------------------------------------------
// iterates all sub-keys for this regkey
// places a new, unopened regkey into regKey
// advances this->m_nEnum
// returns TRUE if successful
BOOL CRegKey::IterateEnumKey(CRegKey& regKey)
	{
	if(m_nEnum < 0)
		return FALSE;

	FILETIME fTime;

	DWORD nSize = 512;
	CString sTemp;

	SetOSResult(::RegEnumKeyEx(GetKey(), m_nEnum, sTemp.GetBuffer(nSize-1), &nSize,
		NULL, NULL, NULL, &fTime));

	sTemp.ReleaseBuffer();

	if(HasErrors())
		{
		if(GetOSResult() == ERROR_NO_MORE_ITEMS)
			SetOSResult(0);
		return FALSE;
		}

	if(regKey.m_pKeyName != NULL)
		{
		delete regKey.m_pKeyName;
		regKey.m_pKeyName = NULL;
		}

	if(GetKeyName() && *GetKeyName())
		{
		CString sRoot(GetKeyName());

		sTemp = sRoot + "\\" + sTemp;
		}

	regKey.m_pKeyName = new TCHAR[sTemp.GetLength()+1];

	_tcscpy(regKey.m_pKeyName, sTemp);

	m_nEnum += m_nEnumDirection;

	regKey.SetParentKey(GetParentKey());

	regKey.m_hKey = NULL;
	regKey.m_nEnum = 0;
	regKey.m_nEnumDirection = 0;
	regKey.m_bIsCopy = FALSE;

	return TRUE;
	}

//-------------------------------------------------------------------------------
CRegKeyInfo CRegKey::ReadKeyInfo()
	{
	CRegKeyInfo info(*this);
	return info;
	}