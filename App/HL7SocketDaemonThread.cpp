//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "HL7SocketDaemonThread.h"

#include <SmallSocket.h>

#include <DebugPrintf.h>

#include "globals.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//--------------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(CHL7SocketDaemonThread, CHL7InputDaemonThread)

//--------------------------------------------------------------------------------
CHL7SocketDaemonThread::CHL7SocketDaemonThread()
	{
	}

//--------------------------------------------------------------------------------
CHL7SocketDaemonThread::~CHL7SocketDaemonThread()
	{
	}

//--------------------------------------------------------------------------------
void CHL7SocketDaemonThread::Init(const CHL7InputDaemonInit* pInit, CHL7InputDaemon* pParentObj)
	{
	GetIO()->Output(IOMASK_7|IOMASK_CONST, "CHL7SocketDaemonThread::Init entry");

	CHL7InputDaemonThread::Init(pInit, pParentObj);

	m_mainSock.CreateNewSocket();

	if(! m_mainSock.GetSocket()->Create(pInit->GetPort(), pInit->GetAddress()))
		{
		GetIO()->FormatOutput(IOMASK_ERR, "error %ld creating socket %d %s ", 
				::WSAGetLastError(), pInit->GetPort(), pInit->GetAddress());
		::AfxEndThread(0);
		}

	GetIO()->FormatOutput(IOMASK_10, "socket %d %s created", pInit->GetPort(), pInit->GetAddress());

	m_mainSock.GetSocket()->NonBlockingMode();
	GetIO()->Output(IOMASK_8|IOMASK_CONST, "CHL7SocketDaemonThread::Init exit");
	}

//--------------------------------------------------------------------------------
bool CHL7SocketDaemonThread::GetConnection(bool bForceNew)
	{
	if(EventStopSignaled())
		return false;

	CSingleLock lock(&m_curSock.GetMutex(), false);
	if(! lock.Lock())
		{
		GetIO()->Output(IOMASK_ERR|IOMASK_CONST, "Unable to lock current socket (CHL7SocketDaemonThread::GetConnection)");
		return false;
		}

	if(m_pInit->IsClient())
		{
		// when in client mode, only create a connection when it's requested
		// this is so that a connection is made only when we're about to send a string
		if(bForceNew)
			{
			m_curSock.CreateNewSocket();
			m_curSock.GetSocket()->Create();
			m_curSock.GetSocket()->NonBlockingMode();

			if(! m_curSock.GetSocket()->Connect(m_pInit->GetOutAddr(), m_pInit->GetOutPort()))
				{
				m_curSock.DestroySocket();
				GetIO()->FormatOutput(IOMASK_10, "error %ld connecting to socket %d %s ", 
						::WSAGetLastError(), m_pInit->GetOutPort(), m_pInit->GetOutAddr());
				}
			else
				{
				CString sTemp;
				UINT nPort;

				m_curSock.GetSocket()->NonBlockingMode();
				m_curSock.GetSocket()->GetSockName(sTemp, nPort);
				GetIO()->FormatOutput(IOMASK_10, "Opened client connection on port %d", nPort);
				}
			}

		return m_curSock.IsValidSocket();
		}
	else //server
		{
		// no main socket
		if(! m_mainSock.IsValidSocket())
			return false;

		// there's already a socket
		if(m_curSock.IsValidSocket())
			{
			// if forcing a new one
			if(bForceNew)
				m_curSock.DestroySocket();
			else
				// theres already an open socket
				// so just return
				return true;
			}

		// get a new connection
		if(m_mainSock.GetSocket()->Listen(1))
			{
			m_curSock.CreateNewSocket();

			if(m_mainSock.GetSocket()->Accept(*m_curSock.GetSocket()))
				{
				m_curSock.GetSocket()->NonBlockingMode();
				return true;
				}
			else
				m_curSock.DestroySocket();
			}
		}

	return false;
	}

