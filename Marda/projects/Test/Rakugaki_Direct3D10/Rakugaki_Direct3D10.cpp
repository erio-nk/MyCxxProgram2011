// Rakugaki_Direct3D10.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "Rakugaki_Direct3D10.h"
	
using namespace Marda;

#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;								// 現在のインターフェイス
TCHAR szTitle[MAX_LOADSTRING];					// タイトル バーのテキスト
TCHAR szWindowClass[MAX_LOADSTRING];			// メイン ウィンドウ クラス名

ApplicationFrame g_app;

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: ここにコードを挿入してください。
	MSG msg;
	HACCEL hAccelTable;

	// グローバル文字列を初期化しています。
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_RAKUGAKI_DIRECT3D10, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// アプリケーションの初期化を実行します:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RAKUGAKI_DIRECT3D10));

	// メインループ。
	for (;;)
	{
		if (PeekMessage(&msg, (HWND)NULL, 0, 0, PM_REMOVE)) 
		{
			if (msg.message == WM_QUIT) 
			{
				break;
			}
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		} 
		else 
		{
			g_app.UpdateFrame();
		}
	}
	
	g_app.Release();

	return (int) msg.wParam;
}



//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
//  コメント:
//
//    この関数および使い方は、'RegisterClassEx' 関数が追加された
//    Windows 95 より前の Win32 システムと互換させる場合にのみ必要です。
//    アプリケーションが、関連付けられた
//    正しい形式の小さいアイコンを取得できるようにするには、
//    この関数を呼び出してください。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RAKUGAKI_DIRECT3D10));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_RAKUGAKI_DIRECT3D10);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します。
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // グローバル変数にインスタンス処理を格納します。

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   if (failed(g_app.Create(hWnd)))
   {
     return FALSE;
   }

   return TRUE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:  メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND	- アプリケーション メニューの処理
//  WM_PAINT	- メイン ウィンドウの描画
//  WM_DESTROY	- 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 選択されたメニューの解析:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 描画コードをここに追加してください...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

#define SAFE_RELEASE(iface) if (is_not_null(iface)) { iface->Release(); iface = NULL; }

#define FONT_HEIGHT	16

// アプリケーションフレームクラスの実装部。
struct ApplicationFrame::Impl
{
	Window _window;

	ID3D10Device* _pGraphicsDevice;
	IDXGISwapChain* _pSwapChain;
	ID3D10RenderTargetView* _pRenderTargetView;
	ID3D10Texture2D* _pDepthStencil;
	ID3D10DepthStencilView* _pDepthStencilView;
	ID3D10DepthStencilState* _pDepthStencilState;
	D3D10_VIEWPORT _viewport[1];

	ID3DX10Sprite* _pSprite;
	
	ID3D10ShaderResourceView* _pTextureSRV;
	D3D10_TEXTURE2D_DESC _textureDesc;

	ID3DX10Font* _pFont;
	
	FrameRateController _frameRateController;

	enum FLAGS
	{
		SCREEN_OCCLUDED			= 0x01,
		FRAME_SKIP				= 0x02,
		INVALID_DEPTH_STENCIL	= 0x04,
	};
	Flags _flags;

	Core _core;

	Impl()
	{
	}
	
	~Impl()
	{
	}

	void Exit(int exitCode)
	{
		PostQuitMessage(exitCode);
	}

	// 初期化。
	bool Create(HWND hWnd)
	{
		if (failed(_window.Create(hWnd)))
		{
			return false;
		}
		
		{
			using namespace std::placeholders;
			_window.AddMessageHandler(WM_SIZE, std::bind(&Impl::OnWindowSizeMessage, this, _1, _2, _3, _4));
			_window.AddMessageHandler(WM_KEYUP, std::bind(&Impl::OnWindowKeyUpMessage, this, _1, _2, _3, _4));
		}
		
		if (failed(CreateDirect3D()))
		{
			return false;
		}
		
		return true;
	}
	
