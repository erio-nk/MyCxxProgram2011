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
 *	�s��̉��Z
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
 *	�������s��
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
 *	�s�x�N�g�����s��
 */
Vector2 operator * (const Vector2& v, const Matrix2& m) {
	Vector2 _v;
	Vector2Multiply(_v, v, m);
	return _v;
}

/**
 *	�s�񁖗�x�N�g��
 */
Vector2 operator * (const Matrix2& m, const Vector2& v)
{
	Vector2 _v;
	Matrix2Multiply(_v, m, v);
	return _v;
}

/**
 *	Vector2Multiply()
 *	�s�x�N�g���Ɨ�x�N�g���̐ς��v�Z����B
 *
 *	(Vx, Vy)|Ux| = Vx*Ux + VyUy = ���� = V�U
 *			|Uy|
 *
 *	@param	v	�s�x�N�g��
 *	@param	u	��x�N�g��
 */
float Vector2Multiply(const Vector2& v, const Vector2& u)
{
	return v.x*u.x + v.y*u.y;
}


/**
 *	Vector2Multiply()
 *	�s�x�N�g���ƍs��̐ς��v�Z����B
 *
 *	�s��M���x�N�g���̏W�� = |U1, U2|
 *	�Ƃ������A
 *	VM = V|U1, U2| = (VU1, VU2)
 *
 *	�s�x�N�g��V�Ɨ�x�N�g��Un�̐ς́AV�Un�ׁ̈A
 *	(VU1, VU2) = (V�U1, V�U2) = (VxU1x + VyU1y, VxU2x + VyU2y)
 *
 *	@param	_v	[out]��
 *	@param	v	�s�x�N�g��
 *	@param	m	������s��
 */
void Vector2Multiply(Vector2& _v, const Vector2& v, const Matrix2& m)
{
	_v.x = v.x*m.p + v.y*m.r;
	_v.y = v.x*m.q + v.y*m.s;
}


/**
 *	Matrix2Multiply()
 *	�s��Ɨ�x�N�g���̐ς��v�Z����B
 *
 *	�s��M���s�x�N�g���̏W�� = |U1|
 *							  |U2|
 *	�Ƃ������A
 *	MV = |U1|V = |U1V|
 *		 |U2|	 |U2V|
 *
 *	�s�x�N�g��Un�Ɨ�x�N�g��V�̐ς́AUn�V�ׁ̈A
 *	|U1V| = |U1�V| = |U1xVx + U1yVy|
 *	|U2V|   |U2�V|   |U2xVx + U2yVy|
 *
 *	@param	_v	[out]��
 *	@param	m	�s��
 *	@param	v	�������x�N�g��
 */
void Matrix2Multiply(Vector2& _v, const Matrix2& m, const Vector2& v)
{
	_v.x = m.a*v.x + m.b*v.y;
	_v.y = m.c*v.x + m.d*v.y;
}


/**
 *	Matrix2Multiply()
 *	�s��̐ς��v�Z����B
 *	@param	_m	[out]��
 *	@param	m	�s��
 *	@param	n	������s��
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
 *	��s����擾����B
 *
 *	|0, 0|
 *	|0, 0|
 *
 *	@param	_m		[out]��s��
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
 *	�P�ʍs����擾����B
 *
 *	|1, 0|
 *	|0, 1|
 *
 *	@param	_m		[out]�P�ʍs��
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
 *	�s�񎮂��v�Z����B
 *
 *	M = |a, b|
 *		|c, d|
 *	detM= ad - bc
 *
 *	@param	m	�s��
 *	@return		�s��
 */
float Matrix2Determinant(const Matrix2& m)
{
	return m.a*m.d - m.b*m.c;
}

/**
 *	Matrix2Inverse()
 *	�t�s����v�Z����B
 *
 *	M	= |a, b|
 *		  |c, d|
 *	M^-1= 1 / detM * |d, -b|
 *					 |-c, a|
 *
 *	@param	_m	[out]�t�s��
 *	@param	m	�s��
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
 *	�Q�����x�N�g������]����s����擾����B
 *
 *	|cos��,  sin��|
 *	|-sin��, cos��|
 *
 *	@param	_m		[out]��]�s��
 *	@param	theta	�p�x(���W�A��)
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
 *	�Q�����x�N�g�����g�傷��s����擾����B
 *
 *	|sx,  0|
 *	| 0, sy|
 *
 *	@param	_m	[out]�g��s��
 *	@param	sx	x���W�̊g��W��
 *	@param	sy	y���W�̊g��W��
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
 *	�s��̉��Z
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
 *	�s��̐�
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
 *	�������s��
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
 *	�s�x�N�g�����s��
 */
