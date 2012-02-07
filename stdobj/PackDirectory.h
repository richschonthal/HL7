//--------------------------------------------------------------------------------
//
// Copyright (c) 2001 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _PACKDIRECTORY_H_
#define _PACKDIRECTORY_H_

//--------------------------------------------------------------------------------
class CPackDirectory
	{
	private:
		class Entry
			{
			public:
				enum
					{
					TypeFile, TypeBeginDir, TypeEndDir
					};

			public:
				// if type = 0, then name = filename, len = len of file
				// if type = 1, then name = dirname, len = 0
				// means begining of sub dir
				// if type = 2 then name = null, len = 0
				// means end of sub dir
				BYTE m_nType;
				DWORD m_nLen;
				DWORD m_nAttr;
				FILETIME m_ftCreate;
				FILETIME m_ftAccess;
				FILETIME m_ftWrite;
				LPTSTR m_pName;

			public:
				Entry() : m_nType(0), m_pName(NULL), m_nLen(0) {};
				~Entry()
					{delete[] m_pName;}

				void Write(CFile&, CFile* = NULL);
				bool Read(CFile&);
			};

		// theoretical file structure
		// Entry m_data[];

	public:
		static bool Pack(LPCTSTR, LPCTSTR, bool bRecursive = true);
		static bool Unpack(LPCTSTR, LPCTSTR, bool bOverwrite = false);

		static bool Pack(CFile&, LPCTSTR, bool bRecursive = true);
		static bool Unpack(CFile&, LPCTSTR, bool bOverwrite = false);
	};

#endif //_PackDirectory_H_