	// Direct3D初期化。
	bool CreateDirect3D()
	{
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		
		SIZE sz;
		_window.GetClientSize(sz);

		sd.BufferCount = 1;
		sd.BufferDesc.Width = sz.cx;
		sd.BufferDesc.Height = sz.cy;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = _window.GetHandle();
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 1;
		sd.Windowed = TRUE;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		HRESULT hRet = D3D10CreateDeviceAndSwapChain(
			NULL,
			D3D10_DRIVER_TYPE_HARDWARE,
			NULL,
			0,
			D3D10_SDK_VERSION,
			&sd,
			&_pSwapChain,
			&_pGraphicsDevice
			);
		if (FAILED(hRet))
		{
			return false;
		}
		
		if (failed(CreateBackBuffer(sz)))
		{
			return false;
		}
		
		if (failed(CreateSprite(sz)))
		{
			return false;
		}
		
		if (failed(CreateTexture()))
		{
			return false;
		}
		
		if (failed(CreateFont()))
		{
			return false;
		}

		return true;
	}
	
	bool CreateBackBuffer(SIZE sz)
	{
		HRESULT hRet;

		// バックバッファのレンダーターゲットビュー。
		ID3D10Texture2D* pBackBuffer;
		hRet = _pSwapChain->GetBuffer(
			0,
			__uuidof(ID3D10Texture2D),
			(LPVOID*)&pBackBuffer
			);
		if (FAILED(hRet))
		{
			return false;
		}
		
		hRet = _pGraphicsDevice->CreateRenderTargetView(
			pBackBuffer,
			NULL,
			&_pRenderTargetView
			);
		SAFE_RELEASE(pBackBuffer);
		if (FAILED(hRet))
		{
			return false;
		}

		// 深度ステンシルバッファ。
		D3D10_TEXTURE2D_DESC descDepth;
		descDepth.Width = sz.cx;
		descDepth.Height = sz.cy;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D32_FLOAT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D10_USAGE_DEFAULT;
		descDepth.BindFlags = D3D10_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		hRet = _pGraphicsDevice->CreateTexture2D(
			&descDepth,
			NULL,
			&_pDepthStencil
			);
		if (FAILED(hRet))
		{
			return false;
		}

		// 深度ステンシルビュー。
		D3D10_DEPTH_STENCIL_VIEW_DESC descDSV;
		descDSV.Format = descDepth.Format;
		descDSV.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		hRet = _pGraphicsDevice->CreateDepthStencilView(
			_pDepthStencil,
			&descDSV,
			&_pDepthStencilView
			);
		if (FAILED(hRet))
		{
			return false;
		}
		
		// 深度ステンシルステート。
		// 全てデフォルト値。
		D3D10_DEPTH_STENCIL_DESC dsDesc;
		dsDesc.DepthEnable = TRUE;
		dsDesc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D10_COMPARISON_LESS;
		dsDesc.StencilEnable = FALSE;
		dsDesc.StencilReadMask = D3D10_DEFAULT_STENCIL_READ_MASK;
		dsDesc.StencilWriteMask = D3D10_DEFAULT_STENCIL_WRITE_MASK;
		dsDesc.FrontFace.StencilFailOp = D3D10_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilDepthFailOp = D3D10_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilPassOp = D3D10_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFunc = D3D10_COMPARISON_ALWAYS;
		dsDesc.BackFace.StencilFailOp = D3D10_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilDepthFailOp = D3D10_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilPassOp = D3D10_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilFunc = D3D10_COMPARISON_ALWAYS;
		hRet = _pGraphicsDevice->CreateDepthStencilState(
			&dsDesc,
			&_pDepthStencilState
			);
		if (FAILED(hRet))
		{
			return false;
		}
		
//		_pGraphicsDevice->OMSetRenderTargets(
//			1,
//			&_pRenderTargetView,
//			NULL
//			);

 
		_viewport[0].TopLeftX = 0;
		_viewport[0].TopLeftY = 0;
		_viewport[0].Width = sz.cx;
		_viewport[0].Height = sz.cy;
		_viewport[0].MinDepth = 0.f;
		_viewport[0].MaxDepth = 1.f;
		
//		_pGraphicsDevice->RSSetViewports(1, _viewport);
		
		return true;
	}
	