Vector3 operator * (const Vector3& v, const Matrix3& m)
{
	Vector3 _v;
	Vector3Multiply(_v, v, m);
	return _v;
}

/**
 *	�s�񁖗�x�N�g��
 */
Vector3 operator * (const Matrix3& m, const Vector3& v)
{
	Vector3 _v;
	Matrix3Multiply(_v, m, v);
	return _v;
}


/**
 *	Vector3Multiply()
 *	�s�x�N�g���Ɨ�x�N�g���̐ς��v�Z����B
 *
 *	(Vx, Vy, Vz)|Ux| = Vx*Ux + VyUy + VzUz = ���� = V�U
 *				|Uy|
 *				|Uz|
 *
 *	@param	v	�s�x�N�g��
 *	@param	r	��x�N�g��
 */
float Vector3Multiply(const Vector3& v, const Vector3& u)
{
	return v.x*u.x + v.y*u.y + v.z*u.z;
}


/**
 *	Vector3Multiply()
 *	�s�x�N�g���ƍs��̐ς��v�Z����B
 *
 *	�s��M���x�N�g���̏W�� = |U1, U2, U3|
 *	�Ƃ������A
 *	VM = V|U1, U2, U3| = (VU1, VU2, VU3)
 *
 *	�s�x�N�g��V�Ɨ�x�N�g��Un�̐ς́AV�Un�ׁ̈A
 *	(VU1, VU2, VU3) = (V�U1, V�U2, V�U3)
 *		= (VxU1x + VyU1y + VzU1z,
 *		   VxU2x + VyU2y + VzU2z,
 *		   VxU3x + VyU3y + VzU3z)
 *		= (VxM[0,0] + VyM[1,0] + VzM[2,0],
 *		   VxM[0,1] + VyM[1,1] + VzM[2,1],
 *		   VxM[0,2] + VyM[1,2] + VzM[2,2])
 *	@param	_v	[out]��
 *	@param	v	�s�x�N�g��
 *	@param	m	������s��
 */
void Vector3Multiply(Vector3& _v, const Vector3& v, const Matrix3& m)
{
	_v.x = v.x*m(0,0) + v.y*m(1,0) + v.z*m(2,0);
	_v.y = v.x*m(0,1) + v.y*m(1,1) + v.z*m(2,1);
	_v.z = v.x*m(0,2) + v.y*m(1,2) + v.z*m(2,2);
}


/**
 *	Matrix3Multiply()
 *	�s��Ɨ�x�N�g���̐ς��v�Z����B
 *
 *	�s��M���s�x�N�g���̏W�� = |U1|
 *							  |U2|
 *							  |U3|
 *	�Ƃ������A
 *	MV = |U1|V = |U1V|
 *		 |U2|	 |U2V|
 *		 |U3|	 |U3V|
 *
 *	�s�x�N�g��Un�Ɨ�x�N�g��V�̐ς́AUn�V�ׁ̈A
 *	|U1V| = |U1�V| = |U1xVx + U1yVy + U1zVz|
 *	|U2V|   |U2�V|   |U2xVx + U2yVy + U2zVz|
 *	|U3V|   |U3�V|   |U3xVx + U3yVy + U3zVz|
 *		  = |M[0,0]Vx + M[0,1]Vy + M[0,2]Vz|
 *		    |M[1,0]Vx + M[1,1]Vy + M[1,2]Vz|
 *		    |M[2,0]Vx + M[2,1]Vy + M[2,2]Vz|
 *
 *	@param	_v	[out]��
 *	@param	m	�s��
 *	@param	v	�������x�N�g��
 */
void Matrix3Multiply(Vector3& _v, const Matrix3& m, const Vector3& v)
{
	_v.x = m(0,0)*v.x + m(0,1)*v.y + m(0,2)*v.z;
	_v.y = m(1,0)*v.x + m(1,1)*v.y + m(1,2)*v.z;
	_v.z = m(2,0)*v.x + m(2,1)*v.y + m(2,2)*v.z;
}


