//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "globals.h"
#include "HL7InputDaemon.h"
#include "HL7FileDaemon.h"

#include <handle.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//--------------------------------------------------------------------------------
CHL7FileDaemon::CHL7FileDaemon()
	{
	m_pCurFile = NULL;
	m_pAckFile = NULL;
	}

//--------------------------------------------------------------------------------
CHL7FileDaemon::~CHL7FileDaemon()
	{
	delete m_pCurFile;
	delete m_pAckFile;

	m_pCurFile = NULL;
	m_pAckFile = NULL;
	}

//--------------------------------------------------------------------------------
void CHL7FileDaemon::MainLoop()
	{
	CString sTemp;
	// looking at all files
	CString sFilename("\\*.*");
	sFilename = m_pInit->GetPath() + sFilename;

	// \\\\?\\ is NT's way of transending the MAX_PATH limit
	// this cause the wide char version of FindFirstFile to be used
	// this feature is available in most NT file-based functions

	WIN32_FIND_DATA findData;

	{// hand scope
	CFindFileHandle hand = ::FindFirstFile("\\\\?\\" + sFilename, &findData);

	if(! hand.IsValid())
		{
		GetIO()->Output(IOMASK_ERR|IOMASK_CONST, "Attempt to open files failed");
		return;
		}

	CHL7Message* pCurMsg = NULL;
	TRY
		{
		for(;;)
			{
			// . and .. come up as files so we want to skip them
			if(findData.cFileName[0] == '.')
				m_pCurFile = NULL;
			else
				{
				// open the file
				m_pCurFile = new CStdioFile;
				sFilename = m_pInit->GetPath();
				sFilename += '\\';
				sFilename += findData.cFileName;

				if(m_pCurFile->Open("\\\\?\\" + sFilename, CFile::modeRead|CFile::shareDenyNone))
					{
					pCurMsg = new CHL7Message;

					ASSERT(pCurMsg != NULL);

					GetIO()->Output(IOMASK_12|IOMASK_CONST, "Processing file: " + sFilename);

					CString sPrevString;
					char cEscape = '\\';

					// read stuff
					while(m_pCurFile->ReadString(sTemp))
						{
						GetIO()->Output(IOMASK_12|IOMASK_CONST, sTemp);
						if(EventStopSignaled())
							{
							delete m_pCurFile;
							m_pCurFile = NULL;
							goto label_exit;
							}

						if(sTemp.GetLength() == 0)
							continue;

						// figure out what field terminator they're using
						if(m_pCurMsg->GetFieldDelim() == 0)
							{
							sTemp.TrimLeft();
							if(sTemp.GetLength() < 4)
								continue;
							if(sTemp.Left(3) != "MSH")
								continue;
							m_pCurMsg->SetFieldDelim(sTemp[3]);
							if(sTemp.GetLength() <7)
								continue;
							cEscape = sTemp[6];
							}

						// see if this is a line that is continued on the next line
						if(sTemp[sTemp.GetLength() - 1] == cEscape)
							{
							sPrevString += sTemp.Left(sTemp.GetLength() - 2);
							continue;
							}

						switch(pCurMsg->AddSegment(sPrevString + sTemp))
							{
							case CHL7Message::MSG_START_NEW:
								ProcessMessage(pCurMsg);
								delete pCurMsg;
								pCurMsg = new CHL7Message;
								pCurMsg->AddSegment(sTemp);
								break;
							}

						sPrevString.Empty();
						sTemp.Empty();
						}

					// send the last msg
					ProcessMessage(pCurMsg);

					delete pCurMsg;

					m_pCurFile->Close();
					CFile::Remove(sFilename);
					}

				delete m_pCurFile;
				m_pCurFile = NULL;

				GetIO()->FormatOutput(IOMASK_12, "File processing done (%s)", (LPCTSTR) sFilename);
				}

			if(! ::FindNextFile(hand, &findData))
				break;
			}
		}
	CATCH(CFileException, e)
		{
		e->GetErrorMessage(sTemp.GetBuffer(1024), 1024);
		sTemp.ReleaseBuffer();
		GetIO()->FormatOutput(IOMASK_12|IOMASK_12, "File exception: %s", (LPCTSTR) sTemp);
		}
	CATCH_ALL(e)
		{
		e->GetErrorMessage(sTemp.GetBuffer(1024), 1024);
		sTemp.ReleaseBuffer();
		GetIO()->FormatOutput(IOMASK_12|IOMASK_12, "Exception: %s", (LPCTSTR) sTemp);
		}
	END_CATCH_ALL
	}//end hand scope

label_exit:

	CHL7InputDaemon::MainLoop(p1, p2);
	}

//--------------------------------------------------------------------------------
bool CHL7FileDaemon::SendString(LPCTSTR pStr)
	{
	if(m_pAckFile == NULL)
		{
		if(m_pInit->GetOutPath() == NULL)
			return false;

		m_pAckFile = new CStdioFile;
		CString sFilename(m_pInit->GetOutPath());
		if(m_pInit->GetName() != NULL && *m_pInit->GetName() != 0)
			sFilename += "\\" + CString(m_pInit->GetName()) + " ACK.txt";
		else
			{
			CString sTemp;
			sTemp.Format("%ld", ::GetCurrentProcessId());
			sFilename += "\\" + sTemp + " ACK.txt";
			}

		if(! m_pAckFile->Open("\\\\?\\" + sFilename, CFile::modeCreate|CFile::modeWrite|CFile::shareDenyWrite|CFile::typeText))
			return false;
		CString sLine('-', 80);
		CString sTemp(sLine + "\nACK messages for input from: ");
		sTemp += m_pInit->GetPath();
		sTemp += '\n' + sLine + "\n\n";
		m_pAckFile->WriteString(sTemp);
		GetIO()->Output(IOMASK_12|IOMASK_CONST, sTemp);
		}

	CString sTemp;

	// 
	while(*pStr)
		{
		switch(*pStr)
			{
			case '\r':
				sTemp += '\n';
				break;

			case '\n':
				sTemp += *pStr;
				break;

			default:
				if(*pStr >= ' ')
					sTemp += *pStr;
				break;
			}

		pStr++;
		}

	m_pAckFile->WriteString(sTemp + '\n');
	m_pAckFile->Flush();
	return true;
	}


