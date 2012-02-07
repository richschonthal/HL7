//--------------------------------------------------------------------------------
// Copyright (c) 2001 @COMPANY, Inc.
// Created...: 6/30/01
// Author....: Rich Schonthal
//--------------------------------------------------------------------------------

#ifndef _SOCKETSERVERQUEUE_H_
#define _SOCKETSERVERQUEUE_H_

//--------------------------------------------------------------------------------
#include <StdobjQueue.h>

//--------------------------------------------------------------------------------
class CSocketServerQueueObj;

//--------------------------------------------------------------------------------
class CSocketServerQueue
	{
	public:
		// the queue names are relative to the client
		// data from the socket
		CSOQueue<CSocketServerQueueObj*, CSocketServerQueueObj*> m_qInput;
		// data to the socket
		CSOQueue<CSocketServerQueueObj*, CSocketServerQueueObj*> m_qOutput;
	};

//--------------------------------------------------------------------------------
class CSocketServerQueueObj
	{
	public:
		enum eMsgType
			{
			SSQO_NOOP				= 0x00000000,
			SSQO_ERROR				= 0x00000001,
			SSQO_CONNECTION_MADE	= 0x00000002,
			SSQO_DATA				= 0x00000004,
			SSQO_ABORT				= 0x00000008,
			SSQO_SOCKET_CLOSE		= 0x00000010
			};

	private:
		eMsgType m_nMsgType;

	protected:
		CSocketServerQueueObj(eMsgType);
		virtual ~CSocketServerQueueObj();

	public:
		eMsgType GetMsgType() const;
	};

//--------------------------------------------------------------------------------
inline CSocketServerQueueObj::CSocketServerQueueObj(eMsgType nMsgType)
		: m_nMsgType(nMsgType) {}

//--------------------------------------------------------------------------------
class CSocketServerQueueDataObj : public CSocketServerQueueObj
	{
	private:
		UCHAR* m_pData;
		int m_nLen;

	public:
		CSocketServerQueueDataObj(const void* pData, int nLen);
		virtual ~CSocketServerQueueDataObj();

		const void* GetData() const;
		int GetDataLength() const;
	};

//--------------------------------------------------------------------------------
inline CSocketServerQueueDataObj::CSocketServerQueueDataObj(const void* pData, int nLen)
		: CSocketServerQueueObj(SSQO_DATA)
		, m_nLen(nLen)
		
	{
	if(nLen == 0)
		m_pData = NULL;
	else
		{
		m_pData = new UCHAR[nLen];
		memcpy(m_pData, pData, nLen);
		}
	}

//--------------------------------------------------------------------------------
inline CSocketServerQueueDataObj::~CSocketServerQueueDataObj()
	{
	delete[] m_pData;
	}

//--------------------------------------------------------------------------------
inline const void* CSocketServerQueueDataObj::GetData() const { return m_pData; }
inline int CSocketServerQueueDataObj::GetDataLength() const { return m_nLen; }

#endif //_SOCKETSERVERQUEUE_H_