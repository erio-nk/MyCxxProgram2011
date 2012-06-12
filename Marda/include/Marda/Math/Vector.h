/*!
	@file	Vector.h
	
			�x�N�g���N���X�B
			
	@author	E.Nakayama
*/
#pragma once

namespace Marda
{
	// ------------------------------------------------------------------------
	// 2D�x�N�g���B

	/*!
		@brief	2D�x�N�g���N���X�B
	*/
	struct Vector2
	{
		float x;
		float y;

		inline Vector2() {}
		
		inline Vector2(float x, float y);
		
		/*!
			@brief	�Y���ŗv�f�ɃA�N�Z�X����B
		*/
		inline float& operator [] (size_t index)
		{
			return *(&x + index);
		}
		
		//! ������Z�q�Bassignment operators
		//@{
		//@}
		
		//! �P�����Z�q�Bunary operators
		//@{
		
		//@}
		
		//! �񍀉��Z�q�Bbinary operators
		//@{
		inline Vector2 operator + (const Vector2& rhs) const;
		//@}
		
		/*!
			@brief	�������v�Z����B
		*/
		inline float Length() const;
		
		/*!
			@brief	�O�ς��v�Z����B
			@note	
				V�~U	= Vx*Uy - Vy*Ux
						= |V||U|sin��
				�����AV�~U �� 
					0 �̎��AV��U�͓��������܂��͔��Ό����ł���B
					0 > �̎��AU��V�̍����ɂ���i��V��x���Ƃ��Č������AU��V��������������ɂ���j�B
					0 < �̎��AU��V�̉E���ɂ���i��V��x���Ƃ��Č������AU��V��艺�����������ɂ���j�B
		*/
		static inline float CrossProduct(const Vector2& lhs, const Vector2& rhs);
		
	};	// end struct Vector2


	// ------------------------------------------------------------------------
	// 3D�x�N�g���B
	
	/*!
		@brief	3D�x�N�g���N���X�B
	*/
	struct Vector3
	{
		float x;
		float y;
		float z;

		inline Vector3() {}

		inline Vector3(float x, float y, float z);
		
		/*!
			@brief	�Y���ŗv�f�ɃA�N�Z�X����B
		*/
		inline float& operator [] (size_t index)
		{
			return *(&x + index);
		}

		//! ������Z�q�Bassignment operators
		//@{
		inline Vector3& operator -= (const Vector3& rhs);
		//@}

		//! �񍀉��Z�q�Bbinary operators
		//@{
		inline Vector3 operator + (const Vector3& rhs) const;
		inline Vector3 operator - (const Vector3& rhs) const;
		//@}

		static const Vector3 ZERO;
		
	};	// end struct Vector3

}	// end namespace Marda

#include "Vector_inl.h"
