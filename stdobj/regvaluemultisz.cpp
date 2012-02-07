//********************************************************************************
// Reg Lib
// Classes for reading and writing registry stuff
//
// designed and coded by Rich Schonthal
// Copyright (c) 1997 Rich Schonthal
//
// this code is made public domain by the author
// it may be used in products commercial or otherwise
// as long as all copyright notices remain in tact
//********************************************************************************
// CRegValueMultiSz

#include "stdafx.h"

//********************************************************************************
BOOL CRegValueMultiSz::ReadValue(CStringArray& sArray, REGSAM regsam)
   {
   BYTE* pBuffer;

   if(! CRegValue::ReadValue(&pBuffer, regsam))
      return FALSE;

   CString sTemp;
   int nInsertPt = sArray.GetSize();
   BYTE* pBase = pBuffer;

   for(int i = 0; *pBuffer ; i++)
      {
      sTemp += (TCHAR) *pBuffer++;
      if(! *pBuffer)
         {
         sArray.SetAtGrow(nInsertPt++, sTemp);
         sTemp.Empty();
         pBuffer++;
         }
      }

   delete pBase;
   return TRUE;
   }

//********************************************************************************
BOOL CRegValueMultiSz::WriteValue(const CStringArray& sArray)
   {
   int nSize = 0;

   // lets see how big this sucker is
   for(int i = 0; i < sArray.GetSize(); i++)
      nSize += sArray[i].GetLength() + 1;

   // add 1 for the final null
   nSize += 1;

   TCHAR* pBuffer = new TCHAR[nSize];
   TCHAR* pMultiSz = (TCHAR*) pBuffer;

   for(i = 0; i < sArray.GetSize(); i++)
      {
      _tcscpy(pMultiSz, sArray[i]);
      pMultiSz += sArray[i].GetLength() + 1;
      }

   *pMultiSz = 0;
   
   CRegValue::WriteValue((BYTE*) pBuffer, nSize);

   delete pBuffer;

   return GetOSResult() == ERROR_SUCCESS;
   }