	bool CreateSprite(SIZE& sz)
	{
		HRESULT hRet = D3DX10CreateSprite(_pGraphicsDevice, 128, &_pSprite);
		if (FAILED(hRet))
		{
			return false;
		}

		D3DXMATRIX m;
		//D3DXVECTOR3 vEye(0.0f, 0.0f,  3.0f);	// 視点(カメラの位置)
		D3DXVECTOR3 vEye(0.0f, 0.0f,  -3.0f);	// 視点(カメラの位置)
		D3DXVECTOR3 vAt(0.0f, 0.0f,  0.0f);	// 注視点
		D3DXVECTOR3 vUp(0.0f, 1.0f,  0.0f);		// カメラの上方向

		D3DXMatrixLookAtLH(&m, &vEye, &vAt, &vUp);
		_pSprite->SetViewTransform(&m);

		if (failed(SetSpriteProjectionTransform(sz)))
		{
			return false;
		}

		return true;
	}
	
	bool SetSpriteProjectionTransform(SIZE& sz)
	{
		D3DXMATRIX m;
#if 0
		D3DXMatrixOrthoLH(
			&m,
			2.0f * static_cast<float>(sz.cx) / static_cast<float>(sz.cy), // 幅
			2.0f,	// 高さ
			0.0f,	// 前方投影面までの距離
			5.0f	// 後方投影面までの距離
			);
#else
		D3DXMatrixPerspectiveFovLH(
			&m,
			(float)D3DXToRadian(60.0f),		// 視野角60°
			static_cast<float>(sz.cx) / static_cast<float>(sz.cy), // アスペクト比
			1.0f,							// 前方投影面までの距離
			5.0f);							// 後方投影面までの距離
#endif
		_pSprite->SetProjectionTransform(&m);
		return true;
	}
	
	bool CreateTexture()
	{
		HRESULT hRet;
		D3DX10CreateShaderResourceViewFromFile(_pGraphicsDevice, _T("..\\Rakugaki_Direct3D10_dat\\img.jpg"), NULL, NULL, &_pTextureSRV, &hRet);
		if (FAILED(hRet))
		{
			return false;
		}

		ID3D10Resource* pResource;
		_pTextureSRV->GetResource(&pResource);
		
		ID3D10Texture2D* pTexture2D = static_cast<ID3D10Texture2D*>(pResource);
		
		pTexture2D->GetDesc(&_textureDesc);

		SAFE_RELEASE(pResource);
		
		return true;
	}
	
	bool CreateFont()
	{
		HRESULT hRet;
		
		hRet = D3DX10CreateFont(
			_pGraphicsDevice,
			FONT_HEIGHT,			// 高さ
			0,			// 平均文字幅
			FW_NORMAL,	// 太さ(標準)
			1,			// ミップマップレベル
			FALSE,		// 斜体(指定しない)
			DEFAULT_CHARSET,				// キャラクタ・セット(デフォルト)
			OUT_DEFAULT_PRECIS,				// 出力精度
			DEFAULT_QUALITY,				// 品質
			DEFAULT_PITCH || FF_DONTCARE,	// ピッチ
			_T("Arial"),					// フォント名
			&_pFont);
		if (FAILED(hRet))
		{
			return false;
		}
		
		return true;
	}
	
	// 終了処理。
	void Release()
	{
		ReleaseDirect3D();
	}
	
