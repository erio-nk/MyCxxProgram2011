#include <windows.h>

#include "Windows/Main.h"
#include "Windows/Window.h"
#include "Windows/Bitmap.h"

#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "Math/Quaternion.h"

void Run();

//
Window wnd;		// メインウィンドウ
Bitmap backBuf;	// バックバッファ

//
Vector3 rect[8];

//
void Main()
{
	wnd.Create("てすと", 0, 0, 500, 500, 
		WindowOption::StandardStyle|
		WindowOption::ClientSize|
		WindowOption::BGWhite|
		WindowOption::Centering);
	wnd.Show();

	// Initialize
	backBuf.Create(500, 500);

	rect[0] = Vector3(-50.0, -50.0, -50.0);
	rect[1] = Vector3(-50.0, 50.0, -50.0);
	rect[2] = Vector3(50.0, 50.0, -50.0);
	rect[3] = Vector3(50.0, -50.0, -50.0);

	rect[4] = Vector3(-50.0, -50.0, 50.0);
	rect[5] = Vector3(-50.0, 50.0, 50.0);
	rect[6] = Vector3(50.0, 50.0, 50.0);
	rect[7] = Vector3(50.0, -50.0, 50.0);


	/*Matrix3 A;
	A(0,0) = 1;	A(0,1) = 3;	A(0,2) = 5;
	A(1,0) = 2;	A(1,1) = 3;	A(1,2) = 5;
	A(2,0) = 1;	A(2,1) = 2;	A(2,2) = 4;
	
	Matrix3 B;
	B(0,0) = 7;	B(0,1) = 0;	B(0,2) = 1;
	B(1,0) = 4;	B(1,1) = 5;	B(1,2) = 1;
	B(2,0) = 2;	B(2,1) = 5;	B(2,2) = 8;

	Matrix3 C = A * B;
	// |29, 40, 44|
	// |36, 40, 45|
	// |23, 30, 35|が正解
	*/

	/*Matrix2 M, Mi;
	M.a = 7;
	M.b = 3;
	M.c = 2;
	M.d = 1;

	Matrix2Inverse(Mi, M);
	// | 1, -3|
	// |-2,  7|が正解

	M *= Mi;
	*/

	/*Matrix3 A;
	A(0,0) = 1;	A(0,1) = 3;	A(0,2) = 5;
	A(1,0) = 2;	A(1,1) = 3;	A(1,2) = 5;
	A(2,0) = 1;	A(2,1) = 2;	A(2,2) = 4;

	Matrix2 T;
	Matrix3Tilde(T, A, 2, 2);*/

	{
		Matrix3 A;
		A(0,0) = 1;	A(0,1) = -2;	A(0,2) = 3;
		A(1,0) = 3;	A(1,1) = -1;	A(1,2) = 2;
		A(2,0) = -1;	A(2,1) = -2;	A(2,2) = 3;

		Matrix3 Ai;
		Matrix3Inverse(Ai, A);
		// |0.5,  0, -0.5|
		// |-5.5, 3,  3.5|
		// |-3.5, 2,  2.5|が正解

		A = A * Ai;
	}

	{
		Matrix4 A;
		A(0,0) = 15;	A(0,1) = 9;	A(0,2) = 3;	A(0,3) = 4;
		A(1,0) = 5;	A(1,1) = 1;	A(1,2) = 2;	A(1,3) = 10;
		A(2,0) = 8;	A(2,1) = 6;	A(2,2) = 11;	A(2,3) = 12;
		A(3,0) = 13;	A(3,1) = 14;	A(3,2) = 7;	A(3,3) = 16;

		Matrix4 Ai;
		Matrix4Inverse(Ai, A);

		A = A * Ai;
		A = A;
	}

	{
		Quaternion q(1, 2, 3, 4);
		Quaternion p;
		QuaternionInverse(p, q);

		q = q * p;
		q = q;
	}

	{
		Quaternion q, r;
		Quaternion p(0, 1, 0, 0), p2;

		QuaternionRotationAxis(q, 180.0f/180.0f*3.141592f, Vector3(0, 0, 1));
		QuaternionConjugate(r, q);

		p2 = r * p * q;
		p2 = p2;
	}

	{
		Quaternion q;
		QuaternionRotationAxis(q, 180.0f/180.0f*3.141592f, Vector3(0, 0, 1));
		Matrix4 m;
		Matrix4RotationQuaternion(m, q);

		Vector4 v(1, 0, 0, 1);
		v = v * m;
		v = v;
	}

	{
		Matrix4 m;
		Vector3 v(1, 1, 0);
		v.Normalize();
		Matrix4RotationAxis(m, 45.0f/180.0f*3.141592f, v);

		v = v;
	}

	// Application Loop
	MainLoop(Run);

	// Finalize
	backBuf.Release();
}



// Run()
void Run()
{
	HDC backDC = backBuf.GetDC();
	
	// 塗りつぶし
	RECT rc;
	rc.left = rc.top = 0;
	rc.right = backBuf.GetWidth();
	rc.bottom = backBuf.GetHeight();

	FillRect(backDC, &rc, (HBRUSH)GetStockObject(BLACK_BRUSH));

	// 図形演算
	Vector3 r[8];
	Matrix3 rot;
	static int t = 0;
	t += 1;

	Vector3 p(0, 0, 500.0f-25.0f);	// 物体位置

	float f = 500.0f;	// 焦点距離

	//Matrix3RotationY(rot, t/180.0f*3.14f);
	Vector3 axis(1.0f, 1.0f, 0.0f);
	axis.Normalize();
	Matrix3RotationAxis(rot, t/180.0f*3.14f, axis);

	for (int i = 0; i < 8; ++i) {
		r[i] = rect[i] * rot + p;

		// とりあえず射影
		r[i].x = (f / r[i].z ) * r[i].x;
		r[i].y = (f / r[i].z) * r[i].y;

		// とりあえずスクリーン座標に並行移動
		r[i].x += backBuf.GetWidth() >> 1;
		r[i].y += backBuf.GetHeight() >> 1;
	}

	// 図形描画
	HPEN hPenOld, hPen;

	hPen = CreatePen(PS_SOLID, 0, RGB(0xff, 0, 0));
	hPenOld = (HPEN)SelectObject(backDC, hPen);
	for (int i = 0; i < 4; ++i) {
		MoveToEx(backDC, r[i].x, r[i].y, NULL);
		LineTo(backDC, r[(i+1)&3].x, r[(i+1)&3].y);

		MoveToEx(backDC, r[i+4].x, r[i+4].y, NULL);
		LineTo(backDC, r[4+((i+1)&3)].x, r[4+((i+1)&3)].y);

		MoveToEx(backDC, r[i].x, r[i].y, NULL);
		LineTo(backDC, r[i+4].x, r[i+4].y);
	}
	SelectObject(backDC, hPenOld);
	DeleteObject(hPen);
	

	// フリップ
	HDC hDC = GetDC(wnd.GetHandle());
	BitBlt(hDC, 0, 0, rc.right, rc.bottom, backDC, 0, 0, SRCCOPY);
	ReleaseDC(wnd.GetHandle(), hDC);

	Sleep(100);
}
