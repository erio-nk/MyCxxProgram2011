/*!
	@file	Vector.h
	
			ベクトルクラス。
			
	@author	E.Nakayama
*/
#pragma once

namespace Marda
{
	// ------------------------------------------------------------------------
	// 2Dベクトル。

	/*!
		@brief	2Dベクトルクラス。
	*/
	struct Vector2
	{
		float x;
		float y;

		inline Vector2() {}
		
		inline Vector2(float x, float y);
		
		/*!
			@brief	添字で要素にアクセスする。
		*/
		inline float& operator [] (size_t index)
		{
			return *(&x + index);
		}
		
		//! 代入演算子。assignment operators
		//@{
		//@}
		
		//! 単項演算子。unary operators
		//@{
		
		//@}
		
		//! 二項演算子。binary operators
		//@{
		inline Vector2 operator + (const Vector2& rhs) const;
		//@}
		
		/*!
			@brief	長さを計算する。
		*/
		inline float Length() const;
		
		/*!
			@brief	外積を計算する。
			@note	
				V×U	= Vx*Uy - Vy*Ux
						= |V||U|sinθ
				即ち、V×U が 
					0 の時、VとUは同じ向きまたは反対向きである。
					0 > の時、UはVの左側にある（＝Vをx軸として見た時、UはVより上方＝正方向にある）。
					0 < の時、UはVの右側にある（＝Vをx軸として見た時、UはVより下方＝負方向にある）。
		*/
		static inline float CrossProduct(const Vector2& lhs, const Vector2& rhs);
		
	};	// end struct Vector2


	// ------------------------------------------------------------------------
	// 3Dベクトル。
	
	/*!
		@brief	3Dベクトルクラス。
	*/
	struct Vector3
	{
		float x;
		float y;
		float z;

		inline Vector3() {}

		inline Vector3(float x, float y, float z);
		
		/*!
			@brief	添字で要素にアクセスする。
		*/
		inline float& operator [] (size_t index)
		{
			return *(&x + index);
		}

		//! 代入演算子。assignment operators
		//@{
		inline Vector3& operator -= (const Vector3& rhs);
		//@}

		//! 二項演算子。binary operators
		//@{
		inline Vector3 operator + (const Vector3& rhs) const;
		inline Vector3 operator - (const Vector3& rhs) const;
		//@}

		static const Vector3 ZERO;
		
	};	// end struct Vector3

}	// end namespace Marda

#include "Vector_inl.h"
