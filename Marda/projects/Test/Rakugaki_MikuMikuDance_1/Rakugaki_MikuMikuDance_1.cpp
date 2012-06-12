// Rakugaki_MikuMikuDance_1.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "Rakugaki_MikuMikuDance_1.h"

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
	LoadString(hInstance, IDC_RAKUGAKI_MIKUMIKUDANCE_1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// アプリケーションの初期化を実行します:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RAKUGAKI_MIKUMIKUDANCE_1));

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RAKUGAKI_MIKUMIKUDANCE_1));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_RAKUGAKI_MIKUMIKUDANCE_1);
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


// ----------------------------------------------------------------------------
using namespace Marda;
using namespace Direct3D10;
using namespace D3DXHelpers;

// ----------------------------------------------------------------------------
class MyRenderer : public Renderer
{
public:
	enum SHADER_KIND
	{
		PMD_DEFAULT,	//!< 頂点カラーのみ。
		PMD_TEXTURE,	//!< テクスチャマッピング有り。

		SHADER_KIND_MAX
	};

	struct ShaderInput
	{
		D3DXVECTOR3 position;
		float color[3];
		D3DXVECTOR2 uv;
	};
		
	bool Create(Device& device)
	{
		if (failed(Renderer::Create(device)))
		{
			return false;
		}
		
		// シェーダの作成。
		const TCHAR* SHADER_FILE_PATH = _T("..\\Rakugaki_MikuMikuDance_1\\pmd.sh");
		_shaderDependencyObjects.resize(SHADER_KIND_MAX);

		ShaderDependencyObjects& shaderDefault = _shaderDependencyObjects[PMD_DEFAULT];
		shaderDefault._shader = device.CreateShader(
			SHADER_FILE_PATH,
			"VS", "vs_4_0", 
			NULL, NULL,
			"PS_default", "ps_4_0"
			);
		if (shaderDefault._shader == ShaderHandle::INVALID)
		{
			return false;
		}
		
		ShaderDependencyObjects& shaderTexture = _shaderDependencyObjects[PMD_TEXTURE];
		shaderTexture._shader = device.CreateShader(
			SHADER_FILE_PATH,
			"VS", "vs_4_0", 
			NULL, NULL,
			"PS_texture", "ps_4_0"
			);
		if (shaderDefault._shader == ShaderHandle::INVALID)
		{
			return false;
		}
		
		// 入力レイアウトの作成。
		D3D10_INPUT_ELEMENT_DESC layout[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(ShaderInput, position), D3D10_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(ShaderInput, color), D3D10_INPUT_PER_VERTEX_DATA, 0},
			{"TEXTURE",  0, DXGI_FORMAT_R32G32_FLOAT,    0, offsetof(ShaderInput, uv), D3D10_INPUT_PER_VERTEX_DATA, 0},
		};
		
		InputLayoutHandle inputLayout = 
			device.CreateInputLayout(
				layout,
				_countof(layout),
				shaderDefault._shader
				);
		if (inputLayout == InputLayoutHandle::INVALID)
			return false;

		shaderDefault._inputLayout = inputLayout;
		shaderTexture._inputLayout = inputLayout;

		return true;
	}
	
	ShaderHandle GetShader(int shaderKind) const
	{
		return _shaderDependencyObjects[shaderKind]._shader;
	}
	
	InputLayoutHandle GetInputLayout(int shaderKind) const
	{
		return _shaderDependencyObjects[shaderKind]._inputLayout;
	}
	
	void SetShaderViewProjection(D3DXMATRIX mViewSrc, D3DXMATRIX mProjSrc)
	{
		D3DXMatrixTranspose(&mViewSrc, &mViewSrc);
		D3DXMatrixTranspose(&mProjSrc, &mProjSrc);
		
		*static_cast<D3DXMATRIX*>(GetShaderGlobalVariable(GetShader(PMD_DEFAULT), "proj")) = mViewSrc;
		*static_cast<D3DXMATRIX*>(GetShaderGlobalVariable(GetShader(PMD_DEFAULT), "view")) = mProjSrc;

		*static_cast<D3DXMATRIX*>(GetShaderGlobalVariable(GetShader(PMD_TEXTURE), "proj")) = mViewSrc;
		*static_cast<D3DXMATRIX*>(GetShaderGlobalVariable(GetShader(PMD_TEXTURE), "view")) = mProjSrc;
	}

