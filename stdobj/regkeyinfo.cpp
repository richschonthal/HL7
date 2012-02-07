//-------------------------------------------------------------------------------
// Reg Lib
// Classes for reading and writing registry stuff
//
// designed and coded by Rich Schonthal
// Copyright (c) 1997 Rich Schonthal
//
// this code is made public domain by the author
// it may be used in products commercial or otherwise
// as long as all copyright notices remain in tact
//-------------------------------------------------------------------------------
// regkey info

#include "stdafx.h"

//-------------------------------------------------------------------------------
CRegKeyInfo::CRegKeyInfo(const CRegKey& regkey)
   {
   m_pClassName = NULL;
   if(! regkey.IsOpen())
      {
      SetResultCode(errKeyNotOpen);
      return;
      }
   DWORD nSize = 0;

   SetOSResult(::RegQueryInfoKey(regkey.GetKey(), NULL, &nSize, 
      NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL));

   if(GetOSResult() != ERROR_SUCCESS)
      {
      SetResultCode(errRegistry);
      return;
      }
   
   nSize += 1;
   m_pClassName = new TCHAR[nSize];
   SetOSResult(::RegQueryInfoKey(regkey.GetKey(), m_pClassName, &nSize, NULL,
      &m_nNumOfSubKeys, &m_nMaxSubKeyName, &m_nMaxClassName, &m_nNumOfValues,
      &m_nMaxValueName, &m_nMaxValueLen, &m_nSecurityDescLen, &m_ftLastWrite));

   if(GetOSResult() != ERROR_SUCCESS)
      {
      delete m_pClassName;
      m_pClassName = NULL;
      SetResultCode(errRegistry);
      }
   }

//********************************************************************************
CRegKeyInfo::~CRegKeyInfo()
   {
   delete m_pClassName;
   }
