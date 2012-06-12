// Rakugaki_Direct3D10_2.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "Rakugaki_Direct3D10_2.h"

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
	LoadString(hInstance, IDC_RAKUGAKI_DIRECT3D10_2, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// アプリケーションの初期化を実行します:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RAKUGAKI_DIRECT3D10_2));

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RAKUGAKI_DIRECT3D10_2));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_RAKUGAKI_DIRECT3D10_2);
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

   if (Marda::failed(g_app.Create(hWnd)))
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


using namespace Marda;
using namespace Direct3D10;

// アプリケーションフレームクラスの実装部。
struct ApplicationFrame::Impl
{
	Window _window;
	Device _device;
	Renderer _renderer;
	
	ID3DX10Sprite* _pSprite;
	ID3D10ShaderResourceView* _pTextureSRV;

	InputLayoutHandle _inputLayout;
	VertexBufferHandle _vertices;
	IndexBufferHandle _indices;
	ShaderHandle _shader;

	struct Vertex
	{
		D3DXVECTOR3 Position;   // 座標値
		D3DXVECTOR3 Color;     // 色
	};

	// 定数バッファのデータ定義①　変更しないデータ
	struct cbNeverChanges {
		D3DXMATRIX Projection;   // 透視変換行列
	};

	// 定数バッファのデータ定義②　変更頻度の低いデータ
	struct cbChangesEveryFrame {
		D3DXMATRIX  View;  // ビュー変換行列
		D3DXVECTOR3 Light; // 光源座標(ワールド座標系)
		FLOAT       dummy; // ダミー
	};

	// 定数バッファのデータ定義③　変更頻度の高いデータ
	struct cbChangesEveryObject {
		D3DXMATRIX World;      // ワールド変換行列
	};

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

	// 終了処理。
	void Release()
	{
		safe_release(_pTextureSRV);
		safe_release(_pSprite);

		_renderer.Release();
		_device.Release();
	}
	
	// 初期化。
	bool Create(HWND hWnd)
	{
		if (failed(_window.Create(hWnd)))
		{
			return false;
		}
		SIZE sz;
		_window.GetClientSize(sz);

		{
			using namespace std::placeholders;
			_window.AddMessageHandler(WM_SIZE, std::bind(&Impl::OnWindowSizeMessage, this, _1, _2, _3, _4));
			_window.AddMessageHandler(WM_KEYUP, std::bind(&Impl::OnWindowKeyUpMessage, this, _1, _2, _3, _4));
		}
		
		if (failed(_device.Create(hWnd)))
		{
			return false;
		}
		if (failed(_renderer.Create(_device)))
		{
			return false;
		}

		if (failed(CreateSprite()))
		{
			return false;
		}
		
		if (failed(CreateTexture()))
		{
			return false;
		}
		
		if (failed(CreateShader()))
		{
			return false;
		}
		
		if (failed(CreateVertices()))
		{
			return false;
		}
		
		ResetProjectionTransform(sz);

		return true;
	}
	
	bool CreateSprite()
	{
		auto_interface<ID3D10Device> pNativeDevice(_device.GetNativeDevice());
		HRESULT hRet = D3DX10CreateSprite(pNativeDevice, 128, &_pSprite);
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

		return true;
	}
	
	bool CreateTexture()
	{
		auto_interface<ID3D10Device> pNativeDevice(_device.GetNativeDevice());

		HRESULT hRet;
		D3DX10CreateShaderResourceViewFromFile(pNativeDevice, _T("..\\Rakugaki_Direct3D10_dat\\img.jpg"), NULL, NULL, &_pTextureSRV, &hRet);
		if (FAILED(hRet))
		{
			return false;
		}

		return true;
	}
	
	bool CreateShader()
	{
		const TCHAR* SHADER_FILE_PATH = _T("..\\Rakugaki_Direct3D10_2\\D3D10Sample12.sh");

		_shader = _device.CreateShader(
			SHADER_FILE_PATH,
			"VS", "vs_4_0", "GS", "gs_4_0", "PS", "ps_4_0"
			);
		if (_shader == ShaderHandle::INVALID)
		{
			return false;
		}

		return true;
	}

