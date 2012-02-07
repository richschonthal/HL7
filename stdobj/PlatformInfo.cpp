//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "PlatformInfo.h"
#include "Handle.h"

//--------------------------------------------------------------------------------
CDWordArray CPlatformInfo::m_nP3SerialNumbers;

//--------------------------------------------------------------------------------
CPlatformInfo::CPlatformInfo()
	{
	dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	if (! GetVersionEx((OSVERSIONINFO*) this))
		SetResultCode(errOS);
	}

//--------------------------------------------------------------------------------
CPlatformInfo::~CPlatformInfo()
	{

	}

//--------------------------------------------------------------------------------
void CPlatformInfo::GetVersion(CString& sVersion)
	{
	if(HasErrors())
		return;

	CString sServPackRel(szCSDVersion);

	switch(dwPlatformId)
		{
		case VER_PLATFORM_WIN32s: //Win32s on Windows 3.1.
			sVersion = "Microsoft® Windows 3.1(TM)";
			break;

		case VER_PLATFORM_WIN32_WINDOWS: //WIN32 on 95 or 98
			//determine if Win95 or Win98
			if (dwMinorVersion == 0)
				sVersion = "Microsoft® Windows 95(TM) " + sServPackRel;
			else
				sVersion = "Microsoft® Windows 98(TM) " + sServPackRel;
			break;

		case VER_PLATFORM_WIN32_NT: //Win32 on Windows NT.
			sVersion = "Microsoft® Windows NT(TM) "  + sServPackRel;
			break;

		default:
			break;
		} //end switch
	}

//--------------------------------------------------------------------------------
int CPlatformInfo::GetVersion()
	{
	if(HasErrors())
		return -1;

	switch(dwPlatformId)
		{
		case VER_PLATFORM_WIN32s: //Win32s on Windows 3.1.
			return VER_WIN31;

		case VER_PLATFORM_WIN32_WINDOWS: //WIN32 on 95 or 98
			//determine if Win95 or Win98
			if (dwMinorVersion == 0)
				return VER_WIN95;
			else
				VER_WIN98;

		case VER_PLATFORM_WIN32_NT: //Win32 on Windows NT.
			return VER_WINNT4;

		default:
			return -1;
		} //end switch
	}

//--------------------------------------------------------------------------------
bool CPlatformInfo::IsWin31()
	{
	return dwPlatformId == VER_PLATFORM_WIN32s;
	}

//--------------------------------------------------------------------------------
bool CPlatformInfo::IsWin95()
	{
	return dwPlatformId == VER_PLATFORM_WIN32_WINDOWS && dwMinorVersion == 0;
	}

//--------------------------------------------------------------------------------
bool CPlatformInfo::IsWin95OS2R()
	{
	return dwPlatformId == VER_PLATFORM_WIN32_WINDOWS && dwMinorVersion == 0 && LOWORD(dwBuildNumber) > 1080;
	}

//--------------------------------------------------------------------------------
bool CPlatformInfo::IsWin98()
	{
	return dwPlatformId == VER_PLATFORM_WIN32_WINDOWS && dwMinorVersion != 0;
	}

//--------------------------------------------------------------------------------
bool CPlatformInfo::IsWinNT()
	{
	return dwPlatformId == VER_PLATFORM_WIN32_NT;
	}

//--------------------------------------------------------------------------------
bool CPlatformInfo::IsWin2000()
	{
	return dwPlatformId == VER_PLATFORM_WIN32_NT && dwMajorVersion == 5;
	}

//--------------------------------------------------------------------------------
void CPlatformInfo::GetComputerName(CString& sName)
	{
	DWORD nSize = MAX_COMPUTERNAME_LENGTH + 1;

	if(! ::GetComputerName(sName.GetBuffer(nSize), &nSize))
		{
		sName.ReleaseBuffer();
		if(GetLastError() == ERROR_BUFFER_OVERFLOW)
			{
			::GetComputerName(sName.GetBuffer(nSize), &nSize);
			sName.ReleaseBuffer();
			}
		}
	else
		sName.ReleaseBuffer();
	}

