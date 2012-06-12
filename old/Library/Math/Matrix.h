/**
 *	Math/Matrix.h
 *
 *	行列クラス。
 *	主に勉強用なので速度は重視してない。
 *	ベクトルクラスも使うのでインクルードする事。
 *
 *	@author	Dila
 */

#pragma once

#include <math.h>
#include <assert.h>

/* ------------------------------------------------------------------------- */
// Matrix2

/**
 *	Matrix2
 *	２＊２行列クラス。
 *
 *	Matrix2 m;
 *	m(0,1) = 2;
 */
class Matrix2 {
public:
	union {
		struct { float a, b, c, d; };	// 数学の教科書にあわせて作っただけ
		struct { float p, q, r, s; };	// 同上
		float m[2][2];
	};

	// 要素参照	
	float& operator () (int r, int c) {
		assert((0 <= r && r <= 1) || (0 <= c && c <= 1));
		return m[r][c];
	}
	float operator () (int r, int c) const {
		assert((0 <= r && r <= 1) || (0 <= c && c <= 1));
		return m[r][c];
	}

	// 演算
	Matrix2 operator + (const Matrix2& n);
	Matrix2 operator - (const Matrix2& n);
	Matrix2& operator += (const Matrix2& n);
	Matrix2& operator -= (const Matrix2& n);

	Matrix2 operator * (const Matrix2& n);
	Matrix2& operator *= (const Matrix2& n);
};

// 実数＊行列
Matrix2 operator * (float k, const Matrix2& m);

// 行ベクトル＊行列
Vector2 operator * (const Vector2& v, const Matrix2& m);

// 行列＊列ベクトル
Vector2 operator * (const Matrix2& m, const Vector2& v);


// 行ベクトルと列ベクトルの積
float Vector2Multiply(const Vector2& v, const Vector2 u);

// 行ベクトルと行列の積
void Vector2Multiply(Vector2& _v, const Vector2& v, const Matrix2& m);

// 行列と列ベクトルの積
void Matrix2Multiply(Vector2& _v, const Matrix2& m, const Vector2& v);

// 行列の積
void Matrix2Multiply(Matrix2& _m, const Matrix2& m, const Matrix2& n);


// 零行列
void Matrix2Zero(Matrix2& _m);

// 単位行列
void Matrix2Identity(Matrix2& _m);

// 行列式
float Matrix2Determinant(const Matrix2& m);

// 逆行列
void Matrix2Inverse(Matrix2& _m, const Matrix2& m);

// ２次元ベクトルを回転させる行列
void Matrix2Rotation(Matrix2& _m, float theta);

// ２次元ベクトルを拡大させる行列
void Matrix2Scaling(Matrix2& _m, float sx, float sy);


/* ------------------------------------------------------------------------- */
// Matrix3

/**
 *	Matrix3
 *	３＊３行列クラス。
 */
class Matrix3 {
public:
	float m[3][3];

	// 要素参照
	float& operator () (int r, int c) {
		assert((0 <= r && r <= 2) || (0 <= c && c <= 2));
		return m[r][c];
	}
	float operator () (int r, int c) const {
		assert((0 <= r && r <= 2) || (0 <= c && c <= 2));
		return m[r][c];
	}


	// 演算
	Matrix3 operator + (const Matrix3& n);
	Matrix3 operator - (const Matrix3& n);
	Matrix3& operator += (const Matrix3& n);
	Matrix3& operator -= (const Matrix3& n);

	Matrix3 operator * (const Matrix3& n);
	Matrix3& operator *= (const Matrix3& n);
};

// 実数＊行列
Matrix3 operator * (float k, const Matrix3& m);

// 行ベクトル＊行列
Vector3 operator * (const Vector3& v, const Matrix3& m);

// 行列＊列ベクトル
Vector3 operator * (const Matrix3& m, const Vector3& v);



// 行ベクトルと列ベクトルの積
float Vector3Multiply(const Vector3& v, const Vector3& u);

// 行ベクトルと行列の積
void Vector3Multiply(Vector3& _v, const Vector3& v, const Matrix3& m);

// 行列と列ベクトルの積
void Matrix3Multiply(Vector3& _v, const Matrix3& m, const Vector3& v);

// 行列の積
void Matrix3Multiply(Matrix3& _m, const Matrix3& m, const Matrix3& n);


// 零行列
void Matrix3Zero(Matrix3& _m);

// 単位行列
void Matrix3Identity(Matrix3& _m);
// 行列式
float Matrix3Determinant(const Matrix3& m);

// 行列のi行j列を除いた行列
void Matrix3Tilde(Matrix2& _m, const Matrix3& m, int i, int j);

