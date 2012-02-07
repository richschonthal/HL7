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

#ifndef _LOCALNUMBER_H_
#define _LOCALNUMBER_H_

class CNumberString;

class CLocalNumber
   {
   public:
      enum SignFlag
         {
         UNSIGNED,
         SIGNED
         };

   private:
      DWORD m_nLocale;
      LPTSTR m_pNumberString;

   public:
      CLocalNumber(DWORD = LOCALE_USER_DEFAULT);
      CLocalNumber(DWORD, double, int = 4);
      CLocalNumber(DWORD, ULONG, SignFlag = SIGNED);

      void Store(const CNumberString&, int nPrecision = -1);
      void StoreFloat(double, int = 4);
      void StoreLong(ULONG, SignFlag = SIGNED);

      ~CLocalNumber();

      DWORD GetLocale()       { return m_nLocale; }

      operator LPCTSTR() const      { return m_pNumberString; }
   };

#endif // _LOCALNUMBER_H_