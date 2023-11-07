#pragma once
#ifndef _REUBEN_MATH_RANDOM_H_
#define _REUBEN_MATH_RANDOM_H_

//-- Period Parameters
#define MATH_N			624
#define MATH_M			397
#define MATH_MATRIX_A	0x9908B0DF
#define MATH_UPPER_MASK	0x80000000
#define MATH_LOWER_MASK 0x7FFFFFFF

//-- Tempering Parameters
#define MATH_TEMPERING_MASK_B		0x9D2C5680
#define MATH_TEMPERING_MASK_C		0xEFC60000
#define MATH_TEMPERING_SHIFT_U(y)	(y >> 11)
#define MATH_TEMPERING_SHIFT_S(y)	(y >> 7)
#define MATH_TEMPERING_SHIFT_T(y)	(y >> 15)
#define MATH_TEMPERING_SHIFT_L(y)	(y >> 18)

namespace Reuben { namespace Math {

	class Random
	{
	private:

		Int32 m_nSeed;
		Int m_nMTI;
		Int32 m_anMT[MATH_N];

	public:

		//-- Constructor / Destructor
		Random(VOID);

		//-- Property
		VOID SetSeed(CONST Int32 mSeed);
		INLINE CONST Int32 GetSeed(VOID) CONST;

		//-- Function
		CONST Int GetRandom(CONST Int nRange);
		VOID Randomize(VOID);
	};

	INLINE CONST Int Random::GetSeed(VOID) CONST
	{
		return (m_nSeed);
	}

};}; // Reuben::Math

#endif // _REUBEN_MATH_RANDOM_H_
