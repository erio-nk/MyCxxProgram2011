/**
 *	Game/Test3D.cpp
 *
 *	@author	Dila
 */

#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "Game/Test3D.h"

#include <stdio.h>

/**
 *	TestSphereBox()
 *	����BOX�̏Փ˔���B
 *	BOX�̊e�ӂ͍��W���ɒ������Ă���K�v������B
 *
 *	@param	vSphere	���̒��S���W
 *	@param	radius	���̔��a
 *	@param	vBox	BOX�̒��S���W
 *	@param	boxW	BOX�̉���/2
 *	@param	boxH	BOX�̏c��/2
 *	@param	boxD	BOX�̉��s��/2
 */
bool TestSphereBox(
	const Vector3& vSphere, float radius,
	const Vector3& vBox,
	float boxW, float boxH, float boxD)
{
	Vector3 v = vSphere - vBox, c;

	c.x = Clamp(v.x, -boxW*0.5f, boxW*0.5f);
	c.y = Clamp(v.y, -boxH*0.5f, boxH*0.5f);
	c.z = Clamp(v.z, -boxD*0.5f, boxD*0.5f);

	v = c - v;

	if (v.LengthSq() < radius*radius) {
		return true;
	}

	return false;
}


// ���������܂�����ɍ쐬�r���B

/**
 *	TestMovingSphereMovingBox()
 *	�������Ɠ���BOX�̏Փ˔���B
 *	�Q�l�F�Q�[���v���O���~���O�̂��߂̂R�c�O���t�B�b�N�X���w
 *			8.2.2 ���ƃ{�b�N�X�̏Փ�
 *	@param	vSphere		���̍��W
 *	@param	radius		���̔��a
 *	@param	vSphereSpd	���̑��x
 *	@param	vBox		BOX�̍��W
 *	@param	vBoxW		BOX�̉����������x�N�g��
 *	@param	vBoxH		BOX�̏c���������x�N�g��
 *	@param	vBoxD		BOX�̉��s���������x�N�g��
 *	@param	vBoxSpd		BOX�̑��x
 *	@param	pvTest		[OUT]�Փ˂������̋��̍��W
 *						NULL���w�肵���ꍇ�͖��������
 */
bool TestMovingSphereMovingBox(
	const Vector3& vSphere,
	float radius,
	const Vector3& vSphereSpd,
	const Vector3& vBox,
	const Vector3& vBoxW,
	const Vector3& vBoxH,
	const Vector3& vBoxD,
	const Vector3& vBoxSpd,
	Vector3* pvTest)
{
	Vector4 V(vSphereSpd - vBoxSpd, 0);	// BOX���猩�����̑��Α��x

	// BOX�̒��_G�����_�ɂ������W�n�ɂ���}�g���N�X
	Vector3 R = vBoxW;
	Vector3 S = vBoxH;
	Vector3 T = vBoxD;
	Vector3 G = vBox - 0.5f*(R + S + T);

	Vector3 _R = R.Normal();
	Vector3 _S = S.Normal();
	Vector3 _T = T.Normal();

	float Rlen = R.Length();
	float Slen = S.Length();
	float Tlen = T.Length();

	Matrix4 M, Mi;
	Matrix4Translation(M, (-_R).Dot(G), (-_S).Dot(G), (-_T).Dot(G));
	M(0,0) = _R.x;	M(1,0) = _R.y;	M(2,0) = _R.z;
	M(0,1) = _S.x;	M(1,1) = _S.y;	M(2,1) = _S.z;
	M(0,2) = _T.x;	M(1,2) = _T.y;	M(2,2) = _T.z;
	Matrix4Inverse(Mi, M);

	// ����ϊ�����
	Vector4 P(vSphere, 1);
	P = P * M;	// ���̏����ʒu
	V = V * M;	// ���̑��x

	//// BOX�𔼌a�����g�債�����̂Ƃ̃e�X�g������
	float t, d;

	// 6�̖ʕ��i�̖@���Ƒ��x�x�N�g�����`�F�b�N���āA�e�X�g����ʂ����肷��
	bool men[6] = { true, true, true, true, true, true };
	if ((d = V.Dot(Vector4(-1,0,0,0))) < 0.0f)	{ men[1] = false; }
	else if (d > 0.0f)							{ men[0] = false; }
	if ((d = V.Dot(Vector4(0,-1,0,0))) < 0.0f)	{ men[3] = false; }
	else if (d > 0.0f)							{ men[2] = false; }
	if ((d = V.Dot(Vector4(0,0,-1,0))) < 0.0f)	{ men[5] = false; }
	else if (d > 0.0f)							{ men[4] = false; }

	// 6�̖ʕ��i�Ƃ̃e�X�g
	bool TestMovingSphereMovingBox_Test1(
		float Px, float Py, float Pz, float Vx, float Vy, float Vz,
		float r, float R, float Slen, float Tlen, float& t);
	bool b =
		(men[0] && TestMovingSphereMovingBox_Test1(P.x,P.y,P.z, V.x,V.y,V.z, radius, -radius,	  Slen, Tlen, t)) ||
		(men[1] && TestMovingSphereMovingBox_Test1(P.x,P.y,P.z, V.x,V.y,V.z, radius, Rlen+radius, Slen, Tlen, t)) ||
		(men[2] && TestMovingSphereMovingBox_Test1(P.y,P.x,P.z, V.y,V.x,V.z, radius, -radius,	  Rlen, Tlen, t)) ||
		(men[3] && TestMovingSphereMovingBox_Test1(P.y,P.x,P.z, V.y,V.x,V.z, radius, Slen+radius, Rlen, Tlen, t)) ||
		(men[4] && TestMovingSphereMovingBox_Test1(P.z,P.x,P.y, V.z,V.x,V.y, radius, -radius,	  Rlen, Slen, t)) ||
		(men[5] && TestMovingSphereMovingBox_Test1(P.z,P.x,P.y, V.z,V.x,V.y, radius, Tlen+radius, Rlen, Slen, t));

	// 6�̖ʕ��i�Ƃ̃e�X�g
	//bool TestMovingSphereMovingBox_Test1(
	//	float Px, float Py, float Pz, float Vx, float Vy, float Vz,
	//	float r,const Vector4& V, const Vector4& N,
	//	float Rlen,	float Slen, float Tlen, float& t);
	//bool b =
	//	TestMovingSphereMovingBox_Test1(P.x, P.y, P.z, V.x, V.y, V.z, radius, V, Vector4(-1,0,0,0), Rlen, Slen, Tlen, t) ||
	//	TestMovingSphereMovingBox_Test1(P.y, P.x, P.z, V.y, V.x, V.z, radius, V, Vector4(0,-1,0,0), Slen, Rlen, Tlen, t) ||
	//	TestMovingSphereMovingBox_Test1(P.z, P.x, P.y, V.z, V.x, V.y, radius, V, Vector4(0,0,-1,0), Tlen, Rlen, Slen, t);
	
	if (b) {
		if (pvTest != 0) {
			Vector4 v(P.x+t*V.x, P.y+t*V.y, P.z+t*V.z, 1);
			Matrix4 Mi;
			Matrix4Inverse(Mi, M);
			v = v * Mi;	// ���̍��W�n�ɖ߂�
			pvTest->Set(v.x, v.y, v.z);
		}
		return true;
	}

	// 12�̉~�����i�Ƃ̃e�X�g
	bool TestMovingSphereMovingBox_Test2(
		float Px, float Py, float Pz, float Vx, float Vy, float Vz,
		float r, float Tlen, float& t);

	if (b) {
		if (pvTest != 0) {
			Vector4 v(P.x+t*V.x, P.y+t*V.y, P.z+t*V.z, 1);
			Matrix4 Mi;
			Matrix4Inverse(Mi, M);
			v = v * Mi;	// ���̍��W�n�ɖ߂�
			pvTest->Set(v.x, v.y, v.z);
		}
		return true;
	}

	return false;

}

