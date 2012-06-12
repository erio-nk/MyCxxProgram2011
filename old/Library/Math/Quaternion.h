/**
 *	@file Math/Quaternion.h
 *
 *	�N�H�[�^�j�I��(�l����)�N���X
 *
 *	��ɕ׋��p�Ȃ̂ő��x�͏d�����ĂȂ��B
 *	�x�N�g���A�s��N���X���g���̂ŃC���N���[�h���鎖�B
 *
 *	�N�H�[�^�j�I���̕\�L
 *	Q = (w; x, y, z) = (w; V) = w + xi + yj + zk
 *
 *	�N�H�[�^�j�I���ɂ���]
 *	��V����ɃƉ�]����N�H�[�^�j�I����Q�Ƃ��A
 *	��]������x�N�g�����N�H�[�^�j�I��P = (0; x,y,z)�ŕ\�����ꍇ�A
 *	��]��̃x�N�g����\���N�H�[�^�j�I��P'�́A
 *	Q�̋���R�ƃT���h�C�b�`�Ɋ|�����킹�Čv�Z����B
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
 *	�N�H�[�^�j�I���N���X
 */
class Quaternion
{
public:
	float w;	/// w�v�f
	float x;	/// x�v�f
	float y;	/// y�v�f
	float z;	/// z�v�f

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

	// �N�H�[�^�j�I���̉��Z
	Quaternion operator + (const Quaternion& p);

	// �N�H�[�^�j�I���̌��Z
	Quaternion operator - (const Quaternion& p);

	// �N�H�[�^�j�I���̉��Z���
	Quaternion& operator += (const Quaternion& p);

	// �N�H�[�^�j�I���̌��Z���
	Quaternion& operator -= (const Quaternion& p);

	// �N�H�[�^�j�I���̏�Z
	Quaternion operator * (const Quaternion& p);

	// �N�H�[�^�j�I���̏�Z���
	Quaternion& operator *= (const Quaternion& p);

	// �N�H�[�^�j�I���̒�����Ԃ�
	float Length() const;

	// �N�H�[�^�j�I���̒����̓���Ԃ�
	float LengthSq() const;

	// �N�H�[�^�j�I���𐳋K������
	void Normalize();
};

// �X�J���[�ƃN�H�[�^�j�I���̐ς��v�Z����
Quaternion operator * (float c, const Quaternion& q);

// �N�H�[�^�j�I���̐ς��v�Z����
void QuaternionMultiply(Quaternion& _q, const Quaternion& q, const Quaternion& p);


// �P�ʃN�H�[�^�j�I��
void QuaternionIdentity(Quaternion& _q);

// �����N�H�[�^�j�I��
void QuaternionConjugate(Quaternion& _q, const Quaternion& q);

// �t�N�H�[�^�j�I��
void QuaternionInverse(Quaternion& _q, const Quaternion& q);



// �R�����x�N�g����C�ӂ̎�����ɉ�]������N�H�[�^�j�I��
void QuaternionRotationAxis(Quaternion& _q, float theta, const Vector3& v);


// �N�H�[�^�j�I������s��
void Matrix4RotationQuaternion(Matrix4& _m, const Quaternion& q);


/// @}
