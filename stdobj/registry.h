//********************************************************************************
// Reg Lib
// Classes for reading and writing registry stuff
//
// designed and coded by Rich Schonthal
// Copyright (c) 1997 Rich Schonthal
//
// this code is made public domain by the author
// it may be used in products commercial or otherwise
// as long as all copyright notices remain in tact
//********************************************************************************

#ifndef _REGISTRY_H_
#define _REGISTRY_H_

#include "result.h"

#define REGCLASSNAME_Sz _T("REG_SZ")
#define REGCLASSNAME_Binary _T("REG_BINARY")
#define REGCLASSNAME_ExpandSz _T("REG_EXPAND_SZ")
#define REGCLASSNAME_Link _T("REG_LINK")
#define REGCLASSNAME_MultiSz _T("REG_MULTI_SZ")
#define REGCLASSNAME_None _T("REG_NONE")
#define REGCLASSNAME_ResourceList _T("REG_RESOURCE_LIST")
#define REGCLASSNAME_DWord _T("REG_DWORD")
#define REGCLASSNAME_DWordLittleEndian _T("REG_DWORD_LITTLE_ENDIAN")
#define REGCLASSNAME_DWordBigEndian _T("REG_DWORD_BIG_ENDIAN")

class CRegKeyInfo;

//********************************************************************************
class CRegKeyResult : public CResult
	{
	public:
		enum
		 {
		 errRegistry = errOS + 1,
		 errWindows,

		 errNotKeyOwner = BeginErrorRange,
		 errKeyNotOpen,
		 errKeyAlreadyOpen,
		 errRegistryNotSupported,
		 errUnsupportedConnection,
		 errUninitializedValueType,
		 errInvalidSaveFileName,
		 errKeyNotLoaded,
		 errSecurityDescNotSet,
		 errValueTypeMismatch,
		 HighestRegKeyError
		 };

	DECLARE_RESULTCLASS(CRegKeyResult)
	DECLARE_RESULTMAP(m_nMessageMap)
	};

