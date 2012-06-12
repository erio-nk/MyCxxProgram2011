/**
 *	MyApp.cpp
 */

#include "MyApp.h"


//
MyApp MyApp::theInstance;

//
void Run();



/**
 *	Main()
 */
void Main()
{
	MyApp::GetInstance().Initialize();

	MainLoop(Run);

	MyApp::GetInstance().Finalize();
}


/**
 *	Run()
 */
void Run()
{
	MyApp::GetInstance().Update();
}



/**
 *	MyApp::Initialize()
 */
bool MyApp::Initialize()
{
	// ウィンドウ初期化
	wnd.Create(
		"DirectGraphicsSample",
		0, 0, SCREEN_W, SCREEN_H,
		WindowOption::Centering |
		WindowOption::ClientSize |
		WindowOption::StandardStyle,
		NULL,
		NULL);
	wnd.Show();

	// フレームレート設定
	fps.SetFrameRate(30);

	//
	pD3D = NULL;
	pD3DDev = NULL;

	pTeapot = NULL;

	// DirectGraphics初期化
	pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (pD3D == NULL) {
		return false;
	}

	ZeroMemory(&D3DPP, sizeof(D3DPP));

	D3DPP.Windowed = TRUE;	// ウィンドウモード
	D3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;	// スワップエフェクト
	D3DPP.BackBufferFormat = D3DFMT_UNKNOWN;	// バックバッファのフォーマット

	HRESULT hr = pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL, wnd.GetHandle(),
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&D3DPP, &pD3DDev);
	if (FAILED(hr)) {
		return false;
	}


	// ビューポートの設定
	D3DVIEWPORT9 vp;
	vp.X = 0;
	vp.Y = 0;
	vp.Width = D3DPP.BackBufferWidth;
	vp.Height = D3DPP.BackBufferHeight;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;
	hr = pD3DDev->SetViewport(&vp);
	if (FAILED(hr)) {
		return false;
	}

	// ティーポットの作成
	hr = D3DXCreateTeapot(pD3DDev, &pTeapot, NULL);
	if (FAILED(hr)) {
		return false;
	}

	// マテリアル
	ZeroMemory(&mat, sizeof(D3DMATERIAL9));
	mat.Diffuse.r = mat.Ambient.r = 1.0f;
	mat.Diffuse.g = mat.Ambient.g = 1.0f;
	mat.Diffuse.b = mat.Ambient.b = 1.0f;
	mat.Diffuse.a = mat.Ambient.a = 1.0f;

	// ライト
	D3DXVECTOR3 vLightDir(-1.0f, -1.0f, 2.0f);
	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vLightDir);
	light.Position.x = -1.0f;
	light.Position.y = -1.0f;
	light.Position.z = 2.0f;
	light.Range = 1000.0f;

	pD3DDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	return true;
}


/**
 *	MyApp::Finalize()
 */
bool MyApp::Finalize()
{
	RELEASE(pTeapot);

	RELEASE(pD3DDev);
	RELEASE(pD3D);
	return true;
}


/**
 *	MyApp::Update()
 */
void MyApp::Update()
{
	// シーンクリア
	pD3DDev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,80), 1.0f, 0);

	// シーン描画開始
	HRESULT hr = pD3DDev->BeginScene();
	if (SUCCEEDED(hr)) {
		//// シーン描画

		// ワールド変換行列
		D3DXMATRIX mWorld;
		D3DXMatrixIdentity(&mWorld);
		pD3DDev->SetTransform(D3DTS_WORLD, &mWorld);

		// ビュー変換行列
		D3DXMATRIX mView;
		D3DXVECTOR3 vEye(0.0f, 0.0f, -5.0f);
		D3DXVECTOR3 vAt(0.0f, 0.0f ,0.0f);
		D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtLH(&mView, &vEye, &vAt, &vUp);
		pD3DDev->SetTransform(D3DTS_VIEW, &mView);

		// 透視変換行列
		D3DXMATRIX mProj;
		D3DXMatrixPerspectiveFovLH(&mProj,
			D3DXToRadian(60),	// 視野角60度
			(float)SCREEN_W/(float)SCREEN_H,
			3.0f,
			15.0f);
		#if 0
			// Fovじゃない方で実験してみる
			float nearH = tan(D3DXToRadian(30)) * 3.0f * 2.0f;		// 近平面の縦幅 = tan(Yの視野角/2) * nearZ * 2
			float nearW = nearH * (float)SCREEN_W/(float)SCREEN_H;	// 近平面の横幅 = 近平面の縦幅 * アスペクト比
			D3DXMatrixPerspectiveLH(&mProj,	nearW, nearH, 3.0f, 15.0f);
		#endif
		#if 0
			// 正射影やってみる
			float nearH = tan(D3DXToRadian(30)) * 3.0f * 2.0f;		// 近平面の縦幅 = tan(Yの視野角/2) * nearZ * 2
			float nearW = nearH * (float)SCREEN_W/(float)SCREEN_H;	// 近平面の横幅 = 近平面の縦幅 * アスペクト比
			D3DXMatrixOrthoLH(&mProj, nearW, nearH, 3.0f, 15.0f);
		#endif
		pD3DDev->SetTransform(D3DTS_PROJECTION, &mProj);

		// マテリアル
		pD3DDev->SetMaterial(&mat);

		// ライト
		pD3DDev->SetLight(0, &light);
		pD3DDev->LightEnable(0, TRUE);

		// ティーポット描画
		pTeapot->DrawSubset(0);

		// シーン描画終了
		pD3DDev->EndScene();
	}

	// シーンの表示
	hr = pD3DDev->Present(NULL, NULL, NULL, NULL);
	if (hr == D3DERR_DEVICELOST) ;

	//
	fps.Wait();
}


