// MCTempFile.h: interface for the CMCTempFile class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _MCTEMPFILE_H_
#define _MCTEMPFILE_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <afxtempl.h>

class CMCTempFile : public CObject  
{
public:
	CMCTempFile();
	virtual ~CMCTempFile();
public:
	// Set to true if delete all files upon object destroyed.
	bool	m_bAutoDelete;	
public:	
	int		DeleteFiles();	
	int		GetCount() const;	
	int		AddFile(LPCTSTR lpszFileName,bool bDelete=false);
	void	AutoDelete(bool bAutoDeleteOn=true);
private:
	CStringArray		m_ArrayFileNames;
	CArray <bool,bool>	m_ArrayDelete;
};

#endif // #ifndef _MCTEMPFILE_H_