//--------------------------------------------------------------------------------
void CHL7SocketDaemonThread::MainLoop(WPARAM p1, LPARAM p2)
	{
	if(EventStopSignaled())
		return;

	const int nBufLen = 65535;
	char pBuf[nBufLen];
	char cEndOfMsg = (char) 0x1c;
	char cEscape = 0;
	char cPrevLastChar = 0;
	char cLastChar = 0;

#define SetLastChar(c) {cPrevLastChar = cLastChar; cLastChar = c; }

	int nLen = 0;
	int nErr = 0;

	CSyncObject* sockLocks[2] = { &m_mainSock.GetMutex(), &m_curSock.GetMutex() };
	CMultiLock lock(sockLocks, 2);

	if(lock.Lock() == -1)
		{
		GetIO()->Output(IOMASK_ERR|IOMASK_CONST, "Unable to lock sockets (CHL7SocketDaemonThread::MainLoop)");
		return;
		}

	CHL7Message* pCurMsg = NULL;
	// any connections waiting?
	if(GetConnection())
		{
		__int64 nLastDataReceived = ::GetTickCount();

		CString sTemp;

		for(bool bDone = false;! bDone;)
			{
			if(EventStopSignaled())
				return;

			// there's no socket so bail
			if(! m_curSock.IsValidSocket())
				break;

			MSG msg;
			if(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{
				if(msg.message == ITWM_SEND_STRING)
					{
					SendString((LPCTSTR) msg.wParam);
					nLastDataReceived = ::GetTickCount();
					::Sleep(0);
					continue;
					}
				}

			// read the data
			nLen = 0;
			nErr = 0;

			// read the data
			nLen = m_curSock.GetSocket()->Receive(pBuf, nBufLen);

			// there's incomming data to process
			if(nLen > 0)
				{
				nLastDataReceived = ::GetTickCount();

				// make sure the trace will be displayed otherwise dont waste time
				if(GetIO()->IsDisplayed(IOMASK_11))
					{
					CString sOut;
					for(int i = 0; i < nLen; i++)
						{
						CString sTemp;
						TCHAR c = pBuf[i];
						sTemp.Format("|0x%02x '%c' ", c, (isprint(c) ? c : '.'));
						sOut += sTemp;
						// limit each line to 20 entries or spit out the last line
						if( (i != 0 && (i%19) == 0) || i == nLen - 1)
							{
							sOut += '\n';
							GetIO()->Output(IOMASK_11|IOMASK_CONST, (LPCTSTR) sOut);
							sOut.Empty();
							}
						}
					}

				// the string may have several segments in it so parse it out
				for(int i = 0; i < nLen; i++)
					{
					if(pCurMsg != NULL && cEscape == 0)
						cEscape = pCurMsg->GetEscapeChar();

					// see if we need to do any escape char processing
					if(cEscape != 0)
						{
						if(cPrevLastChar == cEscape && cLastChar == '\r' && pBuf[i] != '\n')
							{
							SetLastChar(pBuf[i]);
							sTemp += cLastChar;
							continue;
							}

						if(cLastChar == cEscape)
							{
							// see if the last char on the line was a backslash which means
							// the line is continued on the next line
							// further escape sequence processing will be done in CSegment::Parse()
							if(pBuf[i] == '\r' || pBuf[i] == '\n')
								{
								SetLastChar(pBuf[i]);
								sTemp += pBuf[i];
								continue;
								}

							// 2 escapes in a row which means insert literal
							if(pBuf[i] == cEscape)
								{
								sTemp += cEscape;
								SetLastChar(0);
								continue;
								}
							else
								{
								// insert the escape sequence and continue
								sTemp += cEscape;
								sTemp += pBuf[i];
								SetLastChar(pBuf[i]);
								continue;
								}
							}

						// save the escape as the last char but dont insert it yet
						// if a cr/lf follows then we wont insert the escape
						// otherwise we will
						if(pBuf[i] == cEscape)
							{
							SetLastChar(pBuf[i]);
							continue;
							}
						}

					if((pBuf[i] == '\n' || pBuf[i] == '\r' || pBuf[i] == 0) && ! sTemp.IsEmpty())
						{
						if(pCurMsg == NULL)
							pCurMsg = new CHL7Message;

						switch(pCurMsg->AddSegment(sTemp))
							{
							case CHL7Message::MSG_START_NEW:
								GetIO()->Output(IOMASK_10|IOMASK_CONST, "sending message(1)");
								ProcessMessage(pCurMsg);
								pCurMsg = new CHL7Message;
								pCurMsg->AddSegment(sTemp);
								break;
							}

						sTemp.Empty();
						continue;
						}

					if(pBuf[i] == cEndOfMsg)
						{
						bDone = true;
						break;
						}
					else
						{
						SetLastChar(pBuf[i]);
						// if escape hasnt been defined then we dont check for it
						// otherwise if the last char was escape then we'll insert this
						// control character
						if((pBuf[i] < 0x20 && cEscape == 0) || (pBuf[i] < 0x20 && cPrevLastChar != cEscape))
							continue;

						sTemp += pBuf[i];
						}

					continue;
					}
				}

			// the connection has gone away
			if(nLen == 0)
				{
				m_curSock.DestroySocket();

				if(lock.IsLocked(0) || lock.IsLocked(1))
					lock.Unlock();

				bDone = true;
				break;
				}

			// there's an error
			if(nLen == SOCKET_ERROR)
				{
				nErr = ::WSAGetLastError();
				switch(nErr)
					{
					case 0:
						GetIO()->Output(IOMASK_ERR|IOMASK_CONST, "The system has gone ape shit");
						nErr = -1;
						break;
					default:
						GetIO()->FormatOutput(IOMASK_ERR|IOMASK_CONST, "A socket error has occured (%d)", nErr);
						break;
					case WSAEACCES:
						GetIO()->Output(IOMASK_ERR|IOMASK_CONST, "Access denied on socket");
						break;
					case WSAEMFILE:
						GetIO()->Output(IOMASK_ERR|IOMASK_CONST, "Too many sockets open");
						break;
					case WSAENETDOWN:
						GetIO()->Output(IOMASK_ERR|IOMASK_CONST, "The network is down");
						break;
					case WSAENETUNREACH:
						GetIO()->Output(IOMASK_ERR|IOMASK_CONST, "Network unreachable");
						break;
					case WSAECONNABORTED:
						GetIO()->Output(IOMASK_ERR|IOMASK_CONST, "Connection aborted by the OS");
						break;
					// since this is a non-blocking socket, WSAEWOULDBLOCK means there's no data - go figure
					case WSAEWOULDBLOCK:
						nErr = 0;
						nLen = 0;
						if(! bDone)
							{
							__int64 nNow = ::GetTickCount();
							if(nNow < nLastDataReceived)
								nNow += (__int64) 0xffffffff;
							bDone = (nNow - nLastDataReceived) > m_pInit->GetMaxIdle();
							}
						break;
					// connection reset
					case WSAECONNRESET:
					// not connected
					case WSAENOTCONN:
					// connection was shut down
					case WSAESHUTDOWN:
					// the connection timed out
					case WSAETIMEDOUT:
					// the host is down
					case WSAEHOSTDOWN:
					// graceful shutdown
					case WSAEDISCON:
					case WSATRY_AGAIN:
						GetIO()->FormatOutput(IOMASK_10, "socket error %ld", nErr);
						// try and restart the connection
						if(! GetConnection(true))
							bDone = true;
						nErr = 0;
						continue;
					// winsock reported that the target address is not a socket
					// this probably means somethings not running so
					// we should keep trying until this socket comes up
					// this happens with the Meditech HL7 interface
					case WSAENOTSOCK:
						GetIO()->FormatOutput(IOMASK_10, "socket error %ld", nErr);
						::Sleep(4000);
						// try and restart the connection
						if(! GetConnection(true))
							bDone = true;
						nErr = 0;
						continue;
					}
				}
			}
		}

	if(lock.IsLocked(0) || lock.IsLocked(1))
		lock.Unlock();

	if(pCurMsg != NULL && m_curSock.IsValidSocket())
		{
		GetIO()->Output(IOMASK_10|IOMASK_CONST, "sending message");
		ProcessMessage(pCurMsg);
		}
	else
		delete pCurMsg;

	::Sleep(0);

	// tricky thing here
	// call the base class function which posts
	// a message which calls us! HA!!
	CHL7InputDaemonThread::MainLoop(p1, p2);
	}

//--------------------------------------------------------------------------------
bool CHL7SocketDaemonThread::SendString(LPCTSTR pStr)
	{
	if(pStr == NULL)
		{
		GetIO()->Output(IOMASK_10|IOMASK_CONST, "CHL7SocketDaemonThread::SendString(NULL)");
		return false;
		}

	CSingleLock lock(&m_curSock.GetMutex(), false);
	if(! lock.Lock(1000))
		{
		GetIO()->Output(IOMASK_ERR|IOMASK_CONST, "Unable to lock current socket (CHL7SocketDaemonThread::SendString)");
		return false;
		}

	if(! m_curSock.IsValidSocket())
		if(! GetConnection(true))
			{
			GetIO()->Output(IOMASK_ERR|IOMASK_CONST, "Can't open socket");
			return false;
			}

	CString sTemp;
	sTemp.Format("%s", pStr);

	// return true if # of bytes sent = nSize
	int nSize = sTemp.GetLength();
	int nSent;

	for(;;)
		{
		nSent = m_curSock.GetSocket()->Send((LPCTSTR) sTemp, nSize, CSmallSocket::WAITFORWOULDBLOCK, 10000);

//		GetIO()->FormatOutput(IOMASK_2, "%ld milliseconds\n", ::GetTickCount() - g_n1);
		GetIO()->FormatOutput(IOMASK_11, "CHL7SocketDaemonThread::SendString (%d chars @ %s) - %s\n",
			nSent, (LPCTSTR) sTemp, pStr);

		if(nSent != nSize)
			{
			int nErr = ::WSAGetLastError();
			if(nErr == WSAEINPROGRESS)
				{
				::Sleep(100);
				continue;
				}

			GetIO()->FormatOutput(IOMASK_10|IOMASK_13, "CHL7SocketDaemonThread::SendString socket err=%ld", nErr);
			m_curSock.DestroySocket();
			}
		else
			TRACE("SENT %s\n", pStr);

		break;
		}

	return nSent == nSize;
	}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
CSocketWrapper::CSocketWrapper()
		: m_pSocket(NULL)
	{
	}

//--------------------------------------------------------------------------------
CSocketWrapper::~CSocketWrapper()
	{
	CSingleLock lock(&m_mutex, false);
	VERIFY(lock.Lock());
	delete m_pSocket;
	}

//--------------------------------------------------------------------------------
void CSocketWrapper::CreateNewSocket()
	{
	CSingleLock lock(&m_mutex, false);
	VERIFY(lock.Lock());
	
	DestroySocket();
	m_pSocket = new CSmallSocket;
	}

//--------------------------------------------------------------------------------
CSmallSocket* CSocketWrapper::GetSocket()
	{
	return m_pSocket;
	}

//--------------------------------------------------------------------------------
void CSocketWrapper::DestroySocket()
	{
	CSingleLock lock(&m_mutex, false);
	VERIFY(lock.Lock());

	CSmallSocket* pSock = m_pSocket;
	m_pSocket = NULL;
	delete pSock;
	}

//--------------------------------------------------------------------------------
bool CSocketWrapper::IsValidSocket() const
	{
	return m_pSocket != NULL;
	}
