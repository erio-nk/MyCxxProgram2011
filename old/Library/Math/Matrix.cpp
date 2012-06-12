/**
 *	Math/Matrix.cpp
 *
 *	@author	Dila
 */

#include "Math/Vector.h"
#include "Math/Matrix.h"


/* ------------------------------------------------------------------------- */
// Matrix2

/**
 *	行列の演算
 */

Matrix2 Matrix2::operator + (const Matrix2& n)
{
	Matrix2 _m;
	_m.a = a + n.p;
	_m.b = b + n.q;
	_m.c = c + n.r;
	_m.d = d + n.s;
	return _m;
}

Matrix2 Matrix2::operator - (const Matrix2& n)
{
	Matrix2 _m;
	_m.a = a - n.p;
	_m.b = b - n.q;
	_m.c = c - n.r;
	_m.d = d - n.s;
	return _m;
}

Matrix2& Matrix2::operator += (const Matrix2& n)
{
	a += n.p;
	b += n.q;
	c += n.r;
	d += n.s;
	return *this;
}

Matrix2& Matrix2::operator -= (const Matrix2& n)
{
	a -= n.p;
	b -= n.q;
	c -= n.r;
	d -= n.s;
	return *this;
}

Matrix2 Matrix2::operator * (const Matrix2& n)
{
	Matrix2 _m;
	_m.a = a*n.p + b*n.r;
	_m.b = a*n.q + b*n.s;
	_m.c = c*n.p + d*n.r;
	_m.d = c*n.q + d*n.s;
	return _m;
}

Matrix2& Matrix2::operator *= (const Matrix2& n)
{
	Matrix2 _m = *this;
	a = _m.a*n.p + _m.b*n.r;
	b = _m.a*n.q + _m.b*n.s;
	c = _m.c*n.p + _m.d*n.r;
	d = _m.c*n.q + _m.d*n.s;
	return *this;
}

/**
 *	実数＊行列
 */
Matrix2 operator * (float k, const Matrix2& m)
{
	Matrix2 _m;
	_m.a = k * m.a;
	_m.b = k * m.b;
	_m.c = k * m.c;
	_m.d = k * m.d;
	return _m;
}


/**
 *	行ベクトル＊行列
 */
Vector2 operator * (const Vector2& v, const Matrix2& m) {
	Vector2 _v;
	Vector2Multiply(_v, v, m);
	return _v;
}

/**
 *	行列＊列ベクトル
 */
Vector2 operator * (const Matrix2& m, const Vector2& v)
{
	Vector2 _v;
	Matrix2Multiply(_v, m, v);
	return _v;
}

/**
 *	Vector2Multiply()
 *	行ベクトルと列ベクトルの積を計算する。
 *
 *	(Vx, Vy)|Ux| = Vx*Ux + VyUy = 内積 = V･U
 *			|Uy|
 *
 *	@param	v	行ベクトル
 *	@param	u	列ベクトル
 */
float Vector2Multiply(const Vector2& v, const Vector2& u)
{
	return v.x*u.x + v.y*u.y;
}


/**
 *	Vector2Multiply()
 *	行ベクトルと行列の積を計算する。
 *
 *	行列Mを列ベクトルの集合 = |U1, U2|
 *	とした時、
 *	VM = V|U1, U2| = (VU1, VU2)
 *
 *	行ベクトルVと列ベクトルUnの積は、V･Unの為、
 *	(VU1, VU2) = (V･U1, V･U2) = (VxU1x + VyU1y, VxU2x + VyU2y)
 *
 *	@param	_v	[out]積
 *	@param	v	行ベクトル
 *	@param	m	かける行列
 */
void Vector2Multiply(Vector2& _v, const Vector2& v, const Matrix2& m)
{
	_v.x = v.x*m.p + v.y*m.r;
	_v.y = v.x*m.q + v.y*m.s;
}


/**
 *	Matrix2Multiply()
 *	行列と列ベクトルの積を計算する。
 *
 *	行列Mを行ベクトルの集合 = |U1|
 *							  |U2|
 *	とした時、
 *	MV = |U1|V = |U1V|
 *		 |U2|	 |U2V|
 *
 *	行ベクトルUnと列ベクトルVの積は、Un･Vの為、
 *	|U1V| = |U1･V| = |U1xVx + U1yVy|
 *	|U2V|   |U2･V|   |U2xVx + U2yVy|
 *
 *	@param	_v	[out]積
 *	@param	m	行列
 *	@param	v	かける列ベクトル
 */
void Matrix2Multiply(Vector2& _v, const Matrix2& m, const Vector2& v)
{
	_v.x = m.a*v.x + m.b*v.y;
	_v.y = m.c*v.x + m.d*v.y;
}


/**
 *	Matrix2Multiply()
 *	行列の積を計算する。
 *	@param	_m	[out]積
 *	@param	m	行列
 *	@param	n	かける行列
 */
void Matrix2Multiply(Matrix2& _m, const Matrix2& m, const Matrix2 n)
{
	_m.a = m.a*n.p + m.b*n.r;
	_m.b = m.a*n.q + m.b*n.s;
	_m.c = m.c*n.p + m.d*n.r;
	_m.d = m.c*n.q + m.d*n.s;
}


/**
 *	Matrix2Zero()
 *	零行列を取得する。
 *
 *	|0, 0|
 *	|0, 0|
 *
 *	@param	_m		[out]零行列
 */
void Matrix2Zero(Matrix2& _m)
{
	_m.a = 0.0f;
	_m.b = 0.0f;
	_m.c = 0.0f;
	_m.d = 0.0f;
}


/**
 *	Matrix2Identity()
 *	単位行列を取得する。
 *
 *	|1, 0|
 *	|0, 1|
 *
 *	@param	_m		[out]単位行列
 */
void Matrix2Identity(Matrix2& _m)
{
	_m.a = 1.0f;
	_m.b = 0.0f;
	_m.c = 0.0f;
	_m.d = 1.0f;
}


/**
 *	Matrix2Determinant()
 *	行列式を計算する。
 *
 *	M = |a, b|
 *		|c, d|
 *	detM= ad - bc
 *
 *	@param	m	行列
 *	@return		行列式
 */
float Matrix2Determinant(const Matrix2& m)
{
	return m.a*m.d - m.b*m.c;
}

/**
 *	Matrix2Inverse()
 *	逆行列を計算する。
 *
 *	M	= |a, b|
 *		  |c, d|
 *	M^-1= 1 / detM * |d, -b|
 *					 |-c, a|
 *
 *	@param	_m	[out]逆行列
 *	@param	m	行列
 */
void Matrix2Inverse(Matrix2& _m, const Matrix2& m)
{
	float d = Matrix2Determinant(m);
	assert(d != 0.0f);
	d = 1.0f / d;
	_m.a = d * m.d;
	_m.b = d * -m.b;
	_m.c = d * -m.c;
	_m.d = d * m.a;
}



/**
 *	Matrix2Rotation()
 *	２次元ベクトルを回転する行列を取得する。
 *
 *	|cosθ,  sinθ|
 *	|-sinθ, cosθ|
 *
 *	@param	_m		[out]回転行列
 *	@param	theta	角度(ラジアン)
 */
void Matrix2Rotation(Matrix2& _m, float theta)
{
	_m.a = cos(theta);
	_m.b = sin(theta);
	_m.c = -_m.b;
	_m.d = _m.a;
}

/**
 *	Matrix2Scaling()
 *	２次元ベクトルを拡大する行列を取得する。
 *
 *	|sx,  0|
 *	| 0, sy|
 *
 *	@param	_m	[out]拡大行列
 *	@param	sx	x座標の拡大係数
 *	@param	sy	y座標の拡大係数
 */
void Matrix2Scaling(Matrix2& _m, float sx, float sy)
{
	_m.a = sx;
	_m.b = 0;
	_m.c = 0;
	_m.d = sy;
}


/* ------------------------------------------------------------------------- */
// Matrix3

/**
 *	行列の演算
 */

