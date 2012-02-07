//************************************************************************************
//
// general-purpose MFC-based classes
//
// Copyright (c) 1997 Rich Schonthal
//
//
//************************************************************************************
// module: common definitions

#ifndef _COMMONDEFINES_H_
#define _COMMONDEFINES_H_

#ifdef NO_DLL
#define DllExport
#define DllImport
#else
#define DllExport	__declspec( dllexport )
#define DllImport	__declspec( dllimport )
#endif

#ifdef _WINDLL
#define DllSpec DllExport
#else
#define DllSpec DllImport
#endif

#endif // _COMMONDEFINES_H_
