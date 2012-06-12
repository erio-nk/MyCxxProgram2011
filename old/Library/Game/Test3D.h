/**
 *	Game/Test3D.h
 *
 *	３Ｄ立体の衝突判定を取る関数。
 *	ベクトルクラスとマトリクスクラスを使っているので
 *	プロジェクトに追加する事。
 *
 *	@author	Dila
 */

#pragma once

#include "Math/Vector.h"
#include "Math/Matrix.h"

////

// 球＊BOX
bool TestSphereBox(
		const Vector3& vSphere, float radius,
		const Vector3& vBox,
		float boxW, float boxH, float boxD);

// 動く球＊動くBOX
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