// 逆行列
void Matrix3Inverse(Matrix3& _m, const Matrix3& m);


// ３次元ベクトルをX軸周りに回転させる行列
void Matrix3RotationX(Matrix3& _m, float theta);

// ３次元ベクトルをY軸周りに回転させる行列
void Matrix3RotationY(Matrix3& _m, float theta);

// ３次元ベクトルをZ軸周りに回転させる行列
void Matrix3RotationZ(Matrix3& _m, float theta);

// ３次元ベクトルを任意の軸周りに回転させる行列
void Matrix3RotationAxis(Matrix3& _m, float theta, const Vector3& v);

// ３次元ベクトルを拡大させる行列
void Matrix3Scaling(Matrix3& _m, float sx, float sy, float sz);


// ２次元同次ベクトルを回転させる行列 (HV = Homogeneous Vector = 同次ベクトル)
void Matrix3RotationHV(Matrix3& _m, float theta);

// ２次元同次ベクトルを平行移動させる行列
void Matrix3TranslationHV(Matrix3& _m, float x, float y);



/* ------------------------------------------------------------------------- */
// Matrix4

/**
 *	Matrix4
 *	４＊４行列クラス。
 */
class Matrix4 {
public:
	float m[4][4];

	// 要素参照
	float& operator () (int r, int c) {
		assert((0 <= r && r <= 3) || (0 <= c && c <= 3));
		return m[r][c];
	}
	float operator () (int r, int c) const {
		assert((0 <= r && r <= 3) || (0 <= c && c <= 3));
		return m[r][c];
	}


	// 演算
	Matrix4 operator + (const Matrix4& n);
	Matrix4 operator - (const Matrix4& n);

	Matrix4& operator += (const Matrix4& n);
	Matrix4& operator -= (const Matrix4& n);

	Matrix4 operator * (const Matrix4& n);
	Matrix4& operator *= (const Matrix4& n);
};

// 実数＊行列
Matrix4 operator * (float k, const Matrix4& m);

// 行ベクトル＊行列
Vector4 operator * (const Vector4& v, const Matrix4& m);

// 行列＊列ベクトル
Vector4 operator * (const Matrix4& m, const Vector4& v);


// 零行列
void Matrix4Zero(Matrix4& _m);

// 単位行列
void Matrix4Identity(Matrix4& _m);

// 行ベクトルと列ベクトルの積
float Vector4Multiply(const Vector4& v, const Vector3& u);

// 行ベクトルと行列の積
void Vector4Multiply(Vector4& _v, const Vector4& v, const Matrix4& m);

// 行列と列ベクトルの積
void Matrix4Multiply(Vector4& _v, const Matrix4& m, const Vector4& v);

// 行列の積
void Matrix4Multiply(Matrix4& _m, const Matrix4& m, const Matrix4& n);

// 行列式
float Matrix4Determinant(const Matrix4& m);

// 行列のi行j列を除いた行列
void Matrix4Tilde(Matrix3& _m, const Matrix4& m, int i, int j);

// 逆行列
void Matrix4Inverse(Matrix4& _m, const Matrix4& m);


// ↓４次元ベクトルについて使う事は少ないのでHVは省略･･･

// ３次元同次ベクトルをX軸周りに回転させる行列
void Matrix4RotationX(Matrix4& _m, float theta);

// ３次元同次ベクトルをY軸周りに回転させる行列
void Matrix4RotationY(Matrix4& _m, float theta);

// ３次元同次ベクトルをZ軸周りに回転させる行列
void Matrix4RotationZ(Matrix4& _m, float theta);

// ３次元同次ベクトルを任意の軸周りに回転させる行列
void Matrix4RotationAxis(Matrix4& _m, float theta, const Vector3& v);

// ３次元同次ベクトルを任意のベクトルの向きに回転させる行列
void Matrix4RotationVector(Matrix4& _m, const Vector3& v);

// ３次元同次ベクトルを拡大させる行列
void Matrix4Scaling(Matrix4& _m, float sx, float sy, float sz);

// ３次元同次ベクトルを平行移動させる行列
void Matrix4Translation(Matrix4& _m, float x, float y, float z);

// ３次元同次ベクトルをカメラ座標系に変換する行列
void Matrix4View(Matrix4& _m, const Vector3& eye, const Vector3& at, const Vector3& up);

// ３次元同次ベクトルを透視射影変換する行列
void Matrix4Projection(Matrix4& _m, float fovV, float aspect, float zn, float zf);

// 透視射影した３次元同次ベクトルをスクリーン座標に変換する行列
void Matrix4Screen(Matrix4& _m, float width, float height);

