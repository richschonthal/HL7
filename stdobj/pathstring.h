//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// PathString.h: interface for the CPathString class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PATHSTRING_H__3CADFEC1_5CB0_11D3_A6BD_005004A1C5F3__INCLUDED_)
#define AFX_PATHSTRING_H__3CADFEC1_5CB0_11D3_A6BD_005004A1C5F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
#include <tchar.h>
#include <Shlwapi.h>

//--------------------------------------------------------------------------------
class CPathString : public CString  
	{
	public:
		enum ECharTypes
			{
			Invalid = GCT_INVALID,		//The character is not valid in a path.  
			LFNChar = GCT_LFNCHAR,		//The character is valid in a long file name.  
			Seperator = GCT_SEPARATOR,	//The character is a path separator.  
			ShortChar = GCT_SHORTCHAR,	//The character is valid in a short (8.3) file name.  
			Wild = GCT_WILD				//The character is a wildcard character.  
			};

	public:
		CPathString(const CString);
		CPathString(LPCTSTR);

		virtual ~CPathString();

		// Shlwapi encapsulations
		bool Append(LPCTSTR pMore);
		int CommonPrefix(LPCTSTR pFile, CPathString& sDest);
		static ECharTypes GetCharType(_TUCHAR ch);
		bool IsContentType(LPCTSTR);
		bool IsHTMLFile();
		bool MakePretty();
		bool MatchSpec(LPCTSTR pSpec);
		void QuoteSpaces();
		bool RelativePathTo(LPCTSTR pFrom, DWORD nAttrFrom, LPCTSTR pTo, DWORD nAttrTo);
		void RemoveArgs();
		void RemoveBlanks();
		void RemoveExtension();
		void RemoveFileSpec();
		bool RenameExtension(LPCTSTR pExt);
		void UnquoteSpaces();

		// CPathString specific
		CString GetExtension();
	};

#endif // !defined(AFX_PATHSTRING_H__3CADFEC1_5CB0_11D3_A6BD_005004A1C5F3__INCLUDED_)
