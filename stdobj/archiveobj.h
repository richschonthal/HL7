//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _ARCHIVEOBJ_H_
#define _ARCHIVEOBJ_H_

// when using this macro, the class you use it in must be
// serializable and must be derived from a CResult


#define DECLARE_ARCHIVE_OBJECT public:\
	inline void Store(CFile* pOut) { \
		AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
		TRY { \
		CArchive outArch(pOut, CArchive::store); \
		Serialize(outArch); \
		outArch.Close(); \
		pOut->Flush();\
		} CATCH_ALL(e) \
		{ SetResultCode(CResult::errOS); SetOSResult(GetLastError()); } \
		END_CATCH_ALL \
		} \
	inline void Retrieve(CFile* pOut) { \
		AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
		TRY { \
		CArchive outArch(pOut, CArchive::load); \
		Serialize(outArch); \
		outArch.Close(); \
		} CATCH_ALL(e) \
		{ SetResultCode(CResult::errOS); SetOSResult(GetLastError()); } \
		END_CATCH_ALL \
		}


#endif // _ARCHIVEOBJ_H_
