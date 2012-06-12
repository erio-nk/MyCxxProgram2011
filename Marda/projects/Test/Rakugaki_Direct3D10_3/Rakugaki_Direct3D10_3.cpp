// Rakugaki_Direct3D10_3.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "Rakugaki_Direct3D10_3.h"

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
                     LPTSTR    lpCmdTriangle,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdTriangle);

 	// TODO: ここにコードを挿入してください。
	MSG msg;
	HACCEL hAccelTable;

	// グローバル文字列を初期化しています。
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_RAKUGAKI_DIRECT3D10_3, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// アプリケーションの初期化を実行します:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RAKUGAKI_DIRECT3D10_3));

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RAKUGAKI_DIRECT3D10_3));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_RAKUGAKI_DIRECT3D10_3);
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
	
	FrameRateController _frameRateController;

	enum FLAGS
	{
		SCREEN_OCCLUDED			= 0x01,
		FRAME_SKIP				= 0x02,
		INVALID_DEPTH_STENCIL	= 0x04,
	};
	Flags _flags;

	Core _core;

	ShaderHandle _shader;
	ID3D10Buffer* _pCube;
	VertexBufferHandle _triangleVertices;
	InputLayoutHandle _inputLayout;

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


		const TCHAR* SHADER_FILE_PATH = _T("..\\Rakugaki_Direct3D10_3\\simple.sh");
		_shader = _device.CreateShader(
			SHADER_FILE_PATH,
			"VS", "vs_4_0", 
			NULL, NULL, //"GS", "gs_4_0", 
			"PS", "ps_4_0"
			);
		if (_shader == ShaderHandle::INVALID)
		{
			return false;
		}
		
		if (failed(CreateGeometry()))
		{
			return false;
		}

		ResetProjectionTransform(sz);

		return true;
	}
	
	void ResetProjectionTransform(SIZE sz)
	{	
		D3DXMATRIX& mProj = *static_cast<D3DXMATRIX*>(_renderer.GetShaderGlobalVariable(_shader, "Proj"));
		D3DXMATRIX& mView = *static_cast<D3DXMATRIX*>(_renderer.GetShaderGlobalVariable(_shader, "View"));
		D3DXMATRIX mViewProj;

		D3DXMatrixPerspectiveFovLH(&mProj,
				(float)D3DXToRadian(30.0f),		// 視野角30°
			static_cast<float>(sz.cx) / static_cast<float>(sz.cy), // アスペクト比
				1.0f,							// 前方投影面までの距離
				100.0f);							// 後方投影面までの距離

		D3DXMatrixLookAtLH(&mView,
			&D3DXVECTOR3(0.0f, 5.0f, -5.0f),  // 視点(カメラの位置)
			&D3DXVECTOR3(0.0f, 0.0f,  0.0f),  // 注視点
			&D3DXVECTOR3(0.0f, 1.0f,  0.0f)); // カメラの上方向
			
		mViewProj = mView * mProj;

		D3DXMatrixTranspose(&mView, &mView);
		D3DXMatrixTranspose(&mProj, &mProj);
		D3DXMatrixTranspose(&mViewProj, &mViewProj);
	}
	
	bool CreateGeometry()
	{
		auto_interface<Device::NativeDevice> pNativeDevice(_device.GetNativeDevice());
		
		D3D10_INPUT_ELEMENT_DESC layout[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
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
		
		{
			const size_t NUM_VERTICES = 3;
			_triangleVertices = _device.CreateVertexBuffer(D3D10_USAGE_DYNAMIC, D3D10_CPU_ACCESS_WRITE, sizeof(D3DXVECTOR3), NUM_VERTICES);
			if (_triangleVertices == VertexBufferHandle::INVALID)
				return false;
		}
		
		return true;
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

			DrawTriangle(Color::RED);

			
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
	
	void DrawTriangle(const Color& color)
	{
		_renderer.SetShader(_shader);

		D3DXMATRIX& mWorld = *static_cast<D3DXMATRIX*>(_renderer.GetShaderGlobalVariable(_shader, "World"));;
//		D3DXMatrixIdentity(&mWorld);
		D3DXMatrixTranslation(&mWorld, 0.f, 0.f, 0.f);
		D3DXMatrixTranspose(&mWorld, &mWorld);

		float* pColor = static_cast<float*>(_renderer.GetShaderGlobalVariable(_shader, "color"));
		color.ToFloat4(pColor);

		_renderer.ApplyShader();

		{
			BufferPtr ptr;
			if (succeeded(_device.GetVertexBufferPtr(_triangleVertices, ptr, D3D10_MAP_WRITE_DISCARD)))
			{
				D3DXVECTOR3* p = static_cast<D3DXVECTOR3*>(ptr.Get());
				p[0] = D3DXVECTOR3(-1.0f,1.0f,-1.0f);
				p[1] = D3DXVECTOR3(1.0f,1.0f,-1.0f);
				p[2] = D3DXVECTOR3(1.0f,-1.0f,-1.0f);
			}
		}

		auto_interface<Device::NativeDevice> pNativeDevice(_device.GetNativeDevice());
		
		_renderer.SetVertexBuffer(_triangleVertices);
		_renderer.SetInputLayout(_inputLayout);
		pNativeDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		_renderer.SetDefaultRenderTargets();
		_renderer.SetDefaultRasterizerState();
		_renderer.SetDefaultBlendState();
		_renderer.SetDefaultDepthStencilState();
		
		pNativeDevice->Draw(3, 0);
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
