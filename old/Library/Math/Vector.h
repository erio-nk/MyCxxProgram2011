/**
 *	@file Math/Vector.h
 *
 *	ベクトルクラス。
 *
 *	主に勉強用なので速度は重視してない。
 *
 *	@author	Dila
 */

/// @addtogroup Math 数学クラス
/// @{

#pragma once

#include <math.h>
#include <assert.h>

/**
 *	2Dベクトルクラス
 */
class Vector2 {
public:
	float x;	/// x要素
	float y;	/// y要素

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

	/// 要素を設定する
	void Set(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	///	ベクトルの加算
	Vector2 operator + (const Vector2& v) const
	{
		return Vector2(x + v.x, y + v.y);
	}

	/// ベクトルの減算
	Vector2 operator - (const Vector2& v) const
	{
		return Vector2(x - v.x, y - v.y);
	}

	/// ベクトルとスカラーの乗算
	Vector2 operator * (float k) const
	{
		return Vector2(x * k, y * k);
	}

	/// ベクトルとスカラーの除算
	Vector2 operator / (float k) const
	{
		return Vector2(x / k, y / k);
	}

	/// ベクトルの加算代入
	Vector2& operator += (const Vector2& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	/// ベクトルの減算代入
	Vector2& operator -= (const Vector2& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	/// ベクトルとスカラーの乗算代入
	Vector2& operator *= (float k)
	{
		x *= k;
		y *= k;
		return *this;
	}

	/// ベクトルとスカラーの除算代入
	Vector2& operator /= (float k)
	{
		x /= k;
		y /= k;
		return *this;
	}

	/// このベクトルの正のベクトルを返す
	Vector2 operator + () const
	{
		return *this;
	}

	/// このベクトルの負のベクトルを返す
	Vector2 operator - () const
	{
		return Vector2(-x, -y);
	}

	/// このベクトルの長さの二乗を返す
	float LengthSq() const
	{
		return x*x + y*y;
	}

	/// このベクトルの長さを返す
	float Length() const
	{
		return sqrt(LengthSq());
	}

	/// このベクトルが零ベクトルかどうかを返す
	bool IsZero() const
	{
		return LengthSq() == 0.0f;
	}

	/// このベクトルを正規化したベクトルを返す
	Vector2 Normal() const
	{
		assert(!IsZero());
		float len = Length();
		return Vector2(x / len, y / len);
	}

	/// このベクトルを正規化する
	void Normalize()
	{
		assert(!IsZero());
		float len = Length();
		x /= len;
		y /= len;
	}

	/**
	 *	このベクトルとvの内積を計算する
	 *
	 *	V･U = Vx*Ux + Vy*Uy = |V||U|cosθ
	 *	@return	内積
	 */
	float Dot(const Vector2& v) const
	{
		return (x*v.x) + (y*v.y);
	}

	/**
	 *	このベクトルとvの外積？を計算する
	 *
	 *	V×U = Vx*Uy - Vy*Ux = |V||U|sinθ
	 *	@return	外積
	 */
	float Cross(const Vector2& v) const
	{
		return (x*v.y) - (y*v.x);
	}
};

/// スカラーとベクトルの積を計算する
inline Vector2 operator * (float k, const Vector2& v)
{
	return Vector2(k * v.x, k * v.y); 
}

/**
 *	2Dベクトルを回転したベクトルを得る
 *	@param	[out] _v	回転後のベクトル
 *	@param	v		回転させるベクトル
 *	@param	theta	回転させる角度(ラジアン)
 */
inline void Vector2Rotate(Vector2& _v, const Vector2& v, float theta)
{
	_v.x = v.x*cos(theta) - v.y*sin(theta);
	_v.y = v.x*sin(theta) + v.y*cos(theta);
}



/**
 *	3Dベクトルクラス
 */
class Vector3 {
public:
	float x;	/// x要素
	float y;	/// y要素
	float z;	/// z要素

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
	
	/// 要素を設定する
	void Set(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}


	/// ベクトルの加算
	Vector3 operator + (const Vector3& v) const
	{
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	/// ベクトルの減算
	Vector3 operator - (const Vector3& v) const
	{
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	/// ベクトルとスカラーの乗算
	Vector3 operator * (float k) const
	{
		return Vector3(x * k, y * k, z * k);
	}

	/// ベクトルとスカラーの除算
	Vector3 operator / (float k) const
	{
		return Vector3(x / k, y / k, z / k);
	}

	/// ベクトルの加算代入
	Vector3& operator += (const Vector3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	/// ベクトルの減算代入
	Vector3& operator -= (const Vector3& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	/// ベクトルとスカラーの乗算代入
	Vector3& operator *= (float k)
	{
		x *= k;
		y *= k;
		z *= k;
		return *this;
	}

	/// ベクトルとスカラーの除算代入
	Vector3& operator /= (float k)
	{
		x /= k;
		y /= k;
		z /= k;
		return *this;
	}

	/// このベクトルの正のベクトルを返す
	Vector3 operator + () const
	{
		return *this;
	}

	/// このベクトルの負のベクトルを返す
	Vector3 operator - () const
	{
		return Vector3(-x, -y, -z);
	}


	/// このベクトルの長さの二乗を返す
	float LengthSq() const
	{
		return x*x + y*y + z*z;
	}

	/// このベクトルの長さを返す
	float Length() const
	{
		return sqrt(LengthSq());
	}

	/// このベクトルが零ベクトルかどうかを返す
	bool IsZero() const
	{
		return LengthSq() == 0.0f;
	}

	/// このベクトルを正規化したベクトルを返す
	Vector3 Normal() const
	{
		assert(!IsZero());
		float len = Length();
		return Vector3(x / len, y / len, z / len);
	}

	/// このベクトルを正規化する
	void Normalize()
	{
		assert(!IsZero());
		float len = Length();
		x /= len;
		y /= len;
		z /= len;
	}

	/**
	 *	このベクトルとvの内積を計算する
	 *
	 *	V･U = Vx*Ux + Vy*Uy + VzUz = |V||U|cosθ
	 *	@return	内積
	 */
	float Dot(const Vector3& v) const
	{
		return (x*v.x) + (y*v.y) + (z*v.z);
	}

	/**
	 *	このベクトルとvの外積を計算する
	 *
	 *	V×U = (VyUz-VzUy, VzUx-VxUz, VxUy-VyUx)
	 *	@return	外積
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



/// スカラーとベクトルの積を計算する
inline Vector3 operator * (float k, const Vector3& v)
{
	return Vector3(k * v.x, k * v.y, k * v.z); 
}



/**
 *	4Dベクトルクラス。
 */
class Vector4 {
public:
	float x;	/// x要素
	float y;	/// y要素
	float z;	/// z要素
	float w;	/// w要素

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
	
	/// 要素を設定する
	void Set(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	/// ベクトルの加算
	Vector4 operator + (const Vector4& v) const
	{
		return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
	}

	/// ベクトルの減算
	Vector4 operator - (const Vector4& v) const
	{
		return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
	}

	/// ベクトルとスカラーの乗算
	Vector4 operator * (float k) const
	{
		return Vector4(x * k, y * k, z * k, w * k);
	}

	/// ベクトルとスカラーの除算
	Vector4 operator / (float k) const
	{
		return Vector4(x / k, y / k, z / k, w / k);
	}

	/// ベクトルの加算代入
	Vector4& operator += (const Vector4& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return *this;
	}

	/// ベクトルの減算代入
	Vector4& operator -= (const Vector4& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
		return *this;
	}

	/// ベクトルとスカラーの乗算代入
	Vector4& operator *= (float k)
	{
		x *= k;
		y *= k;
		z *= k;
		w *= k;
		return *this;
	}

	/// ベクトルとスカラーの除算代入
	Vector4& operator /= (float k)
	{
		x /= k;
		y /= k;
		z /= k;
		w /= k;
		return *this;
	}

	/// このベクトルの正のベクトルを返す
	Vector4 operator + () const
	{
		return *this;
	}

	/// このベクトルの負のベクトルを返す
	Vector4 operator - () const
	{
		return Vector4(-x, -y, -z, -w);
	}


	/// このベクトルの長さの二乗を返す
	float LengthSq() const
	{
		return x*x + y*y + z*z + w*w;
	}

	/// このベクトルの長さを返す
	float Length() const
	{
		return sqrt(LengthSq());
	}

	/// このベクトルが零ベクトルかどうかを返す
	bool IsZero() const
	{
		return LengthSq() == 0.0f;
	}

	/// このベクトルを正規化したベクトルを返す
	Vector4 Normal() const
	{
		assert(!IsZero());
		float len = Length();
		return Vector4(x / len, y / len, z / len, w / len);
	}

	/// このベクトルを正規化する
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
	 *	このベクトルとvの内積を計算する
	 *
	 *	V･U = Vx*Ux + Vy*Uy + VzUz + VwUw = |V||U|cosθ
	 *	@return	内積
	 */
	float Dot(const Vector4& v) const
	{
		return (x*v.x) + (y*v.y) + (z*v.z) + (w*v.z);
	}

	// 外積？ D3DXVec4Cross？
};


/// スカラーとベクトルの積を計算する
inline Vector4 operator * (float k, const Vector4& v)
{
	return Vector4(k * v.x, k * v.y, k * v.z, k * v.w); 
}

/// @}
