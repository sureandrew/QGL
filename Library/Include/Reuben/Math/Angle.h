#pragma once
#ifndef _REUBEN_MATH_ANGLE_H_
#define _REUBEN_MATH_ANGLE_H_

#include "Math.h"

#define MATH_ANGLE_DEGREE		(MATH_PI / C_REAL(180.0))
#define MATH_ANGLE_RADIAN		C_REAL(1.0)
#define MATH_ANGLE_10_DEGREES	(MATH_PI * C_REAL(10.0) / C_REAL(180.0))
#define MATH_ANGLE_30_DEGREES	(MATH_PI * C_REAL(30.0) / C_REAL(180.0))
#define MATH_ANGLE_45_DEGREES	(MATH_PI * C_REAL(0.25))
#define MATH_ANGLE_60_DEGREES	(MATH_PI * C_REAL(60.0) / C_REAL(180.0))
#define MATH_ANGLE_90_DEGREES	(MATH_PI * C_REAL(0.5))
#define MATH_ANGLE_180_DEGREES	MATH_PI
#define MATH_ANGLE_270_DEGREES	(MATH_PI * C_REAL(1.5))
#define MATH_ANGLE_360_DEGREES	(MATH_PI * C_REAL(2.0))

namespace Reuben { namespace Math {

	//-- Type Def

	namespace Angle {

		//-- Function

		CONST Real GetArcTangent2(
			CONST Real sinus,
			CONST Real cosinus
			);

		//--

		CONST Real GetFromSinusCosinus(
			CONST Real sinus,
			CONST Real cosinus
			);

		//--

		CONST Real GetFromXY(
			CONST Real x,
			CONST Real y,
			CONST Real default_angle = C_REAL(0.0),
			CONST Real precision = C_REAL(0.001)
			);

		//--

		CONST Boolean IsEquivalent(
			CONST Real angle,
			CONST Real other_angle
			);

		//--

		CONST Boolean IsRoughlyEquivalent(
			CONST Real angle,
			CONST Real other_angle,
			CONST Real precision
			);

		//--

		CONST Real GetClosest(
			CONST Real angle,
			CONST Real other_angle
			);

		//--

		INLINE VOID SetClosest(
			Real& angle,
			CONST Real other_angle
			)
		{
			angle = GetClosest(angle, other_angle);
		}

		//--

		INLINE CONST Real GetDegrees(CONST Real angle)
		{
			return (angle * C_REAL(180.0) / MATH_PI);
		}

		//--

		INLINE CONST Real GetRadians(CONST Real angle)
		{
			return (angle);
		}

		//--

		INLINE CONST Real GetFromDegrees(CONST Real angle)
		{
			return (angle * MATH_PI / C_REAL(180.0));
		}

		//--

		INLINE CONST Real GetFromRadians(CONST Real angle)
		{
			return (angle);
		}

		//--

		INLINE CONST Real GetSmallest(CONST Real angle)
		{
			return (GetClosest(angle, C_REAL(0.0)));
		}

		//--

		INLINE CONST Real SetSmallest(Real& angle)
		{
			return (angle = GetSmallest(angle));
		}

		//--

		INLINE CONST Real GetClosestDifference(
			CONST Real angle,
			CONST Real other_angle
			)
		{
			return (angle - GetClosest(other_angle, angle));
		}

	}; // Angle

};}; // Reuben::Math

#endif // _REUBEN_MATH_ANGLE_H_