//********************************************************************************
class CRegKey : public CRegKeyResult
	{
	protected:
		HKEY m_hParent;
		HKEY m_hKey;
		LPTSTR m_pKeyName;
		LPTSTR m_pRemoteMachineName;
		LPTSTR m_pSaveFileName;
		LONG m_nEnum;
		CHAR m_nEnumDirection;  // 1 for StartEnumKeyFirst, -1 for StartEnumKeyLast
		BOOL m_bIsCopy;
		BOOL m_bIsLoaded;
		BOOL m_bIsRemote;
		DWORD m_nPlatform;
		PSECURITY_DESCRIPTOR m_pSecurityDescriptor;

		void SetParentKey(HKEY p)  { m_hParent = p; }

	private:
		void SetDefaultMemberValues();
		static void SetDefaultMemberValues(CRegKey& rk)	 {rk.SetDefaultMemberValues();}

		void SetMemberString(LPTSTR&, LPCTSTR);
		void SetKeyName(LPCTSTR s)						 { SetMemberString(m_pKeyName, s); }
		void SetKeyName(LPCTSTR, LPCTSTR);
		void AppendToKeyName(LPCTSTR);

		BOOL ConnectRemote(LPCTSTR, const HKEY);

	public:
		// constructors and destructor
		CRegKey(LPCTSTR = NULL, LPCTSTR = NULL);
		CRegKey(const HKEY);
		CRegKey(LPCTSTR, const HKEY);
		CRegKey(const HKEY, LPCTSTR, LPCTSTR = NULL);
		CRegKey(const HKEY, const CRegKey&);
		CRegKey(const CRegKey&);
		CRegKey(const CRegKey&, LPCTSTR);
		virtual ~CRegKey();

		// key access
		HKEY GetParentKey() const 			 { return m_hParent; }
		HKEY GetKey() const					 { return m_hKey; }
		PHKEY GetKeyPtr() 					 { return &m_hKey; }

		// key info
		LPCTSTR GetKeyName() const			 { return (LPCTSTR) m_pKeyName; }
		virtual LPTSTR GetValueClass()		 { return REGCLASSNAME_None; }

		CRegKeyInfo ReadKeyInfo();

		LPTSTR GetRemoteMachineName() const	 { return m_pRemoteMachineName; }
		void SetRemoteMachineName(LPCTSTR s)	 { SetMemberString(m_pRemoteMachineName, s); }

		LPTSTR GetSaveFileName() const		 { return m_pSaveFileName; }
		void SetSaveFileName(LPCTSTR s)		 { SetMemberString(m_pSaveFileName, s); }

		DWORD GetPlatform() const 			 { return m_nPlatform; }

		// registry actions
		BOOL CreateKey(DWORD* pDisposition = NULL, DWORD = REG_OPTION_NON_VOLATILE, REGSAM = KEY_ALL_ACCESS);
		BOOL OpenKey(REGSAM = KEY_ALL_ACCESS);
		BOOL CloseKey();
		BOOL DeleteKey();
		BOOL FlushKey();

		BOOL SaveKey(LPSECURITY_ATTRIBUTES = NULL, LPCTSTR = NULL);
		BOOL LoadKey(LPCTSTR);
		BOOL UnLoadKey();

		BOOL ReplaceKey(LPCTSTR, LPCTSTR = NULL);
		BOOL RestoreKey(DWORD = 0, LPCTSTR = NULL);

		BOOL NotifyChangeKeyValue(BOOL, DWORD, HANDLE, BOOL);

		inline void AllocateSecurityDescriptor(int nSize)
		 {m_pSecurityDescriptor = new BYTE[nSize]; }

		BOOL GetKeySecurity(SECURITY_INFORMATION);
		BOOL SetKeySecurity(SECURITY_INFORMATION);

		// enum operations
		void StartEnumFirst() 		{ m_nEnum = 0; m_nEnumDirection = 1; }
		BOOL StartEnumLast(BOOL = TRUE); // TRUE for keys FALSE for values
		BOOL IterateEnumKey(CRegKey&);
		int GetEnumValue()			{ return m_nEnum; }

		// status stuff
		BOOL IsOpen() const				{ return m_hKey != NULL; }
		BOOL IsCopy() const				{ return m_bIsCopy; }
		BOOL IsLoaded() const 			{ return m_bIsLoaded; }
		BOOL IsRemote() const 			{ return m_bIsRemote; }

		// operators
		const CRegKey& operator=(const CRegKey&);
		CRegKey operator+(const CRegKey&);
		CRegKey operator+=(const CRegKey&);
		CRegKey operator+(LPCTSTR);
	};

//********************************************************************************
class CRegKeyInfo : public CRegKeyResult
	{
	private:
		LPTSTR m_pClassName;
		DWORD m_nReserved;
		DWORD m_nNumOfSubKeys;
		DWORD m_nMaxSubKeyName;
		DWORD m_nMaxClassName;
		DWORD m_nNumOfValues;
		DWORD m_nMaxValueName;
		DWORD m_nMaxValueLen;
		DWORD m_nSecurityDescLen;
		FILETIME m_ftLastWrite;

	public:
		CRegKeyInfo(const CRegKey&);
		~CRegKeyInfo();

		LPTSTR GetClassName()				{ return m_pClassName; }
		DWORD GetSubKeyCount()				{ return m_nNumOfSubKeys; }
		DWORD GetReserved()					{ return m_nReserved; }
		DWORD GetMaxlenSubKeyName() 		{ return m_nMaxSubKeyName; }
		DWORD GetMaxlenClassName()			{ return m_nMaxClassName; }
		DWORD GetValueCount()				{ return m_nNumOfValues; }
		DWORD GetMaxlenValueName()			{ return m_nMaxValueName; }
		DWORD GetMaxlenValue()				{ return m_nMaxValueLen; }
		DWORD GetSizeofSecurityDescriptor() { return m_nSecurityDescLen; }
		FILETIME& GetLastWriteTime()		{ return m_ftLastWrite; }
	};