/**
 *	Matrix3Multiply()
 *	�s��̐ς��v�Z����B
 *
 *	�s��M���s�x�N�g���̏W�� = |V1|
 *							  |V2|
 *							  |V3|
 *	�s��N���x�N�g���̏W�� = (U1, U2, U3)
 *	�Ƃ������A
 *	MN = |V1U1, V1U2, V1U3|
 *		 |V2U1, V2U2, V2U3|
 *		 |V3U1, V3U2, V3U3|
 *	�s�x�N�g��V�Ɨ�x�N�g��Un�̐ς́AV�Un�ׁ̈A
 *	   = |V1�U1, V1�U2, V1�U3|
 *		 |V2�U1, V2�U2, V2�U3|
 *		 |V3�U1, V3�U2, V3�U3|
 *	   = |V1xU1x+V1yU1y+V1zU1z, V1xU2x+V1yU2y+V1zU2z, V1xU3x+V1yU3y+V1zU3z|
 *		 |V2xU1x+V2yU1y+V2zU1z, V2xU2x+V2yU2y+V2zU2z, V2xU3x+V2yU3y+V2zU3z|
 *		 |V3xU1x+V3yU1y+V3zU1z, V3xU2x+V3yU2y+V3zU2z, V3xU3x+V3yU3y+V3zU3z|
 *	   = |M[0,0]N[0,0]+M[0,1]N[1,0]+M[0,2]N[2,0], M[0,0]N[0,1]+M[0,1]N[1,1]+M[0,2]N[2,1], M[0,0]N[0,2]+M[0,1]N[1,2]+M[0,2]N[2,2]|
 *		 |M[1,0]N[0,0]+M[1,1]N[1,0]+M[1,2]N[2,0], M[1,0]N[0,1]+M[1,1]N[1,1]+M[1,2]N[2,1], M[1,0]N[0,2]+M[1,1]N[1,2]+M[1,2]N[2,2]|
 *		 |M[2,0]N[0,0]+M[2,1]N[1,0]+M[2,2]N[2,0], M[2,0]N[0,1]+M[2,1]N[1,1]+M[2,2]N[2,1], M[2,0]N[0,2]+M[2,1]N[1,2]+M[2,2]N[2,2]|
 *	@param	_m	[out]��
 *	@param	m	�s��
 *	@param	n	������s��
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

	// �W�Jver.
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
 *	��s����擾����B
 *
 *	|0, 0, 0|
 *	|0, 0, 0|
 *	|0, 0, 0|
 *
 *	@param	_m		[out]��s��
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
 *	�P�ʍs����擾����B
 *
 *	|1, 0, 0|
 *	|0, 1, 0|
 *	|0, 0, 1|
 *
 *	@param	_m		[out]�P�ʍs��
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
 *	Matrix3�̍s�񎮂��v�Z����B
 *
 *	M	= |a, b, c|
 *		  |d, e, f|
 *		  |g, h, i|
 *
 *	�s��detM��
 *	detM= aei + dhc + gbf - ahf - dbi - gec
 *
 *	@param	m	Matrix3
 *	@return		�s��
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
 *	�s���i�sj����������s����擾����B
 *
 *	M����i�sj�����苎�����s���M~ij�Ƃ���(���͗�)
 *	M~12 = |a21, a23|
 *		   |a31, a33|
 *	
 *	@param	_m	[out]�s��
 *	@param	m	�s��
 *	@param	i	��菜���s
 *	@param	j	��菜����
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
 *	�t�s����v�Z����B
 *
 *	M	= |a11, a12, a13|
 *		  |a21, a22, a23|
 *		  |a31, a32, a33|
 *
 *	M����i�sj�����苎�����s���M~ij�Ƃ���(���͗�)
 *	M~12 = |a21, a23|
 *		   |a31, a33|
 *
 *	�t�s��M^-1��
 *	M^-1= 1 / detM * | detM~11, -detM~21,  detM~31|
 *					 |-detM~12,  detM~22, -detM~32|
 *					 | detM~13, -detM~23,  detM~33|
 *		= 1 / detM * |a22a33-a23a32, a32a13-a33a12, a12a23-a13a22|
 *					 |a23a31-a21a33, a33a11-a31a13, a13a21-a11a23|
 *					 |a21a32-a22a31, a31a12-a32a11, a11a22-a12a21|
 *
 *	@param	_m	[out]�t�s��
 *	@param	m	�s��
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

	// �W�Jver.
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
 *	�R�����x�N�g����X������ɉ�]������s����擾����B
 *
 *	|1, 0,		0    |
 *	|0, cos��,  sin��|
 *	|0, -sin��, cos��|
 *
 *	@param	_m		[out]��]�s��
 *	@param	theta	�p�x(���W�A��)
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
 *	�R�����x�N�g����Y������ɉ�]������s����擾����B
 *
 *	|cos��, 0,	-sin��|
 *	|0,		1,	0	  |
 *	|sin��, 0,	cos�� |
 *
 *	@param	_m		[out]��]�s��
 *	@param	theta	�p�x(���W�A��)
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
 *	�R�����x�N�g����Z������ɉ�]������s����擾����B
 *
 *	|cos��,  sin��, 0|
 *	|-sin��, cos��, 0|
 *	|0,		 0,		1|
 *
 *	@param	_m		[out]��]�s��
 *	@param	theta	�p�x(���W�A��)
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
 *	�R�����x�N�g����C�ӂ̎�����ɉ�]������s����擾����B
 *
 *	|c+(1-c)Vx^2,	(1-c)VxVy+sVz,	(1-c)VxVz-sVy|
 *	|(1-c)VxVy-sVz, c+(1-c)Vy^2,	(1-c)VyVz+sVx|
 *	|(1-c)VxVz+sVy,	(1-c)VyVz-sVx,	c+(1-c)Vz^2  |
 *	��cos�� = c, sin�� = s
 *
 *	@param	_m		[out]��]�s��
 *	@param	theta	�p�x(���W�A��)
 *	@param	v		����\���R�����x�N�g��(���K���ς݂̂���)
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
 *	�R�����x�N�g�����g�傷��s����擾����B
 *
 *	|sx,  0,  0|
 *	| 0, sy,  0|
 *	| 0,  0, sz|
 *
 *	@param	_m	[out]�g��s��
 *	@param	sx	x���W�̊g��W��
 *	@param	sy	y���W�̊g��W��
 *	@param	sz	z���W�̊g��W��
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
 *	�Q���������x�N�g������]������s����擾����B
 *
 *	|cos��,  sin��, 0|
 *	|-sin��, cos��, 0|
 *	|0,		 0,		1|
 *
 *	Vector3 v(x, y, 1);	// z��1
 *	Matrix3 m;
 *	Matrix3Rotation4V2(m, ��);
 *	v = v * m;	// ��]������
 *
 *	@param	_m		[out]��]�s��
 *	@param	theta	�p�x(���W�A��)
 */
