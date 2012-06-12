// Rakugaki_MikuMikuDance_5.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "Rakugaki_MikuMikuDance_5.h"

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
	LoadString(hInstance, IDC_RAKUGAKI_MIKUMIKUDANCE_5, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// アプリケーションの初期化を実行します:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RAKUGAKI_MIKUMIKUDANCE_5));

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RAKUGAKI_MIKUMIKUDANCE_5));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_RAKUGAKI_MIKUMIKUDANCE_5);
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
		PMD_NONE,			//!< ライティングなし。
		PMD_FLAT,			//!< フラットシェーディング。
		PMD_GOURAUD,		//!< グーローシェーディング。
		PMD_PHONG,			//!< フォンシェーディング。
		PMD_TOON,			//!< トゥーンシェーディング。
		PMD_MESH_MAX,

		PMD_BONE = PMD_MESH_MAX,	//!< ボーン描画用。

		SHADER_KIND_MAX
	};
		
	struct ShaderLight
	{
		float diffuse[3];
		float padding1;
		float ambient[3];
		float padding2;
		//D3DXVECTOR3 positionInViewSpace;
		D3DXVECTOR3 directionInViewSpace;
	};

	struct ShaderInputVertex
	{
		D3DXVECTOR3 position;
		D3DXVECTOR3 normal;
		float diffuse[3];
		float specular[3];
		float ambient[3];
		float shininess;
		D3DXVECTOR2 uv;
		unsigned int boneIndices[2];
		float weight;
		unsigned int toonIndex;
	};
	struct ShaderInputOffset
	{
		D3DXVECTOR3 offset;
	};
	
	struct BoneShaderInput
	{
		D3DXVECTOR3 position;
		float color[3];
	};
		
	bool Create(Device& device)
	{
		if (failed(Renderer::Create(device)))
		{
			return false;
		}
		
		//// シェーダの作成。
		const TCHAR* SHADER_FILE_PATH = _T("..\\Rakugaki_MikuMikuDance_5\\pmd.sh");
		_shaderDependencyObjects.resize(SHADER_KIND_MAX);

		// ライティング無し用。
		ShaderDependencyObjects& shaderNone = _shaderDependencyObjects[PMD_NONE];
		shaderNone._shader = device.CreateShader(
			SHADER_FILE_PATH,
			"VS_None", "vs_4_0", 
			NULL, NULL,
			"PS_Default", "ps_4_0"
			);
		if (shaderNone._shader == ShaderHandle::INVALID)
		{
			return false;
		}

		// フラットシェーディング用。
		ShaderDependencyObjects& shaderFlat = _shaderDependencyObjects[PMD_FLAT];
		shaderFlat._shader = device.CreateShader(
			SHADER_FILE_PATH,
			"VS_Flat", "vs_4_0", 
			"GS_Flat", "gs_4_0",
			"PS_Default", "ps_4_0"
			);
		if (shaderFlat._shader == ShaderHandle::INVALID)
		{
			return false;
		}

		// グーローシェーディング用。
		ShaderDependencyObjects& shaderGouraud = _shaderDependencyObjects[PMD_GOURAUD];
		shaderGouraud._shader = device.CreateShader(
			SHADER_FILE_PATH,
			"VS_Gouraud", "vs_4_0", 
			NULL, NULL,
			"PS_Default", "ps_4_0"
			);
		if (shaderGouraud._shader == ShaderHandle::INVALID)
		{
			return false;
		}

		// フォンシェーディング用。
		ShaderDependencyObjects& shaderPhong = _shaderDependencyObjects[PMD_PHONG];
		shaderPhong._shader = device.CreateShader(
			SHADER_FILE_PATH,
			"VS_Phong", "vs_4_0", 
			NULL, NULL,
			"PS_Phong", "ps_4_0"
			);
		if (shaderPhong._shader == ShaderHandle::INVALID)
		{
			return false;
		}
		
		// トゥーンシェーディング用。
		ShaderDependencyObjects& shaderToon = _shaderDependencyObjects[PMD_TOON];
		shaderToon._shader = device.CreateShader(
			SHADER_FILE_PATH,
			"VS_Toon", "vs_4_0", 
			NULL, NULL,
			"PS_Toon", "ps_4_0"
			);
		if (shaderToon._shader == ShaderHandle::INVALID)
		{
			return false;
		}
		
		const TCHAR* filePaths[] =
		{
			_T("..\\Rakugaki_MikuMikuDance_dat\\toon01.bmp"),
			_T("..\\Rakugaki_MikuMikuDance_dat\\toon02.bmp"),
			_T("..\\Rakugaki_MikuMikuDance_dat\\toon03.bmp"),
			_T("..\\Rakugaki_MikuMikuDance_dat\\toon04.bmp"),
			_T("..\\Rakugaki_MikuMikuDance_dat\\toon05.bmp"),
			_T("..\\Rakugaki_MikuMikuDance_dat\\toon06.bmp"),
			_T("..\\Rakugaki_MikuMikuDance_dat\\toon07.bmp"),
			_T("..\\Rakugaki_MikuMikuDance_dat\\toon08.bmp"),
			_T("..\\Rakugaki_MikuMikuDance_dat\\toon09.bmp"),
			_T("..\\Rakugaki_MikuMikuDance_dat\\toon10.bmp"),
		};

		_texToons = device.CreateTextureArrayFromFiles(filePaths, _countof(filePaths));

		SetShaderTextureArray(shaderToon._shader, "texToonArray", _texToons);

		// ボーン描画用。
		ShaderDependencyObjects& shaderBone = _shaderDependencyObjects[PMD_BONE];
		shaderBone._shader = device.CreateShader(
			SHADER_FILE_PATH,
			"VS_Bone", "vs_4_0", 
			NULL, NULL,
			"PS_Bone", "ps_4_0"
			);
		if (shaderBone._shader == ShaderHandle::INVALID)
		{
			return false;
		}
		
		// 入力レイアウトの作成。
		{
			D3D10_INPUT_ELEMENT_DESC layout[] = {
				{"POSITION",     0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(ShaderInputVertex, position), D3D10_INPUT_PER_VERTEX_DATA, 0},
				{"NORMAL",       0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(ShaderInputVertex, normal), D3D10_INPUT_PER_VERTEX_DATA, 0},
				{"COLOR",        0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(ShaderInputVertex, diffuse), D3D10_INPUT_PER_VERTEX_DATA, 0},
				{"COLOR",        1, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(ShaderInputVertex, specular), D3D10_INPUT_PER_VERTEX_DATA, 0},
				{"COLOR",        2, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(ShaderInputVertex, ambient), D3D10_INPUT_PER_VERTEX_DATA, 0},
				{"SHININESS",    0, DXGI_FORMAT_R32_FLOAT,       0, offsetof(ShaderInputVertex, shininess), D3D10_INPUT_PER_VERTEX_DATA, 0},
				{"TEXTURE",      0, DXGI_FORMAT_R32G32_FLOAT,    0, offsetof(ShaderInputVertex, uv), D3D10_INPUT_PER_VERTEX_DATA, 0},
				{"BONE_INDICES", 0, DXGI_FORMAT_R32G32_UINT,     0, offsetof(ShaderInputVertex, boneIndices), D3D10_INPUT_PER_VERTEX_DATA, 0},
				{"WEIGHT",       0, DXGI_FORMAT_R32_FLOAT,       0, offsetof(ShaderInputVertex, weight), D3D10_INPUT_PER_VERTEX_DATA, 0},
				{"TOON_INDEX",   0, DXGI_FORMAT_R32_UINT,        0, offsetof(ShaderInputVertex, toonIndex), D3D10_INPUT_PER_VERTEX_DATA, 0},
				{"POSITION",     1, DXGI_FORMAT_R32G32B32_FLOAT, 1, offsetof(ShaderInputOffset, offset), D3D10_INPUT_PER_VERTEX_DATA, 0},
			};
			
			InputLayoutHandle inputLayout = 
				device.CreateInputLayout(
					layout,
					_countof(layout),
					shaderNone._shader
					);
			if (inputLayout == InputLayoutHandle::INVALID)
				return false;

			for (int i = 0; i < SHADER_KIND_MAX; ++i)
			{
				if (i == PMD_BONE)
					continue;
				_shaderDependencyObjects[i]._inputLayout = inputLayout;
			}
		}
		
		{
			D3D10_INPUT_ELEMENT_DESC layout[] = {
				{"POSITION",     0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(BoneShaderInput, position), D3D10_INPUT_PER_VERTEX_DATA, 0},
				{"COLOR",        0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(BoneShaderInput, color), D3D10_INPUT_PER_VERTEX_DATA, 0},
			};
			
			InputLayoutHandle inputLayout = 
				device.CreateInputLayout(
					layout,
					_countof(layout),
					shaderBone._shader
					);
			if (inputLayout == InputLayoutHandle::INVALID)
				return false;

			shaderBone._inputLayout = inputLayout;
		}
		
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
	
	void SetShaderProjection(D3DXMATRIX src)
	{
		D3DXMatrixTranspose(&src, &src);
		for (int i = 0; i < SHADER_KIND_MAX; ++i)
		{
			*static_cast<D3DXMATRIX*>(GetShaderGlobalVariable(GetShader(i), "proj")) = src;
		}
	}

	void SetShaderViewAndEye(D3DXMATRIX view, const D3DXVECTOR3& /*eye*/)
	{
		D3DXMatrixTranspose(&view, &view);
		for (int i = 0; i < SHADER_KIND_MAX; ++i)
		{
			*static_cast<D3DXMATRIX*>(GetShaderGlobalVariable(GetShader(i), "view")) = view;
		}
//		*static_cast<D3DXVECTOR3*>(GetShaderGlobalVariable(GetShader(PMD_TOON_EDGE1), "eye")) = eye;
	}
	
	void SetShaderLight(const ShaderLight& light)
	{
		for (int i = 0; i < SHADER_KIND_MAX; ++i)
		{
			if (i == PMD_BONE)
				continue;
			*static_cast<ShaderLight*>(GetShaderGlobalVariable(GetShader(i), "light")) = light;
		}
	}
	
	TextureHandle _texToons;

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

namespace
{
	static const float MOTION_FPS = 1.f / 30.f;	// MMD は 30FPS。
	
}	// end namespace

class DrawablePMD
{
	struct BoneInfo;
	struct IKInfo;
	struct BoneKeyFrame;
public:
	
	enum INTERPOLATION_TYPE
	{
		INTERPOLATION_NONE,
		INTERPOLATION_LINEAR,
		INTERPOLATION_USE_PARAMETER,
		INTERPOLATION_TYPE_MAX,
	};
		
	void Release()
	{
		// TODO:
	}
		
	//! 初期化。
	bool Create(Device& device, const TCHAR* pmdPath, const TCHAR* vmdPath)
	{
		SetShaderKind(MyRenderer::PMD_TOON);

		// モデルデータの読み込み。
		MMD::PMD pmd;

		if (failed(LoadPMD(pmd, pmdPath)))
			return false;

		static TCHAR dirPath[256];
		GetDirectoryPath(pmdPath, dirPath, _countof(dirPath));
			
		// メッシュの頂点情報のコピー。
		std::vector<MyRenderer::ShaderInputVertex> meshVertices;
		meshVertices.resize(pmd.vert_count);
		for (unsigned int i = 0; i < pmd.vert_count; ++i)
		{
			meshVertices[i].position = ToD3DXVECTOR3(pmd.vertex[i].pos);
			meshVertices[i].uv = ToD3DXVECTOR2(pmd.vertex[i].uv);
			meshVertices[i].normal = ToD3DXVECTOR3(pmd.vertex[i].normal_vec);
			meshVertices[i].boneIndices[0] = pmd.vertex[i].bone_num[0];
			meshVertices[i].boneIndices[1] = pmd.vertex[i].bone_num[1];
			meshVertices[i].weight = pmd.vertex[i].bone_weight / 100.f;
		}
		
		// インデックスバッファを作成する。
		_indexBuffer = device.CreateIndexBuffer(D3D10_USAGE_IMMUTABLE, 0, sizeof(unsigned short), pmd.face_vert_count, &pmd.face_vert_index[0]);
		if (_indexBuffer == IndexBufferHandle::INVALID)
			return false;
		_numIndices = pmd.face_vert_count;
		
		// デフォルトテクスチャの読み込み。
		int textureIndexDefault = 0;
		{
			TextureHandle tex = device.CreateTextureFromFile(_T("..\\Rakugaki_MikuMikuDance_dat\\texture_default.bmp"));
			if (tex == TextureHandle::INVALID)
				return false;
			_textures.push_back(tex);
		}
			
		// マテリアル情報の解析。
		_drawPlans.push_back(DrawPlan());
		DrawPlan* pPlan = &_drawPlans.back();
		pPlan->_startIndex = 0;
		pPlan->_texture = textureIndexDefault;
		unsigned int total = 0;
		for (unsigned int i = 0, k = 0; i < pmd.material_count; ++i)
		{
			MMD::PMD::Material& material = pmd.material[i];

			// マテリアルの対応する頂点にマテリアル情報を設定する。
			total += material.face_vert_count;
			for (; k < total; ++k)
			{
				MyRenderer::ShaderInputVertex& vertex = meshVertices[pmd.face_vert_index[k]];
				vertex.diffuse[0] = material.diffuse_color[0];
				vertex.diffuse[1] = material.diffuse_color[1];
				vertex.diffuse[2] = material.diffuse_color[2];

				vertex.specular[0] = material.specular_color[0];
				vertex.specular[1] = material.specular_color[1];
				vertex.specular[2] = material.specular_color[2];

				vertex.ambient[0] = material.mirror_color[0];
				vertex.ambient[1] = material.mirror_color[1];
				vertex.ambient[2] = material.mirror_color[2];
				
				vertex.shininess = material.specularity;
				
				vertex.toonIndex = material.toon_index;
				if (vertex.toonIndex == -1)
				{
					vertex.toonIndex = 9;	// 10番目のトゥーン用テクスチャにしておく。
				}
			}
				
			// テクスチャで描画プランを分ける。
			int textureIndex = textureIndexDefault;
			if (strlen(material.texture_file_name) > 0)
			{
				// テクスチャを読み込む。
				TString fileName;
				{
					std::string texFileName = material.GetTextureFileName();
					StringToTString(texFileName, fileName);
				}
				TString texPath = dirPath;
				texPath += fileName;
				DEBUG_TRACE(texPath.c_str());
				TextureHandle tex = device.CreateTextureFromFile(texPath.c_str());
				if (tex == TextureHandle::INVALID)
					return false;
				_textures.push_back(tex);

				textureIndex = static_cast<int>(_textures.size() - 1);
			}
				
			if (pPlan->_texture != textureIndex)	// この行をコメントすれば、マテリアル単位に DrawPlan が分かれる。
			{
				_drawPlans.push_back(*pPlan);
				pPlan = &_drawPlans.back();
				pPlan->_startIndex = pPlan->_startIndex + pPlan->_numIndices;
				pPlan->_numIndices = 0;
			}
			pPlan->_numIndices += material.face_vert_count;
			pPlan->_texture = textureIndex;
		}

		// メッシュの頂点バッファ作成。
		_meshVertexBuffer = device.CreateVertexBuffer(D3D10_USAGE_IMMUTABLE, 0, sizeof(MyRenderer::ShaderInputVertex), meshVertices.size(), &meshVertices[0]);
		if (_meshVertexBuffer == VertexBufferHandle::INVALID)
			return false;

		// ボーンの解析。
		_bones.resize(pmd.bone_count);
		_skinningMatrices.resize(pmd.bone_count);
		_drawBoneVertices.resize(pmd.bone_count);

		std::vector<unsigned short> drawBoneIndices;
		drawBoneIndices.reserve(pmd.bone_count*2);

		unsigned short k = 0;
		for (unsigned int i = 0; i < pmd.bone_count; ++i)
		{
			MMD::PMD::Bone& boneHead = pmd.bone[i];

			// ボーン情報の登録。
			BoneInfo& boneInfo = _bones[i];
			boneInfo._name = boneHead.bone_name;
				
			Vector3 headPosParentSpace;
				
			boneInfo._parent = boneHead.parent_bone_index;
			if (boneHead.parent_bone_index != MMD::PMD::Bone::INVALID_PARENT)
			{
				// 親ボーンに子ボーンを登録。
				MMD::PMD::Bone& boneParent = pmd.bone[boneHead.parent_bone_index];
				BoneNameIndexMap::iterator found = _boneNameIndexMap.find(boneParent.bone_name);
				if (found == _boneNameIndexMap.end())
					continue;	// あるとめんどくさい…。
				_bones[found->second]._childs.push_back(i);
					
				headPosParentSpace = boneHead.bone_head_pos - boneParent.bone_head_pos;
			}
			else
			{
				headPosParentSpace = boneHead.bone_head_pos;
				_bonesRoot.push_back(i);
			}

			D3DXMatrixTranslation(&boneInfo._transformModelSpaceToBindBoneSpace, -boneHead.bone_head_pos.x, -boneHead.bone_head_pos.y, -boneHead.bone_head_pos.z);
			D3DXMatrixTranslation(&boneInfo._transformToParentSpace, headPosParentSpace.x, headPosParentSpace.y, headPosParentSpace.z);
			D3DXMatrixIdentity(&boneInfo._transformInLocalSpace);

			D3DXMatrixIdentity(&_skinningMatrices[i]);
			D3DXMatrixTranspose(&_skinningMatrices[i], &_skinningMatrices[i]);

			_boneNameIndexMap.insert(std::make_pair(boneHead.bone_name, i));			

			// 描画用頂点のセットアップ。
			MyRenderer::BoneShaderInput vertexHead;
			vertexHead.position = ToD3DXVECTOR3(boneHead.bone_head_pos);
			Color::WHITE.ToFloat3(vertexHead.color);
			_drawBoneVertices[i] = vertexHead;

			if (boneHead.tail_pos_bone_index != MMD::PMD::Bone::INVALID_TAIL)
			{
				drawBoneIndices.push_back(i);
				drawBoneIndices.push_back(boneHead.tail_pos_bone_index);
			}
				
		}	// end for (unsigned int i = 0; ...)
			
		_drawBoneVertexBuffer = device.CreateVertexBuffer(D3D10_USAGE_DEFAULT, 0, sizeof(MyRenderer::BoneShaderInput), _drawBoneVertices.size(), &_drawBoneVertices[0]);
		if (_drawBoneVertexBuffer == VertexBufferHandle::INVALID)
			return false;

		_numDrawBoneVertices = static_cast<unsigned int>(drawBoneIndices.size());
		_drawBoneIndexBuffer = device.CreateIndexBuffer(D3D10_USAGE_IMMUTABLE, 0, sizeof(unsigned short), _numDrawBoneVertices, &drawBoneIndices[0]);
		if (_drawBoneIndexBuffer == IndexBufferHandle::INVALID)
			return false;

		// IKデータの解析。
		_iks.resize(pmd.ik_data_count);
		for (unsigned int i = 0; i < pmd.ik_data_count; ++i)
		{
			MMD::PMD::IKData& ik = pmd.ik_data[i];
			IKInfo& ikInfo = _iks[i];
			ikInfo._target = ik.ik_bone_index;
			ikInfo._effector = ik.ik_target_bone_index;
			ikInfo._chains.resize(ik.ik_child_bone_index.size());
			for (size_t k = 0, ksz = ikInfo._chains.size(); k < ksz; ++k)
			{
				IKInfo::ChainInfo& chainInfo = ikInfo._chains[k];
				chainInfo._index = ik.ik_child_bone_index[k];
				chainInfo._flags.clear();
				BoneInfo& boneChain = _bones[chainInfo._index];
				if (boneChain._name.find("ひざ") != std::string::npos)
				{
					chainInfo._flags.set(IKInfo::ChainInfo::ROTATE_LIMIT_X | IKInfo::ChainInfo::ROTATE_LIMIT_Y | IKInfo::ChainInfo::ROTATE_LIMIT_Z);
					chainInfo._rotateLimitX._min = static_cast<float>(D3DXToRadian(-160.f));
					chainInfo._rotateLimitX._max = static_cast<float>(D3DXToRadian(-1.f));
					chainInfo._rotateLimitY._min = static_cast<float>(D3DXToRadian(0.f));
					chainInfo._rotateLimitY._max = -chainInfo._rotateLimitY._min;
					chainInfo._rotateLimitZ._min = static_cast<float>(D3DXToRadian(0.f));
					chainInfo._rotateLimitZ._max = -chainInfo._rotateLimitZ._min;
				}
				else
				if (boneChain._name.compare("右足") == 0 || boneChain._name.compare("左足") == 0)
				{
					chainInfo._flags.set(/*IKInfo::ChainInfo::ROTATE_LIMIT_X |*/ IKInfo::ChainInfo::ROTATE_LIMIT_Y /*| IKInfo::ChainInfo::ROTATE_LIMIT_Z*/);
					chainInfo._rotateLimitX._min = static_cast<float>(D3DXToRadian(-90.f));
					chainInfo._rotateLimitX._max = -chainInfo._rotateLimitX._min;
					chainInfo._rotateLimitY._min = static_cast<float>(D3DXToRadian(-90.f));
					chainInfo._rotateLimitY._max = -chainInfo._rotateLimitY._min;
					chainInfo._rotateLimitZ._min = static_cast<float>(D3DXToRadian(-90.f));
					chainInfo._rotateLimitZ._max = -chainInfo._rotateLimitZ._min;
				}
			}
			ikInfo._iterations = ik.iterations;
			ikInfo._controlWeight = ik.control_weight * 4;
		}
		
		// 表情データの解析。
		_skins = pmd.skin;
		_skinIndexBase = 0;
		for (unsigned int i = 0; i < pmd.skin_count; ++i)
		{
			if (pmd.skin[i].skin_type == MMD::PMD::Skin::TYPE_BASE)
				_skinIndexBase = i;
			_skinNameIndexMap.insert(std::make_pair(pmd.skin[i].skin_name, i));
		}

		// 表情アニメ用の頂点オフセットバッファを作る。
		// ShaderInputVertex に offset を入れると、更新が一切無い UV や 頂点カラーまで毎フレーム転送する事になる為、別で作成した。きっとこっちのが早いよね？
		MyRenderer::ShaderInputOffset shaderInputOffsetInitial;
		shaderInputOffsetInitial.offset = D3DXVECTOR3(0.f, 0.f, 0.f);
		_meshOffsets.resize(meshVertices.size(), shaderInputOffsetInitial);

		_meshOffsetBuffer = device.CreateVertexBuffer(D3D10_USAGE_DEFAULT, 0, sizeof(MyRenderer::ShaderInputOffset), _meshOffsets.size(), &_meshOffsets[0]);
		if (_meshOffsetBuffer == VertexBufferHandle::INVALID)
			return false;
		
		// モーションデータの読み込み。対応するボーンごとに整理しておく。
		MMD::VMD vmd;
		if (failed(LoadVMD(vmd, vmdPath)))
			return false;
		_allBoneKeyFrames.resize(_bones.size());
		_keyFrameMax = 0;
		for (unsigned int i = 0; i < vmd.motion_count; ++i)
		{
			MMD::VMD::Motion& motion = vmd.motion[i];
			BoneNameIndexMap::iterator found = _boneNameIndexMap.find(motion.BoneName);
			if (found != _boneNameIndexMap.end())
			{
				BoneKeyFrames& keyFrames = _allBoneKeyFrames[found->second];

				BoneKeyFrame keyFrame;
				keyFrame._frame = motion.Frame;
				keyFrame._time = motion.Frame * MOTION_FPS;
				keyFrame._translation = ToD3DXVECTOR3(motion.Location);
				keyFrame._rotation = *reinterpret_cast<D3DXQUATERNION*>(motion.Rotation);

				keyFrame._interpolation_x.p1 = D3DXVECTOR2(motion.Interpolation[0],	 motion.Interpolation[4]);
				keyFrame._interpolation_x.p2 = D3DXVECTOR2(motion.Interpolation[8],	 motion.Interpolation[12]);
				keyFrame._interpolation_y.p1 = D3DXVECTOR2(motion.Interpolation[1],	 motion.Interpolation[5]);
				keyFrame._interpolation_y.p2 = D3DXVECTOR2(motion.Interpolation[9],	 motion.Interpolation[13]);
				keyFrame._interpolation_z.p1 = D3DXVECTOR2(motion.Interpolation[2],	 motion.Interpolation[6]);
				keyFrame._interpolation_z.p2 = D3DXVECTOR2(motion.Interpolation[10], motion.Interpolation[14]);
				keyFrame._interpolation_r.p1 = D3DXVECTOR2(motion.Interpolation[3],	 motion.Interpolation[7]);
				keyFrame._interpolation_r.p2 = D3DXVECTOR2(motion.Interpolation[11], motion.Interpolation[15]);

				keyFrames.insert(std::make_pair(motion.Frame, keyFrame));

				if (_keyFrameMax < motion.Frame)
					_keyFrameMax = motion.Frame;
			}
				
			#if defined(_DEBUG)
			TString bone_name;
			StringToTString(motion.BoneName, bone_name);
			DEBUG_TRACE(_T("Bone[%u] : name=%s, frame=%u, trans=(%.2f, %.2f, %.2f), rot=(%.2f, %.2f, %.2f, %.2f)"),
				i, bone_name.c_str(), motion.Frame, motion.Location.x, motion.Location.y, motion.Location.z,
				motion.Rotation[0], motion.Rotation[1], motion.Rotation[2], motion.Rotation[3]);
			#endif
		}

		SetInterpolationType(INTERPOLATION_LINEAR);
		
		// 表情アニメーションデータの整理。
		_allSkinKeyFrames.resize(_skins.size());
		for (unsigned int i = 0; i < vmd.skin_count; ++i)
		{
			MMD::VMD::Skin& skin = vmd.skin[i];
			SkinNameIndexMap::iterator found = _skinNameIndexMap.find(skin.SkinName);
			if (found != _skinNameIndexMap.end())
			{
				SkinKeyFrames& keyFrames = _allSkinKeyFrames[found->second];
				SkinKeyFrame keyFrame;
				keyFrame._frame = skin.Frame;
				keyFrame._time = skin.Frame * MOTION_FPS;
				keyFrame._weight = skin.Weight;
				keyFrames.insert(std::make_pair(skin.Frame, keyFrame));

				if (_keyFrameMax < skin.Frame)
					_keyFrameMax = skin.Frame;
			}
		}
		
		// ラスタライザステート。
		D3D10_RASTERIZER_DESC rsDesc;
		rsDesc.FillMode = D3D10_FILL_SOLID;
		rsDesc.CullMode = D3D10_CULL_NONE;
		rsDesc.FrontCounterClockwise = FALSE;
		rsDesc.DepthBias = 0;
		rsDesc.DepthBiasClamp = 0.f;
		rsDesc.SlopeScaledDepthBias = 0.f;
		rsDesc.DepthClipEnable = TRUE;
		rsDesc.ScissorEnable = FALSE;
		rsDesc.MultisampleEnable = FALSE;
		rsDesc.AntialiasedLineEnable = FALSE;
		_rasterizerState = device.CreateRasterizerState(rsDesc);
		
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
	
	// モーション時間を設定し、ボーンのローカルトランスフォームを更新する。
	void SetMotion(float time)
	{
		D3DXMATRIX m;

		_motionTime = time;

		unsigned int keyFrame = GetMotionKeyFrame();
		float validMotionTime = GetMotionTime();
		
		// モーション。
		for (size_t i = 0, sz = _allBoneKeyFrames.size(); i < sz; ++i)
		{
			GetMotionMatrix(keyFrame, validMotionTime, i, m);
			BoneInfo& bone = _bones[i];
			bone._transformInLocalSpace = m;
		}
		
		// 表情アニメーション。
		MMD::PMD::Skin& skinBase = _skins[_skinIndexBase];
		for (size_t i = 0, sz = skinBase.skin_vert.size(); i < sz; ++i)
		{
			// 初期化。
			unsigned int vert_index = skinBase.skin_vert[i].index;
			_meshOffsets[vert_index].offset = D3DXVECTOR3(0.f, 0.f, 0.f);
		}
		
		float weight;
		for (size_t i = 0, isz = _allSkinKeyFrames.size(); i < isz; ++i)
		{
			if (_skinIndexBase == i)
				continue;
			weight = GetFacialAnimationWeight(keyFrame, validMotionTime, i);
			if (weight <= 0.f)
				continue;
			// オフセットを更新。
			MMD::PMD::Skin& skinNobase = _skins[i];
			for (size_t k = 0, ksz = skinNobase.skin_vert.size(); k < ksz; ++k)
			{
				MMD::PMD::Skin::Vert& vertOffset = skinNobase.skin_vert[k];
				unsigned int vert_index = skinBase.skin_vert[vertOffset.index].index;
				_meshOffsets[vert_index].offset = ToD3DXVECTOR3(vertOffset.pos) * weight;
			}
		}
	}

	#define LERP(x0, x1, t) ((x1 - x0) * t + x0) 

	// 指定ボーンの、指定のキーフレーム・モーション時間の時のマトリクスを取得する。
	void GetMotionMatrix(unsigned int keyFrame, float motionTime, size_t boneIndex, D3DXMATRIX& m)
	{
		BoneKeyFrames& keyFrames = _allBoneKeyFrames[boneIndex];
		if (keyFrames.empty())
		{
			// キーフレームリストを作る際に、必ず1要素以上入れるようにしておけば分岐不要。
			D3DXMatrixIdentity(&m);
			return;
		}
		
		BoneKeyFrames::const_iterator found = keyFrames.lower_bound(keyFrame);
		if (found == keyFrames.end() || found->first > keyFrame)
		{
			--found;
		}
		BoneKeyFrames::const_iterator next = found;
		++next;
		if (next == keyFrames.end())
		{
			const BoneKeyFrame& boneKeyFrame = found->second;
			D3DXMatrixTranslation(&m, boneKeyFrame._translation.x, boneKeyFrame._translation.y, boneKeyFrame._translation.z);
			D3DXMATRIX mRot;
			D3DXMatrixRotationQuaternion(&mRot, &boneKeyFrame._rotation);
			m = mRot * m;
			return;
		}

		const BoneKeyFrame& boneKeyFrame1 = found->second;
		const BoneKeyFrame& boneKeyFrame2 = next->second;

		// 補間処理。
		(this->*_pfnInterpolateBoneKeyFrame)(boneKeyFrame1, boneKeyFrame2, motionTime, m);
	}
	
	// 指定表情の、指定のキーフレーム・モーション時間の時のウェイトを取得する。
	float GetFacialAnimationWeight(unsigned int keyFrame, float motionTime, size_t skinIndex)
	{
		SkinKeyFrames& keyFrames = _allSkinKeyFrames[skinIndex];
		if (keyFrames.empty())
		{
			return 0.f;
		}
		
		SkinKeyFrames::const_iterator found = keyFrames.lower_bound(keyFrame);
		if (found == keyFrames.end() || found->first > keyFrame)
		{
			--found;
		}
		SkinKeyFrames::const_iterator next = found;
		++next;
		if (next == keyFrames.end())
		{
			const SkinKeyFrame& skinKeyFrame = found->second;
			return skinKeyFrame._weight;
		}

		const SkinKeyFrame& skinKeyFrame1 = found->second;
		const SkinKeyFrame& skinKeyFrame2 = next->second;

		// 線形補間。
		float t = (motionTime - skinKeyFrame1._time) / (skinKeyFrame2._time - skinKeyFrame1._time);
		return LERP(skinKeyFrame1._weight, skinKeyFrame2._weight, t);
	}

	// 補間無し。
	void BoneKeyFrameNoInterpolation(const BoneKeyFrame& boneKeyFrame1, const BoneKeyFrame& /*boneKeyFrame2*/, float /*motionTime*/, D3DXMATRIX& m)
	{
		D3DXMATRIX mRot;
		D3DXMatrixTranslation(&m, boneKeyFrame1._translation.x, boneKeyFrame1._translation.y, boneKeyFrame1._translation.z);
		D3DXMatrixRotationQuaternion(&mRot, &boneKeyFrame1._rotation);
		m = mRot * m;
	}

	// 線形補間＆球面線形補間。
	void BoneKeyFrameLinearInterpolation(const BoneKeyFrame& boneKeyFrame1, const BoneKeyFrame& boneKeyFrame2, float motionTime, D3DXMATRIX& m)
	{
		D3DXMATRIX mRot;

		float t = (motionTime - boneKeyFrame1._time) / (boneKeyFrame2._time - boneKeyFrame1._time);
		
		D3DXVECTOR3 translation;
		D3DXVec3Lerp(&translation, &boneKeyFrame1._translation, &boneKeyFrame2._translation, t);
		D3DXMatrixTranslation(&m, translation.x, translation.y, translation.z);
		
		D3DXQUATERNION rotation;
		D3DXQuaternionSlerp(&rotation, &boneKeyFrame1._rotation, &boneKeyFrame2._rotation, t);
		D3DXMatrixRotationQuaternion(&mRot, &rotation);

		m = mRot * m;
	}
	
	// 線形補間パラメータtの算出に、補間パラメータを使用したベジエ曲線を使った補間処理。
	// これが本家と同じ処理なのかは不明、miku_test4で比べてみるとなんか本家のほうがもっとキレがある。
	void BoneKeyFrameUseParameterInterpolation(const BoneKeyFrame& boneKeyFrame1, const BoneKeyFrame& boneKeyFrame2, float motionTime, D3DXMATRIX& m)
	{
		D3DXMATRIX mRot;

		float bt = (motionTime - boneKeyFrame1._time) / (boneKeyFrame2._time - boneKeyFrame1._time);
		float ltx = boneKeyFrame2._interpolation_x.Bezier(bt).y;	// boneKeyFrame2 のパラメータを使う。線形補間の t には y を使う。
		float lty = boneKeyFrame2._interpolation_y.Bezier(bt).y;	
		float ltz = boneKeyFrame2._interpolation_z.Bezier(bt).y;	
		float ltr = boneKeyFrame2._interpolation_r.Bezier(bt).y;
		float ltw = boneKeyFrame2._interpolation_x.Bezier(0.5).y;
		DEBUG_TRACE(_T("bt=%.2f, ltw=%.2f"), bt, ltw);
		
		D3DXVECTOR3 translation;
		translation.x = LERP(boneKeyFrame1._translation.x, boneKeyFrame2._translation.x, ltx);
		translation.y = LERP(boneKeyFrame1._translation.y, boneKeyFrame2._translation.y, lty);
		translation.z = LERP(boneKeyFrame1._translation.z, boneKeyFrame2._translation.z, ltz);
		D3DXMatrixTranslation(&m, translation.x, translation.y, translation.z);
		
		D3DXQUATERNION rotation;
		D3DXQuaternionSlerp(&rotation, &boneKeyFrame1._rotation, &boneKeyFrame2._rotation, ltr);
		D3DXMatrixRotationQuaternion(&mRot, &rotation);

		m = mRot * m;
	}
	
	// もろもろ更新して確定させる。
	void Update()
	{
		// 各ボーンのモデル空間トランスフォームを計算し、IKなしの姿勢を一旦確定させる。
		D3DXMATRIX mIdentity;
		D3DXMatrixIdentity(&mIdentity);
		for (size_t i = 0, sz = _bonesRoot.size(); i < sz; ++i)
		{
			unsigned int boneIndex = _bonesRoot[i];
			BoneInfo& rootBone = _bones[boneIndex];
			Transform(rootBone, boneIndex, mIdentity);
		}

		// IKを解決する。
		UpdateIK();
		
		// スキニング用マトリクスを更新する。
		UpdateSkinningMatrices();
	}
	
	// ボーンをモデル空間にトランスフォームする。
	void Transform(BoneInfo& bone, unsigned int boneIndex, const D3DXMATRIX& transformToParentModelSpace)
	{
		bone._transformFixInModelSpace = bone._transformInLocalSpace * bone._transformToParentSpace * transformToParentModelSpace;
		MyRenderer::BoneShaderInput& drawBoneVertex = _drawBoneVertices[boneIndex];
		drawBoneVertex.position = GetTranslationVector(bone._transformFixInModelSpace);
		
		for (size_t i = 0, sz = bone._childs.size(); i < sz; ++i)
		{
			unsigned int boneIndex = bone._childs[i];
			BoneInfo& childBone = _bones[boneIndex];
			Transform(childBone, boneIndex, bone._transformFixInModelSpace);
		}
	}
	
	// スキニング用のマトリクスを更新する。
	void UpdateSkinningMatrices()
	{
		D3DXMATRIX skinning;
		for (size_t i = 0, sz = _bones.size(); i < sz; ++i)
		{
			BoneInfo& bone = _bones[i];
			skinning = bone._transformModelSpaceToBindBoneSpace * bone._transformFixInModelSpace;
			D3DXMatrixTranspose(&_skinningMatrices[i], &skinning);
		}
	}
	
	// CCD法によるIK解決。
	void UpdateIK()
	{
		for (size_t i = 0, isz = _iks.size(); i < isz; ++i)
		{
			ResolveIK(_iks[i]);
		}
	}
	
	void ResolveIK(const IKInfo& ik)
	{
		//DEBUG_TRACE(_T("==============="));
		unsigned short boneIndexTarget = ik._target;
		unsigned short boneIndexEffector = ik._effector;
		BoneInfo& boneTarget = _bones[boneIndexTarget];
		BoneInfo& boneEffector = _bones[boneIndexEffector];

		for (unsigned short i = 0; i < ik._iterations; ++i)
		{
			//DEBUG_TRACE(_T("-------------"));
			// ターゲットは外部から入力される位置。エフェクタはターゲットに合わせたいノード（IKチェーンの先端）。
			// チェーンリストには、エフェクタから辿って近いレベルの順に、チェーンボーンが格納されている。
			// このチェーンリストを一つ一つ辿って各チェーンを基準とし、基準からエフェクタへ向かうベクトルを、
			// 基準からターゲットへ向かうベクトルへ合わせるという作業を一定回数繰り返す事で近づける。
			for (size_t k = 0, ksz = ik._chains.size(); k < ksz; ++k)
			{
				const IKInfo::ChainInfo& chain = ik._chains[k];
				unsigned short boneIndexBasis = chain._index;

				BoneInfo& boneBasis = _bones[boneIndexBasis];
				
				// エフェクタボーンとターゲットボーンとの距離が最短になる様に chain ボーンの角度を調整する。
				// control_weight の使い方がイマイチわからん…。
				D3DXVECTOR3 axis;

				D3DXVECTOR3 posChain = GetTranslationVector(boneBasis._transformFixInModelSpace);
				D3DXMATRIX transformModelSpaceToChainBoneSpace;
				D3DXMatrixInverse(&transformModelSpaceToChainBoneSpace, NULL, &boneBasis._transformFixInModelSpace);

				D3DXVECTOR3 toTarget = GetTranslationVector(boneTarget._transformFixInModelSpace);
				D3DXVec3TransformCoord(&toTarget, &toTarget, &transformModelSpaceToChainBoneSpace);
				D3DXVECTOR3 toEffector = GetTranslationVector(boneEffector._transformFixInModelSpace);
				D3DXVec3TransformCoord(&toEffector, &toEffector, &transformModelSpaceToChainBoneSpace);
				
				D3DXVec3Normalize(&toTarget, &toTarget);
				D3DXVec3Normalize(&toEffector, &toEffector);
				D3DXVec3Cross(&axis, &toEffector, &toTarget);
				D3DXVec3Normalize(&axis, &axis);
				float cosTheta = D3DXVec3Dot(&toEffector, &toTarget);
				cosTheta = Clamp(cosTheta, -1.f, 1.f);
				float angle = acos(cosTheta);
				if (angle > 1.0e-5f)
				{
					angle = min(ik._controlWeight, angle);
					//DEBUG_TRACE(_T("cos=%.2f, angle=%.2f"), cosTheta, angle);
					D3DXMATRIX adjust;
					D3DXMatrixRotationAxis(&adjust, &axis, angle);
					boneBasis._transformInLocalSpace *= adjust;
					if (!chain._flags.empty())
					{
						D3DXVECTOR3 angles = GetEulerAnglesZXY(boneBasis._transformInLocalSpace);
						if (chain._flags.is(IKInfo::ChainInfo::ROTATE_LIMIT_X))
							angles.x = Clamp(angles.x, chain._rotateLimitX._min, chain._rotateLimitX._max);
						if (chain._flags.is(IKInfo::ChainInfo::ROTATE_LIMIT_Y))
							angles.y = Clamp(angles.y, chain._rotateLimitY._min, chain._rotateLimitY._max);
						if (chain._flags.is(IKInfo::ChainInfo::ROTATE_LIMIT_Z))
							angles.z = Clamp(angles.z, chain._rotateLimitZ._min, chain._rotateLimitZ._max);
						D3DXMatrixRotationYawPitchRoll(&boneBasis._transformInLocalSpace, angles.y, angles.x, angles.z);
					}
				
					// chain ボーンとその子ボーン（自動的にエフェクタも含まれる）のモデル空間トランスフォームを再計算する（ちょっと無駄な計算が多いが…）。
					D3DXMATRIX transformParent;
					if (boneBasis._parent != MMD::PMD::Bone::INVALID_PARENT)
					{
						transformParent = _bones[boneBasis._parent]._transformFixInModelSpace;
					}
					else
					{
						D3DXMatrixIdentity(&transformParent);
					}
				
					Transform(boneBasis, boneIndexBasis, transformParent);
				}
			}
				
			// 十分に近くなったら処理を打ち切る。
			const float errToleranceSq = 0.000001f;
			D3DXVECTOR3 targetToEffector = GetTranslationVector(boneEffector._transformFixInModelSpace) - GetTranslationVector(boneTarget._transformFixInModelSpace);
			if (D3DXVec3LengthSq(&targetToEffector) < errToleranceSq)
				return;
		}
	}
	
	
	
	//
	void SetMotionLoop(bool loop)
	{
		_flags.set(MOTION_LOOP, loop);
	}

	float GetMotionTime() const
	{
		if (GetMotionLoop())
			return fmod(_motionTime, _keyFrameMax * MOTION_FPS);
		return _motionTime;
	}
	
	unsigned int GetMotionKeyFrame() const
	{
		unsigned int motionFrame = static_cast<unsigned int>(_motionTime * (1.f/MOTION_FPS));
		if (GetMotionLoop())
			motionFrame %= _keyFrameMax;
		return motionFrame;
	}
	
	bool GetMotionLoop() const
	{
		return _flags.is(MOTION_LOOP);
	}
	
	INTERPOLATION_TYPE GetInterpolationType() const
	{
		return _interpolationType;
	}
	
	void SetInterpolationType(INTERPOLATION_TYPE type)
	{
		switch (type)
		{
			case INTERPOLATION_LINEAR:
				_pfnInterpolateBoneKeyFrame = &DrawablePMD::BoneKeyFrameLinearInterpolation;
				break;
			case INTERPOLATION_USE_PARAMETER:
				_pfnInterpolateBoneKeyFrame = &DrawablePMD::BoneKeyFrameUseParameterInterpolation;
				break;
			default:
				_pfnInterpolateBoneKeyFrame = &DrawablePMD::BoneKeyFrameNoInterpolation;
				break;
		};
		_interpolationType = type;
	}
	
	int GetShaderKind() const
	{
		return _shaderKind;
	}
	
	void SetShaderKind(int shaderKind)
	{
		_shaderKind = shaderKind;
	}
		
	void SetWorldMatrix(const D3DXMATRIX& world)
	{
		_transformToWorld = world;
	}

	// メッシュ描画。
	void DrawMesh(MyRenderer& renderer)
	{
		DrawMesh(renderer, _shaderKind);
	}
	
	void DrawMesh(MyRenderer& renderer, int shaderKind)
	{
		ShaderHandle shader = renderer.GetShader(shaderKind);
		
		// スキニング行列をシェーダ定数に設定。
		D3DXMATRIX* pBoneMatrices = static_cast<D3DXMATRIX*>(renderer.GetShaderConstantBuffer(shader, "cbBoneMatrices"));
		memcpy(pBoneMatrices, &_skinningMatrices[0], sizeof(D3DXMATRIX)*_skinningMatrices.size());

		// 表情アニメーション用のオフセット用頂点バッファを更新。
		renderer.UpdateVertexBuffer(_meshOffsetBuffer, &_meshOffsets[0]);

//		if (shaderKind == MyRenderer::PMD_TOON)
//		{
//			renderer.SetShaderTextureArray(shader, "texToonArray", renderer._texToons);
//		}
				
		// 描画計画毎に描画する。
		for (size_t i = 0; i < _drawPlans.size(); ++i)
		{
			DrawPlan& plan = _drawPlans[i];
			
			renderer.SetShader(shader);
			renderer.SetInputLayout(renderer.GetInputLayout(shaderKind));

			D3DXMATRIX& world = *static_cast<D3DXMATRIX*>(renderer.GetShaderGlobalVariable(shader, "world"));
			D3DXMatrixTranspose(&world, &_transformToWorld);

			renderer.SetShaderTexture(shader, "tex", _textures[plan._texture]);
			renderer.SetShaderSamplerState(shader, "samp", _sampler);

			renderer.ApplyShader();

			auto_interface<Device::NativeDevice> pNativeDevice(renderer.GetNativeDevice());
			
			renderer.SetVertexBuffer(0, _meshVertexBuffer);
			renderer.SetVertexBuffer(1, _meshOffsetBuffer);
			renderer.SetIndexBuffer(_indexBuffer);
			pNativeDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			//renderer.SetDefaultRenderTargets();

			renderer.SetRasterizerState(_rasterizerState);
			//renderer.SetDefaultRasterizerState();
			renderer.SetDefaultBlendState();
			renderer.SetDefaultDepthStencilState();
			
			pNativeDevice->DrawIndexed(plan._numIndices, plan._startIndex, 0);
		}
	}
	
	// ボーン描画。
	void DrawBone(MyRenderer& renderer)
	{
		ShaderHandle shader = renderer.GetShader(MyRenderer::PMD_BONE);
		renderer.SetShader(shader);
		renderer.SetInputLayout(renderer.GetInputLayout(MyRenderer::PMD_BONE));

		D3DXMATRIX& world = *static_cast<D3DXMATRIX*>(renderer.GetShaderGlobalVariable(shader, "world"));
		D3DXMatrixTranspose(&world, &_transformToWorld);

		renderer.ApplyShader();

		renderer.UpdateVertexBuffer(_drawBoneVertexBuffer, &_drawBoneVertices[0]);

		auto_interface<Device::NativeDevice> pNativeDevice(renderer.GetNativeDevice());
		
		renderer.SetVertexBuffer(_drawBoneVertexBuffer);
		renderer.SetIndexBuffer(_drawBoneIndexBuffer);
		pNativeDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);

		//renderer.SetDefaultRenderTargets();

		renderer.SetDefaultRasterizerState();
		renderer.SetDefaultBlendState();
		renderer.SetDefaultDepthStencilState();
		
		pNativeDevice->DrawIndexed(_numDrawBoneVertices, 0, 0);
	}
	
private:
	// メッシュ＆テクスチャ描画用。
	VertexBufferHandle _meshVertexBuffer;	//!< メッシュそのものの頂点バッファ。
	VertexBufferHandle _meshOffsetBuffer;	//!< メッシュに対するオフセットを格納した頂点バッファ（表情アニメ用）。
	std::vector<MyRenderer::ShaderInputOffset> _meshOffsets;
	IndexBufferHandle _indexBuffer;
	unsigned int _numIndices;
	std::vector<TextureHandle> _textures;
	
	int _shaderKind;
	RasterizerStateHandle _rasterizerState;
	SamplerStateHandle _sampler;

	// ボーン描画用。
	VertexBufferHandle _drawBoneVertexBuffer;
	IndexBufferHandle _drawBoneIndexBuffer;
	std::vector<MyRenderer::BoneShaderInput> _drawBoneVertices;
	unsigned int _numDrawBoneVertices;
	
	//! ボーン情報。
	struct BoneInfo
	{
		std::string _name;

		D3DXMATRIX _transformModelSpaceToBindBoneSpace;	//!< モデル空間からバインドポーズ時のボーン空間へのトランスフォーム（ボーンオフセット行列）。
		D3DXMATRIX _transformToParentSpace;	//!< 親ボーン空間上の位置へのトランスフォーム。

		D3DXMATRIX _transformInLocalSpace;	//!< ボーン空間上の（モーションによる）トランスフォーム。
		D3DXMATRIX _transformFixInModelSpace;	//!< 確定したモデル空間上のトランスフォーム。

		unsigned short _parent;	//!< 親ボーンのインデックス。
		std::vector<unsigned short> _childs;	//!< 子ボーンのインデックス。
	};
	std::vector<BoneInfo> _bones;
	std::vector<unsigned short> _bonesRoot;	//!< ルートボーンのインデックスリスト。
	
	typedef std::map<std::string, unsigned short> BoneNameIndexMap;
	BoneNameIndexMap _boneNameIndexMap;	//!< ボーン名とインデックスのマッピング。初期化時にしか利用していないが、確認用に。
	
	std::vector<D3DXMATRIX> _skinningMatrices;	//!< スキニング用のマトリクス。頂点をボーン座標系に直してから、ボーンと同じトランスフォームをさせる為の行列。
	
	//! 表情情報。
	std::vector<MMD::PMD::Skin> _skins;
	
	typedef std::map<std::string, unsigned int> SkinNameIndexMap;
	SkinNameIndexMap _skinNameIndexMap;	//!< 表情名とインデックスのマッピング。初期化時にしか利用していないが、確認用に。
	
	unsigned int _skinIndexBase;	//!< 基本情報の格納された表情情報のインデックス。
	
	//! IK情報。
	struct IKInfo
	{
		unsigned short _target;	//!< ターゲットボーンのインデックス。
		unsigned short _effector;	//!< エフェクタボーンのインデックス。
		unsigned short _iterations;
		float _controlWeight;	//!< 回転角度制限っぽい？
		
		struct ChainInfo
		{
			unsigned short _index;
			enum FLAGS
			{
				ROTATE_LIMIT_X = 0x01,
				ROTATE_LIMIT_Y = 0x02,
				ROTATE_LIMIT_Z = 0x04,
			};
			Flags _flags;
			struct Limit
			{
				float _min;
				float _max;
			};
			Limit _rotateLimitX;
			Limit _rotateLimitY;
			Limit _rotateLimitZ;
		};
		std::vector<ChainInfo> _chains;	//!< チェインボーンリスト。
	};
	std::vector<IKInfo> _iks;
	
	//! モーション情報。
	struct BoneKeyFrame
	{
		unsigned int _frame;
		float _time;
		D3DXVECTOR3 _translation;
		D3DXQUATERNION _rotation;

		struct Interpolation
		{
			D3DXVECTOR2 p1;	//!< ３次ベジェ曲線の制御点 p1。
			D3DXVECTOR2 p2;	//!< ３次ベジェ曲線の制御点 p2。
			
			D3DXVECTOR2 Bezier(float t) const
			{
				#define SQ(x) ((x)*(x))
				#define CB(x) ((x)*(x)*(x))
				#define CALC(p0, p1, p2, p3, t)	(CB(1.f-t)*p0 + 3.f*SQ(1.f-t)*t*p1 + 3.f*(1.f-t)*SQ(t)*p2 + CB(t)*p3)
				D3DXVECTOR2 p0(0.f, 0.f);
				D3DXVECTOR2 p3(127.f, 127.f);
				return 
					D3DXVECTOR2(
						CALC(p0.x, p1.x, p2.x, p3.x, t) / 127.f,
						CALC(p0.y, p1.y, p2.y, p3.y, t) / 127.f
						);
			}
		};

		Interpolation _interpolation_x;
		Interpolation _interpolation_y;
		Interpolation _interpolation_z;
		Interpolation _interpolation_r;
	};
	
	typedef std::map<unsigned int, BoneKeyFrame> BoneKeyFrames;
	std::vector<BoneKeyFrames> _allBoneKeyFrames;
	
	struct SkinKeyFrame
	{
		unsigned int _frame;
		float _time;
		float _weight;
	};

	typedef std::map<unsigned int, SkinKeyFrame> SkinKeyFrames;
	std::vector<SkinKeyFrames> _allSkinKeyFrames;

	unsigned int _keyFrameMax;
	
	float _motionTime;	//!< モーションの現在時間。

	INTERPOLATION_TYPE _interpolationType;
	void (DrawablePMD::*_pfnInterpolateBoneKeyFrame)(const BoneKeyFrame&, const BoneKeyFrame&, float, D3DXMATRIX&);

	//! モデルのワールドトランスフォーム。
	D3DXMATRIX _transformToWorld;
	
	//! 描画計画。同一のシェーダとテクスチャで描画出来てしまうインデックスを全てまとめておき、出来る限り最小の回数で描画出来る様にする。
	struct DrawPlan
	{
		unsigned int _startIndex;
		unsigned int _numIndices;
		int _texture;
	};
	std::vector<DrawPlan> _drawPlans;
	
	//! フラグ。
	enum FLAGS
	{
		MOTION_LOOP	= 0x0001,
	};
	Flags _flags;


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
	
	struct Camera
	{
		D3DXVECTOR3 _eye;
		D3DXVECTOR3 _at;
		D3DXVECTOR3 _up;
		
		Camera()
			: _eye(0.0f, 12.0f, -35.0f)
			, _at(_eye + D3DXVECTOR3(0.f, -0.5f, 5.f))
			, _up(0.0f, 1.0f, 0.f)
		{
		}
		
		void GetViewMatrix(D3DXMATRIX& view) const
		{
			D3DXMatrixLookAtLH(&view, &_eye, &_at, &_up);
		}
		
		void GetEyeVector(D3DXVECTOR3& eye) const
		{
			eye = _at - _eye;
			D3DXVec3Normalize(&eye, &eye);
		}
	};
	Camera _camera;

	struct Light
	{
		//D3DXVECTOR3 _position;
		D3DXVECTOR3 _direction;
		
		Light()
		{
			//_position = D3DXVECTOR3(-5.f, 10.f, -5.f);
			//_direction = D3DXVECTOR3(1.f, 0.f, 1.f);
			//_direction = D3DXVECTOR3(1.f, -1.f, 1.f);
			_direction = D3DXVECTOR3(-0.5f, -1.f, 0.5f);
			D3DXVec3Normalize(&_direction, &_direction);
		}
		
		void GetShaderLight(MyRenderer::ShaderLight& light, const Camera& camera)
		{
			Color::GRAY.ToFloat3(light.diffuse);
			Color::WHITE.ToFloat3(light.ambient);
			D3DXMATRIX view;
			camera.GetViewMatrix(view);
			//D3DXVec3TransformCoord(&light.positionInViewSpace, &_position, &view);
			D3DXVec3TransformNormal(&light.directionInViewSpace, &_direction, &view);
		}
	};
	Light _light;
	
	
	DrawablePMD _pmd;
	Timer _motionTimer;

	Impl()
	{
		_flags.set(INVALID_BONE_DRAW);
		//_flags.set(INVALID_MESH_DRAW);
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

		D3DXMatrixPerspectiveFovLH(&mProj,
				(float)D3DXToRadian(45.0f),
			static_cast<float>(sz.cx) / static_cast<float>(sz.cy), // アスペクト比
				1.0f,							// 前方投影面までの距離
				1000.0f);							// 後方投影面までの距離

		_renderer.SetShaderProjection(mProj);
	}
	
	bool CreateModel()
	{
//		if (failed(_pmd.Create(_device, _T("..\\Rakugaki_MikuMikuDance_dat\\桃音ピク\\桃音ピク（仮）.pmd"), _T("..\\Rakugaki_MikuMikuDance_dat\\きしめん.vmd"))))
//		if (failed(_pmd.Create(_device, _T("..\\Rakugaki_MikuMikuDance_dat\\初音ミクmetal.pmd"), _T("..\\Rakugaki_MikuMikuDance_dat\\きしめん.vmd"))))
//		if (failed(_pmd.Create(_device, _T("..\\Rakugaki_MikuMikuDance_dat\\初音ミク+.pmd"), _T("..\\Rakugaki_MikuMikuDance_dat\\【MMD】みくみくにしてあげる♪【してやんよ】【Project DIVA】.vmd"))))
//		if (failed(_pmd.Create(_device, _T("..\\Rakugaki_MikuMikuDance_dat\\初音ミク+.pmd"), _T("..\\Rakugaki_MikuMikuDance_dat\\きしめん.vmd"))))
		if (failed(_pmd.Create(_device, _T("..\\Rakugaki_MikuMikuDance_dat\\初音ミク.pmd"), _T("..\\Rakugaki_MikuMikuDance_dat\\きしめん.vmd"))))
//		if (failed(_pmd.Create(_device, _T("..\\Rakugaki_MikuMikuDance_dat\\初音ミク.pmd"), _T("..\\Rakugaki_MikuMikuDance_dat\\miku_test6.vmd"))))
//		if (failed(_pmd.Create(_device, _T("..\\Rakugaki_MikuMikuDance_dat\\初音ミク.pmd"), _T("..\\Rakugaki_MikuMikuDance_dat\\miku_test5.vmd"))))
//		if (failed(_pmd.Create(_device, _T("..\\Rakugaki_MikuMikuDance_dat\\初音ミク.pmd"), _T("..\\Rakugaki_MikuMikuDance_dat\\miku_test4.vmd"))))
//		if (failed(_pmd.Create(_device, _T("..\\Rakugaki_MikuMikuDance_dat\\初音ミク.pmd"), _T("..\\Rakugaki_MikuMikuDance_dat\\miku_test3.vmd"))))
//		if (failed(_pmd.Create(_device, _T("..\\Rakugaki_MikuMikuDance_dat\\初音ミク.pmd"), _T("..\\Rakugaki_MikuMikuDance_dat\\miku_test2.vmd"))))
//		if (failed(_pmd.Create(_device, _T("..\\Rakugaki_MikuMikuDance_dat\\初音ミク.pmd"), _T("..\\Rakugaki_MikuMikuDance_dat\\miku_test.vmd"))))
//		if (failed(_pmd.Create(_device, _T("..\\Rakugaki_MikuMikuDance_dat\\ピクピク\\ピクピク.pmd"), _T("..\\Rakugaki_MikuMikuDance_dat\\【MMD】みくみくにしてあげる♪【してやんよ】【Project DIVA】.vmd"))))
//		if (failed(_pmd.Create(_device, _T("..\\Rakugaki_MikuMikuDance_dat\\ピクピク\\ピクピク.pmd"), _T("..\\Rakugaki_MikuMikuDance_dat\\きしめん.vmd"))))
//		if (failed(_pmd.Create(_device, _T("..\\Rakugaki_MikuMikuDance_dat\\ピクピク\\ピクピク.pmd"), _T("..\\Rakugaki_MikuMikuDance_dat\\miku_test2.vmd"))))
			return false;
		//_pmd.SetShaderKind(MyRenderer::PMD_NONE);
		//_pmd.SetShaderKind(MyRenderer::PMD_FLAT);
		_motionTimer.Start();
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
			// シェーダーにカメラ情報＆ライトを設定。
			{
				D3DXMATRIX view;
				_camera.GetViewMatrix(view);
				D3DXVECTOR3 eye;
				_camera.GetEyeVector(eye);
				_renderer.SetShaderViewAndEye(view, eye);
				
//				float angle = _frameRateController.GetElapsedTime() * 0.25f;
//				D3DXMatrixRotationY(&m, angle);
//				D3DXVec3TransformNormal(&_light._direction, &_light._direction, &m);
				
				MyRenderer::ShaderLight light;
				_light.GetShaderLight(light, _camera);
				_renderer.SetShaderLight(light);
			}

			// モデル描画。
			{
				// モーション更新。
				_pmd.SetMotion(_motionTimer.GetTime());
				//_pmd.SetMotion(0.0f);
				//_pmd.SetMotion(3.6f);
				_pmd.Update();
				
				// ワールド行列を生成して描画。
				D3DXMATRIX world;
				//D3DXMatrixIdentity(&world);
				static float angle = 0.f;
				//angle += _frameRateController.GetElapsedTime() * 0.5f;
				D3DXMatrixRotationY(&world, angle);
				//D3DXMatrixTranslation(&world, -1.f, 0.f, 0.f);
				_pmd.SetWorldMatrix(world);
				
				if (!_flags.is(INVALID_MESH_DRAW))
				{
					_renderer.SetDefaultRenderTargets();
					_renderer.Clear(true, true, false, Color::BLUE);
					_pmd.DrawMesh(_renderer);
				}

				if (!_flags.is(INVALID_BONE_DRAW))
				{
					_renderer.SetDefaultRenderTargets();
					_pmd.DrawBone(_renderer);
				}
			}
			
			// 情報表示。
			int y = 0, h = _device.GetFontHeight(_renderer.GetDefaultFont());

			_renderer.DrawText(0, y, Color::GREEN, _T("FPS=%.2f (%.2fms)"), 
				_frameRateController.GetCurrentFPS(),
				_frameRateController.GetElapsedTime() * 1000.f
				);
			y += h;

			_renderer.DrawText(0, y, Color::WHITE, _T("Frame=%u / Time=%.2f"), _pmd.GetMotionKeyFrame(), _pmd.GetMotionTime());
			y += h;
			_renderer.DrawText(0, y, Color::WHITE, _T("[F2] メッシュ描画=") TFMT_BOOL, format_bool(!_flags.is(INVALID_MESH_DRAW)));
			y += h;
			_renderer.DrawText(0, y, Color::WHITE, _T("[F3] ボーン描画=") TFMT_BOOL, format_bool(!_flags.is(INVALID_BONE_DRAW)));
			y += h;
			_renderer.DrawText(0, y, Color::WHITE, _T("[F4] モーションリスタート"));
			y += h;
			_renderer.DrawText(0, y, Color::WHITE, _T("[F5] モーションループ=") TFMT_BOOL, format_bool(_pmd.GetMotionLoop()));
			y += h;

			const TCHAR* pInterpolationMethod = _T("無し");
			DrawablePMD::INTERPOLATION_TYPE interpolationType = _pmd.GetInterpolationType();
			switch (interpolationType)
			{
				case DrawablePMD::INTERPOLATION_LINEAR: pInterpolationMethod = _T("線形補間"); break;
				case DrawablePMD::INTERPOLATION_USE_PARAMETER: pInterpolationMethod = _T("パラメータ使用（ベジェ曲線＋線形補間）"); break;
			}
			_renderer.DrawText(0, y, Color::WHITE, _T("[F6] モーション補間=%s"), pInterpolationMethod);
			y += h;

			const TCHAR* pShaderKind = _T("ライティング無し");
			switch (_pmd.GetShaderKind())
			{
				case MyRenderer::PMD_FLAT: pShaderKind = _T("フラットシェーディング"); break;
				case MyRenderer::PMD_GOURAUD: pShaderKind = _T("グーローシェーディング"); break;
				case MyRenderer::PMD_PHONG: pShaderKind = _T("フォンシェーディング"); break;
				case MyRenderer::PMD_TOON: pShaderKind = _T("トゥーンシェーディング"); break;
			}
			_renderer.DrawText(0, y, Color::WHITE, _T("[F7] シェーダ=%s"), pShaderKind);
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
		switch (params.GetVirtKey())
		{
		case VK_F2:
			_flags = _flags.not(INVALID_MESH_DRAW);
			break;
		case VK_F3:
			_flags = _flags.not(INVALID_BONE_DRAW);
			break;
		case VK_F4:
			_motionTimer.Reset();
			_motionTimer.Start();
			break;
		case VK_F5:
			_pmd.SetMotionLoop(!_pmd.GetMotionLoop());
			break;
		case VK_F6:
			_pmd.SetInterpolationType(static_cast<DrawablePMD::INTERPOLATION_TYPE>(cursor_forward_rot(_pmd.GetInterpolationType(), 1, DrawablePMD::INTERPOLATION_TYPE_MAX)));
			break;
		case VK_F7:
			{
				int next = static_cast<int>(cursor_forward_rot(_pmd.GetShaderKind(), 1, MyRenderer::PMD_MESH_MAX));
				_pmd.SetShaderKind(next);
			}
			break;
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