	// Direct3D終了処理。
	void ReleaseDirect3D()
	{
		if (is_not_null(_pGraphicsDevice))
		{
			_pGraphicsDevice->ClearState();
		}
		
		SAFE_RELEASE(_pFont);
		
		SAFE_RELEASE(_pTextureSRV);
		SAFE_RELEASE(_pSprite);
		
		SAFE_RELEASE(_pDepthStencilState);
		SAFE_RELEASE(_pDepthStencil);
		SAFE_RELEASE(_pDepthStencilView);
		
		SAFE_RELEASE(_pRenderTargetView);
		SAFE_RELEASE(_pSwapChain);
		SAFE_RELEASE(_pGraphicsDevice);
	}
	
	// フレーム更新。
	void UpdateFrame()
	{
		_flags.reset(FRAME_SKIP);
//		if (_frameRateController.GetElapsedTime() > _frameRateController.GetFrameTime() * 2)
//		{
//			// TODO: あまりにもお粗末な処理…ちゃんと考える。
//			_flags.set(FRAME_SKIP);
//		}

		Draw();
		
		_frameRateController.ChangeFrame();
	}
	
	// 描画。
	void Draw()
	{
		HRESULT hRet;
		
		if (_flags.is(SCREEN_OCCLUDED | FRAME_SKIP))
		{
			hRet = _pSwapChain->Present(0, DXGI_PRESENT_TEST);
		}
		else
		{
			ID3D10DepthStencilView* pDepthStencilView = _pDepthStencilView;
			if (_flags.is(INVALID_DEPTH_STENCIL))
				pDepthStencilView = NULL;
				
			float ClearColor[4] = { 0.f, 0.f, 1.f, 1.0f };
			_pGraphicsDevice->ClearRenderTargetView(
				_pRenderTargetView,
				ClearColor
				);

			_pGraphicsDevice->ClearDepthStencilView(
				_pDepthStencilView,
				D3D10_CLEAR_DEPTH /*| D3D10_CLEAR_STENCIL*/,
				1.f,	// 深度バッファのクリア値。1.f が一番遠い値。
				0		// ステンシルバッファのクリア値。
				);

			_pGraphicsDevice->RSSetViewports(1, _viewport);
	
			_pGraphicsDevice->OMSetRenderTargets(
				1,
				&_pRenderTargetView,
				pDepthStencilView
				);


			DrawSprite();
			
			int y = 0;

			DrawText(0, y, _T("Test"));
			y += FONT_HEIGHT;

			DrawText(0, y, _T("FPS=%.2f (%.2fms)"), 
				_frameRateController.GetCurrentFPS(),
				_frameRateController.GetElapsedTime() * 1000.f
				);
			y += FONT_HEIGHT;

			DrawText(0, y, _T("DepthStencil=") TFMT_BOOL _T(" [F2]"), format_bool(!_flags.is(INVALID_DEPTH_STENCIL)));
			y += FONT_HEIGHT;
		
			hRet = _pSwapChain->Present(0, 0);
		}

		// Present の結果判定。
		switch (hRet)
		{
			case S_OK:
				_flags.reset(SCREEN_OCCLUDED);
				break;
			case DXGI_STATUS_OCCLUDED:
				// 表示部分が存在しないので描画を無効にする。
				_flags.set(SCREEN_OCCLUDED);
				break;
			case DXGI_ERROR_DEVICE_HUNG:
			case DXGI_ERROR_DEVICE_RESET:
				// デバイスロスト。
				ReleaseDirect3D();
				if (failed(CreateDirect3D()))
				{
					Exit(0);
					return;
				}
				break;
			case DXGI_ERROR_DEVICE_REMOVED:
			case DXGI_ERROR_DRIVER_INTERNAL_ERROR:
			case E_OUTOFMEMORY:
			default:
				Exit(0);
				break;
		}
	}
	