/*bool TestMovingSphereMovingBox_Test1(
	float Px, float Py, float Pz, float Vx, float Vy, float Vz,
	float r,const Vector4& V, const Vector4& N,
	float Rlen,	float Slen, float Tlen, float& t)
{
	float d;
	t = -1.0f;

	if ((d = V.Dot(N)) < 0.0f) {
		// ����[x=-r]�ƏՓ˂���t = (-r-Px)/Vx
		t = (-r - Px) / Vx;
	}
	else if (d > 0.0f) {
		// ����[x=r+|R|]�ƏՓ˂���t = (|R|+r-Px)/Vx
		t = (Rlen + r - Px) / Vx;
	}
	if (0.0f <= t && t <= 1.0f) {
		// x�����ʂɏՓ˂��鎞��yz���w��̋�ԓ��ł���ΏՓ�
		float y = Py + t*Vy;
		float z = Pz + t*Vz;
		if (0.0f <= y && y <= Slen &&
			0.0f <= z && z <= Tlen)
		{
			return true;
		}
	}
	
	return false;
}*/

bool TestMovingSphereMovingBox_Test1(
	float Px, float Py, float Pz, float Vx, float Vy, float Vz,
	float r, float R, float Slen, float Tlen, float& t)
{
	t = -1.0f;

	// ����[x=R]�ƏՓ˂���t = (R-Px)/Vx
	t = (R - Px) / Vx;

	if (0.0f <= t && t <= 1.0f) {
		// x�����ʂɏՓ˂��鎞��yz���w��̋�ԓ��ł���ΏՓ�
		float y = Py + t*Vy;
		float z = Pz + t*Vz;
		if (0.0f <= y && y <= Slen &&
			0.0f <= z && z <= Tlen)
		{
			return true;
		}
	}
	
	return false;
}

bool TestMovingSphereMovingBox_Test2(
	float Px, float Py, float Pz, float Vx, float Vy, float Vz,
	float r, float Tlen, float& t)
{
	float a = Vx*Vx + Vy*Vy;
	float b = 2*(Px*Vx + Py*Vy);
	float c = Px*Px + Py*Py - r*r;
	float D = b*b - 4*a*c;
	if (D <= 0.0f) return false;
	
	t = (-b - sqrt(D)) / 2*a;

	if (0.0f <= t && t <= 1.0f) {
		float z = Pz + t*Vz;
		if (0.0f <= z && z <= Tlen) {
			return true;
		}
	}
	return false;
}

