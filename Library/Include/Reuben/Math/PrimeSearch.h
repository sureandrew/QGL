#pragma once
#ifndef _REUBEN_MATH_PRIME_SEARCH_H_
#define _REUBEN_MATH_PRIME_SEARCH_H_

//-- Local
#include "Random.h"

#include <boost/optional/optional.hpp>

namespace Reuben { namespace Math {

	class PrimeSearch
	{
	private:

		Int m_nSkip;
		Int m_nCurrentPosition;
		Int m_nMaxElements;
		Int* m_pCurrentPrime;
		Int m_nSearches;

		Reuben::Math::Random m_Random;

	public:

		PrimeSearch(Int nElements);

		VOID Create(CONST Int nElements);
		BoostOptional<Int> GetNext(Boolean bRestart = BOOLEAN_FALSE);
		INLINE VOID Restart(VOID);

	private:

		INLINE CONST Boolean IsDone(VOID);

	};

	INLINE VOID PrimeSearch::Restart(VOID)
	{
		m_nCurrentPosition = 0;
		m_nSearches = 0;
	}

	INLINE CONST Boolean PrimeSearch::IsDone(VOID)
	{
		return (m_nSearches == *m_pCurrentPrime);
	}

};}; // Reuben::Math

#endif // _REUBEN_MATH_PRIME_SEARCH_H_