private:
	struct ShaderDependencyObjects
	{
		ShaderHandle _shader;
		InputLayoutHandle _inputLayout;
	};
	
	std::vector<ShaderDependencyObjects> _shaderDependencyObjects;
	
};	// end struct MyRenderer

// ----------------------------------------------------------------------------
// PMD モデル描画用。
class DrawablePMD
{
public:
	
	bool Create(Device& device, const TCHAR* filePath)
	{
		MMD::PMD pmd;

		if (failed(LoadPMD(pmd, filePath)))
			return false;
		
		static TCHAR dirPath[256];
		GetDirectoryPath(filePath, dirPath, _countof(dirPath));
		
		// メッシュの頂点情報のコピー。
		std::vector<MyRenderer::ShaderInput> vertices;
		vertices.resize(pmd.vert_count);
		for (unsigned int i = 0; i < pmd.vert_count; ++i)
		{
			vertices[i].position = ToD3DXVECTOR3(pmd.vertex[i].pos);
			vertices[i].uv = ToD3DXVECTOR2(pmd.vertex[i].uv);
		}
		
		// インデックスバッファを作成する。
		_indices = device.CreateIndexBuffer(D3D10_USAGE_IMMUTABLE, 0, sizeof(unsigned short), pmd.face_vert_count, &pmd.face_vert_index[0]);
		if (_indices == IndexBufferHandle::INVALID)
			return false;
		_numIndices = pmd.face_vert_count;
		
		// マテリアル情報の解析。
		_drawPlans.push_back(DrawPlan());
		DrawPlan* pPlan = &_drawPlans.back();
		pPlan->_shader = -1;
		pPlan->_startIndex = 0;
		pPlan->_texture = -1;
		unsigned int total = 0;
		for (unsigned int i = 0, k = 0; i < pmd.material_count; ++i)
		{
			MMD::PMD::Material& material = pmd.material[i];

			// マテリアルの対応する頂点にカラーを設定する。
			total += material.face_vert_count;
			for (; k < total; ++k)
			{
				MyRenderer::ShaderInput& vertex = vertices[pmd.face_vert_index[k]];
				vertex.color[0] = material.diffuse_color[0];
				vertex.color[1] = material.diffuse_color[1];
				vertex.color[2] = material.diffuse_color[2];
			}
			
			// テクスチャありなしで描画プランを分ける。
			int shader = MyRenderer::PMD_DEFAULT;
			int textureIndex = -1;
			if (strlen(material.texture_file_name) > 0)
			{
				shader = MyRenderer::PMD_TEXTURE;
				
				// テクスチャを読み込む。
				static TCHAR fileName[256];
				fileName[0] = 0;
				MultiByteToWideChar(CP_ACP, 0, material.texture_file_name, -1, fileName, _countof(fileName)); 
				static TCHAR texPath[256];
				_tcscpy_s(texPath, dirPath);
				_tcscat_s(texPath, fileName);
				DEBUG_TRACE(texPath);
				TextureHandle tex = device.CreateTextureFromFile(texPath);
				if (tex == TextureHandle::INVALID)
					return false;
				_textures.push_back(tex);

				textureIndex = static_cast<int>(_textures.size() - 1);
			}
			
			if (pPlan->_shader < 0)
			{
				pPlan->_shader = shader;
			}
			else
			if (pPlan->_shader != shader || pPlan->_texture != textureIndex)
			{
				_drawPlans.push_back(*pPlan);
				pPlan = &_drawPlans.back();
				pPlan->_shader = shader;
				pPlan->_startIndex = pPlan->_startIndex + pPlan->_numIndices;
				pPlan->_numIndices = 0;
			}
			pPlan->_numIndices += material.face_vert_count;
			pPlan->_texture = textureIndex;
		}
		
		// メッシュの頂点バッファ作成。
		_vertices = device.CreateVertexBuffer(D3D10_USAGE_IMMUTABLE, 0, sizeof(MyRenderer::ShaderInput), vertices.size(), &vertices[0]);
		if (_vertices == VertexBufferHandle::INVALID)
			return false;
		
		// ボーンを解析して描画用の頂点列を作る。
		vertices.resize(0);
		for (unsigned i = 0; i < pmd.bone_count; ++i)
		{
			//if (i != 50) continue;
			MMD::PMD::Bone& bone = pmd.bone[i];
//			if (bone.parent_bone_index == MMD::PMD::Bone::INVALID_PARENT)
//				continue;
			if (bone.tail_pos_bone_index == MMD::PMD::Bone::INVALID_TAIL)
				continue;
			MMD::PMD::Bone& boneTail = pmd.bone[bone.tail_pos_bone_index];
			
			MyRenderer::ShaderInput boneFrom;
			boneFrom.position = ToD3DXVECTOR3(bone.bone_head_pos);
			Color::WHITE.ToFloat3(boneFrom.color);
			boneFrom.uv = D3DXVECTOR2(0.f, 0.f);
			
			MyRenderer::ShaderInput boneTo;
			boneTo.position = ToD3DXVECTOR3(boneTail.bone_head_pos);
			Color::GRAY.ToFloat3(boneTo.color);
			boneTo.uv = D3DXVECTOR2(0.f, 0.f);
			
			vertices.push_back(boneFrom);
			vertices.push_back(boneTo);
		}
		_numBones = pmd.bone_count;
		
		_bones = device.CreateVertexBuffer(D3D10_USAGE_IMMUTABLE, 0, sizeof(MyRenderer::ShaderInput), vertices.size(), &vertices[0]);
		if (_bones == VertexBufferHandle::INVALID)
			return false;
		
		// サンプラ。
		D3D10_SAMPLER_DESC sampDesc;
		sampDesc.Filter = D3D10_FILTER_MIN_MAG_MIP_POINT;
		sampDesc.AddressU = D3D10_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D10_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D10_TEXTURE_ADDRESS_WRAP;
		sampDesc.MipLODBias = 0.0f;
		sampDesc.MaxAnisotropy = 16;
		sampDesc.ComparisonFunc = D3D10_COMPARISON_NEVER;
		sampDesc.BorderColor[0] = 0.f;
		sampDesc.BorderColor[1] = 0.f;
		sampDesc.BorderColor[2] = 0.f;
		sampDesc.BorderColor[3] = 0.f;
		sampDesc.MinLOD = 0.f;
		sampDesc.MaxLOD = FLT_MAX;
		_sampler = device.CreateSamplerState(sampDesc);
		if (_sampler == SamplerStateHandle::INVALID)
			return false;
		
		return true;
	}
	