void Matrix3RotationHV(Matrix3& _m, float theta)
{
	Matrix3RotationZ(_m, theta);
}

/**
 *	Matrix3TranslationHV()
 *	�Q���������x�N�g���𕽍s�ړ�������s����擾����B
 *
 *	|1, 0, x|
 *	|0, 1, y|
 *	|0,	0, 1|
 *
 *	Vector3 v(x, y, 1);	// z��1
 *	Matrix3 m;
 *	Matrix3Translation4V2(m, ��);
 *	v = v * m;	// ���s�ړ�������
 *
 *	@param	_m		[out]���s�ړ��s��
 *	@param	x, y	�ړ���
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
 *	���Z
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
 *	�s�񁖍s��
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
 *	�������s��
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
 *	�s�x�N�g�����s��
 */
Vector4 operator * (const Vector4& v, const Matrix4& m)
{
	Vector4 _v;
	Vector4Multiply(_v, v, m);
	return _v;
}

/**
 *	�s�񁖗�x�N�g��
 */
Vector4 operator * (const Matrix4& m, const Vector4& v)
{
	Vector4 _v;
	Matrix4Multiply(_v, m, v);
	return _v;
}


/**
 *	Vector4Multiply()
 *	�s�x�N�g���Ɨ�x�N�g���̐ς��v�Z����B
 *
 *	(Vx, Vy, Vz, Vw)|Ux| = Vx*Ux + VyUy + VzUz + VwUw = ���� = V�U
 *					|Uy|
 *					|Uz|
 *					|Uw|
 *
 *	@param	v	�s�x�N�g��
 *	@param	r	��x�N�g��
 */
