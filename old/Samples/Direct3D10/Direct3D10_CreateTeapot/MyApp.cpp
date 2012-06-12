/**
	@file MyApp.cpp

	@author Dila
 */

#include "MyApp.h"

MyApp MyApp::theInstance;

namespace
{
	// エフェクト（シェーダー）ファイル名
	const TCHAR	EFFECT_FILE_NAME[] = _T("MyShader.fx");

	// シェーダーモデル
	const char SHADER_MODEL[] = "fx_4_0";

	// エフェクトのテクニック名
	const char TECHNIQUE_NAME[] = "Render";
}


/**
	Main()
 */
void Main()
{
	if (MyApp::GetInstance().Initialize()) {
		MainLoop(&MyApp::GetInstance(), &MyApp::Update);
	}
	MyApp::GetInstance().Finalize();
}


/**
	MyApp::Initialize()
 */
bool MyApp::Initialize()
{
	// ウィンドウ初期化
	_wnd.Create(
		_T("Direct3D10 Render the teapot"),
		0, 0, SCREEN_W, SCREEN_H);
	_wnd.Show();

	// フレームレート設定
	_fps.SetFrameRate(30);

	// タイマー開始
	_timer.Start();


	//// Direct3D10 初期化

	// スワップチェインの設定
	DXGI_SWAP_CHAIN_DESC sc;
	ZeroMemory(&sc, sizeof(sc));
	sc.BufferCount = 1;
	sc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sc.OutputWindow = _wnd.GetHandle();
	sc.SampleDesc.Count = 1;
	sc.Windowed = TRUE;

	// デバイスとスワップチェインの作成（直でデバイス作成になったみたい）
	HRESULT hr;
	hr = D3D10CreateDeviceAndSwapChain(	
		NULL,
		D3D10_DRIVER_TYPE_HARDWARE,	// ハードウェアで処理
		NULL,
		NULL,
		D3D10_SDK_VERSION,
		&sc,
		&_pSwapChain,
		&_pDevice);
	if (FAILED(hr)) {
		assert(0);
		return false;
	}

	// バックバッファ(描画ターゲット)のビューを作成
	ID3D10Texture2D* pBuffer;
	hr = _pSwapChain->GetBuffer(	// バックバッファを取得する
		0,
		__uuidof(ID3D10Texture2D),
		(LPVOID*)&pBuffer);
	if (FAILED(hr)) {
		assert(0);
		return false;
	}

	hr = _pDevice->CreateRenderTargetView(
		pBuffer,
		NULL,
		&_pBackBuf);
	if (FAILED(hr)) {
		assert(0);
		return false;
	}

	RELEASE(pBuffer);	// Getしたインタフェースは参照カウントを減らしておく

	// 深度/ステンシルバッファを作成
	D3D10_TEXTURE2D_DESC td;
	td.Width = SCREEN_W;
	td.Height = SCREEN_H;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_D32_FLOAT;	// 32bit float
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.Usage = D3D10_USAGE_DEFAULT;
	td.BindFlags = D3D10_BIND_DEPTH_STENCIL;	// 深度/ステンシルとして使用
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;

	hr = _pDevice->CreateTexture2D(&td, NULL, &pBuffer);
	if (FAILED(hr)) {
		assert(0);
		return false;
	}

	// 深度/ステンシルバッファのビューを作成
	D3D10_DEPTH_STENCIL_VIEW_DESC dsd;
	dsd.Format = td.Format;	// バッファの方と同じフォーマット
	dsd.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
	dsd.Texture2D.MipSlice = 0;

	hr = _pDevice->CreateDepthStencilView(
		pBuffer,
		&dsd,
		&_pZBuf);
	if (FAILED(hr)) {
		assert(0);
		return false;
	}

	RELEASE(pBuffer);	// Getしたｒｙ

	// バックバッファのビューと深度/ステンシルバッファのビューをデバイスに設定する
    _pDevice->OMSetRenderTargets(1, &_pBackBuf, _pZBuf);


	// ビューポートの設定
	D3D10_VIEWPORT v;
	v.TopLeftX = 0;		// D3DVIEWPORT9::X
	v.TopLeftY = 0;		// D3DVIEWPORT9::Y
	v.Width  = SCREEN_W;
	v.Height = SCREEN_H;
	v.MinDepth = 0.0f;	// D3DVIEWPORT9::MinZ
	v.MaxDepth = 1.0f;	// D3DVIEWPORT9::MaxZ
	_pDevice->RSSetViewports(
		1,
		&v);

	// メッシュ作成
	hr = DXUTCreateTeapot(_pDevice, &_pMesh);
	if (FAILED(hr)) {
		assert(0);
		return false;
	}

	// エフェクトの読み込み
	hr = D3DX10CreateEffectFromFile(EFFECT_FILE_NAME,
		NULL, NULL,
		SHADER_MODEL,
		D3D10_SHADER_ENABLE_STRICTNESS,
		0,
		_pDevice,
		NULL, NULL,
		&_pEffect,
		NULL, &hr);
	if (FAILED(hr)) {
		assert(0);
		return false;
	}
	
	// 頂点データのレイアウト定義（FVFに代わるものみたい？）
	// 　頂点バッファ等で使われ、シェーダではセマンティック(POSITION,COLOR)に使われるそうな。
	D3D10_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR"   , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
	};

	UINT numElements = sizeof(layout)/sizeof(layout[0]);

	ID3D10EffectTechnique* pTechnique;
	pTechnique = _pEffect->GetTechniqueByName(TECHNIQUE_NAME);

	D3D10_PASS_DESC PassDesc;
	pTechnique->GetPassByIndex(0)->GetDesc(&PassDesc);
	
	hr = _pDevice->CreateInputLayout(
		layout,
		numElements,
		PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize,
		&_pLayout);

	if (FAILED(hr)) {
		assert(0);
		return false;
	}

    _pDevice->IASetInputLayout(_pLayout);	// デバイスに設定する

	//RELEASE(pTechnique);	// テクニックのReleaseは要らないらしい

	return true;
}