	void Draw(MyRenderer& renderer)
	{
		for (size_t i = 0; i < _drawPlans.size(); ++i)
		{
			DrawPlan& plan = _drawPlans[i];
			
			ShaderHandle shader = renderer.GetShader(plan._shader);
			renderer.SetShader(shader);
			renderer.SetInputLayout(renderer.GetInputLayout(plan._shader));

			D3DXMATRIX& mWorld = *static_cast<D3DXMATRIX*>(renderer.GetShaderGlobalVariable(shader, "world"));
			D3DXMatrixTranspose(&mWorld, &_transformToWorld);

			if (plan._texture >= 0)
			{
				// TODO: 出来ればこうやって分けたくは無いが…。NullHandle を作ろうかな？？
				renderer.SetShaderTexture(shader, "tex", _textures[plan._texture]);
				renderer.SetShaderSamplerState(shader, "samp", _sampler);
			}

			renderer.ApplyShader();

			auto_interface<Device::NativeDevice> pNativeDevice(renderer.GetNativeDevice());
			
			renderer.SetVertexBuffer(_vertices);
			renderer.SetIndexBuffer(_indices);
			pNativeDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			renderer.SetDefaultRenderTargets();

	//		renderer.SetRasterizerState(_rasterizerState);
			renderer.SetDefaultRasterizerState();
			renderer.SetDefaultBlendState();
			renderer.SetDefaultDepthStencilState();
			
			pNativeDevice->DrawIndexed(plan._numIndices, plan._startIndex, 0);
		}
	}
	
	// ボーン描画。
	void DrawBone(MyRenderer& renderer)
	{
		ShaderHandle shader = renderer.GetShader(MyRenderer::PMD_DEFAULT);
		renderer.SetShader(shader);
		renderer.SetInputLayout(renderer.GetInputLayout(MyRenderer::PMD_DEFAULT));

		D3DXMATRIX& mWorld = *static_cast<D3DXMATRIX*>(renderer.GetShaderGlobalVariable(shader, "world"));
		D3DXMatrixTranspose(&mWorld, &_transformToWorld);

		renderer.ApplyShader();

		auto_interface<Device::NativeDevice> pNativeDevice(renderer.GetNativeDevice());
		
		renderer.SetVertexBuffer(_bones);
		pNativeDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);

		renderer.SetDefaultRenderTargets();

		renderer.SetDefaultRasterizerState();
		renderer.SetDefaultBlendState();
		renderer.SetDefaultDepthStencilState();
		