Matrix3 Matrix3::operator + (const Matrix3& n)
{
	Matrix3 _m;
	_m(0,0) = m[0][0] + n(0,0);
	_m(0,1) = m[0][1] + n(0,1);
	_m(0,2) = m[0][2] + n(0,2);
	
	_m(1,0) = m[1][0] + n(1,0);
	_m(1,1) = m[1][1] + n(1,1);
	_m(1,2) = m[1][2] + n(1,2);
	
	_m(2,0) = m[2][0] + n(2,0);
	_m(2,1) = m[2][1] + n(2,1);
	_m(2,2) = m[2][2] + n(2,2);
	return _m;
}

Matrix3 Matrix3::operator - (const Matrix3& n)
{
	Matrix3 _m;
	_m(0,0) = m[0][0] - n(0,0);
	_m(0,1) = m[0][1] - n(0,1);
	_m(0,2) = m[0][2] - n(0,2);
	
	_m(1,0) = m[1][0] - n(1,0);
	_m(1,1) = m[1][1] - n(1,1);
	_m(1,2) = m[1][2] - n(1,2);
	
	_m(2,0) = m[2][0] - n(2,0);
	_m(2,1) = m[2][1] - n(2,1);
	_m(2,2) = m[2][2] - n(2,2);
	return _m;
}

Matrix3& Matrix3::operator += (const Matrix3& n)
{
	m[0][0] += n(0,0);
	m[0][1] += n(0,1);
	m[0][2] += n(0,2);
	
	m[1][0] += n(1,0);
	m[1][1] += n(1,1);
	m[1][2] += n(1,2);
	
	m[2][0] += n(2,0);
	m[2][1] += n(2,1);
	m[2][2] += n(2,2);
	return *this;
}

Matrix3& Matrix3::operator -= (const Matrix3& n)
{
	m[0][0] -= n(0,0);
	m[0][1] -= n(0,1);
	m[0][2] -= n(0,2);
	
	m[1][0] -= n(1,0);
	m[1][1] -= n(1,1);
	m[1][2] -= n(1,2);
	
	m[2][0] -= n(2,0);
	m[2][1] -= n(2,1);
	m[2][2] -= n(2,2);
	return *this;
}


/**
 *	行列の積
 */
Matrix3 Matrix3::operator * (const Matrix3& n)
{
	Matrix3 _m;
	Matrix3Multiply(_m, *this, n);
	return _m;
}

Matrix3& Matrix3::operator *= (const Matrix3& n)
{
	Matrix3 _m = *this;
	Matrix3Multiply(*this, _m, n);
	return *this;
}

/**
 *	実数＊行列
 */
Matrix3 operator * (float k, const Matrix3& m)
{
	Matrix3 _m;
	_m(0,0) = k * m(0,0);
	_m(0,1) = k * m(0,1);
	_m(0,2) = k * m(0,2);

	_m(1,0) = k * m(1,0);
	_m(1,1) = k * m(1,1);
	_m(1,2) = k * m(1,2);

	_m(2,0) = k * m(2,0);
	_m(2,1) = k * m(2,1);
	_m(2,2) = k * m(2,2);
	return _m;
}


/**
 *	行ベクトル＊行列
 */
Vector3 operator * (const Vector3& v, const Matrix3& m)
{
	Vector3 _v;
	Vector3Multiply(_v, v, m);
	return _v;
}

/**
 *	行列＊列ベクトル
 */
Vector3 operator * (const Matrix3& m, const Vector3& v)
{
	Vector3 _v;
	Matrix3Multiply(_v, m, v);
	return _v;
}


/**
 *	Vector3Multiply()
 *	行ベクトルと列ベクトルの積を計算する。
 *
 *	(Vx, Vy, Vz)|Ux| = Vx*Ux + VyUy + VzUz = 内積 = V･U
 *				|Uy|
 *				|Uz|
 *
 *	@param	v	行ベクトル
 *	@param	r	列ベクトル
 */
float Vector3Multiply(const Vector3& v, const Vector3& u)
{
	return v.x*u.x + v.y*u.y + v.z*u.z;
}


/**
 *	Vector3Multiply()
 *	行ベクトルと行列の積を計算する。
 *
 *	行列Mを列ベクトルの集合 = |U1, U2, U3|
 *	とした時、
 *	VM = V|U1, U2, U3| = (VU1, VU2, VU3)
 *
 *	行ベクトルVと列ベクトルUnの積は、V･Unの為、
 *	(VU1, VU2, VU3) = (V･U1, V･U2, V･U3)
 *		= (VxU1x + VyU1y + VzU1z,
 *		   VxU2x + VyU2y + VzU2z,
 *		   VxU3x + VyU3y + VzU3z)
 *		= (VxM[0,0] + VyM[1,0] + VzM[2,0],
 *		   VxM[0,1] + VyM[1,1] + VzM[2,1],
 *		   VxM[0,2] + VyM[1,2] + VzM[2,2])
 *	@param	_v	[out]積
 *	@param	v	行ベクトル
 *	@param	m	かける行列
 */
void Vector3Multiply(Vector3& _v, const Vector3& v, const Matrix3& m)
{
	_v.x = v.x*m(0,0) + v.y*m(1,0) + v.z*m(2,0);
	_v.y = v.x*m(0,1) + v.y*m(1,1) + v.z*m(2,1);
	_v.z = v.x*m(0,2) + v.y*m(1,2) + v.z*m(2,2);
}


/**
 *	Matrix3Multiply()
 *	行列と列ベクトルの積を計算する。
 *
 *	行列Mを行ベクトルの集合 = |U1|
 *							  |U2|
 *							  |U3|
 *	とした時、
 *	MV = |U1|V = |U1V|
 *		 |U2|	 |U2V|
 *		 |U3|	 |U3V|
 *
 *	行ベクトルUnと列ベクトルVの積は、Un･Vの為、
 *	|U1V| = |U1･V| = |U1xVx + U1yVy + U1zVz|
 *	|U2V|   |U2･V|   |U2xVx + U2yVy + U2zVz|
 *	|U3V|   |U3･V|   |U3xVx + U3yVy + U3zVz|
 *		  = |M[0,0]Vx + M[0,1]Vy + M[0,2]Vz|
 *		    |M[1,0]Vx + M[1,1]Vy + M[1,2]Vz|
 *		    |M[2,0]Vx + M[2,1]Vy + M[2,2]Vz|
 *
 *	@param	_v	[out]積
 *	@param	m	行列
 *	@param	v	かける列ベクトル
 */
void Matrix3Multiply(Vector3& _v, const Matrix3& m, const Vector3& v)
{
	_v.x = m(0,0)*v.x + m(0,1)*v.y + m(0,2)*v.z;
	_v.y = m(1,0)*v.x + m(1,1)*v.y + m(1,2)*v.z;
	_v.z = m(2,0)*v.x + m(2,1)*v.y + m(2,2)*v.z;
}


/**
 *	Matrix3Multiply()
 *	行列の積を計算する。
 *
 *	行列Mを行ベクトルの集合 = |V1|
 *							  |V2|
 *							  |V3|
 *	行列Nを列ベクトルの集合 = (U1, U2, U3)
 *	とした時、
 *	MN = |V1U1, V1U2, V1U3|
 *		 |V2U1, V2U2, V2U3|
 *		 |V3U1, V3U2, V3U3|
 *	行ベクトルVと列ベクトルUnの積は、V･Unの為、
 *	   = |V1･U1, V1･U2, V1･U3|
 *		 |V2･U1, V2･U2, V2･U3|
 *		 |V3･U1, V3･U2, V3･U3|
 *	   = |V1xU1x+V1yU1y+V1zU1z, V1xU2x+V1yU2y+V1zU2z, V1xU3x+V1yU3y+V1zU3z|
 *		 |V2xU1x+V2yU1y+V2zU1z, V2xU2x+V2yU2y+V2zU2z, V2xU3x+V2yU3y+V2zU3z|
 *		 |V3xU1x+V3yU1y+V3zU1z, V3xU2x+V3yU2y+V3zU2z, V3xU3x+V3yU3y+V3zU3z|
 *	   = |M[0,0]N[0,0]+M[0,1]N[1,0]+M[0,2]N[2,0], M[0,0]N[0,1]+M[0,1]N[1,1]+M[0,2]N[2,1], M[0,0]N[0,2]+M[0,1]N[1,2]+M[0,2]N[2,2]|
 *		 |M[1,0]N[0,0]+M[1,1]N[1,0]+M[1,2]N[2,0], M[1,0]N[0,1]+M[1,1]N[1,1]+M[1,2]N[2,1], M[1,0]N[0,2]+M[1,1]N[1,2]+M[1,2]N[2,2]|
 *		 |M[2,0]N[0,0]+M[2,1]N[1,0]+M[2,2]N[2,0], M[2,0]N[0,1]+M[2,1]N[1,1]+M[2,2]N[2,1], M[2,0]N[0,2]+M[2,1]N[1,2]+M[2,2]N[2,2]|
 *	@param	_m	[out]積
 *	@param	m	行列
 *	@param	n	かける行列
 */