//--------------------------------------------------------------------------------
void CPlatformInfo::GetUserName(CString& sName)
	{
	DWORD nSize = 32;

	if(! ::GetUserName(sName.GetBuffer(nSize), &nSize))
		{
		sName.ReleaseBuffer();
		if(GetLastError() == ERROR_BUFFER_OVERFLOW)
			{
			::GetUserName(sName.GetBuffer(nSize), &nSize);
			sName.ReleaseBuffer();
			}
		}
	else
		sName.ReleaseBuffer();
	}

//--------------------------------------------------------------------------------
void CPlatformInfo::GetSystemDirectory(CString& sName)
	{
	UINT nSize = MAX_PATH + 1;

	nSize = ::GetSystemDirectory(sName.GetBuffer(nSize), nSize);
	if(nSize > MAX_PATH + 1)
		{
		sName.ReleaseBuffer();
		::GetSystemDirectory(sName.GetBuffer(nSize), nSize);
		}
	sName.ReleaseBuffer();
	}

//--------------------------------------------------------------------------------
void CPlatformInfo::GetWindowsDirectory(CString& sName)
	{
	UINT nSize = MAX_PATH + 1;

	nSize = ::GetWindowsDirectory(sName.GetBuffer(nSize), nSize);
	if(nSize > MAX_PATH + 1)
		{
		sName.ReleaseBuffer();
		::GetWindowsDirectory(sName.GetBuffer(nSize), nSize);
		}
	sName.ReleaseBuffer();
	}

//--------------------------------------------------------------------------------
void CPlatformInfo::GetCurrentDirectory(CString& sDir)
	{
	sDir.Empty();
	
	DWORD nSize = ::GetCurrentDirectory(MAX_PATH, sDir.GetBuffer(MAX_PATH + 1));
	sDir.ReleaseBuffer();
	if(nSize > MAX_PATH)
		{
		nSize = ::GetCurrentDirectory(MAX_PATH, sDir.GetBuffer(nSize));
		sDir.ReleaseBuffer();
		}
	}

//--------------------------------------------------------------------------------
bool CPlatformInfo::SetCurrentDirectory(const CString& sDir)
	{
	return ::SetCurrentDirectory(sDir) != 0;
	}

//--------------------------------------------------------------------------------
void CPlatformInfo::ExpandEnvironmentStrings(LPCTSTR pVar, CString& sOut)
	{
	DWORD nSize = 512;

	nSize = ::ExpandEnvironmentStrings(pVar, sOut.GetBuffer(nSize), nSize);
	if(nSize > 512)
		{
		sOut.ReleaseBuffer();
		::ExpandEnvironmentStrings(pVar, sOut.GetBuffer(nSize), nSize);
		}

	sOut.ReleaseBuffer();
	}

//--------------------------------------------------------------------------------
void CPlatformInfo::ParseEnvironmentStrings(LPCTSTR pVar, CStringArray& sOut)
	{
	CString sTemp(pVar);

	ParseEnvironmentStrings(sTemp, sOut);
	}

//--------------------------------------------------------------------------------
void CPlatformInfo::ParseEnvironmentStrings(const CString& sVar, CStringArray& sOut)
	{
	int nIndex = sVar.Find(';');
	if(nIndex == -1)
		{
		sOut.Add(sVar);
		return;
		}

	int nPrev = 0;
	for(bool bDone = false;; )
		{
		sOut.Add(sVar.Mid(nPrev, nIndex - nPrev));
		if(bDone)
			break;
		nPrev = nIndex + 1;
		nIndex = sVar.Find(';', nIndex + 1);
		if(nIndex == -1)
			{
			nIndex = sVar.GetLength();
			bDone = true;
			}
		}
	}

//--------------------------------------------------------------------------------
bool CPlatformInfo::FileExists(LPCTSTR pPath, bool bExcludeDir)
	{
	DWORD nAttr = ::GetFileAttributes(pPath);
	if(nAttr == -1)
		return false;
	if(bExcludeDir)
		return (nAttr & FILE_ATTRIBUTE_DIRECTORY) == 0;
	return true;
	}

