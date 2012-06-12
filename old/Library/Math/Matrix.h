/**
 *	Math/Matrix.h
 *
 *	�s��N���X�B
 *	��ɕ׋��p�Ȃ̂ő��x�͏d�����ĂȂ��B
 *	�x�N�g���N���X���g���̂ŃC���N���[�h���鎖�B
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
 *	�Q���Q�s��N���X�B
 *
 *	Matrix2 m;
 *	m(0,1) = 2;
 */
class Matrix2 {
public:
	union {
		struct { float a, b, c, d; };	// ���w�̋��ȏ��ɂ��킹�č��������
		struct { float p, q, r, s; };	// ����
		float m[2][2];
	};

	// �v�f�Q��	
	float& operator () (int r, int c) {
		assert((0 <= r && r <= 1) || (0 <= c && c <= 1));
		return m[r][c];
	}
	float operator () (int r, int c) const {
		assert((0 <= r && r <= 1) || (0 <= c && c <= 1));
		return m[r][c];
	}

	// ���Z
	Matrix2 operator + (const Matrix2& n);
	Matrix2 operator - (const Matrix2& n);
	Matrix2& operator += (const Matrix2& n);
	Matrix2& operator -= (const Matrix2& n);

	Matrix2 operator * (const Matrix2& n);
	Matrix2& operator *= (const Matrix2& n);
};

// �������s��
Matrix2 operator * (float k, const Matrix2& m);

// �s�x�N�g�����s��
Vector2 operator * (const Vector2& v, const Matrix2& m);

// �s�񁖗�x�N�g��
Vector2 operator * (const Matrix2& m, const Vector2& v);


// �s�x�N�g���Ɨ�x�N�g���̐�
float Vector2Multiply(const Vector2& v, const Vector2 u);

// �s�x�N�g���ƍs��̐�
void Vector2Multiply(Vector2& _v, const Vector2& v, const Matrix2& m);

// �s��Ɨ�x�N�g���̐�
void Matrix2Multiply(Vector2& _v, const Matrix2& m, const Vector2& v);

// �s��̐�
void Matrix2Multiply(Matrix2& _m, const Matrix2& m, const Matrix2& n);


// ��s��
void Matrix2Zero(Matrix2& _m);

// �P�ʍs��
void Matrix2Identity(Matrix2& _m);

// �s��
float Matrix2Determinant(const Matrix2& m);

// �t�s��
void Matrix2Inverse(Matrix2& _m, const Matrix2& m);

// �Q�����x�N�g������]������s��
void Matrix2Rotation(Matrix2& _m, float theta);

// �Q�����x�N�g�����g�傳����s��
void Matrix2Scaling(Matrix2& _m, float sx, float sy);


/* ------------------------------------------------------------------------- */
// Matrix3

/**
 *	Matrix3
 *	�R���R�s��N���X�B
 */
class Matrix3 {
public:
	float m[3][3];

	// �v�f�Q��
	float& operator () (int r, int c) {
		assert((0 <= r && r <= 2) || (0 <= c && c <= 2));
		return m[r][c];
	}
	float operator () (int r, int c) const {
		assert((0 <= r && r <= 2) || (0 <= c && c <= 2));
		return m[r][c];
	}


	// ���Z
	Matrix3 operator + (const Matrix3& n);
	Matrix3 operator - (const Matrix3& n);
	Matrix3& operator += (const Matrix3& n);
	Matrix3& operator -= (const Matrix3& n);

	Matrix3 operator * (const Matrix3& n);
	Matrix3& operator *= (const Matrix3& n);
};

// �������s��
Matrix3 operator * (float k, const Matrix3& m);

// �s�x�N�g�����s��
Vector3 operator * (const Vector3& v, const Matrix3& m);

// �s�񁖗�x�N�g��
Vector3 operator * (const Matrix3& m, const Vector3& v);



// �s�x�N�g���Ɨ�x�N�g���̐�
float Vector3Multiply(const Vector3& v, const Vector3& u);

// �s�x�N�g���ƍs��̐�
void Vector3Multiply(Vector3& _v, const Vector3& v, const Matrix3& m);

// �s��Ɨ�x�N�g���̐�
void Matrix3Multiply(Vector3& _v, const Matrix3& m, const Vector3& v);

// �s��̐�
void Matrix3Multiply(Matrix3& _m, const Matrix3& m, const Matrix3& n);


// ��s��
void Matrix3Zero(Matrix3& _m);

// �P�ʍs��
void Matrix3Identity(Matrix3& _m);
// �s��
float Matrix3Determinant(const Matrix3& m);

// �s���i�sj����������s��
void Matrix3Tilde(Matrix2& _m, const Matrix3& m, int i, int j);

