//--------------------------------------------------------------------------------
//
// Copyright (c) 2001 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _PRIMENUMBERS_H_
#define _PRIMENUMBERS_H_

//--------------------------------------------------------------------------------
class CPrimeNumbers
	{
	public:
		static const LONG m_nPrimes[256];

	public:
		static LONG GetNearestPrime(LONG);
	};

#endif // _PRIMENUMBERS_H_
