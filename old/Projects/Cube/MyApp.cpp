/**
 *	MyApp.cpp
 *
 *	@author	Dila
 */

#include <stdio.h>

#include "MyApp.h"

#include "Windows/Main.h"

#ifdef _DEBUG
	#define PRINTF printf
#else
	#define PRINTF __noop
#endif


// MyApp::theInstance
MyApp MyApp::theInstance;


/**
 *	Run()
 *	メインループ
 */
void Run() {
	MyApp::GetInstance().Update();
}


/**
 *	Main()
 *	メイン関数
 */
void Main() {
	// デバッグコンソール
#ifdef _DEBUG
	if (AllocConsole()) {
		freopen("CONOUT$", "w", stdout);
		freopen("CONIN$", "r", stdin);
	} else {
		// !とりあえず失敗は無視
	}
#endif

	// 初期化
	if (!MyApp::GetInstance().Initialize()) {
		MessageBox(NULL, "初期化失敗", "Error", MB_OK);
		if (!MyApp::GetInstance().Finalize()) {
			MessageBox(NULL, "終了処理失敗", "Error", MB_OK);
		}
		return;
	}

	// アプリケーションループ
	MainLoop(Run);

	// 終了処理
	if (!MyApp::GetInstance().Finalize()) {
		MessageBox(NULL, "終了処理失敗", "Error", MB_OK);
	}
}



/**
 *	MyApp::Initialize()
 *	初期化
 */
bool MyApp::Initialize()
{
	// ウィンドウ初期化
	bool b;
	b = wnd.Create(
		"Cube",
		0, 0, SCREEN_W, SCREEN_H,
		WindowOption::StandardStyle |
		WindowOption::BGNull |
		WindowOption::Centering |
		WindowOption::ClientSize,
		NULL,
		NULL);
	if (!b) return false;

	// バックバッファ初期化
	backBuf.Create(SCREEN_W, SCREEN_H);
	backDC = backBuf.GetDC();

	// ウィンドウ可視化
	wnd.Show();

	// FPS
	fps.SetFrameRate(30);

	//// 立体の初期化	
	cube[0].x = -2.0f;	cube[0].y = -2.0f;	cube[0].z = -2.0f;	cube[0].w = 1.0f;
	cube[1].x = 2.0f;	cube[1].y = -2.0f;	cube[1].z = -2.0f;	cube[1].w = 1.0f;
	cube[2].x = 2.0f;	cube[2].y = 2.0f;	cube[2].z = -2.0f;	cube[2].w = 1.0f;
	cube[3].x = -2.0f;	cube[3].y = 2.0f;	cube[3].z = -2.0f;	cube[3].w = 1.0f;

	cube[4].x = -2.0f;	cube[4].y = -2.0f;	cube[4].z = 2.0f;	cube[4].w = 1.0f;
	cube[5].x = 2.0f;	cube[5].y = -2.0f;	cube[5].z = 2.0f;	cube[5].w = 1.0f;
	cube[6].x = 2.0f;	cube[6].y = 2.0f;	cube[6].z = 2.0f;	cube[6].w = 1.0f;
	cube[7].x = -2.0f;	cube[7].y = 2.0f;	cube[7].z = 2.0f;	cube[7].w = 1.0f;

	QuaternionIdentity(qRot);

	angle = 0.0f;
	scale = 1.0f;

	PRINTF("Initialized.\n");

	return true;
}


/**
 *	MyApp::Finalize()
 *	終了処理
 */
bool MyApp::Finalize()
{
	backBuf.Release();
	return true;
}



///**
// *	MyWindow::OnLButtonDown()
// *	マウスの左を押した
// */
//bool MyWindow::OnLButtonDown(MouseMessage* pMsg)
//{
//	pos.x = (float)pMsg->GetX();
//	pos.y = (float)pMsg->GetY();
//	PRINTF("LDOWN: x=%f, y=%f\n", pos.x, pos.y);
//	return true;
//}
bool MyWindow::OnLButtonDown::Handle()
{
	MyWindow* This = static_cast<MyWindow*>(pWnd);
	This->pos.x = (float)GetX();
	This->pos.y = (float)GetY();
	PRINTF("LDOWN: x=%f, y=%f\n", This->pos.x, This->pos.y);
	return true;
}

