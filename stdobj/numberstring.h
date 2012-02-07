/*=========================================================================
   Copyright (c) 1994, Donovan Data Systems, Inc. All rights reserved.
   This file is proprietary and confidential to Donovan Data Systems, Inc.
   It may not be disclosed, copied, duplicated, or used without express
   written permission of Donovan Data Systems, Inc.

      File.......: localmoney.h
      Author.....: rschddny
      Created....: 17-Jun-97
         
      Project....: Tempo
      Description: header file for numberstring.cpp

=========================================================================*/

#ifndef _NUMBERSTRING_H_
#define _NUMBERSTRING_H_

class CNumberString
   {
   public:
      LPTSTR m_pString;

      void StoreString(char*, int, int);

   protected:
      void StoreFloat(double, int nPrecision = -1);
      void StoreLong(ULONG, BOOL);

   public:
      CNumberString(double, int nPrecision = -1);
      CNumberString(ULONG, BOOL);
      ~CNumberString();

      operator LPCTSTR() const      { return m_pString; }
   };

#endif // _NUMBERSTRING_H_