		pNativeDevice->Draw(_numBones * 2, 0);
	}
	
	void SetWorldMatrix(const D3DXMATRIX& mWorld)
	{
		_transformToWorld = mWorld;
	}
	
private:
	VertexBufferHandle _vertices;
	IndexBufferHandle _indices;
	unsigned int _numIndices;
	std::vector<TextureHandle> _textures;
	
	SamplerStateHandle _sampler;

	VertexBufferHandle _bones;
	unsigned int _numBones;

	D3DXMATRIX _transformToWorld;
	
	struct DrawPlan
	{
		int _shader;
		unsigned int _startIndex;
		unsigned int _numIndices;
		int _texture;
	};
	std::vector<DrawPlan> _drawPlans;

};	// end class PMDDrawer

// ----------------------------------------------------------------------------
// アプリケーションフレームクラスの実装部。
struct ApplicationFrame::Impl
{
	Window _window;
	Device _device;
	MyRenderer _renderer;
	
	FrameRateController _frameRateController;

	enum FLAGS
	{
		SCREEN_OCCLUDED			= 0x01,
		FRAME_SKIP				= 0x02,
		INVALID_MESH_DRAW		= 0x04,
		INVALID_BONE_DRAW		= 0x08,
	};
	Flags _flags;
	
	RasterizerStateHandle _rasterizerState;

	DrawablePMD _pmd;

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
			
		if (failed(CreateModel()))
		{
			return false;
		}

		ResetProjectionTransform(sz);

		return true;
	}
	
	void ResetProjectionTransform(SIZE sz)
	{
		D3DXMATRIX mProj;
		D3DXMATRIX mView;

		D3DXMatrixPerspectiveFovLH(&mProj,
				(float)D3DXToRadian(60.0f),		// 視野角60°
			static_cast<float>(sz.cx) / static_cast<float>(sz.cy), // アスペクト比
				1.0f,							// 前方投影面までの距離
				1000.0f);							// 後方投影面までの距離

		D3DXVECTOR3 vEye(0.0f, 12.0f, -20.0f);
		D3DXVECTOR3 vAt = vEye + D3DXVECTOR3(0.f, -0.5f, 5.f);
		D3DXVECTOR3 vUp(0.0f, 1.0f, 0.f);
		D3DXMatrixLookAtLH(&mView, &vEye, &vAt, &vUp);

		_renderer.SetShaderViewProjection(mProj, mView);
	}
	
	bool CreateModel()
	{
//		if (failed(_pmd.Create(_device, _T("..\\Rakugaki_MikuMikuDance_dat\\cube.pmd"))))
		if (failed(_pmd.Create(_device, _T("..\\Rakugaki_MikuMikuDance_dat\\初音ミク.pmd"))))
//		if (failed(_pmd.Create(_device, _T("..\\Rakugaki_MikuMikuDance_dat\\ピクピク\\ピクピク.pmd"))))
			return false;
		
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

			D3DXMATRIX mWorld;
			{
				//D3DXMatrixIdentity(&mWorld);
				static float angle = 0.f;
				angle += _frameRateController.GetElapsedTime() * 0.5f;
				D3DXMatrixRotationY(&mWorld, angle);
				_pmd.SetWorldMatrix(mWorld);
				
				if (!_flags.is(INVALID_MESH_DRAW))
					_pmd.Draw(_renderer);
				if (!_flags.is(INVALID_BONE_DRAW))
					_pmd.DrawBone(_renderer);
			}
			
			int y = 0, h = _device.GetFontHeight(_renderer.GetDefaultFont());

			_renderer.DrawText(0, y, Color::WHITE, _T("Test"));
			y += h;

			_renderer.DrawText(0, y, Color::GREEN, _T("FPS=%.2f (%.2fms)"), 
				_frameRateController.GetCurrentFPS(),
				_frameRateController.GetElapsedTime() * 1000.f
				);
			y += h;

			_renderer.DrawText(0, y, Color::WHITE, _T("メッシュ描画[F2]=") TFMT_BOOL, format_bool(!_flags.is(INVALID_MESH_DRAW)));
			y += h;

			_renderer.DrawText(0, y, Color::WHITE, _T("ボーン描画[F3]=") TFMT_BOOL, format_bool(!_flags.is(INVALID_BONE_DRAW)));
			y += h;
		}

		_renderer.Present();
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
			_flags = _flags.not(INVALID_MESH_DRAW);
		}
		if (params.GetVirtKey() == VK_F3)
		{
			_flags = _flags.not(INVALID_BONE_DRAW);
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
