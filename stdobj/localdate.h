/*=========================================================================
   Copyright (c) 1994, Donovan Data Systems, Inc. All rights reserved.
   This file is proprietary and confidential to Donovan Data Systems, Inc.
   It may not be disclosed, copied, duplicated, or used without express
   written permission of Donovan Data Systems, Inc.

      File.......: localnumber.h
      Author.....: rschddny
      Created....: 17-Jun-97
         
      Project....: Tempo
      Description: header file for localnumber.cpp

=========================================================================*/

#ifndef _LOCALDATE_H_
#define _LOCALDATE_H_

#include "windef.h"

#include "result.h"

//*********************************************************************************
class CLocalDateResult : public CResult
   {
   public:
      enum
         {
         errInvalidMonth = HighestResultCode,
         errInvalidDay,
         errInvalidYear,
         errInvalidDateString,
         errUnsupportedSystemDateFormat
         };

      DECLARE_RESULTCLASS(CLocalDateResult)
   };

//*********************************************************************************
class CLocalShortDateFormat : public CLocalDateResult
   {
   private:
      LPTSTR m_pFormat;
      LPTSTR m_pMonFormat;
      LPTSTR m_pDayFormat;
      LPTSTR m_pYearFormat;
      CHAR m_cSeperator;

      short m_nMonPos;
      short m_nDayPos;
      short m_nYearPos;

      BOOL ParseDateFormat(LPCTSTR);

   public:
      CLocalShortDateFormat();
      CLocalShortDateFormat(DWORD);
      CLocalShortDateFormat(LPCTSTR, CHAR);
      ~CLocalShortDateFormat();

      LPTSTR GetFullFormat() const     { return m_pFormat; }
      LPCTSTR GetMonthFormat() const   { return m_pMonFormat; }
      LPCTSTR GetDayFormat() const     { return m_pDayFormat; }
      LPCTSTR GetYearFormat() const    { return m_pYearFormat; }
      CHAR GetSeperator() const        { return m_cSeperator; }
                                       
      short GetMonPosition() const     { return m_nMonPos; }
      short GetDayPosition() const     { return m_nDayPos; }
      short GetYearPosition() const    { return m_nYearPos; }
   };

//*********************************************************************************
class CLocalDate : public CLocalDateResult
   {
   private:
      DWORD m_nLocale;
      LPTSTR m_pString;
   
   protected:
      void BuildDateString(DWORD, LPSYSTEMTIME, LPCTSTR pFormat, BOOL shortdate);

   public:
      CLocalDate()            { m_pString = NULL; m_nLocale = LOCALE_USER_DEFAULT; }
      CLocalDate(DWORD loc)   { m_pString = NULL; m_nLocale = loc; }
      CLocalDate(DWORD loc, LPSYSTEMTIME st, BOOL shortdate = TRUE)
         { BuildDateString(loc, st, NULL, shortdate); }

      CLocalDate(DWORD loc, LPSYSTEMTIME st, LPCTSTR fmt, BOOL shortdate = TRUE)
         { BuildDateString(loc, st, fmt, shortdate); }

      CLocalDate(DWORD loc, int, int, int, BOOL shortdate = TRUE);

      ~CLocalDate()           { if(m_pString) delete m_pString; }

      DWORD GetLocale()       { return m_nLocale; }
      operator LPCTSTR()      { return m_pString; }

      BOOL SetDateFromString(DWORD, LPCTSTR);
      int GetShortDatePart(const CLocalShortDateFormat*, LPCTSTR, int);

      CLocalDate operator=(LPCTSTR);

      // date helpers
      // none of the leap year calculations take into account the century divisible by 400 rule
      // therefore these functions are only value for 1904 - 2096
      // i hope they're still useful to you ;->
      static inline BOOL IsLeapYear(int nYear)   { return (nYear % 4) == 0; }
      static inline int NextLeapYear(int nYear)  { return nYear + 4 - (nYear % 4); }
      static inline int PrevLeapYear(int nYear)  { return IsLeapYear(nYear) ? nYear - 4 : nYear - (nYear % 4) ; }
      // # of leap years between Y1 and Y2 inclusivly
      static inline int LeapYearsInSpan(int nY1, int nY2)
         { return ((PrevLeapYear(nY2) - PrevLeapYear(nY1)) / 4) + IsLeapYear(nY2); }
   };

#endif // _LOCALDATE_H_