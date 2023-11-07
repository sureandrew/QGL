#pragma once
#ifndef _REUBEN_MATH_MATH_H_
#define _REUBEN_MATH_MATH_H_

//-- Constant

#define MATH_PI							C_REAL(3.14159265358979323846264338327950288419716939937510)
#define MATH_HALF_PI					(C_REAL(3.14159265358979323846264338327950288419716939937510) / C_REAL(2.0))
#define MATH_SQUARE_ROOT_OF_TWO			C_REAL(1.4142135623730950488016887242097)
#define MATH_SQUARE_ROOT_OF_THREE		C_REAL(1.7320508075688772935274463415059)
#define MATH_SQUARE_ROOT_OF_NOE_THIRD	C_REAL(0.57735026918962576450914878050196)
#define MATH_DEFAULT_PRECISION			C_REAL(0.000001)

namespace Reuben { namespace Math {

	//-- Function

	template <typename _TYPE_> INLINE VOID Swap(_TYPE_& first, _TYPE_& second)
	{
		_TYPE_ temp = first;
		first = second;
		second = temp;
	}

	template <typename _REAL_> INLINE CONST _REAL_ GetSquareRoot(CONST _REAL_ x)
	{
		return (sqrt(x));
	}

	//--

	template <typename _REAL_> INLINE CONST _REAL_ GetAbsoluteValue(CONST _REAL_ x)
	{
		return (fabs(x));
	}

	//--

	template <typename _REAL_> INLINE CONST _REAL_ GetModulo(
		CONST _REAL_ x, 
		CONST _REAL_ y
		)
	{
		return (fmod(x, y));
	}

	//--

	template <typename _REAL_> INLINE CONST _REAL_ GetCeil(CONST _REAL_ x)
	{
		return (ceil(x));
	}

	//--

	template <typename _REAL_> INLINE CONST _REAL_ GetFloor(CONST _REAL_ x)
	{
		return (floor(x));
	}

	//--

	template <typename _REAL_> INLINE CONST _REAL_ GetRound(CONST _REAL_ x)
	{
		return (GetFloor<_REAL_>(x + (x > C_REAL(-0.5) ? C_REAL(0.5) : C_REAL(-0.5))));
	}

	//--

	template <typename _REAL_> INLINE CONST _REAL_ GetNearestRound(CONST _REAL_ x)
	{
		return (GetFloor<_REAL_>(x + C_REAL(0.5)));
	}

	//--

	template <typename _SCALAR_> INLINE CONST _SCALAR_ GetPower(
		CONST _SCALAR_ x,
		CONST _SCALAR_ y
		)
	{
		return (pow(x, y));
	}

	//--

	template <typename _REAL_> INLINE CONST _REAL_ GetEaseInEaseOut(CONST _REAL_ percentage)
	{
		return ((C_REAL(1.0) - cos(percentage * PI)) * C_REAL(0.5))
	}

	//--

	template <typename _REAL_> INLINE CONST _REAL_ GetSinus(CONST _REAL_ x)
	{
		return (sin(x));
	}

	//--

	template <typename _REAL_> INLINE CONST _REAL_ GetConsinus(CONST _REAL_ x)
	{
		return (cos(x));
	}

	//--

	template <typename _REAL_> INLINE CONST _REAL_ GetTangent(CONST _REAL_ x)
	{
		return (tan(x));
	}

	//--

	template <typename _REAL_> INLINE CONST _REAL_ GetArcSinus(CONST _REAL_ x)
	{
		return (asin(x));
	}

	//--

	template <typename _REAL_> INLINE CONST _REAL_ GetArcCosinus(CONST _REAL_ x)
	{
		return (acos(x));
	}

	//--

	template <typename _REAL_> INLINE CONST _REAL_ GetArcTangent(CONST _REAL_ x)
	{
		return (atan(x));
	}

	//--

	template <typename _REAL_> INLINE CONST _REAL_ GetArcTangent2(
		CONST _REAL_ sinus, 
		CONST _REAL_ cosinus
		)
	{
		return (atan2(sinus, cosinus));
	}

	//--

    template <typename _ITEM_> INLINE CONST _ITEM_ GetMinimum(
        CONST _ITEM_ & first_item,
        CONST _ITEM_ & second_item
        )
    {
        if ( first_item < second_item )

            return (first_item);
        else

            return (second_item);
    }

	//--

	template <typename _ITEM_> INLINE CONST _ITEM_ GetMaximum(
		CONST _ITEM_ & first_item,
		CONST _ITEM_ & second_item
		)
	{
		if ( first_item > second_item )

			return (first_item);
		else

			return (second_item);
	}

