/**
 *	Game/Test3D.h
 *
 *	�R�c���̂̏Փ˔�������֐��B
 *	�x�N�g���N���X�ƃ}�g���N�X�N���X���g���Ă���̂�
 *	�v���W�F�N�g�ɒǉ����鎖�B
 *
 *	@author	Dila
 */

#pragma once

#include "Math/Vector.h"
#include "Math/Matrix.h"

////

// ����BOX
bool TestSphereBox(
		const Vector3& vSphere, float radius,
		const Vector3& vBox,
		float boxW, float boxH, float boxD);

// ������������BOX
bool TestMovingSphereMovingBox(
		const Vector3& vSphere, float radius,
		const Vector3& vSphereSpd,
		const Vector3& vBox,
		const Vector3& vBoxW,
		const Vector3& vBoxH,
		const Vector3& vBoxD,
		const Vector3& vBoxSpd,
		Vector3* pvTest);


////

// Clamp()
inline float Clamp(float u, float v, float w) {
	if (w < u ) return w;
	if (v < u && u < w) return u;
	if (u < v) return v;
	return v;
}