/**
 	MyApp::Finalize()
 */
bool MyApp::Finalize()
{
	// 順番どうがいいのかな
	RELEASE(_pLayout);
	RELEASE(_pEffect);
	RELEASE(_pMesh);

	if (_pDevice) _pDevice->ClearState();
	RELEASE(_pZBuf);
	RELEASE(_pBackBuf);
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
	_pDevice->ClearRenderTargetView(_pBackBuf, color);	// バックバッファのクリア
	_pDevice->ClearDepthStencilView(_pZBuf, D3D10_CLEAR_DEPTH, 1.0f, 0);
													// 深度/ステンシルバッファのクリア

	// シーン描画
	{
		// シェーダにトランスフォームのパラメータを設定する
		D3DXMATRIX mWorld;	// ワールド変換行列
		//D3DXMatrixIdentity(&mWorld);
		D3DXMatrixRotationY(&mWorld, D3DXToRadian(_timer.GetTime()*20));
		_pEffect->GetVariableByName("mWorld")->AsMatrix()->SetMatrix((float*)&mWorld);

		D3DXMATRIX mView;	// ビュー変換行列
		D3DXVECTOR3 Eye(0.0f, 2.0f, -4.0f);	// カメラ座標
		D3DXVECTOR3 At(0.0f, 0.0f, 0.0f);	// 注視点
		D3DXVECTOR3 Up(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtLH(&mView, &Eye, &At, &Up);
		_pEffect->GetVariableByName("mView")->AsMatrix()->SetMatrix((float*)&mView);

		D3DXMATRIX mProj;	// 射影変換行列
		D3DXMatrixPerspectiveFovLH(
			&mProj,
			D3DXToRadian(60.0f),	// 視野角
			(float)SCREEN_W/(float)SCREEN_H,	// アスペクト比
			0.1f,	// ニアクリップ
			100.0f);// ファークリップ
		_pEffect->GetVariableByName("mProj")->AsMatrix()->SetMatrix((float*)&mProj);

		// テクニックを取得する
		ID3D10EffectTechnique* pTechnique;
		pTechnique = _pEffect->GetTechniqueByName(TECHNIQUE_NAME);

		D3D10_TECHNIQUE_DESC desc;
		pTechnique->GetDesc(&desc);
		for (UINT i = 0; i < desc.Passes; ++i) {
			// テクニックに書いたパスの事と思われ
			pTechnique->GetPassByIndex(i)->Apply(0);

			// メッシュ描画
			_pMesh->DrawSubset(0);
		}
	}

	// シーンの表示
	_pSwapChain->Present(0, 0);

	// FPS制御
	_fps.Wait();
}