void Matrix3Multiply(Matrix3& _m, const Matrix3& m, const Matrix3& n)
{	
	/*int i, j, k;
	for (i = 0; i < 3; ++i) {
		for (j = 0; j < 3; ++j) {
			_m(i,j) = 0;
			for (k = 0; k < 3; ++k) {
				_m(i,j) += m[i][k]*n(k,j);
			}
		}
	}*/

	// 展開ver.
	//(i,j)      (i,k)  (k,j)    (i,k)  (k,j)    (i,k)  (k,j)
	_m(0,0) =	m(0,0)*n(0,0) + m(0,1)*n(1,0) + m(0,2)*n(2,0);
	_m(0,1) =	m(0,0)*n(0,1) + m(0,1)*n(1,1) + m(0,2)*n(2,1);
	_m(0,2) =	m(0,0)*n(0,2) + m(0,1)*n(1,2) + m(0,2)*n(2,2);
	
	_m(1,0) =	m(1,0)*n(0,0) + m(1,1)*n(1,0) + m(1,2)*n(2,0);
	_m(1,1) =	m(1,0)*n(0,1) + m(1,1)*n(1,1) + m(1,2)*n(2,1);
	_m(1,2) =	m(1,0)*n(0,2) + m(1,1)*n(1,2) + m(1,2)*n(2,2);

	_m(2,0) =	m(2,0)*n(0,0) + m(2,1)*n(1,0) + m(2,2)*n(2,0);
	_m(2,1) =	m(2,0)*n(0,1) + m(2,1)*n(1,1) + m(2,2)*n(2,1);
	_m(2,2) =	m(2,0)*n(0,2) + m(2,1)*n(1,2) + m(2,2)*n(2,2);
}


/**
 *	Matrix3Zero()
 *	零行列を取得する。
 *
 *	|0, 0, 0|
 *	|0, 0, 0|
 *	|0, 0, 0|
 *
 *	@param	_m		[out]零行列
 */
void Matrix3Zero(Matrix3& _m)
{
	_m(0,0) = 0.0f;
	_m(0,1) = 0.0f;
	_m(0,2) = 0.0f;

	_m(1,0) = 0.0f;
	_m(1,1) = 0.0f;
	_m(1,2) = 0.0f;

	_m(2,0) = 0.0f;
	_m(2,1) = 0.0f;
	_m(2,2) = 0.0f;
}


/**
 *	Matrix3Identity()
 *	単位行列を取得する。
 *
 *	|1, 0, 0|
 *	|0, 1, 0|
 *	|0, 0, 1|
 *
 *	@param	_m		[out]単位行列
 */
void Matrix3Identity(Matrix3& _m)
{
	_m(0,0) = 1.0f;
	_m(0,1) = 0.0f;
	_m(0,2) = 0.0f;

	_m(1,0) = 0.0f;
	_m(1,1) = 1.0f;
	_m(1,2) = 0.0f;

	_m(2,0) = 0.0f;
	_m(2,1) = 0.0f;
	_m(2,2) = 1.0f;
}

/**
 *	Matrix3Determinant()
 *	Matrix3の行列式を計算する。
 *
 *	M	= |a, b, c|
 *		  |d, e, f|
 *		  |g, h, i|
 *
 *	行列式detMは
 *	detM= aei + dhc + gbf - ahf - dbi - gec
 *
 *	@param	m	Matrix3
 *	@return		行列式
 */
float Matrix3Determinant(const Matrix3& m)
{
	float a = m(0,0)*m(1,1)*m(2,2);
	float b = m(1,0)*m(2,1)*m(0,2);
	float c = m(2,0)*m(0,1)*m(1,2);
	float d = m(0,0)*m(2,1)*m(1,2);
	float e = m(1,0)*m(0,1)*m(2,2);
	float f = m(2,0)*m(1,1)*m(0,2);
	return m(0,0)*m(1,1)*m(2,2) + m(1,0)*m(2,1)*m(0,2) + m(2,0)*m(0,1)*m(1,2)
		   - m(0,0)*m(2,1)*m(1,2) - m(1,0)*m(0,1)*m(2,2) - m(2,0)*m(1,1)*m(0,2);
}


/**
 *	Matrix3Tilde()
 *	行列のi行j列を除いた行列を取得する。
 *
 *	Mからi行j列を取り去った行列をM~ijとする(↓は例)
 *	M~12 = |a21, a23|
 *		   |a31, a33|
 *	
 *	@param	_m	[out]行列
 *	@param	m	行列
 *	@param	i	取り除く行
 *	@param	j	取り除く列
 */
void Matrix3Tilde(Matrix2& _m, const Matrix3& m, int i, int j)
{
	int I, J;
	int a = 0, b;
	for (I = 0; I < 3; ++I) {
		if (I == i) continue;
		b = 0;
		for (J = 0; J < 3; ++J) {
			if (J == j) continue;
			_m(a,b) = m(I,J);
			++b;
		}
		++a;
	}
}


/**
 *	Matrix3Inverse()
 *	逆行列を計算する。
 *
 *	M	= |a11, a12, a13|
 *		  |a21, a22, a23|
 *		  |a31, a32, a33|
 *
 *	Mからi行j列を取り去った行列をM~ijとする(↓は例)
 *	M~12 = |a21, a23|
 *		   |a31, a33|
 *
 *	逆行列M^-1は
 *	M^-1= 1 / detM * | detM~11, -detM~21,  detM~31|
 *					 |-detM~12,  detM~22, -detM~32|
 *					 | detM~13, -detM~23,  detM~33|
 *		= 1 / detM * |a22a33-a23a32, a32a13-a33a12, a12a23-a13a22|
 *					 |a23a31-a21a33, a33a11-a31a13, a13a21-a11a23|
 *					 |a21a32-a22a31, a31a12-a32a11, a11a22-a12a21|
 *
 *	@param	_m	[out]逆行列
 *	@param	m	行列
 */
void Matrix3Inverse(Matrix3& _m, const Matrix3& m)
{
	float d = Matrix3Determinant(m);
	assert(d != 0.0f);
	d = 1.0f / d;
	
	/*for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			Matrix2 n;
			Matrix3Tilde(n, m, j, i);
			_m(i,j) = (((i+j)&1)==0?1:-1) * d * Matrix2Determinant(n);
		}
	}*/

	// 展開ver.
	_m(0,0) = d * (m(1,1)*m(2,2)-m(1,2)*m(2,1));
	_m(0,1) = d * (m(2,1)*m(0,2)-m(2,2)*m(0,1));
	_m(0,2) = d * (m(0,1)*m(1,2)-m(0,2)*m(1,1));

	_m(1,0) = d * (m(1,2)*m(2,0)-m(1,0)*m(2,2));
	_m(1,1) = d * (m(2,2)*m(0,0)-m(2,0)*m(0,2));
	_m(1,2) = d * (m(0,2)*m(1,0)-m(0,0)*m(1,2));

	_m(2,0) = d * (m(1,0)*m(2,1)-m(1,1)*m(2,0));
	_m(2,1) = d * (m(2,0)*m(0,1)-m(2,1)*m(0,0));
	_m(2,2) = d * (m(0,0)*m(1,1)-m(0,1)*m(1,0));
}

