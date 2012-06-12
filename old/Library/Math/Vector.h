/**
 *	@file Math/Vector.h
 *
 *	�x�N�g���N���X�B
 *
 *	��ɕ׋��p�Ȃ̂ő��x�͏d�����ĂȂ��B
 *
 *	@author	Dila
 */

/// @addtogroup Math ���w�N���X
/// @{

#pragma once

#include <math.h>
#include <assert.h>

/**
 *	2D�x�N�g���N���X
 */
class Vector2 {
public:
	float x;	/// x�v�f
	float y;	/// y�v�f

	Vector2() 
	{
		x = y = 0.0f;
	}

	Vector2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	Vector2(const Vector2& v) 
	{
		x = v.x;
		y = v.y;
	}

	/// �v�f��ݒ肷��
	void Set(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	///	�x�N�g���̉��Z
	Vector2 operator + (const Vector2& v) const
	{
		return Vector2(x + v.x, y + v.y);
	}

	/// �x�N�g���̌��Z
	Vector2 operator - (const Vector2& v) const
	{
		return Vector2(x - v.x, y - v.y);
	}

	/// �x�N�g���ƃX�J���[�̏�Z
	Vector2 operator * (float k) const
	{
		return Vector2(x * k, y * k);
	}

	/// �x�N�g���ƃX�J���[�̏��Z
	Vector2 operator / (float k) const
	{
		return Vector2(x / k, y / k);
	}

	/// �x�N�g���̉��Z���
	Vector2& operator += (const Vector2& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	/// �x�N�g���̌��Z���
	Vector2& operator -= (const Vector2& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	/// �x�N�g���ƃX�J���[�̏�Z���
	Vector2& operator *= (float k)
	{
		x *= k;
		y *= k;
		return *this;
	}

	/// �x�N�g���ƃX�J���[�̏��Z���
	Vector2& operator /= (float k)
	{
		x /= k;
		y /= k;
		return *this;
	}

	/// ���̃x�N�g���̐��̃x�N�g����Ԃ�
	Vector2 operator + () const
	{
		return *this;
	}

	/// ���̃x�N�g���̕��̃x�N�g����Ԃ�
	Vector2 operator - () const
	{
		return Vector2(-x, -y);
	}

	/// ���̃x�N�g���̒����̓���Ԃ�
	float LengthSq() const
	{
		return x*x + y*y;
	}

	/// ���̃x�N�g���̒�����Ԃ�
	float Length() const
	{
		return sqrt(LengthSq());
	}

	/// ���̃x�N�g������x�N�g�����ǂ�����Ԃ�
	bool IsZero() const
	{
		return LengthSq() == 0.0f;
	}

	/// ���̃x�N�g���𐳋K�������x�N�g����Ԃ�
	Vector2 Normal() const
	{
		assert(!IsZero());
		float len = Length();
		return Vector2(x / len, y / len);
	}

	/// ���̃x�N�g���𐳋K������
	void Normalize()
	{
		assert(!IsZero());
		float len = Length();
		x /= len;
		y /= len;
	}

	/**
	 *	���̃x�N�g����v�̓��ς��v�Z����
	 *
	 *	V�U = Vx*Ux + Vy*Uy = |V||U|cos��
	 *	@return	����
	 */
	float Dot(const Vector2& v) const
	{
		return (x*v.x) + (y*v.y);
	}

	/**
	 *	���̃x�N�g����v�̊O�ρH���v�Z����
	 *
	 *	V�~U = Vx*Uy - Vy*Ux = |V||U|sin��
	 *	@return	�O��
	 */
	float Cross(const Vector2& v) const
	{
		return (x*v.y) - (y*v.x);
	}
};

/// �X�J���[�ƃx�N�g���̐ς��v�Z����
inline Vector2 operator * (float k, const Vector2& v)
{
	return Vector2(k * v.x, k * v.y); 
}

/**
 *	2D�x�N�g������]�����x�N�g���𓾂�
 *	@param	[out] _v	��]��̃x�N�g��
 *	@param	v		��]������x�N�g��
 *	@param	theta	��]������p�x(���W�A��)
 */
inline void Vector2Rotate(Vector2& _v, const Vector2& v, float theta)
{
	_v.x = v.x*cos(theta) - v.y*sin(theta);
	_v.y = v.x*sin(theta) + v.y*cos(theta);
}



/**
 *	3D�x�N�g���N���X
 */
class Vector3 {
public:
	float x;	/// x�v�f
	float y;	/// y�v�f
	float z;	/// z�v�f

	Vector3()
	{
		x = y = z = 0.0f;
	}

	Vector3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vector3(const Vector3& v) 
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}

	Vector3(const Vector2& v, float z)
	{
		x = v.x;
		y = v.y;
		this->z = z;
	}
	
	/// �v�f��ݒ肷��
	void Set(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}


	/// �x�N�g���̉��Z
	Vector3 operator + (const Vector3& v) const
	{
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	/// �x�N�g���̌��Z
	Vector3 operator - (const Vector3& v) const
	{
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	/// �x�N�g���ƃX�J���[�̏�Z
	Vector3 operator * (float k) const
	{
		return Vector3(x * k, y * k, z * k);
	}

	/// �x�N�g���ƃX�J���[�̏��Z
	Vector3 operator / (float k) const
	{
		return Vector3(x / k, y / k, z / k);
	}

	/// �x�N�g���̉��Z���
	Vector3& operator += (const Vector3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	/// �x�N�g���̌��Z���
	Vector3& operator -= (const Vector3& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	/// �x�N�g���ƃX�J���[�̏�Z���
	Vector3& operator *= (float k)
	{
		x *= k;
		y *= k;
		z *= k;
		return *this;
	}

	/// �x�N�g���ƃX�J���[�̏��Z���
	Vector3& operator /= (float k)
	{
		x /= k;
		y /= k;
		z /= k;
		return *this;
	}

	/// ���̃x�N�g���̐��̃x�N�g����Ԃ�
	Vector3 operator + () const
	{
		return *this;
	}

	/// ���̃x�N�g���̕��̃x�N�g����Ԃ�
	Vector3 operator - () const
	{
		return Vector3(-x, -y, -z);
	}


	/// ���̃x�N�g���̒����̓���Ԃ�
	float LengthSq() const
	{
		return x*x + y*y + z*z;
	}

	/// ���̃x�N�g���̒�����Ԃ�
	float Length() const
	{
		return sqrt(LengthSq());
	}

	/// ���̃x�N�g������x�N�g�����ǂ�����Ԃ�
	bool IsZero() const
	{
		return LengthSq() == 0.0f;
	}

	/// ���̃x�N�g���𐳋K�������x�N�g����Ԃ�
	Vector3 Normal() const
	{
		assert(!IsZero());
		float len = Length();
		return Vector3(x / len, y / len, z / len);
	}

	/// ���̃x�N�g���𐳋K������
	void Normalize()
	{
		assert(!IsZero());
		float len = Length();
		x /= len;
		y /= len;
		z /= len;
	}

	/**
	 *	���̃x�N�g����v�̓��ς��v�Z����
	 *
	 *	V�U = Vx*Ux + Vy*Uy + VzUz = |V||U|cos��
	 *	@return	����
	 */
	float Dot(const Vector3& v) const
	{
		return (x*v.x) + (y*v.y) + (z*v.z);
	}

	/**
	 *	���̃x�N�g����v�̊O�ς��v�Z����
	 *
	 *	V�~U = (VyUz-VzUy, VzUx-VxUz, VxUy-VyUx)
	 *	@return	�O��
	 */
	Vector3 Cross(const Vector3& v) const
	{
		return Vector3(
			y*v.z - z*v.y,
			z*v.x - x*v.z,
			x*v.y - y*v.x
			);
	}
};



/// �X�J���[�ƃx�N�g���̐ς��v�Z����
inline Vector3 operator * (float k, const Vector3& v)
{
	return Vector3(k * v.x, k * v.y, k * v.z); 
}



/**
 *	4D�x�N�g���N���X�B
 */
class Vector4 {
public:
	float x;	/// x�v�f
	float y;	/// y�v�f
	float z;	/// z�v�f
	float w;	/// w�v�f

	Vector4()
	{
		x = y = z = w = 0.0f;
	}

	Vector4(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	Vector4(const Vector4& v) 
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
	}

	Vector4(const Vector3& v, float w)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		this->w = w;
	}
	
	/// �v�f��ݒ肷��
	void Set(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	/// �x�N�g���̉��Z
	Vector4 operator + (const Vector4& v) const
	{
		return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
	}

	/// �x�N�g���̌��Z
	Vector4 operator - (const Vector4& v) const
	{
		return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
	}

	/// �x�N�g���ƃX�J���[�̏�Z
	Vector4 operator * (float k) const
	{
		return Vector4(x * k, y * k, z * k, w * k);
	}

	/// �x�N�g���ƃX�J���[�̏��Z
	Vector4 operator / (float k) const
	{
		return Vector4(x / k, y / k, z / k, w / k);
	}

	/// �x�N�g���̉��Z���
	Vector4& operator += (const Vector4& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return *this;
	}

	/// �x�N�g���̌��Z���
	Vector4& operator -= (const Vector4& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
		return *this;
	}

	/// �x�N�g���ƃX�J���[�̏�Z���
	Vector4& operator *= (float k)
	{
		x *= k;
		y *= k;
		z *= k;
		w *= k;
		return *this;
	}

	/// �x�N�g���ƃX�J���[�̏��Z���
	Vector4& operator /= (float k)
	{
		x /= k;
		y /= k;
		z /= k;
		w /= k;
		return *this;
	}

	/// ���̃x�N�g���̐��̃x�N�g����Ԃ�
	Vector4 operator + () const
	{
		return *this;
	}

	/// ���̃x�N�g���̕��̃x�N�g����Ԃ�
	Vector4 operator - () const
	{
		return Vector4(-x, -y, -z, -w);
	}


	/// ���̃x�N�g���̒����̓���Ԃ�
	float LengthSq() const
	{
		return x*x + y*y + z*z + w*w;
	}

	/// ���̃x�N�g���̒�����Ԃ�
	float Length() const
	{
		return sqrt(LengthSq());
	}

	/// ���̃x�N�g������x�N�g�����ǂ�����Ԃ�
	bool IsZero() const
	{
		return LengthSq() == 0.0f;
	}

	/// ���̃x�N�g���𐳋K�������x�N�g����Ԃ�
	Vector4 Normal() const
	{
		assert(!IsZero());
		float len = Length();
		return Vector4(x / len, y / len, z / len, w / len);
	}

	/// ���̃x�N�g���𐳋K������
	void Normalize()
	{
		assert(!IsZero());
		float len = Length();
		x /= len;
		y /= len;
		z /= len;
		w /= len;
	}

	/**
	 *	���̃x�N�g����v�̓��ς��v�Z����
	 *
	 *	V�U = Vx*Ux + Vy*Uy + VzUz + VwUw = |V||U|cos��
	 *	@return	����
	 */
	float Dot(const Vector4& v) const
	{
		return (x*v.x) + (y*v.y) + (z*v.z) + (w*v.z);
	}

	// �O�ρH D3DXVec4Cross�H
};


/// �X�J���[�ƃx�N�g���̐ς��v�Z����
inline Vector4 operator * (float k, const Vector4& v)
{
	return Vector4(k * v.x, k * v.y, k * v.z, k * v.w); 
}

/// @}
