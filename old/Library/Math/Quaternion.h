/**
 *	@file Math/Quaternion.h
 *
 *	クォータニオン(四元数)クラス
 *
 *	主に勉強用なので速度は重視してない。
 *	ベクトル、行列クラスも使うのでインクルードする事。
 *
 *	クォータニオンの表記
 *	Q = (w; x, y, z) = (w; V) = w + xi + yj + zk
 *
 *	クォータニオンによる回転
 *	軸V周りにθ回転するクォータニオンをQとし、
 *	回転させるベクトルをクォータニオンP = (0; x,y,z)で表した場合、
 *	回転後のベクトルを表すクォータニオンP'は、
 *	Qの共役Rとサンドイッチに掛け合わせて計算する。
 *	P' = R*P*Q
 *
 *	@see Vector3
 *	@see Matrix4
 *	@author	Dila
 */

 /// @addtogroup Math
 /// @{

#pragma once

#include <math.h>
#include <assert.h>


/**
 *	クォータニオンクラス
 */
class Quaternion
{
public:
	float w;	/// w要素
	float x;	/// x要素
	float y;	/// y要素
	float z;	/// z要素

	Quaternion() {
		w = x = y = z = 0.0f;
	}
	Quaternion(float w, float x, float y, float z) {
		this->w = w;
		this->x = x;
		this->y = y;
		this->z = z;
	};
	Quaternion(float w, const Vector3& v) {
		this->w = w;
		x = v.x;
		y = v.y;
		z = v.z;
	};

	// クォータニオンの加算
	Quaternion operator + (const Quaternion& p);

	// クォータニオンの減算
	Quaternion operator - (const Quaternion& p);

	// クォータニオンの加算代入
	Quaternion& operator += (const Quaternion& p);

	// クォータニオンの減算代入
	Quaternion& operator -= (const Quaternion& p);

	// クォータニオンの乗算
	Quaternion operator * (const Quaternion& p);

	// クォータニオンの乗算代入
	Quaternion& operator *= (const Quaternion& p);

	// クォータニオンの長さを返す
	float Length() const;

	// クォータニオンの長さの二乗を返す
	float LengthSq() const;

	// クォータニオンを正規化する
	void Normalize();
};

// スカラーとクォータニオンの積を計算する
Quaternion operator * (float c, const Quaternion& q);

// クォータニオンの積を計算する
void QuaternionMultiply(Quaternion& _q, const Quaternion& q, const Quaternion& p);


// 単位クォータニオン
void QuaternionIdentity(Quaternion& _q);

// 共役クォータニオン
void QuaternionConjugate(Quaternion& _q, const Quaternion& q);

// 逆クォータニオン
void QuaternionInverse(Quaternion& _q, const Quaternion& q);



// ３次元ベクトルを任意の軸周りに回転させるクォータニオン
void QuaternionRotationAxis(Quaternion& _q, float theta, const Vector3& v);


// クォータニオンから行列
void Matrix4RotationQuaternion(Matrix4& _m, const Quaternion& q);


/// @}
