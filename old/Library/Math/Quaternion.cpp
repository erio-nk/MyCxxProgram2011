/*
 *	Math/Quaternion.cpp
 *
 *	@author	Dila
 */

#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "Math/Quaternion.h"


/**
 *	�N�H�[�^�j�I���̉��Z
 *
 *	Q+P = (Qw+Pw) + (Qx+Px)i + (Qy+Py)j + (Qz+Pz)k
 */
Quaternion Quaternion::operator + (const Quaternion& p)
{
	return Quaternion(w+p.w, x+p.x, y+p.y, z+p.z);
}

/**
 *	�N�H�[�^�j�I���̌��Z
 */
Quaternion Quaternion::operator - (const Quaternion& p)
{
	return Quaternion(w-p.w, x-p.x, y-p.y, z-p.z);
}

/**
 *	�N�H�[�^�j�I���̉��Z���
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
 *	�N�H�[�^�j�I���̌��Z���
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
 *	�N�H�[�^�j�I���̏�Z
 */
Quaternion Quaternion::operator * (const Quaternion& p)
{
	Quaternion _q;
	QuaternionMultiply(_q, *this, p);
	return _q;
}

/**
 *	�N�H�[�^�j�I���̏�Z���
 */
Quaternion& Quaternion::operator *= (const Quaternion& p)
{
	Quaternion _q;
	QuaternionMultiply(_q, *this, p);
	*this = _q;
	return *this;
}

/**
 *	�N�H�[�^�j�I���̒������擾����
 *
 *	|Q| = sqrt(Qw^2 + Qx^2 + Qy^2 + Qz^2)
 *	@return	�N�H�[�^�j�I���̒���
 */
float Quaternion::Length() const
{
	return sqrt(LengthSq());
}

/**
 *	�N�H�[�^�j�I���̒����̓����擾����
 *
 *	Qw^2 + Qx^2 + Qy^2 + Qz^2
 *	@return	�N�H�[�^�j�I���̒����̓��
 */
float Quaternion::LengthSq() const
{
	return w*w + x*x + y*y + z*z;
}


/**
 *	�N�H�[�^�j�I���𐳋K������
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
 *	�X�J���[�ƃN�H�[�^�j�I���̐�
 *
 *	cQ = cQw + cQxi + cQyj + cQzk
 */
Quaternion operator * (float c, const Quaternion& p)
{
	return Quaternion(c*p.w, c*p.x, c*p.y, c*p.z);
}

/**
 *	�N�H�[�^�j�I���̐ς��v�Z����
 *
 *	Q*P = (Qw+Qxi+Qyj+Qzk)(Pw+Pxi+Pyj+Pzk)
 *		= (QwPw-QxPx-QyPy-QzPz) + (QwPx+PwQx+PyQz-PzQy)i
 *		  (QwPy+PwQy+PzQx-PxQz)j+ (QwPz+PwQz+PxQy-PyQx)k
 *	@param	[out] _q	��
 *	@param	q	�N�H�[�^�j�I��
 *	@param	p	������N�H�[�^�j�I��
 */
void QuaternionMultiply(Quaternion& _q, const Quaternion& q, const Quaternion& p)
{
	_q.w = q.w*p.w - q.x*p.x - q.y*p.y - q.z*p.z;
	_q.x = q.w*p.x + p.w*q.x + p.y*q.z - p.z*q.y;
	_q.y = q.w*p.y + p.w*q.y + p.z*q.x - p.x*q.z;
	_q.z = q.w*p.z + p.w*q.z + p.x*q.y - p.y*q.x;
}


/**
 *	�P�ʃN�H�[�^�j�I�����擾����
 *
 *	Q = (1; 0, 0, 0)
 *	@param	[out] _q	�P�ʃN�H�[�^�j�I��
 */
void QuaternionIdentity(Quaternion& _q)
{
	_q.w = 1.0f;
	_q.x = 0.0f;
	_q.y = 0.0f;
	_q.z = 0.0f;
}


/**
 *	�N�H�[�^�j�I���̋������擾����
 *
 *	Q* = Qw - Qx - Qy - Qz
 *
 *	@param	[out] _q	�����N�H�[�^�j�I��
 *	@param	q	�N�H�[�^�j�I��
 */
void QuaternionConjugate(Quaternion& _q, const Quaternion& q)
{
	_q.w = q.w;
	_q.x = -q.x;
	_q.y = -q.y;
	_q.z = -q.z;
}

/**
 *	�t�N�H�[�^�j�I�����擾����
 *
 *	Q�̋�����Q*, ������|Q|�Ƃ������A
 *	�t�N�H�[�^�j�I��Q^-1�́A
 *
 *	Q^-1 = Q* / |Q|^2
 *		 = (w; -V) |Q|^2
 *		 = (w/|Q|^2; x/|Q|^2, y/|Q|^2, z/|Q|^2)
 *
 *	@param	[out] _q	�t�N�H�[�^�j�I��
 *	@param	q	�N�H�[�^�j�I��
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
 *	3D�x�N�g����C�ӂ̎�����ɉ�]������N�H�[�^�j�I�����擾����
 *
 *	�C�ӂ̎�V����ɃƉ�]����N�H�[�^�j�I��Q(��,V)��
 *	Q(��,V) = cos(��/2) + (Vxi + Vyj + Vzk)sin(��/2)
 *
 *	@param	[out] _q	��]�N�H�[�^�j�I��
 *	@param	theta	��]������p�x(���W�A��)
 *	@param	v		��]��
 */
void QuaternionRotationAxis(Quaternion& _q, float theta, const Vector3& v)
{
	_q.w = cos(theta*0.5f);
	_q.x = v.x * sin(theta*0.5f);
	_q.y = v.y * sin(theta*0.5f);
	_q.z = v.z * sin(theta*0.5f);
}



/**
 *	�N�H�[�^�j�I������4x4��]�s����擾����
 *	
 *	|1-2(yy+zz), 2(xy+zw),   2(zx-wy),   0|
 *	|2(xy-zw),   1-2(zz+xx), 2(yz+xw),   0|
 *	|2(zx+wy),   2(yz-wx),   1-2(xx+yy), 0|
 *	|0,			 0,			 0,			 1|
 *
 *	@param	[out] _m	��]�s��
 *	@param	q	�N�H�[�^�j�I��
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