// �t�s��
void Matrix3Inverse(Matrix3& _m, const Matrix3& m);


// �R�����x�N�g����X������ɉ�]������s��
void Matrix3RotationX(Matrix3& _m, float theta);

// �R�����x�N�g����Y������ɉ�]������s��
void Matrix3RotationY(Matrix3& _m, float theta);

// �R�����x�N�g����Z������ɉ�]������s��
void Matrix3RotationZ(Matrix3& _m, float theta);

// �R�����x�N�g����C�ӂ̎�����ɉ�]������s��
void Matrix3RotationAxis(Matrix3& _m, float theta, const Vector3& v);

// �R�����x�N�g�����g�傳����s��
void Matrix3Scaling(Matrix3& _m, float sx, float sy, float sz);


// �Q���������x�N�g������]������s�� (HV = Homogeneous Vector = �����x�N�g��)
void Matrix3RotationHV(Matrix3& _m, float theta);

// �Q���������x�N�g���𕽍s�ړ�������s��
void Matrix3TranslationHV(Matrix3& _m, float x, float y);



/* ------------------------------------------------------------------------- */
// Matrix4

/**
 *	Matrix4
 *	�S���S�s��N���X�B
 */
class Matrix4 {
public:
	float m[4][4];

	// �v�f�Q��
	float& operator () (int r, int c) {
		assert((0 <= r && r <= 3) || (0 <= c && c <= 3));
		return m[r][c];
	}
	float operator () (int r, int c) const {
		assert((0 <= r && r <= 3) || (0 <= c && c <= 3));
		return m[r][c];
	}


	// ���Z
	Matrix4 operator + (const Matrix4& n);
	Matrix4 operator - (const Matrix4& n);

	Matrix4& operator += (const Matrix4& n);
	Matrix4& operator -= (const Matrix4& n);

	Matrix4 operator * (const Matrix4& n);
	Matrix4& operator *= (const Matrix4& n);
};

// �������s��
Matrix4 operator * (float k, const Matrix4& m);

// �s�x�N�g�����s��
Vector4 operator * (const Vector4& v, const Matrix4& m);

// �s�񁖗�x�N�g��
Vector4 operator * (const Matrix4& m, const Vector4& v);


// ��s��
void Matrix4Zero(Matrix4& _m);

// �P�ʍs��
void Matrix4Identity(Matrix4& _m);

// �s�x�N�g���Ɨ�x�N�g���̐�
float Vector4Multiply(const Vector4& v, const Vector3& u);

// �s�x�N�g���ƍs��̐�
void Vector4Multiply(Vector4& _v, const Vector4& v, const Matrix4& m);

// �s��Ɨ�x�N�g���̐�
void Matrix4Multiply(Vector4& _v, const Matrix4& m, const Vector4& v);

// �s��̐�
void Matrix4Multiply(Matrix4& _m, const Matrix4& m, const Matrix4& n);

// �s��
float Matrix4Determinant(const Matrix4& m);

// �s���i�sj����������s��
void Matrix4Tilde(Matrix3& _m, const Matrix4& m, int i, int j);

// �t�s��
void Matrix4Inverse(Matrix4& _m, const Matrix4& m);


// ���S�����x�N�g���ɂ��Ďg�����͏��Ȃ��̂�HV�͏ȗ����

// �R���������x�N�g����X������ɉ�]������s��
void Matrix4RotationX(Matrix4& _m, float theta);

// �R���������x�N�g����Y������ɉ�]������s��
void Matrix4RotationY(Matrix4& _m, float theta);

// �R���������x�N�g����Z������ɉ�]������s��
void Matrix4RotationZ(Matrix4& _m, float theta);

// �R���������x�N�g����C�ӂ̎�����ɉ�]������s��
void Matrix4RotationAxis(Matrix4& _m, float theta, const Vector3& v);

// �R���������x�N�g����C�ӂ̃x�N�g���̌����ɉ�]������s��
void Matrix4RotationVector(Matrix4& _m, const Vector3& v);

// �R���������x�N�g�����g�傳����s��
void Matrix4Scaling(Matrix4& _m, float sx, float sy, float sz);

// �R���������x�N�g���𕽍s�ړ�������s��
void Matrix4Translation(Matrix4& _m, float x, float y, float z);

// �R���������x�N�g�����J�������W�n�ɕϊ�����s��
void Matrix4View(Matrix4& _m, const Vector3& eye, const Vector3& at, const Vector3& up);

// �R���������x�N�g���𓧎��ˉe�ϊ�����s��
void Matrix4Projection(Matrix4& _m, float fovV, float aspect, float zn, float zf);

// �����ˉe�����R���������x�N�g�����X�N���[�����W�ɕϊ�����s��
void Matrix4Screen(Matrix4& _m, float width, float height);

