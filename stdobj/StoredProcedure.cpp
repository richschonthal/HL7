//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "StoredProcedure.h"
#include "PACSDatabase.h"

#include "allocstring.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//--------------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CStoredProcedure, CRecordset)

//--------------------------------------------------------------------------------
CStoredProcedure::CStoredProcedure(CDatabase* pdb, LPCTSTR pProcedureName)
		: CRecordset(pdb)
	{
	//{{AFX_FIELD_INIT(CStoredProcedure)
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
	m_pProcName = NULL;
	ALLOC_STRING(m_pProcName, pProcedureName);
	}

//--------------------------------------------------------------------------------
CStoredProcedure::~CStoredProcedure()
	{
	delete[] m_pProcName;
	}

//--------------------------------------------------------------------------------
CString CStoredProcedure::GetDefaultConnect()
	{
	return _T("");
	}

//--------------------------------------------------------------------------------
void CStoredProcedure::DoFieldExchange(CFieldExchange* pFX)
	{
	//{{AFX_FIELD_MAP(CStoredProcedure)
	//}}AFX_FIELD_MAP
	}

//--------------------------------------------------------------------------------
CString CStoredProcedure::GetDefaultSQL()
	{
	ASSERT(m_pProcName != NULL);

	CString sParam;
	for(UINT i = 0; i < m_nParams; i++)
		{
		sParam += "?, ";
		}

	if(m_nParams > 0)
		// lose the last comma space
		sParam = sParam.Left(sParam.GetLength() - 2);

	CString sTemp;
	sTemp.Format("{call %s%s(%s)}",
		(LPCTSTR) CPACSDatabase::GetProcAccountName(),
		m_pProcName,
		(LPCTSTR) sParam);

	return sTemp;
	}

// copied from CRecordset - overridden to eliminate the Invalid Cursor error
// thats associated with calling a stored procedure that has no output (columns)
// followed by calling MoveNext
//--------------------------------------------------------------------------------
BOOL CStoredProcedure::Open(UINT nOpenType, LPCTSTR lpszSQL, DWORD dwOptions)
	{
	ASSERT(!IsOpen());
	ASSERT_VALID(this);
	ASSERT(lpszSQL == NULL || AfxIsValidString(lpszSQL));
	ASSERT(nOpenType == AFX_DB_USE_DEFAULT_TYPE ||
		nOpenType == dynaset || nOpenType == snapshot ||
		nOpenType == forwardOnly || nOpenType == dynamic);
	ASSERT(!(dwOptions & readOnly && dwOptions & appendOnly));

	// Can only use optimizeBulkAdd with appendOnly recordsets
	ASSERT((dwOptions & optimizeBulkAdd && dwOptions & appendOnly) ||
		!(dwOptions & optimizeBulkAdd));

	// forwardOnly recordsets have limited functionality
	ASSERT(!(nOpenType == forwardOnly && dwOptions & skipDeletedRecords));

	// Cache state info and allocate hstmt
	SetState(nOpenType, lpszSQL, dwOptions);
	if(!AllocHstmt())
		return FALSE;

	// Check if bookmarks upported (CanBookmark depends on open DB)
	ASSERT(dwOptions & useBookmarks ? CanBookmark() : TRUE);

	//TRY
		{
		OnSetOptions(m_hstmt);

		// Allocate the field/param status arrays, if necessary
		BOOL bUnbound = FALSE;
		if (m_nFields > 0 || m_nParams > 0)
			AllocStatusArrays();
		else
			bUnbound = TRUE;

		// Build SQL and prep/execute or just execute direct
		BuildSQL(lpszSQL);
		PrepareAndExecute();

		// Cache some field info and prepare the rowset
		AllocAndCacheFieldInfo();
		AllocRowset();

		// If late binding, still need to allocate status arrays
		if (bUnbound && (m_nFields > 0 || m_nParams > 0))
			AllocStatusArrays();

		// Give derived classes a call before binding
		PreBindFields();

		// Fetch the first row of data
		//MoveNext();

		// If EOF, then result set empty, so set BOF as well
		m_bBOF = m_bEOF;
		}

	return TRUE;
	}

#ifdef _DEBUG
//--------------------------------------------------------------------------------
void CStoredProcedure::AssertValid() const
	{
	CRecordset::AssertValid();
	}

//--------------------------------------------------------------------------------
void CStoredProcedure::Dump(CDumpContext& dc) const
	{
	CRecordset::Dump(dc);
	}
#endif //_DEBUG
