//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// ImageQuality.h: interface for the CImageQuality class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEQUALITY_H__C0FF1087_3E24_11D3_87CF_00104B9E6286__INCLUDED_)
#define AFX_IMAGEQUALITY_H__C0FF1087_3E24_11D3_87CF_00104B9E6286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <allocstring.h>

//--------------------------------------------------------------------------------
class CImageQualityInfo
	{
	public:
		LPTSTR m_pManufacturer;
		LPTSTR m_pModality;
		LPTSTR m_pModel;
		int m_nQuality;

		CImageQualityInfo(LPCTSTR man, LPCTSTR model, LPCTSTR modality, int qual)
				: m_pManufacturer(NULL)
				, m_pModality(NULL)
				, m_pModel(NULL)
			{
			ALLOC_STRING(m_pManufacturer, man);
			ALLOC_STRING(m_pModality, modality);
			ALLOC_STRING(m_pModel, model);
			m_nQuality = qual;
			}

		~CImageQualityInfo()
			{
			delete[] m_pManufacturer;
			delete[] m_pModality;
			delete[] m_pModel;
			}
	};

//--------------------------------------------------------------------------------
class CImageQualityInfoArray : public CTypedPtrArray<CPtrArray, CImageQualityInfo*>
	{
	public:
		~CImageQualityInfoArray()
			{
			for(int i = 0; i < GetSize(); i++)
				delete GetAt(i);
			RemoveAll();
			};
	};

//--------------------------------------------------------------------------------
class CImageQuality
	{
	private:
		static CMutex m_mutex;
		static CImageQualityInfoArray* m_pList;

	public:
		CImageQuality(LPCTSTR = NULL);
		virtual ~CImageQuality();

	public:
		int Find(LPCTSTR manu, LPCTSTR model, LPCTSTR modality);
	};

#endif // !defined(AFX_IMAGEQUALITY_H__C0FF1087_3E24_11D3_87CF_00104B9E6286__INCLUDED_)
