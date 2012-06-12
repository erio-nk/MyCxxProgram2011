/**
	@file MyApp.cpp

	@author Dila
 */

#include "MyApp.h"

MyApp MyApp::theInstance;


/**
	Main()
 */
void Main()
{
	MyApp::GetInstance().Initialize();

	MainLoop(&MyApp::GetInstance(), &MyApp::Update);

	MyApp::GetInstance().Finalize();
}


/**
	MyApp::Initialize()
 */
bool MyApp::Initialize()
{
	// ウィンドウ初期化
	_wnd.Create(
		"Direct3D10 Application Skelton",
		0, 0, SCREEN_W, SCREEN_H);
	_wnd.Show();

	// フレームレート設定
	_fps.SetFrameRate(30);


	//// Direct3D10 初期化

	// スワップチェインの設定
	DXGI_SWAP_CHAIN_DESC sc;
	ZeroMemory(&sc, sizeof(sc));
	sc.BufferCount = 1;		// バックバッファの枚数
	sc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// バックバッファのフォーマット
	sc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// こりゃなんじゃ？
	sc.OutputWindow = _wnd.GetHandle();
	sc.SampleDesc.Count = 1;	// なんのこっちゃ
	sc.Windowed = TRUE;

	// デバイス作成（直でデバイス作成になったみたい）
	HRESULT hr;
	hr = D3D10CreateDeviceAndSwapChain(	// CreateDevice()の方はコンソールアプリでもおｋらしい
		NULL,
		D3D10_DRIVER_TYPE_HARDWARE,	// ハードウェアで処理
		NULL,
		NULL,
		D3D10_SDK_VERSION,
		&sc,
		&_pSwapChain,
		&_pDevice);
	if (FAILED(hr)) {
		return false;
	}

	// レンダーターゲットビュー作成（ビューってとこが重要らしいけどよくわｋｒｎ）
	ID3D10Texture2D* pBuffer;
	hr = _pSwapChain->GetBuffer(	// バックバッファを取得する
		0,
		__uuidof(ID3D10Texture2D),	// なにこれ！
		(LPVOID*)&pBuffer);
	if (FAILED(hr)) {
		return false;
	}

	hr = _pDevice->CreateRenderTargetView(
		pBuffer,
		NULL,
		&_pRTView);
	if (FAILED(hr)) {
		return false;
	}
	
    _pDevice->OMSetRenderTargets(1, &_pRTView, NULL);	// デバイスに設定しているのかな？

	RELEASE(pBuffer);	// COMのお決まり？

	// ビューポートの設定
	D3D10_VIEWPORT v;
	v.TopLeftX = 0;		// D3DVIEWPORT9::X
	v.TopLeftY = 0;		// D3DVIEWPORT9::Y
	v.Width  = SCREEN_W;
	v.Height = SCREEN_H;
	v.MinDepth = 0.0f;	// D3DVIEWPORT9::MinZ
	v.MaxDepth = 1.0f;	// D3DVIEWPORT9::MaxZ
	_pDevice->RSSetViewports(
		1,	// ビューポートの番号。複数設定出来る様になったんだろか？	
		&v);

	return true;
}


/**
 	MyApp::Finalize()
 */
bool MyApp::Finalize()
{
	if (_pDevice) _pDevice->ClearState();	// こんな後始末が増えたらしい
	RELEASE(_pRTView);
	RELEASE(_pSwapChain);
	RELEASE(_pDevice);
	return true;
}


/**
 	MyApp::Update()
 */
void MyApp::Update()
{
	// シーンクリア
	float color[] = { 0.0f, 0.0f, 0.5f, 1.0f };
	_pDevice->ClearRenderTargetView(_pRTView, color);

		// BeginSceneとEndSceneはないのかな？

	// シーンの表示（デバイスからスワップチェインのメソッドになったみたい）
	_pSwapChain->Present(0, 0);

	// FPS制御
	_fps.Wait();
}


