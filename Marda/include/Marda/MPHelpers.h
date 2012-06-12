/*!
	@file	MPHelpers.h
	
			メタプログラミング用のヘルパ群。

	@author	E.Nakayama
*/
#pragma once

namespace Marda
{
	namespace MPHelpers
	{
			
		// --------------------------------------------------------------------
		
		/*!
			@brief	累乗を計算するメタ関数。
			@tparam	x	底。
			@tparam	y	指数。
			@code
			// 2の3乗を求める。
			const int ret = Pow<2, 3>::result;
			@endcode
		*/
		template <int x, int y>
		struct Pow
		{
			enum
			{
				result = x * Pow<x, y-1>::result
			};
		};
		
		template <int x>
		struct Pow<x, 0>
		{
			enum
			{
				result = 1
			};
		};
	
	}	// end namespace MPHelpers

}	// end namespace Marda
