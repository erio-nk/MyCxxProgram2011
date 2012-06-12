/*!
	@file	Math.h
	
			”ŠwŠÖ˜A‚ÌŠÖ”ŒQB
			
	@author	E.Nakayama
*/
#pragma once

namespace Marda
{
	inline bool Equals(float lhs, float rhs, float tolerance = FLT_EPSILON)
	{
		if (fabs(rhs - lhs) <= tolerance)
			return true;
		return false;
	}

	inline float Clamp(float value, float value_min, float value_max)
	{
		return max(value_min, min(value_max, value));
	}

}	// end namespace Marda

#include "Math/Vector.h"