/**
 *	Matrix3RotationX()
 *	３次元ベクトルをX軸周りに回転させる行列を取得する。
 *
 *	|1, 0,		0    |
 *	|0, cosθ,  sinθ|
 *	|0, -sinθ, cosθ|
 *
 *	@param	_m		[out]回転行列
 *	@param	theta	角度(ラジアン)
 */
void Matrix3RotationX(Matrix3& _m, float theta)
{
	Matrix3Identity(_m);
	_m(1,1) = cos(theta);
	_m(1,2) = sin(theta);
	_m(2,1) = -_m(1,2);	// -sin(theta);
	_m(2,2) = _m(1,1);	// cos(theta);
}

/**
 *	Matrix3RotationY()
 *	３次元ベクトルをY軸周りに回転させる行列を取得する。
 *
 *	|cosθ, 0,	-sinθ|
 *	|0,		1,	0	  |
 *	|sinθ, 0,	cosθ |
 *
 *	@param	_m		[out]回転行列
 *	@param	theta	角度(ラジアン)
 */
void Matrix3RotationY(Matrix3& _m, float theta)
{
	Matrix3Identity(_m);
	_m(0,0) = cos(theta);
	_m(2,0) = sin(theta);
	_m(0,2) = -_m(2,0);	// -sin(theta);
	_m(2,2) = _m(0,0);	// cos(theta);
}


/**
 *	Matrix3RotationZ()
 *	３次元ベクトルをZ軸周りに回転させる行列を取得する。
 *
 *	|cosθ,  sinθ, 0|
 *	|-sinθ, cosθ, 0|
 *	|0,		 0,		1|
 *
 *	@param	_m		[out]回転行列
 *	@param	theta	角度(ラジアン)
 */
void Matrix3RotationZ(Matrix3& _m, float theta)
{
	Matrix3Identity(_m);
	_m(0,0) = cos(theta);
	_m(0,1) = sin(theta);
	_m(1,0) = -_m(0,1);	// -sin(theta);
	_m(1,1) = _m(0,0);	// cos(theta); 
}

/**
 *	Matrix3RotationAxis()
 *	３次元ベクトルを任意の軸周りに回転させる行列を取得する。
 *
 *	|c+(1-c)Vx^2,	(1-c)VxVy+sVz,	(1-c)VxVz-sVy|
 *	|(1-c)VxVy-sVz, c+(1-c)Vy^2,	(1-c)VyVz+sVx|
 *	|(1-c)VxVz+sVy,	(1-c)VyVz-sVx,	c+(1-c)Vz^2  |
 *	※cosθ = c, sinθ = s
 *
 *	@param	_m		[out]回転行列
 *	@param	theta	角度(ラジアン)
 *	@param	v		軸を表す３次元ベクトル(正規化済みのもの)
 */
void Matrix3RotationAxis(Matrix3& _m, float theta, const Vector3& v)
{
	Matrix3Identity(_m);
	float c = cos(theta);
	float s = sin(theta);
	
    _m(0,0) = c+(1.0f-c)*v.x*v.x;
    _m(0,1) = (1.0f-c)*v.x*v.y+s*v.z;
    _m(0,2) = (1.0f-c)*v.x*v.z-s*v.y;
	
    _m(1,0) = (1.0f-c)*v.x*v.y-s*v.z;
    _m(1,1) = c+(1.0f-c)*v.y*v.y;
    _m(1,2) = (1.0f-c)*v.y*v.z+s*v.x;
	
    _m(2,0) = (1.0f-c)*v.x*v.z+s*v.y;
    _m(2,1) = (1.0f-c)*v.y*v.z-s*v.x;
    _m(2,2) = c+(1.0f-c)*v.z*v.z;
}


/**
 *	Matrix3Scaling()
 *	３次元ベクトルを拡大する行列を取得する。
 *
 *	|sx,  0,  0|
 *	| 0, sy,  0|
 *	| 0,  0, sz|
 *
 *	@param	_m	[out]拡大行列
 *	@param	sx	x座標の拡大係数
 *	@param	sy	y座標の拡大係数
 *	@param	sz	z座標の拡大係数
 */
void Matrix3Scaling(Matrix3& _m, float sx, float sy, float sz)
{
	Matrix3Identity(_m);
	_m(0,0) = sx;
	_m(1,1) = sy;
	_m(2,2) = sz;
}




/**
 *	Matrix3RotationHV()
 *	２次元同次ベクトルを回転させる行列を取得する。
 *
 *	|cosθ,  sinθ, 0|
 *	|-sinθ, cosθ, 0|
 *	|0,		 0,		1|
 *
 *	Vector3 v(x, y, 1);	// zは1
 *	Matrix3 m;
 *	Matrix3Rotation4V2(m, θ);
 *	v = v * m;	// 回転させる
 *
 *	@param	_m		[out]回転行列
 *	@param	theta	角度(ラジアン)
 */
void Matrix3RotationHV(Matrix3& _m, float theta)
{
	Matrix3RotationZ(_m, theta);
}

/**
 *	Matrix3TranslationHV()
 *	２次元同次ベクトルを平行移動させる行列を取得する。
 *
 *	|1, 0, x|
 *	|0, 1, y|
 *	|0,	0, 1|
 *
 *	Vector3 v(x, y, 1);	// zは1
 *	Matrix3 m;
 *	Matrix3Translation4V2(m, θ);
 *	v = v * m;	// 平行移動させる
 *
 *	@param	_m		[out]平行移動行列
 *	@param	x, y	移動量
 */
void Matrix3TranslationHV(Matrix3& _m, float x, float y)
{
	Matrix3Identity(_m);
	_m(2,0) = x;
	_m(2,1) = y;
}



/* ------------------------------------------------------------------------- */
// Matrix4


/**
 *	演算
 */

Matrix4 Matrix4::operator + (const Matrix4& n)
{
	Matrix4 _m;
	_m(0,0) = m[0][0] + n(0,0);
	_m(0,1) = m[0][1] + n(0,1);
	_m(0,2) = m[0][2] + n(0,2);
	_m(0,3) = m[0][3] + n(0,3);
	
	_m(1,0) = m[1][0] + n(1,0);
	_m(1,1) = m[1][1] + n(1,1);
	_m(1,2) = m[1][2] + n(1,2);
	_m(1,3) = m[1][3] + n(1,3);
	
	_m(2,0) = m[2][0] + n(2,0);
	_m(2,1) = m[2][1] + n(2,1);
	_m(2,2) = m[2][2] + n(2,2);
	_m(2,3) = m[2][3] + n(2,3);

	_m(3,0) = m[3][0] + n(3,0);
	_m(3,1) = m[3][1] + n(3,1);
	_m(3,2) = m[3][2] + n(3,2);
	_m(3,3) = m[3][3] + n(3,3);
	return _m;
}

Matrix4 Matrix4::operator - (const Matrix4& n)
{
	Matrix4 _m;
	_m(0,0) = m[0][0] - n(0,0);
	_m(0,1) = m[0][1] - n(0,1);
	_m(0,2) = m[0][2] - n(0,2);
	_m(0,3) = m[0][3] - n(0,3);
	
	_m(1,0) = m[1][0] - n(1,0);
	_m(1,1) = m[1][1] - n(1,1);
	_m(1,2) = m[1][2] - n(1,2);
	_m(1,3) = m[1][3] - n(1,3);
	
	_m(2,0) = m[2][0] - n(2,0);
	_m(2,1) = m[2][1] - n(2,1);
	_m(2,2) = m[2][2] - n(2,2);
	_m(2,3) = m[2][3] - n(2,3);

	_m(3,0) = m[3][0] - n(3,0);
	_m(3,1) = m[3][1] - n(3,1);
	_m(3,2) = m[3][2] - n(3,2);
	_m(3,3) = m[3][3] - n(3,3);
	return _m;
}