float Vector4Multiply(const Vector4& v, const Vector4& u)
{
	return v.x*u.x + v.y*u.y + v.z*u.z + v.w*u.z;
}


/**
 *	Vector4Multiply()
 *	�s�x�N�g���ƍs��̐ς��v�Z����B
 *
 *	�s��M���x�N�g���̏W�� = |U1, U2, U3, U4|
 *	�Ƃ������A
 *	VM = V|U1, U2, U3, U4| = (VU1, VU2, VU3, VU4)
 *
 *	�s�x�N�g��V�Ɨ�x�N�g��Un�̐ς́AV�Un�ׁ̈A
 *	(VU1, VU2, VU3, VU4) = (V�U1, V�U2, V�U3, V�U4)
 *		= (VxU1x + VyU1y + VzU1z + VwU1w,
 *		   VxU2x + VyU2y + VzU2z + VwU2w,
 *		   VxU3x + VyU3y + VzU3z + VwU3w,
 *		   VwU4x + VyU4y + VzU4z + VwU4w)
 *		= (VxM[0,0] + VyM[1,0] + VzM[2,0] + VzM[3, 0],
 *		   VxM[0,1] + VyM[1,1] + VzM[2,1] + VzM[3, 1],
 *		   VxM[0,2] + VyM[1,2] + VzM[2,2] + VzM[3, 2],
 *		   VxM[0,3] + VyM[1,3] + VzM[2,3] + VzM[3, 3])
 *	@param	_v	[out]��
 *	@param	v	�s�x�N�g��
 *	@param	m	������s��
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
 *	�s��Ɨ�x�N�g���̐ς��v�Z����B
 *	
 *	�s��M���s�x�N�g���̏W�� = |U1|
 *							  |U2|
 *							  |U3|
 *							  |U4|
 *	�Ƃ����ꍇ�A
 *	MV = |U1|V = |U1V|
 *		 |U2|	 |U2V|
 *		 |U3|	 |U3V|
 *		 |U4|	 |U4V|
 *
 *	�s�x�N�g��Un�Ɨ�x�N�g��V�̐ς́AUn�V�ׁ̈A
 *	|U1V| = |U1�V| = |U1xVx + U1yVy + U1zVz + U1wVw|
 *	|U2V|   |U2�V|   |U2xVx + U2yVy + U2zVz + U2wVw|
 *	|U3V|   |U3�V|   |U3xVx + U3yVy + U3zVz + U3wVw|
 *	|U4V|   |U4�V|   |U4xVx + U4yVy + U4zVz + U4wVw|
 *		  = |M[0,0]Vx + M[0,1]Vy + M[0,2]Vz + M[0,3]Vw|
 *		    |M[1,0]Vx + M[1,1]Vy + M[1,2]Vz + M[1,3]Vw|
 *		    |M[2,0]Vx + M[2,1]Vy + M[2,2]Vz + M[2,3]Vw|
 *		    |M[3,0]Vx + M[3,1]Vy + M[3,2]Vz + M[3,3]Vw|
 *	@param	_v	[out]��
 *	@param	m	�s��
 *	@param	v	�������x�N�g��
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
 *	�s��̐ς��v�Z����B
 *
 *	�s��M���s�x�N�g���̏W�� = |V1|
 *							  |V2|
 *							  |V3|
 *							  |V4|
 *	�s��N���x�N�g���̏W�� = (U1, U2, U3, U4)
 *	�Ƃ������A
 *	MN = |V1U1, V1U2, V1U3, V1U4|
 *		 |V2U1, V2U2, V2U3, V2U4|
 *		 |V3U1, V3U2, V3U3, V3U4|
 *		 |V4U1, V4U2, V4U3, V4U4|
 *	�s�x�N�g��V�Ɨ�x�N�g��Un�̐ς́AV�Un�ׁ̈A
 *	   = |V1�U1, V1�U2, V1�U3, V1�U4|
 *		 |V2�U1, V2�U2, V2�U3, V2�U4|
 *		 |V3�U1, V3�U2, V3�U3, V3�U4|
 *		 |V4�U1, V4�U2, V4�U3, V4�U4|
 *	   = |V1xU1x+V1yU1y+V1zU1z+V1wU1w, V1xU2x+V1yU2y+V1zU2z+V1wU2w, V1xU3x+V1yU3y+V1zU3z+V1wU3w, V1xU4x+V1yU4y+V1zU4z+V1wU4w|
 *		 |V2xU1x+V2yU1y+V2zU1z+V2wU1w, V2xU2x+V2yU2y+V2zU2z+V2wU2w, V2xU3x+V2yU3y+V2zU3z+V2wU3w, V2xU4x+V2yU4y+V2zU4z+V2wU4w|
 *		 |V3xU1x+V3yU1y+V3zU1z+V3wU1w, V3xU2x+V3yU2y+V3zU2z+V3wU2w, V3xU3x+V3yU3y+V3zU3z+V3wU3w, V3xU4x+V3yU4y+V3zU4z+V3wU4w|
 *		 |V4xU1x+V4yU1y+V4zU1z+V4wU1w, V4xU2x+V4yU2y+V4zU2z+V4wU2w, V4xU3x+V4yU3y+V4zU3z+V4wU3w, V4xU4x+V4yU4y+V4zU4z+V4wU4w|
 *	   = |M[0,0]N[0,0]+M[0,1]N[1,0]+M[0,2]N[2,0]+M[0,3]N[3,0], M[0,0]N[0,1]+M[0,1]N[1,1]+M[0,2]N[2,1]+M[0,3]N[3,1], M[0,0]N[0,2]+M[0,1]N[1,2]+M[0,2]N[2,2]+M[0,3]N[3,2], M[0,0]N[0,3]+M[0,1]N[1,3]+M[0,2]N[2,3]+M[0,3]N[3,3]|
 *		 |M[1,0]N[0,0]+M[1,1]N[1,0]+M[1,2]N[2,0]+M[1,3]N[3,0], M[1,0]N[0,1]+M[1,1]N[1,1]+M[1,2]N[2,1]+M[1,3]N[3,1], M[1,0]N[0,2]+M[1,1]N[1,2]+M[1,2]N[2,2]+M[1,3]N[3,2], M[1,0]N[0,3]+M[1,1]N[1,3]+M[1,2]N[2,3]+M[1,3]N[3,3]|
 *		 |M[2,0]N[0,0]+M[2,1]N[1,0]+M[2,2]N[2,0]+M[2,3]N[3,0], M[2,0]N[0,1]+M[2,1]N[1,1]+M[2,2]N[2,1]+M[2,3]N[3,1], M[2,0]N[0,2]+M[2,1]N[1,2]+M[2,2]N[2,2]+M[2,3]N[3,2], M[2,0]N[0,3]+M[2,1]N[1,3]+M[2,2]N[2,3]+M[2,3]N[3,3]|
 *		 |M[3,0]N[0,0]+M[3,1]N[1,0]+M[3,2]N[2,0]+M[3,3]N[3,0], M[3,0]N[0,1]+M[3,1]N[1,1]+M[3,2]N[2,1]+M[3,3]N[3,1], M[3,0]N[0,2]+M[3,1]N[1,2]+M[3,2]N[2,2]+M[3,3]N[3,2], M[3,0]N[0,3]+M[3,1]N[1,3]+M[3,2]N[2,3]+M[3,3]N[3,3]|
 *	@param	_m	[out]��
 *	@param	m	�s��
 *	@param	n	������s��
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
 *	��s����擾����B
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
 *	�P�ʍs����擾����B
 *
 *	|1, 0, 0, 0|
 *	|0, 1, 0, 0|
 *	|0, 0, 1, 0|
 *	|0, 0, 0, 1|
 *
 *	@param	_m		[out]�P�ʍs��
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
 *	�s�񎮂��v�Z����B
 *
 *	M	= |a11, a12, a13, a14|
 *		  |a21, a22, a23, a24|
 *		  |a31, a32, a33, a34|
 *		  |a41, a42, a43, a44|
 *
 *	�s��detM��
 *	detM = a11a22a33a44 + a11a23a34a42 + a11a24a32a43
 *		 + a12a21a34a43 + a12a23a31a44 + a12a24a33a41 
 *		 + a13a21a32a44 + a13a22a34a41 + a13a24a31a42
 *		 + a14a21a33a42 + a14a22a31a43 + a14a23a32a41 
 *		 - a11a22a34a43 - a11a23a32a44 - a11a24a33a42
 *		 - a12a21a33a44 - a12a23a34a41 - a12a24a31a43
 *		 - a13a21a34a42 - a13a22a31a44 - a13a24a32a41
 *		 - a14a21a32a43 - a14a22a33a41 - a14a23a31a42
 *		   
 *	@param	m	�s��
 *	@return		�s��
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
 *	�s���i�sj����������s����擾����B
 *
 *	M����i�sj�����苎�����s���M~ij�Ƃ���(���͗�)
 *	M~12 = |a21, a23, a24|
 *		   |a31, a33, a34|
 *		   |a41, a43, a44|
 *	
 *	@param	_m	[out]�s��
 *	@param	m	�s��
 *	@param	i	��菜���s
 *	@param	j	��菜����
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
 *	�t�s����v�Z����B
 *
 *	M	= |a11, a12, a13, a14|
 *		  |a21, a22, a23, a24|
 *		  |a31, a32, a33, a34|
 *		  |a41, a42, a43, a44|
 *
 *	M����i�sj�����苎�����s���M~ij�Ƃ���(���͗�)
 *	M~12 = |a21, a23, a24|
 *		   |a31, a33, a34|
 *		   |a41, a43, a44|
 *
 *	�t�s��M^-1��
 *	M^-1= 1 / detM * | detM~11, -detM~21,  detM~31, -detM~41|
 *					 |-detM~12,  detM~22, -detM~32,  detM~42|
 *					 | detM~13, -detM~23,  detM~33, -detM~43|
 *					 | detM~14, -detM~24,  detM~34, -detM~44|
 *
 *	@param	_m	[out]�t�s��
 *	@param	m	�s��
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

	// �W�Jver. ... �͐����߂�ǂ������̂Ŏ~�߁I
}


/**
 *	Matrix4RotationX()
 *	�R���������x�N�g����X������ɉ�]������s����擾����B
 *
 *	|1, 0,		0,		0|
 *	|0, cos��,  sin��,	0|
 *	|0, -sin��, cos��,	0|
 *	|0, 0,		0,		1|
 *
 *	@para_m	_m		[out]��]�s��
 *	@para_m	theta	�p�x(���W�A��)
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
 *	�R���������x�N�g����Y������ɉ�]������s����擾����B
 *
 *	|cos��, 0,	-sin��,	0|
 *	|0,		1,	0,		0|
 *	|sin��, 0,	cos��,	0|
 *	|0,		0,	0,		1|
 *
 *	@para_m	_m		[out]��]�s��
 *	@para_m	theta	�p�x(���W�A��)
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
 *	�R���������x�N�g����Z������ɉ�]������s����擾����B
 *
 *	|cos��,  sin��, 0, 0|
 *	|-sin��, cos��, 0, 0|
 *	|0,		 0,		1, 0|
 *	|0,		 0,		0, 1|
 *
 *	@para_m	_m		[out]��]�s��
 *	@para_m	theta	�p�x(���W�A��)
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
 *	�R���������x�N�g����C�ӂ̎�����ɉ�]������s����擾����B
 *
 *	|c+(1-c)Vx^2,	(1-c)VxVy+sVz,	(1-c)VxVz-sVy,	0|
 *	|(1-c)VxVy-sVz, c+(1-c)Vy^2,	(1-c)VyVz+sVx,	0|
 *	|(1-c)VxVz+sVy,	(1-c)VyVz-sVx,	c+(1-c)Vz^2,	0|
 *	|0,				0,				0,				1|
 *	��cos�� = c, sin�� = s
 *
 *	@para_m	_m		[out]��]�s��
 *	@para_m	theta	�p�x(���W�A��)
 *	@para_m	v		����\��Vector3(���K���ς݂̂���)
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
 *	�R���������x�N�g����C�ӂ̃x�N�g���̎��������ɉ�]������s����擾����B
 *
 *	�C�ӂ̃x�N�g����z���̐����������������x�N�g���Ƃ݂Ȃ�
 *	vZ = normalize(v)
 *	vX = v(0, 1, 0)�~vZ
 *	vY = vZ�~vX 
 *
 *	|vXx, vXy, vXz, 0|
 *	|vYx, vYy, vYz, 0|
 *	|vZx, vZy, vZz, 0|
 *	|  0,   0,   0, 1|
 *
 *	@param[out]	_m	�ϊ��s��
 *	@param	v	����
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
 *	�R���������x�N�g�����g�k����s����擾����B
 *
 *	|sx,  0,  0,  0|
 *	| 0, sy,  0,  0|
 *	| 0,  0, sz,  0|
 *	| 0,  0,  0,  1|
 *
 *	@param	_m	[out]�ϊ��s��
 *	@param	sx	x���W�̊g�k�W��
 *	@param	sy	y���W�̊g�k�W��
 *	@param	sz	z���W�̊g�k�W��
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
 *	�R���������x�N�g���𕽍s�ړ�����s����擾����B
 *
 *	|1, 0, 0, 0|
 *	|0, 1, 0, 0|
 *	|0, 0, 1, 0|
 *	|x, y, z, 1|
 *
 *	@param	_m	[out]���s�ړ��s��
 *	@param	x	x���W�̈ړ���
 *	@param	y	y���W�̈ړ���
 *	@param	z	z���W�̈ړ���
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
 *	�R���������x�N�g�����J�������W�n�ɕϊ�����s����擾����B
 *	
 *	@param	_m	[out]�r���[�ϊ��s��
 *	@param	eye	�J�������W
 *	@param	at	�����_
 *	@param	up	�J�����̏�̍��W(��ʓI�ɂ�(0,1,0)?)
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
 *	�R���������x�N�g���𓧎��ˉe�ϊ�����s����擾����B
 *
 *	Matarix4 mProj;
 *	Matrix4Projection(mProj, fovV, aspect, zn, zf);
 *	Vector4 v = (x, y, z, w);
 *	v = mProj * v;	// ���ԂɋC������I
 *	v /= v.w;		// �Z�o���ꂽw���W�Ŋ���I
 *
 *	�܂��A���̍s��Ōv�Z���ꂽ�x�N�g��v�́Ax,y�̒l��-1�`1�͈̔͂����B
 *	�ʓrMatrix4Screen()�œ����s��������āA
 *	�ŏI�I�ȃX�N���[�����W�֕ϊ�����K�v������B
 *	
 *	@param	_m		[out]�ˉe�ϊ��s��
 *	@param	fovV	���������̎���p
 *	@param	aspect	�A�X�y�N�g��(��/����)
 *	@param	zn		������̋ߕ��ʂ܂ł̋���
 *	@param	zf		������̉����ʂ܂ł̋���
 */