	bool CreateVertices()
	{
		auto_interface<ID3D10Device> pNativeDevice(_device.GetNativeDevice());
		
		// 頂点バッファ。
		struct Vertex initialVertexDatas[] = {
			{ D3DXVECTOR3(-1.0f,1.0f,-1.0f),	D3DXVECTOR3(0.0f,0.0f,0.0f) },
			{ D3DXVECTOR3(1.0f,1.0f,-1.0f),		D3DXVECTOR3(0.0f,0.0f,1.0f) },
			{ D3DXVECTOR3(1.0f,-1.0f,-1.0f), 	D3DXVECTOR3(0.0f,1.0f,0.0f) },
			{ D3DXVECTOR3(-1.0f,-1.0f,-1.0f),	D3DXVECTOR3(0.0f,1.0f,1.0f) },
			{ D3DXVECTOR3(-1.0f,1.0f,1.0f), 	D3DXVECTOR3(1.0f,0.0f,0.0f) },
			{ D3DXVECTOR3(1.0f,1.0f,1.0f),		D3DXVECTOR3(1.0f,0.0f,1.0f) },
			{ D3DXVECTOR3(1.0f,-1.0f,1.0f), 	D3DXVECTOR3(1.0f,1.0f,0.0f) },
			{ D3DXVECTOR3(-1.0f,-1.0f,1.0f),		D3DXVECTOR3(1.0f,1.0f,1.0f) },
		};
		
		_vertices = _device.CreateVertexBuffer(D3D10_USAGE_DEFAULT, 0, sizeof(Vertex), _countof(initialVertexDatas), initialVertexDatas);
		if (_vertices == VertexBufferHandle::INVALID)
			return false;
			
		// インデックスバッファ。
		UINT initialIndexDatas[] = {
			0,1,3,  1,2,3,  1,5,2,  5,6,2,  5,4,6,  4,7,6,
			4,5,0,  5,1,0,  4,0,7,  0,3,7,  3,2,7,  2,6,7
		};
			
		_indices = _device.CreateIndexBuffer(D3D10_USAGE_IMMUTABLE, 0, sizeof(UINT), _countof(initialIndexDatas), initialIndexDatas);
		if (_indices == IndexBufferHandle::INVALID)
			return false;

		// 入力レイアウト・オブジェクトの作成
		D3D10_INPUT_ELEMENT_DESC layout[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, Position), D3D10_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, Color), D3D10_INPUT_PER_VERTEX_DATA, 0}
		};
		
		_inputLayout = _device.CreateInputLayout(
			layout,
			_countof(layout),
			_shader
			);
		if (_inputLayout == InputLayoutHandle::INVALID)
		{
			return false;
		}
		
		return true;
	}

	void ResetProjectionTransform(SIZE sz)
	{	
		// 定数バッファ①を更新
		cbNeverChanges* pCBuf = static_cast<cbNeverChanges*>(_renderer.GetShaderConstantBuffer(_shader, (unsigned int)0));
		D3DXMatrixPerspectiveFovLH(&pCBuf->Projection,
				(float)D3DXToRadian(30.0f),		// 視野角30°
			static_cast<float>(sz.cx) / static_cast<float>(sz.cy), // アスペクト比
				1.0f,							// 前方投影面までの距離
				20.0f);							// 後方投影面までの距離
		D3DXMatrixTranspose(&pCBuf->Projection, &pCBuf->Projection);

		// スプライトのプロジェクショントランスフォームも更新。
		D3DXMATRIX m;
		D3DXMatrixPerspectiveFovLH(
			&m,
			(float)D3DXToRadian(60.0f),		// 視野角60°
			static_cast<float>(sz.cx) / static_cast<float>(sz.cy), // アスペクト比
			1.0f,							// 前方投影面までの距離
			5.0f);							// 後方投影面までの距離
		_pSprite->SetProjectionTransform(&m);
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
		if (_flags.is(FRAME_SKIP) || _device.IsScreenOccluded())
		{
			// noop
		}
		else
		{
			_renderer.Clear(true, true, false, Color::BLUE);

			DrawSprite();
			
			DrawModel();
			
			int y = 0, h = _device.GetFontHeight(_renderer.GetDefaultFont());

			_renderer.DrawText(0, y, Color::WHITE, _T("Test"));
			y += h;

			_renderer.DrawText(0, y, Color::GREEN, _T("FPS=%.2f (%.2fms)"), 
				_frameRateController.GetCurrentFPS(),
				_frameRateController.GetElapsedTime() * 1000.f
				);
			y += h;
		}

		_renderer.Present();
	}
	
	void DrawSprite()
	{
		D3DXMATRIX mRot;
		D3DXMATRIX mTrans;
		
		_renderer.SetDefaultDepthStencilState();
	
		_pSprite->Begin(0);
		
		#define NUM_SPRITES	2
		D3DX10_SPRITE sprite[NUM_SPRITES];
		int i = 0;
	
		{
			D3DXMatrixTranslation(&mTrans, -1.0f, 0.f, -1.1f);

			static float angle = 0.f;
			angle += _frameRateController.GetElapsedTime();
			D3DXMatrixRotationZ(&mRot, angle);

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
			D3DXMatrixTranslation(&mTrans, -1.0f, 0.f, 0.5f);

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
	}
	
	void DrawModel()
	{
		auto_interface<ID3D10Device> pNativeDevice(_device.GetNativeDevice());

		// 立方体の描画

		// 定数バッファ②を更新
		cbChangesEveryFrame* pCBufEveryFrame = static_cast<cbChangesEveryFrame*>(_renderer.GetShaderConstantBuffer(_shader, 1));
		// ビュー変換行列
		D3DXVECTOR3 eye = D3DXVECTOR3(0.0f, 5.0f, -5.0f);  // 視点(カメラの位置)
		D3DXVECTOR3 at = D3DXVECTOR3(0.0f, 0.0f,  0.0f);  // 注視点
		D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f,  0.0f); // カメラの上方向

		D3DXMatrixLookAtLH(&pCBufEveryFrame->View, &eye, &at, &up);
		D3DXMatrixTranspose(&pCBufEveryFrame->View, &pCBufEveryFrame->View);
		// 点光源座標
		D3DXVECTOR3   vLightPos(3.0f, 3.0f, -3.0f);   // 光源座標(ワールド座標系)
		D3DXVec3TransformCoord(&pCBufEveryFrame->Light, &vLightPos, &pCBufEveryFrame->View);

		// 定数バッファ③を更新
		cbChangesEveryObject* pCBufEveryObject = static_cast<cbChangesEveryObject*>(_renderer.GetShaderConstantBuffer(_shader, 2));
		// ワールド変換行列
		static float angle = 0.f;
		angle += _frameRateController.GetElapsedTime();
		D3DXMatrixRotationY(&pCBufEveryObject->World, angle);
		D3DXMatrixTranspose(&pCBufEveryObject->World, &pCBufEveryObject->World);

		float* pGlobalColor = reinterpret_cast<float*>(_renderer.GetShaderGlobalVariable(_shader, "color"));
		pGlobalColor[0] = 1.0f;
		pGlobalColor[1] = 1.0f;
		pGlobalColor[2] = 1.0f;
		pGlobalColor[3] = 1.0f;

		// ***************************************
		// IAに頂点バッファを設定
		_renderer.SetVertexBuffer(_vertices);
		// IAにインデックス・バッファを設定
		_renderer.SetIndexBuffer(_indices);
		// IAに入力レイアウト・オブジェクトを設定
		_renderer.SetInputLayout(_inputLayout);
		// IAにプリミティブの種類を設定
		pNativeDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// シェーダをデバイスに適用。
		_renderer.SetShader(_shader);
		_renderer.ApplyShader();

		_renderer.SetDefaultRenderTargets();

		_renderer.SetDefaultRasterizerState();
		_renderer.SetDefaultBlendState();
		_renderer.SetDefaultDepthStencilState();

		// ***************************************
		// 頂点バッファのデータをインデックス・バッファを使って描画する
		pNativeDevice->DrawIndexed(
				36, // 描画するインデックス数(頂点数)
				0,  // インデックス・バッファの最初のインデックスから描画開始
				0); // 頂点バッファ内の最初の頂点データから使用開始
	}

	// ウィンドウリサイズ。
	LRESULT OnWindowSizeMessage(HWND /*hWnd*/, UINT /*msg*/, WPARAM wParam, LPARAM lParam)
	{
		WindowSizeMessageParameters params(wParam, lParam);
		SIZE sz;
		sz.cx = params.GetClientWidth();
		sz.cy = params.GetClientHeight();

		if (params.GetType() == SIZE_MINIMIZED)
		{
			return 0;
		}
		
		// なんか↓の処理しなくても上手くできてるように見えるんですけど…。
		if (failed(_device.ResizeBackBuffer(sz)))
		{
			Exit(0);
			return 0;
		}
		
		// 射影トランスフォームをリセット。
		ResetProjectionTransform(sz);
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