Matrix4& Matrix4::operator += (const Matrix4& n)
{
	m[0][0] += n(0,0);
	m[0][1] += n(0,1);
	m[0][2] += n(0,2);
	m[0][3] += n(0,3);
	
	m[1][0] += n(1,0);
	m[1][1] += n(1,1);
	m[1][2] += n(1,2);
	m[1][3] += n(1,3);
	
	m[2][0] += n(2,0);
	m[2][1] += n(2,1);
	m[2][2] += n(2,2);
	m[2][3] += n(2,3);
	
	m[3][0] += n(3,0);
	m[3][1] += n(3,1);
	m[3][2] += n(3,2);
	m[3][3] += n(3,3);
	return *this;
}

Matrix4& Matrix4::operator -= (const Matrix4& n)
{
	m[0][0] -= n(0,0);
	m[0][1] -= n(0,1);
	m[0][2] -= n(0,2);
	m[0][3] -= n(0,3);
	
	m[1][0] -= n(1,0);
	m[1][1] -= n(1,1);
	m[1][2] -= n(1,2);
	m[1][3] -= n(1,3);
	
	m[2][0] -= n(2,0);
	m[2][1] -= n(2,1);
	m[2][2] -= n(2,2);
	m[2][3] -= n(2,3);
	
	m[3][0] -= n(3,0);
	m[3][1] -= n(3,1);
	m[3][2] -= n(3,2);
	m[3][3] -= n(3,3);
	return *this;
}

/**
 *	行列＊行列
 */
Matrix4 Matrix4::operator * (const Matrix4& x)
{
	Matrix4 _m;
	Matrix4Multiply(_m, *this, x);
	return _m;
}

Matrix4& Matrix4::operator *= (const Matrix4& x)
{
	Matrix4 _m = *this;
	Matrix4Multiply(*this, _m, x);
	return *this;
}

/**
 *	実数＊行列
 */
Matrix4 operator * (float k, const Matrix4& m)
{
	Matrix4 _m;
	_m(0,0) = k * m(0,0);
	_m(0,1) = k * m(0,1);
	_m(0,2) = k * m(0,2);
	_m(0,3) = k * m(0,3);

	_m(1,0) = k * m(1,0);
	_m(1,1) = k * m(1,1);
	_m(1,2) = k * m(1,2);
	_m(1,3) = k * m(1,3);

	_m(2,0) = k * m(2,0);
	_m(2,1) = k * m(2,1);
	_m(2,2) = k * m(2,2);
	_m(2,3) = k * m(2,3);

	_m(3,0) = k * m(3,0);
	_m(3,1) = k * m(3,1);
	_m(3,2) = k * m(3,2);
	_m(3,3) = k * m(3,3);
	return _m;
}


/**
 *	行ベクトル＊行列
 */
Vector4 operator * (const Vector4& v, const Matrix4& m)
{
	Vector4 _v;
	Vector4Multiply(_v, v, m);
	return _v;
}

/**
 *	行列＊列ベクトル
 */
Vector4 operator * (const Matrix4& m, const Vector4& v)
{
	Vector4 _v;
	Matrix4Multiply(_v, m, v);
	return _v;
}


/**
 *	Vector4Multiply()
 *	行ベクトルと列ベクトルの積を計算する。
 *
 *	(Vx, Vy, Vz, Vw)|Ux| = Vx*Ux + VyUy + VzUz + VwUw = 内積 = V･U
 *					|Uy|
 *					|Uz|
 *					|Uw|
 *
 *	@param	v	行ベクトル
 *	@param	r	列ベクトル
 */
float Vector4Multiply(const Vector4& v, const Vector4& u)
{
	return v.x*u.x + v.y*u.y + v.z*u.z + v.w*u.z;
}


/**
 *	Vector4Multiply()
 *	行ベクトルと行列の積を計算する。
 *
 *	行列Mを列ベクトルの集合 = |U1, U2, U3, U4|
 *	とした時、
 *	VM = V|U1, U2, U3, U4| = (VU1, VU2, VU3, VU4)
 *
 *	行ベクトルVと列ベクトルUnの積は、V･Unの為、
 *	(VU1, VU2, VU3, VU4) = (V･U1, V･U2, V･U3, V･U4)
 *		= (VxU1x + VyU1y + VzU1z + VwU1w,
 *		   VxU2x + VyU2y + VzU2z + VwU2w,
 *		   VxU3x + VyU3y + VzU3z + VwU3w,
 *		   VwU4x + VyU4y + VzU4z + VwU4w)
 *		= (VxM[0,0] + VyM[1,0] + VzM[2,0] + VzM[3, 0],
 *		   VxM[0,1] + VyM[1,1] + VzM[2,1] + VzM[3, 1],
 *		   VxM[0,2] + VyM[1,2] + VzM[2,2] + VzM[3, 2],
 *		   VxM[0,3] + VyM[1,3] + VzM[2,3] + VzM[3, 3])
 *	@param	_v	[out]積
 *	@param	v	行ベクトル
 *	@param	m	かける行列
 */
void Vector4Multiply(Vector4& _v, const Vector4& v, const Matrix4& m)
{
	_v.x = v.x*m(0,0) + v.y*m(1,0) + v.z*m(2,0) + v.w*m(3,0);
	_v.y = v.x*m(0,1) + v.y*m(1,1) + v.z*m(2,1) + v.w*m(3,1);
	_v.z = v.x*m(0,2) + v.y*m(1,2) + v.z*m(2,2) + v.w*m(3,2);
	_v.w = v.x*m(0,3) + v.y*m(1,3) + v.z*m(2,3) + v.w*m(3,3);
}


/**
 *	Matrix4Multiply()
 *	行列と列ベクトルの積を計算する。
 *	
 *	行列Mを行ベクトルの集合 = |U1|
 *							  |U2|
 *							  |U3|
 *							  |U4|
 *	とした場合、
 *	MV = |U1|V = |U1V|
 *		 |U2|	 |U2V|
 *		 |U3|	 |U3V|
 *		 |U4|	 |U4V|
 *
 *	行ベクトルUnと列ベクトルVの積は、Un･Vの為、
 *	|U1V| = |U1･V| = |U1xVx + U1yVy + U1zVz + U1wVw|
 *	|U2V|   |U2･V|   |U2xVx + U2yVy + U2zVz + U2wVw|
 *	|U3V|   |U3･V|   |U3xVx + U3yVy + U3zVz + U3wVw|
 *	|U4V|   |U4･V|   |U4xVx + U4yVy + U4zVz + U4wVw|
 *		  = |M[0,0]Vx + M[0,1]Vy + M[0,2]Vz + M[0,3]Vw|
 *		    |M[1,0]Vx + M[1,1]Vy + M[1,2]Vz + M[1,3]Vw|
 *		    |M[2,0]Vx + M[2,1]Vy + M[2,2]Vz + M[2,3]Vw|
 *		    |M[3,0]Vx + M[3,1]Vy + M[3,2]Vz + M[3,3]Vw|
 *	@param	_v	[out]積
 *	@param	m	行列
 *	@param	v	かける列ベクトル
 */
void Matrix4Multiply(Vector4& _v, const Matrix4& m, const Vector4& v)
{
	_v.x = m(0,0)*v.x + m(0,1)*v.y + m(0,2)*v.z + m(0,3)*v.w;
	_v.y = m(1,0)*v.x + m(1,1)*v.y + m(1,2)*v.z + m(1,3)*v.w;
	_v.z = m(2,0)*v.x + m(2,1)*v.y + m(2,2)*v.z + m(2,3)*v.w;
	_v.w = m(3,0)*v.x + m(3,1)*v.y + m(3,2)*v.z + m(3,3)*v.w;
}


