#include <windows.h>

#include "Windows/Main.h"
#include "Windows/Window.h"
#include "Windows/Bitmap.h"

#include "Math/Vector.h"
#include "Math/Matrix.h"

void Run();

//
Window wnd;		// メインウィンドウ
Bitmap backBuf;	// バックバッファ

//
Vector2 rect[4];

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

	rect[0] = Vector2(-50.0, -50.0);
	rect[1] = Vector2(-50.0, 50.0);
	rect[2] = Vector2(50.0, 50.0);
	rect[3] = Vector2(50.0, -50.0);


	//Vector3 v(20, 30, 1);
	//Matrix3 m;
	//Matrix3Translation4V2(m, 100, 200);
	//v = v * m;

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

	// 図形描画
	HPEN hPenOld, hPen;

	hPen = CreatePen(PS_SOLID, 0, RGB(0xff, 0, 0));
	hPenOld = (HPEN)SelectObject(backDC, hPen);
	
	Vector2 r[4];
	Vector2 trans(backBuf.GetWidth()>>1, backBuf.GetHeight()>>1);
	Matrix2 rot, sc;
	static int t = 0;
	t += 5;
	Matrix2Rotation(rot, t/180.0f*3.14);
	Matrix2Scaling(sc, 2, 2);
	for (int i = 0; i < 4; ++i) {
		r[i] = rect[i] * sc * rot;
		r[i] = r[i] + trans;
	}
		/*Vector3 r[4];
		Vector3 trans(backBuf.GetWidth()>>1, backBuf.GetHeight()>>1, 0);
		Matrix3 rot, sc;
		static int t = 0;
		t += 5;
		Matrix3RotationHV(rot, t/180.0f*3.14);
		for (int i = 0; i < 4; ++i) {
			r[i].x = rect[i].x;
			r[i].y = rect[i].y;
			r[i] = r[i] * rot;
			r[i] = r[i] + trans;
		}*/
	for (int i = 0; i < 4; ++i) {
		MoveToEx(backDC, r[i].x, r[i].y, NULL);
		LineTo(backDC, r[(i+1)&3].x, r[(i+1)&3].y);
	}
	SelectObject(backDC, hPenOld);
	DeleteObject(hPen);
	

	// フリップ
	HDC hDC = GetDC(wnd.GetHandle());
	BitBlt(hDC, 0, 0, rc.right, rc.bottom, backDC, 0, 0, SRCCOPY);
	ReleaseDC(wnd.GetHandle(), hDC);

	Sleep(100);
}
