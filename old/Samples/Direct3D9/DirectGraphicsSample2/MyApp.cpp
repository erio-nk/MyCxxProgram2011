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
		"DirectGraphicsSample2",
		0, 0, SCREEN_W, SCREEN_H,
		WindowOption::Centering |
		WindowOption::ClientSize |
		WindowOption::StandardStyle,
		NULL,
		NULL);
	wnd.Show();

	// フレームレート設定
	fps.SetFrameRate(30);

	// メンバ変数の初期化
	pD3D = NULL;
	pD3DDev = NULL;

	pVtxBuf = NULL;

	// DirectGraphics初期化
	pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (pD3D == NULL) {
		return false;
	}

	ZeroMemory(&D3DPP, sizeof(D3DPP));

	D3DPP.Windowed = TRUE;	// ウィンドウモード
	D3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;	// スワップエフェクト
	D3DPP.BackBufferFormat = D3DFMT_UNKNOWN;	// バックバッファのフォーマット
	
    D3DPP.EnableAutoDepthStencil = 1;			// Zバッファを作成する
    D3DPP.AutoDepthStencilFormat = D3DFMT_D16;

	HRESULT hr = pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL, wnd.GetHandle(),
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&D3DPP, &pD3DDev);
	if (FAILED(hr)) {
		return false;
	}
	
	// カリング
	pD3DDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	// なし 

	// ライティング
	pD3DDev->SetRenderState(D3DRS_LIGHTING, FALSE);	// なし

	// Zバッファ
	pD3DDev->SetRenderState(D3DRS_ZENABLE, TRUE);	// 有効


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

	// 立方体（頂点バッファ）の作成
	MYVERTEX v[] = {
		{  1, -1,  1, 0xFFFFFFFF},
		{  1, -1, -1, 0xFFFFFFFF},
		{ -1, -1, -1, 0xFFFFFFFF},
		{ -1, -1,  1, 0xFFFFFFFF},	// 下の面

		{ -1, -1,  1, 0xFF00FF00},
		{  1, -1,  1, 0xFF00FF00},
		{  1,  1,  1, 0xFF00FF00},
		{ -1,  1,  1, 0xFF00FF00},	// 奥の面

		{ -1,  1,  1, 0xFFFFFF00},
		{ -1, -1,  1, 0xFFFFFF00},
		{ -1, -1, -1, 0xFFFFFF00},
		{ -1,  1, -1, 0xFFFFFF00},	// 左の面

		{ -1,  1, -1, 0xFF00FFFF},
		{ -1,  1,  1, 0xFF00FFFF},
		{  1,  1,  1, 0xFF00FFFF},
		{  1,  1, -1, 0xFF00FFFF},	// 上の面

		{  1,  1, -1, 0xFF0000FF},
		{  1,  1,  1, 0xFF0000FF},
		{  1, -1,  1, 0xFF0000FF},
		{  1, -1, -1, 0xFF0000FF},	// 右の面

		{  1, -1, -1, 0xFFFF0000},
		{  1,  1, -1, 0xFFFF0000},
		{ -1,  1, -1, 0xFFFF0000},
		{ -1, -1, -1, 0xFFFF0000}	// 手前の面
	};

	hr = pD3DDev->CreateVertexBuffer(
		4*6*sizeof(MYVERTEX),
		0,
		D3DFVF_MYVERTEX,
		D3DPOOL_MANAGED,
		&pVtxBuf,
		NULL);
	if (FAILED(hr)) {
		return false;
	}

	MYVERTEX* _v;
	hr = pVtxBuf->Lock(0, 0, (LPVOID*)&_v, 0);
	if (FAILED(hr)) {
		return false;
	}

	memcpy(_v, v, sizeof(v));

	hr = pVtxBuf->Unlock();
	if (FAILED(hr)) {
		return false;
	}

	return true;
}


/**
 *	MyApp::Finalize()
 */
bool MyApp::Finalize()
{
	RELEASE(pVtxBuf);

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
	pD3DDev->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,	// Zバッファもクリアする
		D3DCOLOR_XRGB(0,0,80), 1.0f, 0);

	// シーン描画開始
	HRESULT hr = pD3DDev->BeginScene();
	if (SUCCEEDED(hr)) {
		//// シーン描画

		// ワールド変換行列
		D3DXMATRIX mWorld;

		static float theta = 0.0f;
		theta += 4.0f;	// Y軸周りにぐるぐる
		D3DXMatrixRotationY(&mWorld, D3DXToRadian(theta));

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
			60.0f/180.0f*3.141592f,	// 視野角60度
			(float)SCREEN_W/(float)SCREEN_H,
			3.0f,
			15.0f);
		pD3DDev->SetTransform(D3DTS_PROJECTION, &mProj);

		// 立方体描画
		pD3DDev->SetStreamSource(0, pVtxBuf, 0, sizeof(MYVERTEX));
		pD3DDev->SetFVF(D3DFVF_MYVERTEX);

		for (int i = 0; i < 6; ++i) {
			pD3DDev->DrawPrimitive(D3DPT_TRIANGLEFAN, 4*i,  2);
		}

		// シーン描画終了
		pD3DDev->EndScene();
	}

	// シーンの表示
	hr = pD3DDev->Present(NULL, NULL, NULL, NULL);
	if (hr == D3DERR_DEVICELOST) ;

	//
	fps.Wait();
}


