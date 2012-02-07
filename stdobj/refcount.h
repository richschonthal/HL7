//********************************************************************************
// Ref count class
// Class for implementing reference counted classes
//
// designed and coded by Rich Schonthal
// Copyright (c) 1997 Rich Schonthal
//
// this code is made public domain by the author
// it may be used in products commercial or otherwise
// as long as all copyright notices remain in tact
//********************************************************************************

#ifndef _REFCOUNT_H_
#define _REFCOUNT_H_

//--------------------------------------------------------------------------------
class CRefCountObj
   {
   public:
      LONG m_nRefCount;

   public:
      CRefCountObj();
      virtual ~CRefCountObj();
   };

//--------------------------------------------------------------------------------
inline CRefCountObj::CRefCountObj() : m_nRefCount(0) {}
inline CRefCountObj::~CRefCountObj() { ASSERT(m_nRefCount == 0); }

//--------------------------------------------------------------------------------
template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
class CRefCountMap : public CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>
	{
	private:
		VALUE& operator[](ARG_KEY key);

	public:
		CRefCountMap(int nBlockSize = 10);
		void SetAt(ARG_KEY key, ARG_VALUE newValue);
	};

//--------------------------------------------------------------------------------
template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
AFX_INLINE void CRefCountMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::SetAt(ARG_KEY key, ARG_VALUE newValue)
	{ newValue->m_nRefCount++; CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::SetAt(key, newValue); }

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
AFX_INLINE CRefCountMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::CRefCountMap(int n)
	: CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>(n) {}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
template<class TYPE, class ARG_TYPE>
class CRefCountList : public CList<TYPE, ARG_TYPE>
	{
	public:
		CRefCountList(int nBlockSize = 10);
		void SetAt(POSITION pos, ARG_TYPE newElement);
	};

//--------------------------------------------------------------------------------
template<class TYPE, class ARG_TYPE>
AFX_INLINE CRefCountList<TYPE, ARG_TYPE>::CRefCountList(int n) : CList<TYPE, ARG_TYPE>(n) {}

template<class TYPE, class ARG_TYPE>
AFX_INLINE void CRefCountList<TYPE, ARG_TYPE>::SetAt(POSITION pos, ARG_TYPE newElement)
	{ newElement->m_nRefCount++; CList<TYPE, ARG_TYPE>::SetAt(pos, newElement); }

//--------------------------------------------------------------------------------
template<class TYPE, class ARG_TYPE>
class CRefCountArray : public CArray<TYPE, ARG_TYPE>
{
public:
	void SetAt(int nIndex, ARG_TYPE newElement);

	// Potentially growing the array
	void SetAtGrow(int nIndex, ARG_TYPE newElement);
	int Add(ARG_TYPE newElement);
	int Append(const CArray& src);
	void Copy(const CArray& src);

	void InsertAt(int nIndex, ARG_TYPE newElement, int nCount = 1);
	void InsertAt(int nStartIndex, CArray* pNewArray);
};

//--------------------------------------------------------------------------------
template<class TYPE, class ARG_TYPE>
AFX_INLINE void CRefCountArray<TYPE, ARG_TYPE>::SetAt(int nIndex, ARG_TYPE newElement)
	{ newElement->m_nRefCount++; CArray<TYPE, ARG_TYPE>::SetAt(nIndex, newElement); }
template<class TYPE, class ARG_TYPE>
AFX_INLINE int CRefCountArray<TYPE, ARG_TYPE>::Add(ARG_TYPE newElement)
	{ newElement->m_nRefCount++; CArray<TYPE, ARG_TYPE>::Add(newElement); }

#endif // _REFCOUNT_H_