///**
// *	MyWindow::OnMouseMove()
// *	マウスカーソルを移動した
// */
//bool MyWindow::OnMouseMove(MouseMessage* pMsg)
//{
//	if (!pMsg->IsLButton()) return true;
//
//	Vector2 v(pMsg->GetX(), pMsg->GetY()), dir;
//	dir = v - pos;
//
//	dir.y = -dir.y;
//	MyApp::GetInstance().Rotate(dir);
//
//	//PRINTF("MMOVE: x=%f, y=%f (%f,%f)\n", dir.x, dir.y, pos.x, pos.y);
//
//	pos = v;
//	return true;
//}
bool MyWindow::OnMouseMove::Handle()
{
	if (!IsLButton()) return true;
	
	MyWindow* This = static_cast<MyWindow*>(pWnd);

	Vector2 v(GetX(), GetY()), dir;
	dir = v - This->pos;

	dir.y = -dir.y;
	MyApp::GetInstance().Rotate(dir);

	PRINTF("MMOVE: x=%f, y=%f (%f,%f)\n", dir.x, dir.y, This->pos.x, This->pos.y);
	PRINTF("LPARAM:%d\n", lParam);

	This->pos = v;
	return true;
}


///**
// *	MyWindow::OnMouseWheel()
// *	マウスホイールを動かした
// */
//bool MyWindow::OnMouseWheel(MouseMessage* pMsg)
//{
//	float z = 0.0f;
//	if (pMsg->GetZ() > 0) z = 0.1f;
//	if (pMsg->GetZ() < 0) z = -0.1f;
//
//	MyApp::GetInstance().Scale(z);
//
//	PRINTF("WHEEL: z=%d\n", pMsg->GetZ());
//	return true;
//}
bool MyWindow::OnMouseWheel::Handle()
{
	float z = 0.0f;
	if (GetZ() > 0) z = 0.1f;
	if (GetZ() < 0) z = -0.1f;

	MyApp::GetInstance().Scale(z);

	PRINTF("WHEEL: z=%d\n", GetZ());
	return true;
}


/**
 *	MyApp::Update()
 *	毎フレームの処理
 */
