/*!
	@file	Math/Vector_inl.h
	
	@author	E.Nakayama
*/
#pragma once

namespace Marda
{
	// ------------------------------------------------------------------------
	// Vector2
	Vector2::Vector2(float vx, float vy)
		: x(vx)
		, y(vy)
	{
	}

	Vector2
	Vector2::operator + (const Vector2& rhs) const
	{
		return Vector2(x + rhs.x, y + rhs.y);
	}

	float
	Vector2::Length() const
	{
		return sqrt(x*x + y*y);
	}

	float
	Vector2::CrossProduct(const Vector2& lhs, const Vector2& rhs)
	{
		return lhs.x * rhs.y - lhs.y + rhs.x;
	}

	// ------------------------------------------------------------------------
	// Vector3
	Vector3::Vector3(float vx, float vy, float vz)
		: x(vx)
		, y(vy)
		, z(vz)
	{
	}

	Vector3&
	Vector3::operator -= (const Vector3& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	Vector3
	Vector3::operator + (const Vector3& rhs) const
	{
		return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
	}
	
	Vector3
	Vector3::operator - (const Vector3& rhs) const
	{
		return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
	}

}	// end namespace Marda