//********************************************************************************
// value classes represent values in the registry, they do not contain any data
// they merely provide method for storing and retrieving data from the registry
// thus they are not derived from CObject
// use GetValueType in leu of IsKindOf

//********************************************************************************
class CRegValue : public CRegKey
	{
	protected:
		LPTSTR m_pValueName;
		DWORD m_nType;

	public:
		CRegValue() { m_pValueName = NULL; m_nType = -1; };

		CRegValue(const CRegKey& k, LPCTSTR s = NULL, DWORD vt = -1) 
		 : CRegKey(k) { m_pValueName = NULL; SetValueName(s); SetValueType(vt); }

		CRegValue(const HKEY hParent, LPCTSTR pKeyName, LPCTSTR s = NULL, DWORD vt = -1) 
			: CRegKey(hParent, pKeyName, NULL) { m_pValueName = NULL; SetValueName(s); SetValueType(vt); }

		~CRegValue()							{ if(m_pValueName) delete m_pValueName; }

		LPCTSTR GetValueName() const			{ return m_pValueName == NULL ? GetKeyName() : m_pValueName; }
		void SetValueName(LPCTSTR);

		virtual DWORD GetValueType()			{ return m_nType; }
		void SetValueType(DWORD t)			{ m_nType = t; }

		BOOL ReadValue(LPBYTE*, REGSAM = KEY_QUERY_VALUE);
		BOOL ReadValue(CString&, REGSAM = KEY_QUERY_VALUE);
		BOOL ReadValue(LPDWORD, REGSAM = KEY_QUERY_VALUE);
		
		BOOL WriteValue(const LPBYTE, DWORD);
		BOOL WriteValue(const CString&);
		BOOL WriteValue(DWORD);
		
		BOOL DeleteValue();

		CRegValue operator=(const CRegValue&);

		BOOL StartEnumLast(BOOL)	 { return CRegKey::StartEnumLast(FALSE); }

		BOOL IterateEnumValue();
	};

//********************************************************************************
#define REGVALCLASS(classname, regtype) \
class CRegValue##classname : public CRegValue \
	{ \
	public: \
		CRegValue##classname(const CRegKey& k, LPCTSTR s) : CRegValue(k, s, regtype) {} \
		CRegValue##classname(const HKEY hParent, LPCTSTR pKeyName, LPCTSTR s) \
			: CRegValue(hParent, pKeyName, s, regtype) {} \
		virtual LPTSTR GetValueClass()				{ return REGCLASSNAME_##classname; } \
		virtual DWORD GetValueType()					{ return regtype; } \
	};


//********************************************************************************
REGVALCLASS(Binary, REG_BINARY)
REGVALCLASS(ExpandSz, REG_EXPAND_SZ)
REGVALCLASS(Link, REG_LINK)
// multisz: special case - see class definition below
//REGVALCLASS(MultiSz, REG_MULTI_SZ)
REGVALCLASS(None, REG_NONE)
REGVALCLASS(Sz, REG_SZ)
REGVALCLASS(DWord, REG_DWORD)
REGVALCLASS(DWordLittleEndian, REG_DWORD_LITTLE_ENDIAN)
REGVALCLASS(DWordBigEndian, REG_DWORD_BIG_ENDIAN)
// resource list included for completness but this data type
// goes beyond the scope of this classes purpose
// if you're doing stuff with device drivers and you implement this class properly
// please email it to me or post it somewhere very public so I can find it
REGVALCLASS(ResourceList, REG_RESOURCE_LIST)


//********************************************************************************
class CRegValueMultiSz : public CRegValue
	{
	public:
		CRegValueMultiSz() {};
		CRegValueMultiSz(const CRegKey& k, LPCTSTR s = NULL) : CRegValue(k, s) {}

		virtual LPTSTR GetValueClass()				{ return REGCLASSNAME_MultiSz; }
		virtual DWORD GetValueType()					{ return REG_MULTI_SZ; }
		
		BOOL ReadValue(CStringArray&, REGSAM = KEY_QUERY_VALUE);
		
		BOOL WriteValue(const CStringArray&);

	};

#endif