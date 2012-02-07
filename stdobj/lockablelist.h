//************************************************************************************
//
// general-purpose MFC-based classes
//
// Copyright (c) 1997 Rich Schonthal
//
//
//************************************************************************************
// module: a lockable coblist

#ifndef _LOCKABLELIST_H_
#define _LOCKABLELIST_H_

#include <afxmt.h>
#include <afxtempl.h>

#include "lockableobject.h"

template<class TYPE>
class TMutexObList : public CTypedPtrList<CObList, TYPE>, public CMutexBase
   {
   };

template<class TYPE>
class TMutexObArray : public CTypedPtrArray<CObArray, TYPE>, public CMutexBase
   {
   };

#endif //_LOCKABLELIST_H_