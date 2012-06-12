/*!
	@file	MPHelpers.h
	
			���^�v���O���~���O�p�̃w���p�Q�B

	@author	E.Nakayama
*/
#pragma once

namespace Marda
{
	namespace MPHelpers
	{
			
		// --------------------------------------------------------------------
		
		/*!
			@brief	�ݏ���v�Z���郁�^�֐��B
			@tparam	x	��B
			@tparam	y	�w���B
			@code
			// 2��3������߂�B
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
