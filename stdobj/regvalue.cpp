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

#include "stdafx.h"


//********************************************************************************
void CRegValue::SetValueName(LPCTSTR pName)
   {
   if(pName == NULL)
      return;

   ASSERT(pName != m_pValueName);
   if(m_pValueName != NULL)
      delete m_pValueName;
   m_pValueName = new TCHAR[_tcslen(pName) + sizeof(TCHAR)];
   _tcscpy(m_pValueName, pName);
   }

//********************************************************************************
BOOL CRegValue::ReadValue(LPBYTE* pValue, const REGSAM samDesired)
   {
   DWORD nSize;
   DWORD nType;

   SetOSResult(::RegQueryValueEx(GetKey(), GetValueName(), 0, &nType, NULL, &nSize));

   if(GetOSResult() != ERROR_SUCCESS)
      return FALSE;

   if(GetValueType() > -1 && nType != GetValueType())
      {
      SetResultCode(errValueTypeMismatch);
      return FALSE;
      }

   *pValue = new BYTE[nSize];

   SetOSResult(::RegQueryValueEx(GetKey(), GetValueName(), 0, &m_nType, *pValue, &nSize));

   if(GetOSResult() != ERROR_SUCCESS)
      {
      delete *pValue;
      return FALSE;
      }
   return TRUE;
   }

//********************************************************************************
BOOL CRegValue::ReadValue(CString& sValue, const REGSAM samDesired)
   {
   DWORD nSize;
   DWORD nType;

   SetOSResult(::RegQueryValueEx(GetKey(), GetValueName(), 0, &nType, NULL, &nSize));

   if(GetOSResult() != ERROR_SUCCESS)
      return FALSE;

   if(GetValueType() > -1 && nType != GetValueType())
      {
      SetResultCode(errValueTypeMismatch);
      return FALSE;
      }

   SetOSResult(::RegQueryValueEx(GetKey(), GetValueName(), 0, &m_nType,
      (UCHAR*) sValue.GetBuffer(nSize), &nSize));

   sValue.ReleaseBuffer();

   return GetOSResult() == ERROR_SUCCESS;
   }

//********************************************************************************
BOOL CRegValue::ReadValue(LPDWORD pValue, REGSAM samDesired)
   {
   DWORD nSize;
   DWORD nType;

   SetOSResult(::RegQueryValueEx(GetKey(), GetValueName(), 0, &nType, NULL, &nSize));

   if(GetOSResult() != ERROR_SUCCESS)
      return FALSE;

   if(GetValueType() > -1 && nSize != sizeof(DWORD))
      {
      SetResultCode(errValueTypeMismatch);
      return FALSE;
      }

   SetOSResult(::RegQueryValueEx(GetKey(), GetValueName(), 0, &nType, (UCHAR*) pValue, &nSize));

   return GetOSResult() == ERROR_SUCCESS;
   }

//********************************************************************************
BOOL CRegValue::WriteValue(const LPBYTE pValue, DWORD nSize)
   {
   if(GetValueType() == -1)
      {
      SetResultCode(errUninitializedValueType);
      return FALSE;
      }

   SetOSResult(::RegSetValueEx(GetKey(), GetValueName(), 0, GetValueType(), (UCHAR*) pValue, nSize));

   return GetOSResult() == ERROR_SUCCESS;
   }

//********************************************************************************
BOOL CRegValue::WriteValue(const CString& sValue)
   {
   DWORD nSize = sValue.GetLength();
   return WriteValue((LPBYTE) (LPCTSTR) sValue, (nSize * sizeof(TCHAR)) + sizeof(TCHAR));
   }

//********************************************************************************
BOOL CRegValue::WriteValue(DWORD nValue)
   {
   return WriteValue((const LPBYTE) &nValue, sizeof(nValue));
   }

//********************************************************************************
BOOL CRegValue::DeleteValue()
   {
   SetOSResult(::RegDeleteValue(GetKey(), GetValueName()));

   return GetOSResult() == ERROR_SUCCESS;
   }

//********************************************************************************
CRegValue CRegValue::operator=(const CRegValue& rv)
   {
   SetValueName(rv.GetValueName());
   (CRegKey) *this = (CRegKey) rv;

   return *this;
   }

//********************************************************************************
BOOL CRegValue::IterateEnumValue()
   {
   if(m_nEnum < 0)
      return FALSE;

   TCHAR sTemp[1024];
   DWORD nSize = sizeof(sTemp);

   SetOSResult(::RegEnumValue(GetKey(), m_nEnum, sTemp, &nSize,
      NULL, &m_nType, NULL, NULL));

   SetValueName(sTemp);

   m_nEnum += m_nEnumDirection;

   return GetOSResult() == ERROR_SUCCESS;
   }