void MyApp::Update()
{
	int i;

	bool _b = false;
	float _x = 0, _y = 0;
	if (HIWORD(GetAsyncKeyState(VK_UP))) {
		_y = 5.0f;	_b = true;
	}
	if (HIWORD(GetAsyncKeyState(VK_DOWN))) {
		_y = -5.0f;	_b = true;
	}
	if (HIWORD(GetAsyncKeyState(VK_LEFT))) {
		_x = -5.0f;	_b = true;
	}
	if (HIWORD(GetAsyncKeyState(VK_RIGHT))) {
		_x = 5.0f;	_b = true;
	}
	if (_b) Rotate(Vector2(_x, _y));



	// 頂点計算
	Vector4 pos(0.0f, 0.0f, 7.0f, 0.0f);	// 立体位置
	float n = 1;	// 焦点距離


	Matrix4 M, m;
	Matrix4Identity(M);

	// 回転
	Quaternion q;
	QuaternionRotationAxis(q, -angle/180.0f*3.141592f, Vector3(rot, 0));
	qRot *= q;
	Matrix4RotationQuaternion(m, qRot);
	M *= m;
	
	angle -= 0.1f;	// ちょっとずつ回転を弱くする
	if (angle < 0.0f) angle = 0.0f;

	// 拡縮
	Matrix4Scaling(m, scale, scale, scale);
	M *= m;

	// 平行移動
	Matrix4Translation(m, pos.x, pos.y, pos.z);
	M *= m;
	
	// ビュー変換
	Matrix4View(m, Vector3(0, 0, 0), Vector3(0, 0, n), Vector3(0, 1, 0));
	M *= m;

	// 射影行列
	Matrix4 Mproj;
	Matrix4Projection(Mproj, 120.0f/180.0f*3.141592f, (float)SCREEN_W/(float)SCREEN_H, 1, 500);

	// スクリーン座標変換行列
	Matrix4 Mscreen;
	Matrix4Screen(Mscreen, SCREEN_W, SCREEN_H);

	for (i = 0; i < CUBE_SIZE; ++i) {
		// 各種変換
		buf[i] = cube[i] * M;

		// 射影変換
		buf[i] = Mproj * buf[i];
		buf[i] /= buf[i].w;

		// スクリーン座標に変換
		buf[i] = buf[i] * Mscreen;
	}


	// 背景描画
	RECT rcBack = {0, 0, SCREEN_W, SCREEN_H};
	FillRect(backDC, &rcBack, (HBRUSH)GetStockObject(BLACK_BRUSH));

	// 愚直な色変化
	static int r = 0xff, g = 0x00, b = 0x00;
	static int a = 1;

	switch (a) {
		case 0: b-=4; if (b <= 0x00) { ++a; b=0x00; }	break;
		case 1: g+=4; if (g >= 0xff) { ++a; g=0xff; }	break;
		case 2: r-=4; if (r <= 0x00) { ++a; r=0x00; }	break;
		case 3: b+=4; if (b >= 0xff) { ++a; b=0xff; }	break;
		case 4: g-=4; if (g <= 0x00) { ++a; g=0x00; }	break;
		case 5: r+=4; if (r >= 0xff) { a=0; r=0xff; }	break;
	}

	// 図形描画
	HPEN hPenOld, hPen;
	hPen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
	hPenOld = (HPEN)SelectObject(backDC, hPen);

	for (i = 0; i < CUBE_SIZE>>1; ++i) {
		MoveToEx(backDC, buf[i].x, buf[i].y, NULL);
		LineTo(backDC, buf[(i+1)&3].x, buf[(i+1)&3].y);

		MoveToEx(backDC, buf[i+4].x, buf[i+4].y, NULL);
		LineTo(backDC, buf[4+((i+1)&3)].x, buf[4+((i+1)&3)].y);

		MoveToEx(backDC, buf[i].x, buf[i].y, NULL);
		LineTo(backDC, buf[i+4].x, buf[i+4].y);
	}

	MoveToEx(backDC, buf[0].x-5, buf[0].y-5, NULL);
	LineTo(backDC, buf[0].x+5, buf[0].y+5);
	MoveToEx(backDC, buf[0].x+5, buf[0].y-5, NULL);
	LineTo(backDC, buf[0].x-5, buf[0].y+5);

	SelectObject(backDC, hPenOld);
	DeleteObject(hPen);

	// 軸描画
	Vector2 axis = rot * 50.0f;
	if (!axis.IsZero()) {
		axis.y = -axis.y;
		Vector2 allow1, allow2, u(SCREEN_W>>1, SCREEN_H>>1);
		
		Vector2Rotate(allow1, axis, (90.0f+45.0f)/180.0f*3.141592f);
		allow1.Normalize();
		allow1 *= 10;
		Vector2Rotate(allow2, axis,-(90.0f+45.0f)/180.0f*3.141592f);
		allow2.Normalize();
		allow2 *= 10;

		axis += u;
		allow1 += axis;
		allow2 += axis;
		
		hPen = CreatePen(PS_SOLID, 0, RGB(0xff, 0, 0));
		hPenOld = (HPEN)SelectObject(backDC, hPen);	

		MoveToEx(backDC, u.x, u.y, NULL);
		LineTo(backDC, axis.x, axis.y);
		
		MoveToEx(backDC, axis.x, axis.y, NULL);
		LineTo(backDC, allow1.x, allow1.y);
		
		MoveToEx(backDC, axis.x, axis.y, NULL);
		LineTo(backDC, allow2.x, allow2.y);

		SelectObject(backDC, hPenOld);
		DeleteObject(hPen);
	}

	// 情報表示
	char str[64];

	SetTextColor(backDC, RGB(0xff, 0xff, 0xff));
	SetBkMode(backDC, TRANSPARENT);
	
	sprintf(str, "回転軸=(%.2f, %.2f)", rot.x, rot.y);
	TextOut(backDC, 0, 0, str, lstrlen(str));

	sprintf(str, "回転角=%.2fﾟ", angle);
	TextOut(backDC, 0, 16, str, lstrlen(str));

	sprintf(str, "回転四元数=(%.2f;%.2f,%.2f,%.2f)", qRot.w, qRot.x, qRot.y, qRot.z);
	TextOut(backDC, 0, 32, str, lstrlen(str));

	sprintf(str, "拡縮係数=%.2f", scale);
	TextOut(backDC, 0, 48, str, lstrlen(str));

	// フリップ
	HDC hDC = GetDC(wnd.GetHandle());
	BitBlt(hDC, 0, 0, SCREEN_W, SCREEN_H, backDC, 0, 0, SRCCOPY);
	ReleaseDC(wnd.GetHandle(), hDC);

	// FPS制御
	fps.Wait();
}


/**
 *	MyApp::Rotate()
 *	立体を回す
 *	@param	v	回す方向のベクトル
 */
void MyApp::Rotate(Vector2& v)
{
	if (v.IsZero()) return;
	Vector2Rotate(rot, v, (90.0f)/180.0f*3.141592f);
	rot.Normalize();
	angle = v.Length();
	if (angle > 20.0f) angle = 20.0f;
	PRINTF("rot: (%f, %f)\n", rot.x, rot.y);
}

/**
 *	MyApp::Scale()
 *	立体を拡縮させる
 *	@param	x	拡縮係数の加算値
 */
void MyApp::Scale(float x)
{
	scale += x;
	if (scale > 2.5f) scale = 2.5f;
	if (scale < 0.0f) scale = 0.0f;
}
