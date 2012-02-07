/*=========================================================================
   Copyright (c) 1994, Donovan Data Systems, Inc. All rights reserved.
   This file is proprietary and confidential to Donovan Data Systems, Inc.
   It may not be disclosed, copied, duplicated, or used without express
   written permission of Donovan Data Systems, Inc.

      File.......: localnumberinfo.cpp
      Author.....: rschddny
      Created....: 17-Jun-97
         
      Project....: Tempo
      Description: class to format numbers into localized displayable strings

=========================================================================*/

#ifndef _LOCALNUMBERINFO_H_
#define _LOCALNUMBERINFO_H_

//***********************************************************************
class CLocalNumberFormatInfo
   {
   private:
      NUMBERFMT m_format;

   public:
      CLocalNumberFormatInfo(DWORD);
      ~CLocalNumberFormatInfo();

      UINT GetNumDigits() const           { return m_format.NumDigits; }
      UINT GetLeadingZero() const         { return m_format.LeadingZero; }
      UINT GetGrouping() const            { return m_format.Grouping; }
      LPCTSTR GetDecimalSep() const       { return m_format.lpDecimalSep; }
      LPCTSTR GetThousandSep() const      { return m_format.lpThousandSep; }
      UINT GetNegativeOrder() const       { return m_format.NegativeOrder; }
      const NUMBERFMT* GetNumberFormat()  { return &m_format; }

      void SetNumDigits(UINT n)           { m_format.NumDigits = n; }
      void SetLeadingZero(UINT n)         { m_format.LeadingZero = n; }
      void SetGrouping(UINT n)            { m_format.Grouping = n; }
      void SetDecimalSep(LPCTSTR);        
      void SetThousandSep(LPCTSTR);       
      void SetNegativeOrder(UINT n)       { m_format.NegativeOrder = n; }

      BOOL IsValid()
         { return m_format.lpDecimalSep != NULL && m_format.lpThousandSep != NULL; }
   };

#endif