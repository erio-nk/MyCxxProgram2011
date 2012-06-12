/**
	@file MyApp.h

	Direct3D10アプリケーションのスケルトンプログラム

	@author Dila
 */

#pragma once

#include "Windows/Main.h"
#include "Windows/Window.h"
#include "Game/FpsTimer.h"

// Direct3D10
#include <d3d10.h>

#pragma comment(lib, "d3d10.lib")

// スクリーンサイズ
#define SCREEN_W 640
#define SCREEN_H 480

// 解放マクロ
#define RELEASE(x) if(x){x->Release(); x=NULL;}


/**
 	MyApp
 */
class MyApp
{
	static MyApp theInstance;

	Window _wnd;

	FpsTimer _fps;

	// Direct3D10 objects
	ID3D10Device* _pDevice;			// デバイス
	IDXGISwapChain* _pSwapChain;	// スワップチェイン（＝バックバッファとかの画面の事）
	ID3D10RenderTargetView* _pRTView;	// 描画対象

public:
	MyApp() :
		_pDevice(NULL),
		_pSwapChain(NULL),
		_pRTView(NULL)
	{}

	static MyApp& GetInstance() { return theInstance; }

	bool Initialize();
	bool Finalize();

	void Update();
};
