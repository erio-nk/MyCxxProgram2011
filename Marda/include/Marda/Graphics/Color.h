/*!
	@file	Color.h
	
	@author	E.Nakayama
*/
#pragma once
	
namespace Marda
{
	struct Color
	{
		float red;
		float green;
		float blue;
		float alpha;

		static const Color RED;
		static const Color YELLOW;
		static const Color GREEN;
		static const Color CYAN;
		static const Color BLUE;
		static const Color MAGENTA;
		static const Color WHITE;
		static const Color BLACK;
		static const Color GRAY;
		
		Color()
			: red(0.f)
			, green(0.f)
			, blue(0.f)
			, alpha(1.f)
		{
		}
		
		Color(float r, float g, float b, float a)
			: red(r)
			, green(g)
			, blue(b)
			, alpha(a)
		{
		}
		
		void ToFloat3(float* dst) const
		{
			memcpy(dst, &red, sizeof(float)*3);
		}
		
		float* ToFloat4()
		{
			return &red;
		}
		
		const float* ToFloat4() const
		{
			return &red;
		}

		void ToFloat4(float* dst) const
		{
			memcpy(dst, &red, sizeof(float)*4);
		}
		
	};	// end struct Color

}	// end namespace Marda
