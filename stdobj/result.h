//********************************************************************************
// Result class
// For reporting results and erros
//
// designed and coded by Rich Schonthal
// Copyright (c) 1997 Rich Schonthal
//
// this code is made public domain by the author
// it may be used in products commercial or otherwise
// as long as all copyright notices remain in tact
//********************************************************************************

#ifndef _RESULT_H_
#define _RESULT_H_

#include <windef.h>
#include "allocstring.h"
#include "commondefines.h"

#define BEGIN_ERROR_RANGE -MAXLONG
#define END_ERROR_RANGE -1

#define IN_RESULT_RANGE(r)	(r <= END_ERROR_RANGE && r >= BEGIN_ERROR_RANGE)

#define EMPTY_RESULTMAP(classname, mapname) \
const LONG classname::mapname[1][2] = {0,0}; \
const int classname::m_nMapLength = 0; 

#define BEGIN_RESULTMAP(classname, mapname) \
const LONG classname::mapname[][2] = {

#define END_RESULTMAP(classname, mapname) \
}; const int classname::m_nMapLength = sizeof(classname::mapname)/sizeof(LONG)/2; 

#define DECLARE_RESULTMAP(mapname) \
private: \
static const int m_nMapLength; \
static const LONG mapname[][2]; public: \
virtual int MapMessage() const \
{ if(! IN_RESULT_RANGE(GetResultCode()) ) return -1; \
for(int i = 0; i<m_nMapLength;i++) \
if(mapname[i][0] == GetResultCode()) return mapname[i][1]; \
return -1; }

//************************************************************************************
// some macros to get around limitations on inheritance
// to ease declarations of classes derived from CResult
// do my bit to prevent carpel tunnel...
#define DECLARE_RESULTCLASS(classname) \
	classname(LONG r = noErr, LONG wr = 0)		{ m_nResult = r; m_nOSResult = wr; m_pErrMsg = NULL;} \
 \
	classname operator=(classname& r) \
		{ SetResultCode(r.GetResultCode()); SetOSResult(r.GetOSResult()); return *this; } \
 \
	BOOL operator==(classname& r) \
		{ return GetResultCode() == r.GetResultCode() && GetOSResult() == r.GetOSResult(); } \
 \
	BOOL operator!=(classname& r) \
		{ return GetResultCode() != r.GetResultCode() && GetOSResult() != r.GetOSResult(); } \
	protected: \
	void SetResult(classname& r) \
		{ *this = r; } \
	public: \
		void SetResult(LONG r, LONG or) { m_nResult = r; m_nOSResult = or; } \
	public:

//************************************************************************************
class CResult
	{
	public:
	enum
		{
		noErr,
		// all values between here and StartErrorRange should indicate an OS error
		// you can declare values to indicate which part of the os returned an error
		// when declaring OS error indicators, start with errOS + 1
		errOS,
		// result codes in this range specify that an error code is in the os result

		// a value > StartErrorRange is an error
		BeginErrorRange = BEGIN_ERROR_RANGE,
		
		// result codes in this range are for application defined results

		// in derived class use HighestResultCode on your first enum result code
		HighestResultCode = BEGIN_ERROR_RANGE + (MAXLONG/2)
		};

	protected:
		LONG m_nResult;
		LONG m_nOSResult;
		LPTSTR m_pErrMsg;

	public:
		DECLARE_RESULTCLASS(CResult)
		virtual ~CResult() { delete[] m_pErrMsg; }

		void ClearResult()							{ m_nResult = 0; m_nOSResult = 0; }
		inline LONG GetResultCode() const			{ return m_nResult; }
		void SetResultCode(LONG r)					{ m_nResult = r; }

		inline LONG GetOSResult() const				{ return m_nOSResult; }
		void SetOSResult(LONG r)					{ m_nOSResult = r; SetErrorMessage(NULL); }

		void SetErrorMessage(LPCTSTR p)				{ ALLOC_STRING(m_pErrMsg, p); }

		// use this function when you wish to check os results and application defined
		// results together.  ie app results will be negative and windows result will be positive
		// note: on the macintosh, os results are negative so app results should be positive
		// to be truly cross-platform in reporting results you should have a function to map
		// results to message strings or id's
		LONG GetError() const
			{ return IN_RESULT_RANGE(m_nResult) ? m_nResult : m_nOSResult; }

		// should return a string resource id for a message corrisponding to
		// GetResultCode()
		virtual int MapMessage() const		{ return -1; }

		// obviously this function may have a moderate memory overhead
		void GetErrorMessage(CString& sOut) const
			{
			if(m_pErrMsg != NULL)
				{
				sOut = m_pErrMsg;
				return;
				}

			if(IN_RESULT_RANGE(GetResultCode()))
				{
				int id = MapMessage();
				if(id == -1)
					return;
				sOut.LoadString(id);
				return;
				}

			if(GetOSResult() == noErr)
				return;

			LPVOID lpMsgBuf;

			FormatMessage( 
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				NULL,
				GetOSResult(),
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				(LPTSTR) &lpMsgBuf,
				0,
				NULL 
			);

			sOut = (LPTSTR) lpMsgBuf;
			LocalFree( lpMsgBuf );
			return;
			}
 

		BOOL HasErrors() const						{ return GetResultCode() || GetOSResult(); }
	};

#endif // _RESULT_H_