//--------------------------------------------------------------------------------
//
// Copyright (c) 2001 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _DATESTRING_H_
#define _DATESTRING_H_

//--------------------------------------------------------------------------------
class CDateString
	{
	private:
		LPTSTR m_pString;

	public:
		int m_nMonth;
		int m_nDay;
		int m_nYear;
		bool m_bParsed;

	public:
		CDateString(LPCTSTR = NULL);
		~CDateString();

		void ParseString(LPCTSTR = NULL);
		static bool ParseString(LPCTSTR, int& nMon, int& nDay, int& nYear, int nDateOrder = -1, LPCTSTR pSeperators = NULL);

		// returns strcmp-type values
		int Compare(const CDateString&);
	};

#endif
