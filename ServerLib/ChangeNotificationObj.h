//--------------------------------------------------------------------------------
//
// Copyright (c) 2001 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _CHANGENOTIFICATIONOBJ_H_
#define _CHANGENOTIFICATIONOBJ_H_

//--------------------------------------------------------------------------------
#include <Registry.h>
#include <FileNotifyInformation.h>

//--------------------------------------------------------------------------------
class CChangeNotificationObj
	{
	private:
		LPCTSTR m_pPath;

	public:
		CChangeNotificationObj(LPCTSTR pPath);

		LPCTSTR GetPath() const;

		enum eNotifyType
			{
			NotifyUnknown,
			NotifyFile,
			NotigyRegistry
			};

		// poor mans rtti
		virtual eNotifyType GetNotificationType() = 0;
	};

//--------------------------------------------------------------------------------
class CFileChangeNotificationObj : public CFileNotifyInformation, public CChangeNotificationObj
	{
	public:
		LPCTSTR m_pOldFilename;

	public:
		CFileChangeNotificationObj(const FILE_NOTIFY_INFORMATION*, LPCTSTR pPath);
		virtual eNotifyType GetNotificationType();
	};

//--------------------------------------------------------------------------------
class CRegChangeNotificationObj : public CChangeNotificationObj
	{
	private:
		HKEY m_hKey;

	public:
		CRegChangeNotificationObj(HKEY);
		virtual eNotifyType GetNotificationType();

		const HKEY GetRegKey() const;
	};

//--------------------------------------------------------------------------------
inline CChangeNotificationObj::CChangeNotificationObj(LPCTSTR p) : m_pPath(p) {}
inline LPCTSTR CChangeNotificationObj::GetPath() const { return m_pPath; }

//--------------------------------------------------------------------------------
inline CFileChangeNotificationObj::CFileChangeNotificationObj(const FILE_NOTIFY_INFORMATION* pInfo, LPCTSTR pPath)
		: CFileNotifyInformation(pInfo)
		, CChangeNotificationObj(pPath)
		, m_pOldFilename(NULL)
	{
	}

inline CChangeNotificationObj::eNotifyType CFileChangeNotificationObj::GetNotificationType() { return NotifyFile; }

//--------------------------------------------------------------------------------
inline CRegChangeNotificationObj::CRegChangeNotificationObj(HKEY hKey)
	: CChangeNotificationObj(NULL) {}
inline CChangeNotificationObj::eNotifyType CRegChangeNotificationObj::GetNotificationType() { return NotigyRegistry; }
inline const HKEY CRegChangeNotificationObj::GetRegKey() const { return m_hKey; }

#endif //_CHANGENOTIFICATIONOBJ_H_
