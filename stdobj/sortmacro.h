
#define QUICKSEARCH 	public:\
	int BSearch(T p) {\
		if(m_nSize == 0)\
			return -1;\
		int lo = 0;\
		int hi = m_nSize-1;\
		int mid;\
\
		while(hi-lo>1)\
			{\
			mid = (hi+lo)/2;\
			int rv = ((T)m_pData[mid])->Compare(m_nSortBy, p);\
			if(rv==0)\
				return mid;\
			if(rv>=0)\
				hi=mid;\
			else\
				lo=mid;\
			}\
		if(((T)m_pData[hi])->Compare(m_nSortBy, p) == 0)\
			return hi;\
\
		if(((T)m_pData[lo])->Compare(m_nSortBy, p) == 0)\
			return lo;\
\
		return -1;\
		}


#define QUICKSORT public: void QuickSort(){\
		if (m_nSize < 2) return;\
		int lo, hi;\
		int mid;\
		int loguy, higuy;\
		unsigned size;				  \
		int lostk[30], histk[30];\
		int stkptr;					  \
		stkptr = 0;\
		lo = 0;\
		hi = m_nSize-1;\
	recurse:\
		size = (hi - lo) + 1; \
		if(size <= CUTOFF) InsertSort(lo, hi);\
		else {\
			mid = lo + (size / 2);\
			Swap(mid, lo);\
			loguy = lo; higuy = hi + 1;\
			for(;;) 	{\
				do { loguy ++; } while (loguy <= hi && COMPARE(loguy, lo) <= 0);\
				do { higuy --; } while (higuy > lo && COMPARE(higuy, lo) >= 0);\
				if(higuy < loguy) break;\
				Swap(loguy, higuy);\
			}\
				Swap(lo, higuy);\
				if( higuy - 1 - lo >= hi - loguy ) {\
					if (lo + 1 < higuy) {\
						 lostk[stkptr] = lo;\
						 histk[stkptr] = higuy - 1;\
						 ++stkptr;\
						}									 /* save big recursion for later */\
					if (loguy < hi) {\
						 lo = loguy;\
						 goto recurse; 			 /* do small recursion */\
						}\
					} else {\
					if (loguy < hi) {\
						 lostk[stkptr] = loguy;\
						 histk[stkptr] = hi;\
						 ++stkptr;					 /* save big recursion for later */\
						}\
				if (lo + 1 < higuy) {\
					 hi = higuy - 1;\
					 goto recurse; 			 /* do small recursion */\
					}\
				}\
		 }\
		 --stkptr;\
		 if (stkptr >= 0) {\
			  lo = lostk[stkptr];\
			  hi = histk[stkptr];\
			  goto recurse;			  /* pop subarray from stack */\
		 } else return;					  /* all subarrays done */\
	}\
private:\
	void InsertSort(int lo, int hi) {\
		 int p, max;\
		 while (hi > lo) {\
			  max = lo;\
			  for (p = lo+1; p <= hi; p++) {\
					if (COMPARE(p, max) > 0) {\
						 max = p;\
					}\
			  }\
			  Swap(max, hi);\
			  hi --;\
		 }\
	}\
	void Swap(int a, int b) {\
		if(a != b) {\
			T tmp = (T) m_pData[a];\
			m_pData[a] = m_pData[b];\
			m_pData[b] = tmp;\
			}\
		}

