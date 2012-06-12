/*
 *	Math/Quaternion.cpp
 *
 *	@author	Dila
 */

#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "Math/Quaternion.h"


/**
 *	クォータニオンの加算
 *
 *	Q+P = (Qw+Pw) + (Qx+Px)i + (Qy+Py)j + (Qz+Pz)k
 */
Quaternion Quaternion::operator + (const Quaternion& p)
{
	return Quaternion(w+p.w, x+p.x, y+p.y, z+p.z);
}

/**
 *	クォータニオンの減算
 */
Quaternion Quaternion::operator - (const Quaternion& p)
{
	return Quaternion(w-p.w, x-p.x, y-p.y, z-p.z);
}

/**
 *	クォータニオンの加算代入
 */
Quaternion& Quaternion::operator += (const Quaternion& p)
{
	w += p.w;
	x += p.x;
	y += p.y;
	z += p.z;
	return *this;
}

/**
 *	クォータニオンの減算代入
 */
Quaternion& Quaternion::operator -= (const Quaternion& p)
{
	w -= p.w;
	x -= p.x;
	y -= p.y;
	z -= p.z;
	return *this;
}

/**
 *	クォータニオンの乗算
 */
Quaternion Quaternion::operator * (const Quaternion& p)
{
	Quaternion _q;
	QuaternionMultiply(_q, *this, p);
	return _q;
}

/**
 *	クォータニオンの乗算代入
 */
Quaternion& Quaternion::operator *= (const Quaternion& p)
{
	Quaternion _q;
	QuaternionMultiply(_q, *this, p);
	*this = _q;
	return *this;
}

/**
 *	クォータニオンの長さを取得する
 *
 *	|Q| = sqrt(Qw^2 + Qx^2 + Qy^2 + Qz^2)
 *	@return	クォータニオンの長さ
 */
float Quaternion::Length() const
{
	return sqrt(LengthSq());
}

/**
 *	クォータニオンの長さの二乗を取得する
 *
 *	Qw^2 + Qx^2 + Qy^2 + Qz^2
 *	@return	クォータニオンの長さの二乗
 */
float Quaternion::LengthSq() const
{
	return w*w + x*x + y*y + z*z;
}


/**
 *	クォータニオンを正規化する
 */
void Quaternion::Normalize()
{
	float len = Length();
	assert(len != 0.0f);
	w /= len;
	x /= len;
	y /= len;
	z /= len;
}

/**
 *	スカラーとクォータニオンの積
 *
 *	cQ = cQw + cQxi + cQyj + cQzk
 */
Quaternion operator * (float c, const Quaternion& p)
{
	return Quaternion(c*p.w, c*p.x, c*p.y, c*p.z);
}

/**
 *	クォータニオンの積を計算する
 *
 *	Q*P = (Qw+Qxi+Qyj+Qzk)(Pw+Pxi+Pyj+Pzk)
 *		= (QwPw-QxPx-QyPy-QzPz) + (QwPx+PwQx+PyQz-PzQy)i
 *		  (QwPy+PwQy+PzQx-PxQz)j+ (QwPz+PwQz+PxQy-PyQx)k
 *	@param	[out] _q	積
 *	@param	q	クォータニオン
 *	@param	p	かけるクォータニオン
 */
void QuaternionMultiply(Quaternion& _q, const Quaternion& q, const Quaternion& p)
{
	_q.w = q.w*p.w - q.x*p.x - q.y*p.y - q.z*p.z;
	_q.x = q.w*p.x + p.w*q.x + p.y*q.z - p.z*q.y;
	_q.y = q.w*p.y + p.w*q.y + p.z*q.x - p.x*q.z;
	_q.z = q.w*p.z + p.w*q.z + p.x*q.y - p.y*q.x;
}


/**
 *	単位クォータニオンを取得する
 *
 *	Q = (1; 0, 0, 0)
 *	@param	[out] _q	単位クォータニオン
 */
void QuaternionIdentity(Quaternion& _q)
{
	_q.w = 1.0f;
	_q.x = 0.0f;
	_q.y = 0.0f;
	_q.z = 0.0f;
}


/**
 *	クォータニオンの共役を取得する
 *
 *	Q* = Qw - Qx - Qy - Qz
 *
 *	@param	[out] _q	共役クォータニオン
 *	@param	q	クォータニオン
 */
void QuaternionConjugate(Quaternion& _q, const Quaternion& q)
{
	_q.w = q.w;
	_q.x = -q.x;
	_q.y = -q.y;
	_q.z = -q.z;
}

/**
 *	逆クォータニオンを取得する
 *
 *	Qの共役をQ*, 長さを|Q|とした時、
 *	逆クォータニオンQ^-1は、
 *
 *	Q^-1 = Q* / |Q|^2
 *		 = (w; -V) |Q|^2
 *		 = (w/|Q|^2; x/|Q|^2, y/|Q|^2, z/|Q|^2)
 *
 *	@param	[out] _q	逆クォータニオン
 *	@param	q	クォータニオン
 */
void QuaternionInverse(Quaternion& _q, const Quaternion& q)
{
	float ql = q.LengthSq();
	assert(ql != 0.0f);
	_q.w = q.w / ql;
	_q.x = -q.x / ql;
	_q.y = -q.y / ql;
	_q.z = -q.z / ql;
}



/**
 *	3Dベクトルを任意の軸周りに回転させるクォータニオンを取得する
 *
 *	任意の軸V周りにθ回転するクォータニオンQ(θ,V)は
 *	Q(θ,V) = cos(θ/2) + (Vxi + Vyj + Vzk)sin(θ/2)
 *
 *	@param	[out] _q	回転クォータニオン
 *	@param	theta	回転させる角度(ラジアン)
 *	@param	v		回転軸
 */
void QuaternionRotationAxis(Quaternion& _q, float theta, const Vector3& v)
{
	_q.w = cos(theta*0.5f);
	_q.x = v.x * sin(theta*0.5f);
	_q.y = v.y * sin(theta*0.5f);
	_q.z = v.z * sin(theta*0.5f);
}



/**
 *	クォータニオンから4x4回転行列を取得する
 *	
 *	|1-2(yy+zz), 2(xy+zw),   2(zx-wy),   0|
 *	|2(xy-zw),   1-2(zz+xx), 2(yz+xw),   0|
 *	|2(zx+wy),   2(yz-wx),   1-2(xx+yy), 0|
 *	|0,			 0,			 0,			 1|
 *
 *	@param	[out] _m	回転行列
 *	@param	q	クォータニオン
 */
void Matrix4RotationQuaternion(Matrix4& _m, const Quaternion& q)
{
	Matrix4Identity(_m);

	_m(0,0) = 1.0f - 2.0f*(q.y*q.y + q.z*q.z);
	_m(0,1) = 2.0f*(q.x*q.y + q.z*q.w);
	_m(0,2) = 2.0f*(q.z*q.x - q.w*q.y);
	
	_m(1,0) = 2.0f*(q.x*q.y - q.z*q.w);
	_m(1,1) = 1.0f - 2.0f*(q.z*q.z + q.x*q.x);
	_m(1,2) = 2.0f*(q.y*q.z + q.x*q.w);

	_m(2,0) = 2.0f*(q.z*q.x + q.w*q.y);
	_m(2,1) = 2.0f*(q.y*q.z - q.w*q.x);
	_m(2,2) = 1.0f - 2.0f*(q.x*q.x + q.y*q.y);
}