	void DrawSprite()
	{
		D3DXMATRIX mRot;
		D3DXMATRIX mTrans;
		
		_pGraphicsDevice->OMSetDepthStencilState(_pDepthStencilState, 0);
	
		_pSprite->Begin(0);
		
		#define NUM_SPRITES	2
		D3DX10_SPRITE sprite[NUM_SPRITES];
		int i = 0;
	
		{
			D3DXMatrixTranslation(&mTrans, -0.5f, 0.f, -1.1f);

//			D3DXMATRIX mAdjustScale;
//			D3DXVECTOR3 vAdjustScale = GetSpriteAdjustScale(_textureDesc);
//			D3DXMatrixScaling(&mAdjustScale, vAdjustScale.x, vAdjustScale.y, vAdjustScale.z);

			static float angle = 0.f;
			//angle = 0.5f*D3DX_PI;
			angle = angle += _frameRateController.GetElapsedTime();
//			D3DXMatrixRotationY(&mRot, angle);	// スプライトも背面カリングされる。設定？
			D3DXMatrixRotationZ(&mRot, angle);

//			sprite[i].matWorld = mRot * mAdjustScale * mTrans;
			sprite[i].matWorld = mRot * mTrans;
			sprite[i].TexCoord.x = 0.0f;
			sprite[i].TexCoord.y = 0.0f;
			sprite[i].TexSize.x = 1.0f;
			sprite[i].TexSize.y = 1.0f;
			sprite[i].ColorModulate.r = 1.0f;
			sprite[i].ColorModulate.g = 1.0f;
			sprite[i].ColorModulate.b = 1.0f;
			sprite[i].ColorModulate.a = 1.0f;
			sprite[i].pTexture = _pTextureSRV;
			sprite[i].TextureIndex = 0;
			++i;
		}
		{
			D3DXMatrixTranslation(&mTrans, 0.0f, 0.f, 0.5f);

			sprite[i].matWorld = mTrans;
			sprite[i].TexCoord.x = 0.0f;
			sprite[i].TexCoord.y = 0.0f;
			sprite[i].TexSize.x = 1.0f;
			sprite[i].TexSize.y = 1.0f;
			sprite[i].ColorModulate.r = 1.0f;
			sprite[i].ColorModulate.g = 1.0f;
			sprite[i].ColorModulate.b = 1.0f;
			sprite[i].ColorModulate.a = 1.0f;
			sprite[i].pTexture = _pTextureSRV;
			sprite[i].TextureIndex = 0;
			++i;
		}
		

		_pSprite->DrawSpritesBuffered(sprite, NUM_SPRITES);

		_pSprite->Flush();
		_pSprite->End();

		//ID3D10DepthStencilState* pDepthStencilState;
		//UINT stencilRef;
		//_pGraphicsDevice->OMGetDepthStencilState(&pDepthStencilState, &stencilRef);
		//if (is_not_null(pDepthStencilState))
		//{
		//	D3D10_DEPTH_STENCIL_DESC dsDesc;
		//	pDepthStencilState->GetDesc(&dsDesc);
		//	SAFE_RELEASE(pDepthStencilState);
		//	DrawText(0, 100, _T("DepthEnable=") TFMT_BOOL, format_bool(dsDesc.DepthEnable));
		//}
	}
	
//	// テクスチャのオリジナルのサイズ比でスクリーンに表示される様にする。
//	// ↑オリジナルのサイズ比で表示されないのはスプライトにビュートランスフォームと射影トランスフォームを設定してないだけだったワロス。
//	// 　↑カメラ座標の設定があるので、カメラを調整しないとオリジナルサイズで表示されない。この調整はめんどいかも・・・視点０注視点０じゃダメだった。
//	// 　　↑でも設定してる射影トランスフォームは正射影なんだが…。
//	D3DXVECTOR3 GetSpriteAdjustScale(D3D10_TEXTURE2D_DESC& desc)
//	{
//		SIZE sz;
//		_window.GetClientSize(sz);
//
//		D3DXVECTOR3 ret;
//		ret.x = static_cast<float>(desc.Width) / sz.cx * 2.f;	// 2倍しているのは、スクリーン座標系の横幅は2(左端-1.0,右端1.0)となっている為である。
//		ret.y = static_cast<float>(desc.Height) / sz.cy * 2.f;
//		ret.z = 1.f;
//		return ret;
//	}