/**
 *	Matrix4Multiply()
 *	行列の積を計算する。
 *
 *	行列Mを行ベクトルの集合 = |V1|
 *							  |V2|
 *							  |V3|
 *							  |V4|
 *	行列Nを列ベクトルの集合 = (U1, U2, U3, U4)
 *	とした時、
 *	MN = |V1U1, V1U2, V1U3, V1U4|
 *		 |V2U1, V2U2, V2U3, V2U4|
 *		 |V3U1, V3U2, V3U3, V3U4|
 *		 |V4U1, V4U2, V4U3, V4U4|
 *	行ベクトルVと列ベクトルUnの積は、V･Unの為、
 *	   = |V1･U1, V1･U2, V1･U3, V1･U4|
 *		 |V2･U1, V2･U2, V2･U3, V2･U4|
 *		 |V3･U1, V3･U2, V3･U3, V3･U4|
 *		 |V4･U1, V4･U2, V4･U3, V4･U4|
 *	   = |V1xU1x+V1yU1y+V1zU1z+V1wU1w, V1xU2x+V1yU2y+V1zU2z+V1wU2w, V1xU3x+V1yU3y+V1zU3z+V1wU3w, V1xU4x+V1yU4y+V1zU4z+V1wU4w|
 *		 |V2xU1x+V2yU1y+V2zU1z+V2wU1w, V2xU2x+V2yU2y+V2zU2z+V2wU2w, V2xU3x+V2yU3y+V2zU3z+V2wU3w, V2xU4x+V2yU4y+V2zU4z+V2wU4w|
 *		 |V3xU1x+V3yU1y+V3zU1z+V3wU1w, V3xU2x+V3yU2y+V3zU2z+V3wU2w, V3xU3x+V3yU3y+V3zU3z+V3wU3w, V3xU4x+V3yU4y+V3zU4z+V3wU4w|
 *		 |V4xU1x+V4yU1y+V4zU1z+V4wU1w, V4xU2x+V4yU2y+V4zU2z+V4wU2w, V4xU3x+V4yU3y+V4zU3z+V4wU3w, V4xU4x+V4yU4y+V4zU4z+V4wU4w|
 *	   = |M[0,0]N[0,0]+M[0,1]N[1,0]+M[0,2]N[2,0]+M[0,3]N[3,0], M[0,0]N[0,1]+M[0,1]N[1,1]+M[0,2]N[2,1]+M[0,3]N[3,1], M[0,0]N[0,2]+M[0,1]N[1,2]+M[0,2]N[2,2]+M[0,3]N[3,2], M[0,0]N[0,3]+M[0,1]N[1,3]+M[0,2]N[2,3]+M[0,3]N[3,3]|
 *		 |M[1,0]N[0,0]+M[1,1]N[1,0]+M[1,2]N[2,0]+M[1,3]N[3,0], M[1,0]N[0,1]+M[1,1]N[1,1]+M[1,2]N[2,1]+M[1,3]N[3,1], M[1,0]N[0,2]+M[1,1]N[1,2]+M[1,2]N[2,2]+M[1,3]N[3,2], M[1,0]N[0,3]+M[1,1]N[1,3]+M[1,2]N[2,3]+M[1,3]N[3,3]|
 *		 |M[2,0]N[0,0]+M[2,1]N[1,0]+M[2,2]N[2,0]+M[2,3]N[3,0], M[2,0]N[0,1]+M[2,1]N[1,1]+M[2,2]N[2,1]+M[2,3]N[3,1], M[2,0]N[0,2]+M[2,1]N[1,2]+M[2,2]N[2,2]+M[2,3]N[3,2], M[2,0]N[0,3]+M[2,1]N[1,3]+M[2,2]N[2,3]+M[2,3]N[3,3]|
 *		 |M[3,0]N[0,0]+M[3,1]N[1,0]+M[3,2]N[2,0]+M[3,3]N[3,0], M[3,0]N[0,1]+M[3,1]N[1,1]+M[3,2]N[2,1]+M[3,3]N[3,1], M[3,0]N[0,2]+M[3,1]N[1,2]+M[3,2]N[2,2]+M[3,3]N[3,2], M[3,0]N[0,3]+M[3,1]N[1,3]+M[3,2]N[2,3]+M[3,3]N[3,3]|
 *	@param	_m	[out]積
 *	@param	m	行列
 *	@param	n	かける行列
 */
void Matrix4Multiply(Matrix4& _m, const Matrix4& m, const Matrix4& n)
{
	_m(0,0) =	m(0,0)*n(0,0) + m(0,1)*n(1,0) + m(0,2)*n(2,0) + m(0,3)*n(3,0);
	_m(0,1) =	m(0,0)*n(0,1) + m(0,1)*n(1,1) + m(0,2)*n(2,1) + m(0,3)*n(3,1);
	_m(0,2) =	m(0,0)*n(0,2) + m(0,1)*n(1,2) + m(0,2)*n(2,2) + m(0,3)*n(3,2);
	_m(0,3) =	m(0,0)*n(0,3) + m(0,1)*n(1,3) + m(0,2)*n(2,3) + m(0,3)*n(3,3);

	_m(1,0) =	m(1,0)*n(0,0) + m(1,1)*n(1,0) + m(1,2)*n(2,0) + m(1,3)*n(3,0);
	_m(1,1) =	m(1,0)*n(0,1) + m(1,1)*n(1,1) + m(1,2)*n(2,1) + m(1,3)*n(3,1);
	_m(1,2) =	m(1,0)*n(0,2) + m(1,1)*n(1,2) + m(1,2)*n(2,2) + m(1,3)*n(3,2);
	_m(1,3) =	m(1,0)*n(0,3) + m(1,1)*n(1,3) + m(1,2)*n(2,3) + m(1,3)*n(3,3);

	_m(2,0) =	m(2,0)*n(0,0) + m(2,1)*n(1,0) + m(2,2)*n(2,0) + m(2,3)*n(3,0);
	_m(2,1) =	m(2,0)*n(0,1) + m(2,1)*n(1,1) + m(2,2)*n(2,1) + m(2,3)*n(3,1);
	_m(2,2) =	m(2,0)*n(0,2) + m(2,1)*n(1,2) + m(2,2)*n(2,2) + m(2,3)*n(3,2);
	_m(2,3) =	m(2,0)*n(0,3) + m(2,1)*n(1,3) + m(2,2)*n(2,3) + m(2,3)*n(3,3);

	_m(3,0) =	m(3,0)*n(0,0) + m(3,1)*n(1,0) + m(3,2)*n(2,0) + m(3,3)*n(3,0);
	_m(3,1) =	m(3,0)*n(0,1) + m(3,1)*n(1,1) + m(3,2)*n(2,1) + m(3,3)*n(3,1);
	_m(3,2) =	m(3,0)*n(0,2) + m(3,1)*n(1,2) + m(3,2)*n(2,2) + m(3,3)*n(3,2);
	_m(3,3) =	m(3,0)*n(0,3) + m(3,1)*n(1,3) + m(3,2)*n(2,3) + m(3,3)*n(3,3);
}

/**
 *	Matrix4Zero()
 *	零行列を取得する。
 */
void Matrix4Zero(Matrix4& _m)
{
	_m(0,0) = 0;
	_m(0,1) = 0;
	_m(0,2) = 0;
	_m(0,3) = 0;
	
	_m(1,0) = 0;
	_m(1,1) = 0;
	_m(1,2) = 0;
	_m(1,3) = 0;
	
	_m(2,0) = 0;
	_m(2,1) = 0;
	_m(2,2) = 0;
	_m(2,3) = 0;
	
	_m(3,0) = 0;
	_m(3,1) = 0;
	_m(3,2) = 0;
	_m(3,3) = 0;
}


/**
 *	Matrix4Identity()
 *	単位行列を取得する。
 *
 *	|1, 0, 0, 0|
 *	|0, 1, 0, 0|
 *	|0, 0, 1, 0|
 *	|0, 0, 0, 1|
 *
 *	@param	_m		[out]単位行列
 */
void Matrix4Identity(Matrix4& _m)
{
	_m(0,0) = 1.0f;
	_m(0,1) = 0.0f;
	_m(0,2) = 0.0f;
	_m(0,3) = 0.0f;

	_m(1,0) = 0.0f;
	_m(1,1) = 1.0f;
	_m(1,2) = 0.0f;
	_m(1,3) = 0.0f;

	_m(2,0) = 0.0f;
	_m(2,1) = 0.0f;
	_m(2,2) = 1.0f;
	_m(2,3) = 0.0f;

	_m(3,0) = 0.0f;
	_m(3,1) = 0.0f;
	_m(3,2) = 0.0f;
	_m(3,3) = 1.0f;
}

