//--------------------------------------------------------------------------------
//
// Copyright (c) 2001 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _CHANGENOTIFICATIONCONTEXTOBJ_H_
#define _CHANGENOTIFICATIONCONTEXTOBJ_H_

//--------------------------------------------------------------------------------
#include <Handle.h>

//--------------------------------------------------------------------------------
class CSystemObject;
class CChangeNotificationObj;
class CFileChangeNotificationObj;

//--------------------------------------------------------------------------------
typedef BOOL (__stdcall *ReadDirectoryChangesWType)(
    IN HANDLE hDirectory,
    IN OUT LPVOID lpBuffer,
    IN DWORD nBufferLength,
    IN BOOL bWatchSubtree,
    IN DWORD dwNotifyFilter,
    OUT LPDWORD lpBytesReturned,
    IN LPOVERLAPPED lpOverlapped,
    IN LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
    );

//--------------------------------------------------------------------------------
class CChangeNotificationContextObj
	{
	friend class CChangeNotificationThread;
	protected:
		CEvent m_event;
		CSystemObject* m_pTarget;
		int m_nMessageId;
		static ReadDirectoryChangesWType m_pReadDirectoryChangesW;
		bool m_bWatchSubDir;
		DWORD m_nFilter;

	public:
		CChangeNotificationContextObj();
		virtual ~CChangeNotificationContextObj();

		CEvent& GetEvent();
		CSystemObject* GetTarget();
		int GetMessageId();

		virtual LPCTSTR GetPath() const;

		virtual bool IsValid();

		virtual bool IsChangeReady();
		virtual bool PostChanges();
		virtual bool GetNextChange();
	};

//--------------------------------------------------------------------------------
inline CEvent& CChangeNotificationContextObj::GetEvent() { return m_event; }
inline CSystemObject* CChangeNotificationContextObj::GetTarget() { return m_pTarget; }
inline int CChangeNotificationContextObj::GetMessageId() { return m_nMessageId; }
inline LPCTSTR CChangeNotificationContextObj::GetPath() const { return NULL; }
inline bool CChangeNotificationContextObj::IsValid() { return false; }
inline bool CChangeNotificationContextObj::IsChangeReady() { return false; }
inline bool CChangeNotificationContextObj::PostChanges() { return false; }
inline bool CChangeNotificationContextObj::GetNextChange() { return false; }

//--------------------------------------------------------------------------------
class CFileChangeNotificationContextObj : public CChangeNotificationContextObj
	{
	private:
		CHandle m_hDir;
		LPCTSTR m_pPath;

		// for the overlapped result
		OVERLAPPED m_overlapped;

		CHandle m_hCompletionPort;

		enum { BUFFER_SIZE = 524288, BUFFER_BLOCK_SIZE = 4096 };
		UCHAR m_pBuffer[BUFFER_SIZE];
		int m_nCurBufIndex;

		DWORD m_nSize;
		UCHAR* m_pBase;
		OVERLAPPED* m_pOverlap;

		CFileChangeNotificationObj* m_pPrevRenameObj;

	public:
		CFileChangeNotificationContextObj();
		virtual ~CFileChangeNotificationContextObj();

		bool Create(LPCTSTR pPath, bool bWatchSubDir, DWORD nFilter, CSystemObject* pTarget, int nMsg);

		virtual LPCTSTR GetPath() const;

		virtual bool IsValid();

		virtual bool IsChangeReady();
		virtual bool PostChanges();
		virtual bool GetNextChange();
	};

//--------------------------------------------------------------------------------
class CRegChangeNotificationContextObj : public CChangeNotificationContextObj
	{
	private:
		HKEY m_hKey;

	public:
		CRegChangeNotificationContextObj();
		virtual ~CRegChangeNotificationContextObj();

		bool Create(HKEY, bool bWatchSubKeys, DWORD nFilter, CSystemObject* pTarget, int nMsg);

		virtual bool IsValid();

		virtual bool IsChangeReady();
		virtual bool PostChanges();
		virtual bool GetNextChange();
	};

//--------------------------------------------------------------------------------
inline bool CRegChangeNotificationContextObj::IsValid() { return m_hKey != NULL; }
inline bool CRegChangeNotificationContextObj::IsChangeReady() { return true; }
inline bool CRegChangeNotificationContextObj::GetNextChange()
	{
	m_event.ResetEvent();
	return ::RegNotifyChangeKeyValue(m_hKey, m_bWatchSubDir, m_nFilter, m_event, TRUE) == ERROR_SUCCESS;
	}

#endif // _CHANGENOTIFICATIONCONTEXTOBJ_H_