	void DrawText(int x, int y, const TCHAR* format, ...)
	{
		static TCHAR buf[256];
		
		va_list va;
		va_start(va, format);
		_vstprintf_s(buf, format, va);
		va_end(va);
		
		RECT rc;
		rc.left		= x;
		rc.top		= y;
		rc.right	= 1;
		rc.bottom	= 1;
		
		int result;
		
		// コレを呼び出すと、どうやら内部でフォント描画用の深度ステンシルステートが設定される様である。
		// 深度ステンシルバッファとビューの作成と設定をして、フォント描画をしなければ、OMGetDepthStencilState では
		// そもそも 深度ステンシルステートインターフェースが取得出来ず、デフォルト設定になっている（深度バッファ有効）。
		// フォントを描画した途端に深度ステンシルステートインターフェースが取得できるようになるが、
		// D3D10_DEPTH_STENCIL_DESC::DepthEnable は FALSE である。
		// つまり、DepthStencilState を自前で作り、フォント描画の後（≒モデルやスプライトの描画前）には
		// そのステートを設定してやらなければならない。
		result = _pFont->DrawText(
			NULL,
			buf,
			-1,
			&rc,
			DT_LEFT | DT_NOCLIP,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
			);
	}

	// ウィンドウリサイズ。
	LRESULT OnWindowSizeMessage(HWND /*hWnd*/, UINT /*msg*/, WPARAM wParam, LPARAM lParam)
	{
		WindowSizeMessageParameters params(wParam, lParam);
		SIZE sz;
		sz.cx = params.GetClientWidth();
		sz.cy = params.GetClientHeight();

		if (is_null(_pGraphicsDevice)
			|| params.GetType() == SIZE_MINIMIZED)
		{
			return 0;
		}
		
		{
			// なんか↓の処理しなくても上手くできてるように見えるんですけど…。
			// 描画ターゲットを解除する。
			_pGraphicsDevice->OMSetRenderTargets(0, NULL, NULL);	// 描画ターゲットの解除。
			SAFE_RELEASE(_pRenderTargetView);	// 描画ターゲット ビューの解放。
			SAFE_RELEASE(_pDepthStencilView);
			SAFE_RELEASE(_pDepthStencil);

			// バッファの変更。
			_pSwapChain->ResizeBuffers(1, 0, 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
			
			// バックバッファの再作成。
			if (failed(CreateBackBuffer(sz)))
			{
				Exit(0);
				return 0;
			}
		}
		
		// スプライトの射影トランスフォームをリセット。
		if (failed(SetSpriteProjectionTransform(sz)))
		{
			Exit(0);
			return 0;
		}
		
		return 0;
	}
	
	// キーが押されて離された。
	LRESULT OnWindowKeyUpMessage(HWND /*hWnd*/, UINT /*msg*/, WPARAM wParam, LPARAM lParam)
	{
		WindowKeyMessageParameters params(wParam, lParam);
		if (params.GetVirtKey() == VK_F2)
		{
			_flags = _flags.not(INVALID_DEPTH_STENCIL);
		}
		return 0;
	}

};	// end struct ApplicationFrame::Impl

// アプリケーションフレームクラスの転送部。
ApplicationFrame::ApplicationFrame()
	: _pimpl(new Impl())
{
}

ApplicationFrame::~ApplicationFrame()
{
}

bool
ApplicationFrame::Create(HWND hWnd)
{
	return _pimpl->Create(hWnd);
}

void
ApplicationFrame::Release()
{
	_pimpl->Release();
}

void
ApplicationFrame::UpdateFrame()
{
	_pimpl->UpdateFrame();
}