/**
 *	Matrix4Determinant()
 *	行列式を計算する。
 *
 *	M	= |a11, a12, a13, a14|
 *		  |a21, a22, a23, a24|
 *		  |a31, a32, a33, a34|
 *		  |a41, a42, a43, a44|
 *
 *	行列式detMは
 *	detM = a11a22a33a44 + a11a23a34a42 + a11a24a32a43
 *		 + a12a21a34a43 + a12a23a31a44 + a12a24a33a41 
 *		 + a13a21a32a44 + a13a22a34a41 + a13a24a31a42
 *		 + a14a21a33a42 + a14a22a31a43 + a14a23a32a41 
 *		 - a11a22a34a43 - a11a23a32a44 - a11a24a33a42
 *		 - a12a21a33a44 - a12a23a34a41 - a12a24a31a43
 *		 - a13a21a34a42 - a13a22a31a44 - a13a24a32a41
 *		 - a14a21a32a43 - a14a22a33a41 - a14a23a31a42
 *		   
 *	@param	m	行列
 *	@return		行列式
 */
float Matrix4Determinant(const Matrix4& m)
{
	return m(0,0)*m(1,1)*m(2,2)*m(3,3) + m(0,0)*m(1,2)*m(2,3)*m(3,1) + m(0,0)*m(1,3)*m(2,1)*m(3,2)
		 + m(0,1)*m(1,0)*m(2,3)*m(3,2) + m(0,1)*m(1,2)*m(2,0)*m(3,3) + m(0,1)*m(1,3)*m(2,2)*m(3,0) 
		 + m(0,2)*m(1,0)*m(2,1)*m(3,3) + m(0,2)*m(1,1)*m(2,3)*m(3,0) + m(0,2)*m(1,3)*m(2,0)*m(3,1)
		 + m(0,3)*m(1,0)*m(2,2)*m(3,1) + m(0,3)*m(1,1)*m(2,0)*m(3,2) + m(0,3)*m(1,2)*m(2,1)*m(3,0) 
		 - m(0,0)*m(1,1)*m(2,3)*m(3,2) - m(0,0)*m(1,2)*m(2,1)*m(3,3) - m(0,0)*m(1,3)*m(2,2)*m(3,1)
		 - m(0,1)*m(1,0)*m(2,2)*m(3,3) - m(0,1)*m(1,2)*m(2,3)*m(3,0) - m(0,1)*m(1,3)*m(2,0)*m(3,2)
		 - m(0,2)*m(1,0)*m(2,3)*m(3,1) - m(0,2)*m(1,1)*m(2,0)*m(3,3) - m(0,2)*m(1,3)*m(2,1)*m(3,0)
		 - m(0,3)*m(1,0)*m(2,1)*m(3,2) - m(0,3)*m(1,1)*m(2,2)*m(3,0) - m(0,3)*m(1,2)*m(2,0)*m(3,1);
}


/**
 *	Matrix4Tilde()
 *	行列のi行j列を除いた行列を取得する。
 *
 *	Mからi行j列を取り去った行列をM~ijとする(↓は例)
 *	M~12 = |a21, a23, a24|
 *		   |a31, a33, a34|
 *		   |a41, a43, a44|
 *	
 *	@param	_m	[out]行列
 *	@param	m	行列
 *	@param	i	取り除く行
 *	@param	j	取り除く列
 */
void Matrix4Tilde(Matrix3& _m, const Matrix4& m, int i, int j)
{
	int I, J;
	int a = 0, b;
	for (I = 0; I < 4; ++I) {
		if (I == i) continue;
		b = 0;
		for (J = 0; J < 4; ++J) {
			if (J == j) continue;
			_m(a,b) = m(I,J);
			++b;
		}
		++a;
	}
}


/**
 *	Matrix4Inverse()
 *	逆行列を計算する。
 *
 *	M	= |a11, a12, a13, a14|
 *		  |a21, a22, a23, a24|
 *		  |a31, a32, a33, a34|
 *		  |a41, a42, a43, a44|
 *
 *	Mからi行j列を取り去った行列をM~ijとする(↓は例)
 *	M~12 = |a21, a23, a24|
 *		   |a31, a33, a34|
 *		   |a41, a43, a44|
 *
 *	逆行列M^-1は
 *	M^-1= 1 / detM * | detM~11, -detM~21,  detM~31, -detM~41|
 *					 |-detM~12,  detM~22, -detM~32,  detM~42|
 *					 | detM~13, -detM~23,  detM~33, -detM~43|
 *					 | detM~14, -detM~24,  detM~34, -detM~44|
 *
 *	@param	_m	[out]逆行列
 *	@param	m	行列
 */
void Matrix4Inverse(Matrix4& _m, const Matrix4& m)
{
	float d = Matrix4Determinant(m);
	assert(d != 0.0f);
	d = 1.0f / d;
	
	int i, j;
	Matrix3 n;
	for (i = 0; i < 4; ++i) {
		for (j = 0; j < 4; ++j) {
			Matrix4Tilde(n, m, j, i);
			_m(i,j) = (((i+j)&1)==0?1:-1) * d * Matrix3Determinant(n);
		}
	}

	// 展開ver. ... は正直めんどくさいので止め！
}


/**
 *	Matrix4RotationX()
 *	３次元同次ベクトルをX軸周りに回転させる行列を取得する。
 *
 *	|1, 0,		0,		0|
 *	|0, cosθ,  sinθ,	0|
 *	|0, -sinθ, cosθ,	0|
 *	|0, 0,		0,		1|
 *
 *	@para_m	_m		[out]回転行列
 *	@para_m	theta	角度(ラジアン)
 */
void Matrix4RotationX(Matrix4& _m, float theta)
{
	Matrix4Identity(_m);
	_m(1,1) = cos(theta);
	_m(1,2) = sin(theta);
	_m(2,1) = -_m(1,2);	// -sin(theta);
	_m(2,2) = _m(1,1);	// cos(theta);
}

/**
 *	Matrix4RotationY()
 *	３次元同次ベクトルをY軸周りに回転させる行列を取得する。
 *
 *	|cosθ, 0,	-sinθ,	0|
 *	|0,		1,	0,		0|
 *	|sinθ, 0,	cosθ,	0|
 *	|0,		0,	0,		1|
 *
 *	@para_m	_m		[out]回転行列
 *	@para_m	theta	角度(ラジアン)
 */
void Matrix4RotationY(Matrix4& _m, float theta)
{
	Matrix4Identity(_m);
	_m(0,0) = cos(theta);
	_m(2,0) = sin(theta);
	_m(0,2) = -_m(2,0);	// -sin(theta);
	_m(2,2) = _m(0,0);	// cos(theta);
}


/**
 *	Matrix4RotationZ()
 *	３次元同次ベクトルをZ軸周りに回転させる行列を取得する。
 *
 *	|cosθ,  sinθ, 0, 0|
 *	|-sinθ, cosθ, 0, 0|
 *	|0,		 0,		1, 0|
 *	|0,		 0,		0, 1|
 *
 *	@para_m	_m		[out]回転行列
 *	@para_m	theta	角度(ラジアン)
 */
void Matrix4RotationZ(Matrix4& _m, float theta)
{
	Matrix4Identity(_m);
	_m(0,0) = cos(theta);
	_m(0,1) = sin(theta);
	_m(1,0) = -_m(0,1);	// -sin(theta);
	_m(1,1) = _m(0,0);	// cos(theta); 
}

/**
 *	Matrix4RotationAxis()
 *	３次元同次ベクトルを任意の軸周りに回転させる行列を取得する。
 *
 *	|c+(1-c)Vx^2,	(1-c)VxVy+sVz,	(1-c)VxVz-sVy,	0|
 *	|(1-c)VxVy-sVz, c+(1-c)Vy^2,	(1-c)VyVz+sVx,	0|
 *	|(1-c)VxVz+sVy,	(1-c)VyVz-sVx,	c+(1-c)Vz^2,	0|
 *	|0,				0,				0,				1|
 *	※cosθ = c, sinθ = s
 *
 *	@para_m	_m		[out]回転行列
 *	@para_m	theta	角度(ラジアン)
 *	@para_m	v		軸を表すVector3(正規化済みのもの)
 */
