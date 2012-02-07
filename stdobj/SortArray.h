//-------------------------------------------------------------------------------
// QuickSort'able ptr array template
//
//-------------------------------------------------------------------------------


#ifndef _SORTARRAY_H_
#define _SORTARRAY_H_

#include <afxtempl.h>

// the quicksort function was adapted from the stdlib qsort function
#include <sortmacro.h>

// use this when basing a class on CObject - use CObArray for array class
//-------------------------------------------------------------------------------
template<class BASECLASS, class T>
class CSortObj : public BASECLASS
   {
   public:
		// return values analogus to strcmp
      virtual int Compare(UINT, T*)	= 0;
   };

// use this when not using CObject as base - use CPtrArray for array class
//-------------------------------------------------------------------------------
template<class T>
class CSortElem
   {
   public:
		// return values analogus to strcmp
      virtual int Compare(UINT, T)	= 0;
   };

//-------------------------------------------------------------------------------
template<class BASECLASS, class T>
class CSortObArray : public CTypedPtrArray<BASECLASS, T>
   {
	private:
		UINT m_nSortBy;
		enum { CUTOFF = 8 };
		inline int COMPARE(int a, int b)	{ return ((T)m_pData[a])->Compare(m_nSortBy, ((T)m_pData[b])); }

   public:
		CSortObArray()				{m_nSortBy=0;}
		void SetSortBy(UINT n)	{ m_nSortBy = n; }
		UINT GetSortBy()			{ return m_nSortBy; }

		// the functions in the macro are:
		// private:
		//    void Swap(int, int)
		//    void InsertSort(int int)
		// public:
		//    void QuickSort()
		//QUICKSORT

		// a simple search - works only on sorted arrays
		// int BSearch(T) - returns index of match or -1
		QUICKSEARCH


public: void QuickSort(){
		if (m_nSize < 2) return;
		int lo, hi;
		int mid;
		int loguy, higuy;
		unsigned size;				  
		int lostk[30], histk[30];
		int stkptr;					  
		stkptr = 0;
		lo = 0;
		hi = m_nSize-1;
	recurse:
		size = (hi - lo) + 1; 
		if(size <= CUTOFF) InsertSort(lo, hi);
		else {
			mid = lo + (size / 2);
			Swap(mid, lo);
			loguy = lo; higuy = hi + 1;
			for(;;) 	{
				do { loguy ++; } while (loguy <= hi && COMPARE(loguy, lo) <= 0);
				do { higuy --; } while (higuy > lo && COMPARE(higuy, lo) >= 0);
				if(higuy < loguy) break;
				Swap(loguy, higuy);
			}
				Swap(lo, higuy);
				if( higuy - 1 - lo >= hi - loguy ) {
					if (lo + 1 < higuy) {
						 lostk[stkptr] = lo;
						 histk[stkptr] = higuy - 1;
						 ++stkptr;
						}									 /* save big recursion for later */
					if (loguy < hi) {
						 lo = loguy;
						 goto recurse; 			 /* do small recursion */
						}
					} else {
					if (loguy < hi) {
						 lostk[stkptr] = loguy;
						 histk[stkptr] = hi;
						 ++stkptr;					 /* save big recursion for later */
						}
				if (lo + 1 < higuy) {
					 hi = higuy - 1;
					 goto recurse; 			 /* do small recursion */
					}
				}
		 }
		 --stkptr;
		 if (stkptr >= 0) {
			  lo = lostk[stkptr];
			  hi = histk[stkptr];
			  goto recurse;			  /* pop subarray from stack */
		 } else return;					  /* all subarrays done */
	}
private:
	void InsertSort(int lo, int hi) {
		 int p, max;
		 while (hi > lo) {
			  max = lo;
			  for (p = lo+1; p <= hi; p++) {
					if (COMPARE(p, max) > 0) {
						 max = p;
					}
			  }
			  Swap(max, hi);
			  hi --;
		 }
	}
	void Swap(int a, int b) {
		if(a != b) {
			T tmp = (T) m_pData[a];
			m_pData[a] = m_pData[b];
			m_pData[b] = tmp;
			}
		}



   };

#if 0
//-------------------------------------------------------------------------------
template<class BASECLASS, class T>
class CSortArray : public CArray<BASECLASS, T>
   {
	private:
		UINT m_nSortBy;
		enum { CUTOFF = 8 };
		inline int COMPARE(int a, int b)	{ return (T)m_pData[a]->Compare(m_nSortBy, (T)m_pData[b]); }

   public:
		void SetSortBy(UINT n)	{ m_nSortBy = n; }
		UINT GetSortBy()			{ return m_nSortBy; }

		QUICKSORT
   };
#endif

#endif // _SORTARRAY_H_