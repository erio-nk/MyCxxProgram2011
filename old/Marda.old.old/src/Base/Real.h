/*!
	@file Real.h

	実数型。


	Copyright(C) 2008-2009 Dila <yamadila@gmail.com>
		All rights reserved.

	@author Dila
 */

#pragma once

#include <float.h>

namespace Marda
{
	// float
	namespace Float
	{
		const float ZERO = 0.0f;	//!< ゼロ
		const float NAN = (ZERO/ZERO);	//!< NaN:Not a Number
		const float INF = (1.0f/ZERO);	//!< Infinity

		const float MAX = FLT_MAX;	//!< 最大値
		const float MIN = FLT_MIN;	//!< 最小値

		inline bool IsZero(float x)
		{
			return ZERO == x;
		}

		inline bool IfZero(float& x, float then)
		{
			if (IsZero(x)) { x = then; return true; }
			return false;
		}

		inline bool IsNan(float x)
		{
			#if WIN32
				return _isnan(x) != 0;
			#else
				return isnan(x);
			#endif
		}
		
		inline bool IfNan(float& x, float then)
		{
			if (IsNan(x)) { x = then; return true; }
			return false;
		}

		inline bool IsInf(float x)
		{
			#if WIN32
				return _finite(x) == 0;
			#else
				return isinf(x);
			#endif
		}
		
		inline bool IfInf(float& x, float then)
		{
			if (IsInf(x)) { x = then; return true; }
			return false;
		}

		inline bool IsBad(float x)
		{
			if (IsNan(x) || IsInf(x)) return true;
			return false;
		}

		inline bool IfBad(float& x, float then)
		{
			return IfNan(x, then) || IfInf(x, then);
		}
	}	// end namespace Float

	// double
	namespace Double
	{
		const double MAX = DBL_MAX;	//!< 最大値
		const double MIN = DBL_MIN;	//!< 最小値
		
		// TODO:
	}	// end namespace Double

	// long double
	typedef long double ldouble;
	namespace LDouble
	{
		const long double MAX = LDBL_MAX;	//!< 最大値
		const long double MIN = LDBL_MIN;	//!< 最小値
		
		// TODO:
	}	// end namespace Double

	// TODO: Float16

	// float32
	typedef float float32;
	namespace Float32 = Float;

	// float64
	typedef double float64;
	namespace Float64 = Double;

	// float128
	typedef ldouble float128;
	namespace Float128 = LDouble;

}	// end namespace Marda