	//--

	template <typename _ITEM_> INLINE CONST _ITEM_ GetMinimum(
		CONST _ITEM_ & first_item,
		CONST _ITEM_ & second_item,
		CONST _ITEM_ & third_item
		)
	{
		if ( first_item < second_item )

			return (( first_item < third_item ) ? first_item : third_item);
		else

			return (( second_item < third_item ) ? second_item : third_item);
	}

	//--

	template <typename _ITEM_>	INLINE CONST _ITEM_ GetMaximum(
		CONST _ITEM_ & first_item,
		CONST _ITEM_ & second_item,
		CONST _ITEM_ & third_item
		)
	{
		if ( first_item > second_item )

			return (( first_item > third_item ) ? first_item : third_item);
		else

			return (( second_item > third_item ) ? second_item : third_item);
	}

	//--

	template <typename _ITEM_> INLINE CONST _ITEM_ GetMinimum(
		CONST _ITEM_ & first_item,
		CONST _ITEM_ & second_item,
		CONST _ITEM_ & third_item,
		CONST _ITEM_ & fourth_item
		)
	{
		return (GetMinimum
			(
			GetMinimum( first_item, second_item ),
			GetMinimum( third_item, fourth_item )
			));
	}

	//--

	template <typename _ITEM_> INLINE CONST _ITEM_ GetMaximum(
		CONST _ITEM_ & first_item,
		CONST _ITEM_ & second_item,
		CONST _ITEM_ & third_item,
		CONST _ITEM_ & fourth_item
		)
	{
		return (GetMaximum
			(
			GetMaximum( first_item, second_item ),
			GetMaximum( third_item, fourth_item )
			));
	}

	//--

	template <typename _SCALAR_> INLINE CONST _SCALAR_ GetAbsolute(CONST _SCALAR_ scalar)
	{
		if ( scalar < 0 )

			return (-scalar);
		else

			return (scalar);
	}

	//--

	template <typename _SCALAR_> INLINE CONST _SCALAR_ GetPowerOfTwo(CONST _SCALAR_ scalar)
	{
		return (scalar << 1);
	}

	//--

	template <typename _SCALAR_> INLINE CONST Boolean IsPowerOfTwo(CONST _SCALAR_ scalar)
	{
		return (!(scalar & (scalar - 1)));
	}

	//--

	template <typename _BIT_> INLINE Count GetBitCount(CONST _BIT_ value)
	{
		Count bit_count;
		
		if (value == 0)
			return (0);
		else
		{
			for (bit_count = 1; value & (~1); ++ bit_count)
				value >>= 1;
			return (bit_count);
		}
	}

	//--

	template <typename _BIT_> INLINE VOID AddBits(
		_BIT_ & bit_pack,
		CONST _BIT_ bit_mask
		)
	{
		bit_pack |= bit_mask;
	}

	//--

	template <typename _BIT_> INLINE VOID RemoveBits(
		_BIT_ & bit_pack,
		CONST _BIT_ bit_mask
		)
	{
		bit_pack &= ~bit_mask;
	}

	//--

	template <typename _BIT_> INLINE VOID ToggleBits(
		_BIT_ & bit_pack,
		CONST _BIT_ bit_mask
		)
	{
		bit_pack ^= bit_mask;
	}

	//--

	INLINE Int32 GetInt32FromBigEndianInt32(CONST Int32 value)
	{
		Int32 first_byte, second_byte, third_byte, fourth_byte;

		first_byte = ((value & 0xFF) >> 0) & 0xFF;
		second_byte = ((value & 0xFF00) >> 8) & 0xFF;
		third_byte = ((value & 0xFF0000) >> 16) & 0xFF;
		fourth_byte = ((value & 0xFF000000) >> 24) & 0xFF;

		return ((first_byte << 24) | (second_byte << 16) | (third_byte << 8) | fourth_byte);
	}

	//--
	
    INLINE Int32 GetUint32FromBigEndianUint32(CONST Int32 value)
	{
		Int32 first_byte, second_byte, third_byte, fourth_byte;

		first_byte = ( ( value & 0xFF ) >> 0 ) & 0xFF;
		second_byte = ( ( value & 0xFF00 ) >> 8 ) & 0xFF;
		third_byte = ( ( value & 0xFF0000 ) >> 16 ) & 0xFF;
		fourth_byte = ( ( value & 0xFF000000 ) >> 24 ) & 0xFF;

		return ((first_byte << 24) | (second_byte << 16) | (third_byte << 8) | fourth_byte);
	}

};}; // Reuben::Math

#endif // _REUBEN_MATH_MATH_H_