void Matrix4Projection(Matrix4& _m, float fovV, float aspect, float zn, float zf)
{
	float R, L, T, B;	// ������̋ߕ��ʂ̎l�ӂ�XY���W

	//T = sin(fovV*0.5f)*zn/cos(fovV*0.5f);
	T = zn * tan(fovV*0.5f);
	B = -T;

	float fovH = atan(T*aspect/zn)*2.0f;	// ���������̎���p�i�����ƍœK������atan�����饥��j
	//R = sin(fovH*0.5f)*zn/cos(fovH*0.5f);
	R = zn * tan(fovH*0.5f);
	L = -R;

	Matrix4Zero(_m);

	_m(0,0) = (2.0f*zn) / (R-L) * -1;	// !x���W�̐��������E��ɂ���悤�Ƀ}�C�i�X�������Ă���
	_m(0,2) = (R+L) / (R-L);
	_m(1,1) = (2.0f*zn) / (T-B);
	_m(1,2) = (T+B) / (T-B);
	_m(2,2) = - (zf+zn) / (zf-zn);
	_m(2,3) = - (2.0f*zn*zf) / (zf-zn);
	_m(3,2) = -1.0f;
}


/**
 *	Matrix4Screen()
 *	�����ˉe�����R���������x�N�g�����X�N���[�����W�ɕϊ�����s����擾����B
 *
 *	|w/2, 0,   0, 0|
 *	|0,   h/2, 0, 0|
 *	|0,   0,   1, 0|
 *	|w/2, h/2, 0, 1|
 *
 *	@param	_m		[out]�X�N���[�����W�ϊ��s��
 *	@param	width	�X�N���[������
 *	@param	height	�X�N���[���c��
 */
void Matrix4Screen(Matrix4& _m, float width, float height)
{
	Matrix4Identity(_m);

	_m(0,0) = width  / 2.0f;
	_m(1,1) = height / 2.0f;
	_m(3,0) = width  / 2.0f;
	_m(3,1) = height / 2.0f;
}