void Matrix4RotationAxis(Matrix4& _m, float theta, const Vector3& v)
{
	Matrix4Identity(_m);
	float c = cos(theta);
	float s = sin(theta);
	
    _m(0,0) = c+(1.0f-c)*v.x*v.x;
    _m(0,1) = (1.0f-c)*v.x*v.y+s*v.z;
    _m(0,2) = (1.0f-c)*v.x*v.z-s*v.y;
	
    _m(1,0) = (1.0f-c)*v.x*v.y-s*v.z;
    _m(1,1) = c+(1.0f-c)*v.y*v.y;
    _m(1,2) = (1.0f-c)*v.y*v.z+s*v.x;
	
    _m(2,0) = (1.0f-c)*v.x*v.z+s*v.y;
    _m(2,1) = (1.0f-c)*v.y*v.z-s*v.x;
    _m(2,2) = c+(1.0f-c)*v.z*v.z;
}

/**
 *	Matrix4RotationVector()
 *	３次元同次ベクトルを任意のベクトルの示す向きに回転させる行列を取得する。
 *
 *	任意のベクトルをz軸の正方向を向いた基底ベクトルとみなし
 *	vZ = normalize(v)
 *	vX = v(0, 1, 0)×vZ
 *	vY = vZ×vX 
 *
 *	|vXx, vXy, vXz, 0|
 *	|vYx, vYy, vYz, 0|
 *	|vZx, vZy, vZz, 0|
 *	|  0,   0,   0, 1|
 *
 *	@param[out]	_m	変換行列
 *	@param	v	向き
 */
void Matrix4RotationVector(Matrix4& _m, const Vector3& v)
{
	Vector3 vZ = v.Normal();
	Vector3 vX = Vector3(0.0f, 1.0f, 0.0f).Cross(vZ);
	Vector3 vY = vZ.Cross(vX);

	_m(0, 0) = vX.x;
	_m(0, 1) = vX.y;
	_m(0, 2) = vX.z;
	_m(0, 3) = 0.0f;

	_m(1, 0) = vY.x;
	_m(1, 1) = vY.y;
	_m(1, 2) = vY.z;
	_m(1, 3) = 0.0f;

	_m(2, 0) = vZ.x;
	_m(2, 1) = vZ.y;
	_m(2, 2) = vZ.z;
	_m(2, 3) = 0.0f;

	_m(3, 0) = 0.0f;
	_m(3, 1) = 0.0f;
	_m(3, 2) = 0.0f;
	_m(3, 3) = 1.0f;
}


/**
 *	Matrix4Scaling()
 *	３次元同次ベクトルを拡縮する行列を取得する。
 *
 *	|sx,  0,  0,  0|
 *	| 0, sy,  0,  0|
 *	| 0,  0, sz,  0|
 *	| 0,  0,  0,  1|
 *
 *	@param	_m	[out]変換行列
 *	@param	sx	x座標の拡縮係数
 *	@param	sy	y座標の拡縮係数
 *	@param	sz	z座標の拡縮係数
 */
void Matrix4Scaling(Matrix4& _m, float sx, float sy, float sz)
{
	Matrix4Identity(_m);
	_m(0,0) = sx;
	_m(1,1) = sy;
	_m(2,2) = sz;
}


/**
 *	Matrix4Translation()
 *	３次元同次ベクトルを平行移動する行列を取得する。
 *
 *	|1, 0, 0, 0|
 *	|0, 1, 0, 0|
 *	|0, 0, 1, 0|
 *	|x, y, z, 1|
 *
 *	@param	_m	[out]平行移動行列
 *	@param	x	x座標の移動量
 *	@param	y	y座標の移動量
 *	@param	z	z座標の移動量
 */
void Matrix4Translation(Matrix4& _m, float x, float y, float z)
{
	Matrix4Identity(_m);

	_m(3,0) = x;
	_m(3,1) = y;
	_m(3,2) = z;
}


/**
 *	Matrix4View()
 *	３次元同次ベクトルをカメラ座標系に変換する行列を取得する。
 *	
 *	@param	_m	[out]ビュー変換行列
 *	@param	eye	カメラ座標
 *	@param	at	注視点
 *	@param	up	カメラの上の座標(一般的には(0,1,0)?)
 */
void Matrix4View(Matrix4& _m, const Vector3& eye, const Vector3& at, const Vector3& up)
{	
	Vector3 vz = (at - eye).Normal();
	Vector3 vx = up.Cross(vz).Normal();
	Vector3 vy = vz.Cross(vx);

	Matrix4Identity(_m);

	_m(0,0) = vx.x;
	_m(1,0) = vx.y;
	_m(2,0) = vx.z;

	_m(0,1) = vy.x;
	_m(1,1) = vy.y;
	_m(2,1) = vy.z;

	_m(0,2) = vz.x;
	_m(1,2) = vz.y;
	_m(2,2) = vz.z;
	
	_m(3,0) = -vx.Dot(eye);
	_m(3,1) = -vy.Dot(eye);
	_m(3,2) = -vz.Dot(eye);
}


/**
 *	Matrix4Projection()
 *	３次元同次ベクトルを透視射影変換する行列を取得する。
 *
 *	Matarix4 mProj;
 *	Matrix4Projection(mProj, fovV, aspect, zn, zf);
 *	Vector4 v = (x, y, z, w);
 *	v = mProj * v;	// 順番に気をつける！
 *	v /= v.w;		// 算出されたw座標で割る！
 *
 *	また、この行列で計算されたベクトルvは、x,yの値が-1～1の範囲を取る。
 *	別途Matrix4Screen()で得た行列をかけて、
 *	最終的なスクリーン座標へ変換する必要がある。
 *	
 *	@param	_m		[out]射影変換行列
 *	@param	fovV	垂直方向の視野角
 *	@param	aspect	アスペクト比(幅/高さ)
 *	@param	zn		視錘台の近平面までの距離
 *	@param	zf		視錘台の遠平面までの距離
 */
void Matrix4Projection(Matrix4& _m, float fovV, float aspect, float zn, float zf)
{
	float R, L, T, B;	// 視錘台の近平面の四辺のXY座標

	//T = sin(fovV*0.5f)*zn/cos(fovV*0.5f);
	T = zn * tan(fovV*0.5f);
	B = -T;

	float fovH = atan(T*aspect/zn)*2.0f;	// 水平方向の視野角（もっと最適化してatan消せる･･･）
	//R = sin(fovH*0.5f)*zn/cos(fovH*0.5f);
	R = zn * tan(fovH*0.5f);
	L = -R;

	Matrix4Zero(_m);

	_m(0,0) = (2.0f*zn) / (R-L) * -1;	// !x座標の正方向が右手にくるようにマイナスをかけてある
	_m(0,2) = (R+L) / (R-L);
	_m(1,1) = (2.0f*zn) / (T-B);
	_m(1,2) = (T+B) / (T-B);
	_m(2,2) = - (zf+zn) / (zf-zn);
	_m(2,3) = - (2.0f*zn*zf) / (zf-zn);
	_m(3,2) = -1.0f;
}


/**
 *	Matrix4Screen()
 *	透視射影した３次元同次ベクトルをスクリーン座標に変換する行列を取得する。
 *
 *	|w/2, 0,   0, 0|
 *	|0,   h/2, 0, 0|
 *	|0,   0,   1, 0|
 *	|w/2, h/2, 0, 1|
 *
 *	@param	_m		[out]スクリーン座標変換行列
 *	@param	width	スクリーン横幅
 *	@param	height	スクリーン縦幅
 */
void Matrix4Screen(Matrix4& _m, float width, float height)
{
	Matrix4Identity(_m);

	_m(0,0) = width  / 2.0f;
	_m(1,1) = height / 2.0f;
	_m(3,0) = width  / 2.0f;
	_m(3,1) = height / 2.0f;
}


