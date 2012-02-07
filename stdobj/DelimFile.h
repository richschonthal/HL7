//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// DelimFile.h: interface for the CDelimFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DelimFile_H__B5AA2891_355B_11D3_87CA_00104B9E6286__INCLUDED_)
#define AFX_DelimFile_H__B5AA2891_355B_11D3_87CA_00104B9E6286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
class CDelimFile : public CStdioFile
	{
	protected:
		char m_cFieldDelim;
		char m_cComment;
		bool m_bStripQuotes;
		int m_nCurLine;

	public:
		CDelimFile(char cDelim = '\t', char cComment = ';', bool bStrip = true);

		CDelimFile(FILE* pOpenStream) :
			CStdioFile(pOpenStream) {};

		CDelimFile(LPCTSTR lpszFileName, UINT nOpenFlags)
			: CStdioFile(lpszFileName, nOpenFlags) {};

		virtual ~CDelimFile();

	public:
		static void ParseLine(char, char, bool, LPCTSTR, CStringArray&);
		void ParseLine(LPCTSTR, CStringArray&);

		bool ReadLine(CStringArray&);

		char GetDelim()	{ return m_cFieldDelim; }
		void SetDelim(char c) { m_cFieldDelim = c; }

		char GetComment() { return m_cComment; }
		void SetComment(char c) {m_cComment =c;}

		bool GetStripQuotes()	{ return m_bStripQuotes; }
		void SetStripQuotes(bool b) { m_bStripQuotes = b; }

		void SetParams(char cDelim = '\t', char cComment = ';', bool bStrip = true);
	};

#endif // !defined(AFX_DelimFile_H__B5AA2891_355B_11D3_87CA_00104B9E6286__INCLUDED_)