//--------------------------------------------------------------------------------
bool CPlatformInfo::IsDirectory(LPCTSTR pPath)
	{
	DWORD nAttr = ::GetFileAttributes(pPath);
	if(nAttr == -1)
		return false;
	return (nAttr & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY;
	}

//--------------------------------------------------------------------------------
HANDLE CPlatformInfo::GetDirHandle(LPCTSTR pPath, bool bLock)
	{
	return ::CreateFile(
		pPath,
		GENERIC_READ|GENERIC_WRITE,
		bLock ? 0 : FILE_SHARE_DELETE|FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL|FILE_FLAG_BACKUP_SEMANTICS,
		NULL
		);
	}

//--------------------------------------------------------------------------------
DWORD CPlatformInfo::GetDiskFreeSpace(LPCTSTR pPath, ULARGE_INTEGER* pResult)
	{
	DWORD nRv = -1;
	bool bError;

	CPlatformInfo info;

	if(info.IsWin95OS2R() || info.IsWinNT())
		{
		ULARGE_INTEGER nAvailToCaller;
		ULARGE_INTEGER nTotalBytes;
		ULARGE_INTEGER nTotalFreeBytes;

		if(pResult == NULL)
			pResult = &nAvailToCaller;

		bError = ::GetDiskFreeSpaceEx(pPath, pResult, &nTotalBytes, &nTotalFreeBytes) == 0;
		nRv = pResult->u.LowPart;
		}
	else
		{
		DWORD nSectorsPerCluster;
		DWORD nBytesPerSector;
		DWORD nNumberOfFreeClusters;
		DWORD nTotalNumberOfClusters;

		bError = ::GetDiskFreeSpace(pPath, &nSectorsPerCluster, &nBytesPerSector,
							&nNumberOfFreeClusters, &nTotalNumberOfClusters) == 0;
		nRv = nSectorsPerCluster * nBytesPerSector * nNumberOfFreeClusters;

		if(pResult != NULL)
			{
			pResult->u.LowPart = nRv;
			pResult->u.HighPart = 0;
			}
 		}

	return nRv;
	}

//--------------------------------------------------------------------------------
bool CPlatformInfo::CreatePath(LPCTSTR pPath)
	{
	if(pPath == NULL || *pPath == 0)
		return false;

	CString sTemp(pPath);
	int nIndex = -1;

	bool bIsWide = false;
	const CString sWide("\\\\?\\");

	// first see if the caller is trying to call the wide char version
	if(sTemp.GetLength() > 4)
		if(sTemp[2] == '?')
			{
			sTemp = sTemp.Mid(4);
			bIsWide = true;
			}

	int nColon = sTemp.Find(':', nIndex + 1);

	if(nColon != -1)
		nIndex = nColon + 1;
	else
		if(sTemp.GetLength() > 2)
			{
			// see if its a UNC name
			if(sTemp[nIndex + 1] == '\\' && sTemp[nIndex + 2] == '\\')
				{
				nIndex = sTemp.Find('\\', nIndex + 3);
				if(nIndex == -1)
					return false;
				// now skip over the share name
				nIndex = sTemp.Find('\\', nIndex + 1);
				if(nIndex == -1)
					return false;
				}
			}

	if(nIndex >= sTemp.GetLength())
		return false;

	for(bool bDone = false;! bDone;)
		{
		nIndex = sTemp.Find('\\', nIndex + 1);
		if(nIndex == -1)
			{
			bDone = true;
			nIndex = sTemp.GetLength();
			}

		if(nIndex == 2)
			continue;

		if(bIsWide)
			::CreateDirectory(sWide + sTemp.Left(nIndex), NULL);
		else
			::CreateDirectory(sTemp.Left(nIndex), NULL);
		int nErr = ::GetLastError();
		}

	return true;
	}

//--------------------------------------------------------------------------------
bool CPlatformInfo::GetLoadLibrarySearchPath(CStringArray& sOut)
	{	
	sOut.RemoveAll();

	// return the same search that LoadLibrary does
	// exe path, cur dir, sys dir, win dir, PATH dirs

	//-- first get the path from which the exe ran
	CString sTemp(::GetCommandLine());

	// sanity check
	if(sTemp.GetLength() < 3)
		return false;

	// if the string starts with a quote then it must end with a quote
	// otherwise it ends with a space
	TCHAR cFind = sTemp[0] == _TCHAR('"') ? _TCHAR('"') : _TCHAR(' ');

	// find the quote or space
	int nIndex = sTemp.Find(cFind, 1);
	// sanity check
	if(nIndex == -1)
		nIndex = sTemp.GetLength();
	// get just the pathname of the exe from the cmdline string
	sTemp = sTemp.Mid(cFind == _TCHAR('"'), nIndex - 1);
	// remove the exe name leaving just the directory
	nIndex = sTemp.ReverseFind(_TCHAR('\\'));
	if(nIndex == -1)
		return false;

	sTemp = sTemp.Left(nIndex);

	sOut.Add(sTemp); 

	//-- now check the current dir
	sTemp.ReleaseBuffer(::GetCurrentDirectory(2048, sTemp.GetBuffer(2048)));

	if(sTemp != sOut[0])		
		sOut.Add(sTemp);		

	//-- now check the system32 dir
	sTemp.ReleaseBuffer(::GetSystemDirectory(sTemp.GetBuffer(2048), 2048));
	sOut.Add(sTemp);

	//-- now check the system dir
	sTemp.ReleaseBuffer(::GetWindowsDirectory(sTemp.GetBuffer(2048), 2048));
	sTemp += _T("\\SYSTEM");
	sOut.Add(sTemp);

	//-- now check the windows dir
	sTemp.ReleaseBuffer(::GetWindowsDirectory(sTemp.GetBuffer(2048), 2048));
	sOut.Add(sTemp);

	CString sPath;
	//-- now parse out the PATH variable
	sPath.ReleaseBuffer(::GetEnvironmentVariable(_T("PATH"), sPath.GetBuffer(32767), 32767));
	if(sPath.IsEmpty())
		return true;

	int nCur = 0;
	for(bool bDone = false;! bDone;)
		{
		nIndex = sPath.Find(_TCHAR(';'), nCur);
		if(nIndex == -1)
			{
			nIndex = sPath.GetLength();
			bDone = true;
			}

		int nLen = nIndex - nCur;
		sTemp = sPath.Mid(nCur, nLen);
		// only check it if we havent done this dir already.
		//-----------------------------------------------------
		// Note: It is possible that there will only be 4 elements
		// in this array instead of 5. If the current directory 
		// happens to be the same as the execution directory, only 
		// ONE CString is added to the CStringArray object (sOut).
		// DO NOT check the 5th element in this array if this is 
		// the case because we will throw an ASSERT().
		if(sTemp.CompareNoCase(sOut[0]) != 0
				&& sTemp.CompareNoCase(sOut[1]) != 0
				&& sTemp.CompareNoCase(sOut[2]) != 0
				&& sTemp.CompareNoCase(sOut[3]) != 0
				&& (sOut.GetUpperBound()==4?sTemp.CompareNoCase(sOut[4]):true) != 0)				
			sOut.Add(sTemp);
		nCur += (nLen+1);
		}

	return true;
	}

//--------------------------------------------------------------------------------
bool CPlatformInfo::GetProcessorSerialNumberOnCurrentThread(DWORD nId[3])
	{
	DWORD nFeature;

	// see if the processor serial number
	// is supported
	_asm
		{
		mov eax, 1;
		cpuid;
		mov nFeature, eax;
		};

	// bit 18 should be 1 if it's supported
	if(nFeature & 0x40000 == 0)
		return false;

	DWORD nUp, nMid, nLow;

	// see intel application note
	// AP-909 (I wonder if my baby's travlin' on the note after that one?)
	_asm
		{
		mov eax, 1;
		cpuid;
		mov nUp, eax;
		mov eax, 3;
		cpuid;
		mov nMid, edx;
		mov nLow, ecx;
		};

	nId[2] = nUp;
	nId[1] = nMid;
	nId[0] = nLow;

	return true;
	}

//--------------------------------------------------------------------------------
bool CPlatformInfo::GetProcessorSerialNumber(DWORD nId[3], int nWhich)
	{
	if(m_nP3SerialNumbers.GetSize() == 0)
		{
		CWinThread* pThread = AfxBeginThread((AFX_THREADPROC) SerialNumberThread, NULL);
		WaitForSingleObject(pThread->m_hThread, INFINITE);
		}

	int i = nWhich * 3;

	if(m_nP3SerialNumbers.GetUpperBound() < i + 2)
		return false;

	nId[0] = m_nP3SerialNumbers[i];
	nId[1] = m_nP3SerialNumbers[i+1];
	nId[2] = m_nP3SerialNumbers[i+2];

	return true;
	}

//--------------------------------------------------------------------------------
bool CPlatformInfo::GetProcessorSerialNumber(CString& sId, int nWhich)
	{
	DWORD nId[3];

	if(! GetProcessorSerialNumber(nId, nWhich))
		return false;

	sId.Format(_T("%08X%08X%08X"), nId[2], nId[1], nId[0]);
	return true;
	}

//--------------------------------------------------------------------------------
void CPlatformInfo::GetProcessorSerialNumber(CString& sId, const DWORD nId[3])
	{
	sId.Format(_T("%08X%08X%08X"), nId[2], nId[1], nId[0]);
	}

//--------------------------------------------------------------------------------
UINT CPlatformInfo::SerialNumberThread(void*)
	{
	m_nP3SerialNumbers.RemoveAll();

	DWORD nId[3];

	for(DWORD nWhich = 1;;nWhich<<=1)
		{
		if(! ::SetThreadAffinityMask(::GetCurrentThread(), nWhich))
			break;

		if(! GetProcessorSerialNumberOnCurrentThread(nId))
			{
			return 0;
			}

		m_nP3SerialNumbers.Add(nId[0]);
		m_nP3SerialNumbers.Add(nId[1]);
		m_nP3SerialNumbers.Add(nId[2]);
		}

	return m_nP3SerialNumbers.GetSize();
	}

//--------------------------------------------------------------------------------
CString CPlatformInfo::GetTempPath()
	{
	CString sTemp;
	::GetTempPath(1024, sTemp.GetBuffer(1024));
	sTemp.ReleaseBuffer();
	return sTemp;
	}

//--------------------------------------------------------------------------------
UINT CPlatformInfo::GetTempFileName(LPCTSTR pPath, LPCTSTR pPrefix, UINT nUnique, CString& sTempFilename)
	{
	UINT nRv;
	if(pPath == NULL)
		nRv = ::GetTempFileName(GetTempPath(), pPrefix, nUnique, sTempFilename.GetBuffer(1024));
	else
		nRv = ::GetTempFileName(pPath, pPrefix, nUnique, sTempFilename.GetBuffer(1024));
	sTempFilename.ReleaseBuffer();
	return nRv;
	}

//--------------------------------------------------------------------------------
CString CPlatformInfo::GetTempFileName(LPCTSTR pPath, LPCTSTR pPrefix, UINT nUnique)
	{
	CString sRv;
	CPlatformInfo::GetTempFileName(pPath, pPrefix, nUnique, sRv);
	return sRv;
	}

//--------------------------------------------------------------------------------
bool CPlatformInfo::GetLnkFileTarget(LPCTSTR pLnkPath, CString& sTarget) 
	{
	IShellLink* psl; 
	// Get a pointer to the IShellLink interface. 
	if(FAILED(CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**) &psl)))
		return false;
	bool bRv = false;
	IPersistFile* ppf; 
	// Get a pointer to the IPersistFile interface. 
	if(SUCCEEDED(psl->QueryInterface(IID_IPersistFile, (void**) &ppf)))
		{
		WCHAR wsz[MAX_PATH];
		// Ensure that the string is Unicode. 
		MultiByteToWideChar(CP_ACP, 0, const_cast<LPCSTR>(pLnkPath), -1, wsz, MAX_PATH); 
		// Load the shortcut. 
		if(SUCCEEDED(ppf->Load(wsz, STGM_READ)))
			{
			// Resolve the link. 
			if(SUCCEEDED(psl->Resolve(NULL, 0)))
				{
				char sGotPath[MAX_PATH]; 
				WIN32_FIND_DATA wfd; 
				// Get the path to the link target. 
				if(SUCCEEDED(psl->GetPath(sGotPath, MAX_PATH, (WIN32_FIND_DATA*)&wfd, SLGP_SHORTPATH)))
					{
					sTarget = sGotPath;
					bRv = true;
					}
				}
			}
		// Release the pointer to the IPersistFile interface. 
		ppf->Release();
		}
	
	// Release the pointer to the IShellLink interface. 
	psl->Release();

	return bRv;
	}
