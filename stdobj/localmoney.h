/*=========================================================================
   Copyright (c) 1994, Donovan Data Systems, Inc. All rights reserved.
   This file is proprietary and confidential to Donovan Data Systems, Inc.
   It may not be disclosed, copied, duplicated, or used without express
   written permission of Donovan Data Systems, Inc.

      File.......: localmoney.h
      Author.....: rschddny
      Created....: 17-Jun-97
         
      Project....: Tempo
      Description: header file for localmoney.cpp

=========================================================================*/

#ifndef _LOCALMONEY_H_
#define _LOCALMONEY_H_

class CLocalMoney
   {
   private:
      DWORD m_nLocale;
      LPTSTR m_pMoneyString;

   public:
      CLocalMoney(DWORD);
      CLocalMoney(DWORD, double);
      ~CLocalMoney();

      void StoreMoney(double);

      DWORD GetLocale()       { return m_nLocale; }

      operator LPCTSTR()      { return m_pMoneyString; }

      CLocalMoney& operator=(double m)    { StoreMoney(m); return *this; }
   };

#endif // _LOCALMONEY_H_