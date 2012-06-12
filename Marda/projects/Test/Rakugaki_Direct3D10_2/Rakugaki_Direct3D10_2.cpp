// Rakugaki_Direct3D10_2.cpp : �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "Rakugaki_Direct3D10_2.h"

#define MAX_LOADSTRING 100

// �O���[�o���ϐ�:
HINSTANCE hInst;								// ���݂̃C���^�[�t�F�C�X
TCHAR szTitle[MAX_LOADSTRING];					// �^�C�g�� �o�[�̃e�L�X�g
TCHAR szWindowClass[MAX_LOADSTRING];			// ���C�� �E�B���h�E �N���X��

ApplicationFrame g_app;

// ���̃R�[�h ���W���[���Ɋ܂܂��֐��̐錾��]�����܂�:
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

 	// TODO: �����ɃR�[�h��}�����Ă��������B
	MSG msg;
	HACCEL hAccelTable;

	// �O���[�o������������������Ă��܂��B
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_RAKUGAKI_DIRECT3D10_2, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// �A�v���P�[�V�����̏����������s���܂�:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RAKUGAKI_DIRECT3D10_2));

	// ���C�����[�v�B
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
//  �֐�: MyRegisterClass()
//
//  �ړI: �E�B���h�E �N���X��o�^���܂��B
//
//  �R�����g:
//
//    ���̊֐�����юg�����́A'RegisterClassEx' �֐����ǉ����ꂽ
//    Windows 95 ���O�� Win32 �V�X�e���ƌ݊�������ꍇ�ɂ̂ݕK�v�ł��B
//    �A�v���P�[�V�������A�֘A�t����ꂽ
//    �������`���̏������A�C�R�����擾�ł���悤�ɂ���ɂ́A
//    ���̊֐����Ăяo���Ă��������B
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
//   �֐�: InitInstance(HINSTANCE, int)
//
//   �ړI: �C���X�^���X �n���h����ۑ����āA���C�� �E�B���h�E���쐬���܂��B
//
//   �R�����g:
//
//        ���̊֐��ŁA�O���[�o���ϐ��ŃC���X�^���X �n���h����ۑ����A
//        ���C�� �v���O���� �E�B���h�E���쐬����ѕ\�����܂��B
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // �O���[�o���ϐ��ɃC���X�^���X�������i�[���܂��B

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
//  �֐�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  �ړI:  ���C�� �E�B���h�E�̃��b�Z�[�W���������܂��B
//
//  WM_COMMAND	- �A�v���P�[�V���� ���j���[�̏���
//  WM_PAINT	- ���C�� �E�B���h�E�̕`��
//  WM_DESTROY	- ���~���b�Z�[�W��\�����Ė߂�
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
		// �I�����ꂽ���j���[�̉��:
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
		// TODO: �`��R�[�h�������ɒǉ����Ă�������...
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

// �o�[�W�������{�b�N�X�̃��b�Z�[�W �n���h���[�ł��B
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

// �A�v���P�[�V�����t���[���N���X�̎������B
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
		D3DXVECTOR3 Position;   // ���W�l
		D3DXVECTOR3 Color;     // �F
	};

	// �萔�o�b�t�@�̃f�[�^��`�@�@�ύX���Ȃ��f�[�^
	struct cbNeverChanges {
		D3DXMATRIX Projection;   // �����ϊ��s��
	};

	// �萔�o�b�t�@�̃f�[�^��`�A�@�ύX�p�x�̒Ⴂ�f�[�^
	struct cbChangesEveryFrame {
		D3DXMATRIX  View;  // �r���[�ϊ��s��
		D3DXVECTOR3 Light; // �������W(���[���h���W�n)
		FLOAT       dummy; // �_�~�[
	};

	// �萔�o�b�t�@�̃f�[�^��`�B�@�ύX�p�x�̍����f�[�^
	struct cbChangesEveryObject {
		D3DXMATRIX World;      // ���[���h�ϊ��s��
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

	// �I�������B
	void Release()
	{
		safe_release(_pTextureSRV);
		safe_release(_pSprite);

		_renderer.Release();
		_device.Release();
	}
	
	// �������B
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
		//D3DXVECTOR3 vEye(0.0f, 0.0f,  3.0f);	// ���_(�J�����̈ʒu)
		D3DXVECTOR3 vEye(0.0f, 0.0f,  -3.0f);	// ���_(�J�����̈ʒu)
		D3DXVECTOR3 vAt(0.0f, 0.0f,  0.0f);	// �����_
		D3DXVECTOR3 vUp(0.0f, 1.0f,  0.0f);		// �J�����̏����

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
		
		// ���_�o�b�t�@�B
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
			
		// �C���f�b�N�X�o�b�t�@�B
		UINT initialIndexDatas[] = {
			0,1,3,  1,2,3,  1,5,2,  5,6,2,  5,4,6,  4,7,6,
			4,5,0,  5,1,0,  4,0,7,  0,3,7,  3,2,7,  2,6,7
		};
			
		_indices = _device.CreateIndexBuffer(D3D10_USAGE_IMMUTABLE, 0, sizeof(UINT), _countof(initialIndexDatas), initialIndexDatas);
		if (_indices == IndexBufferHandle::INVALID)
			return false;

		// ���̓��C�A�E�g�E�I�u�W�F�N�g�̍쐬
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
		// �萔�o�b�t�@�@���X�V
		cbNeverChanges* pCBuf = static_cast<cbNeverChanges*>(_renderer.GetShaderConstantBuffer(_shader, (unsigned int)0));
		D3DXMatrixPerspectiveFovLH(&pCBuf->Projection,
				(float)D3DXToRadian(30.0f),		// ����p30��
			static_cast<float>(sz.cx) / static_cast<float>(sz.cy), // �A�X�y�N�g��
				1.0f,							// �O�����e�ʂ܂ł̋���
				20.0f);							// ������e�ʂ܂ł̋���
		D3DXMatrixTranspose(&pCBuf->Projection, &pCBuf->Projection);

		// �X�v���C�g�̃v���W�F�N�V�����g�����X�t�H�[�����X�V�B
		D3DXMATRIX m;
		D3DXMatrixPerspectiveFovLH(
			&m,
			(float)D3DXToRadian(60.0f),		// ����p60��
			static_cast<float>(sz.cx) / static_cast<float>(sz.cy), // �A�X�y�N�g��
			1.0f,							// �O�����e�ʂ܂ł̋���
			5.0f);							// ������e�ʂ܂ł̋���
		_pSprite->SetProjectionTransform(&m);
	}
	
	// �t���[���X�V�B
	void UpdateFrame()
	{
		_flags.reset(FRAME_SKIP);
//		if (_frameRateController.GetElapsedTime() > _frameRateController.GetFrameTime() * 2)
//		{
//			// TODO: ���܂�ɂ����e���ȏ����c�����ƍl����B
//			_flags.set(FRAME_SKIP);
//		}

		Draw();
		
		_frameRateController.ChangeFrame();
	}
	
	// �`��B
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

		// �����̂̕`��

		// �萔�o�b�t�@�A���X�V
		cbChangesEveryFrame* pCBufEveryFrame = static_cast<cbChangesEveryFrame*>(_renderer.GetShaderConstantBuffer(_shader, 1));
		// �r���[�ϊ��s��
		D3DXVECTOR3 eye = D3DXVECTOR3(0.0f, 5.0f, -5.0f);  // ���_(�J�����̈ʒu)
		D3DXVECTOR3 at = D3DXVECTOR3(0.0f, 0.0f,  0.0f);  // �����_
		D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f,  0.0f); // �J�����̏����

		D3DXMatrixLookAtLH(&pCBufEveryFrame->View, &eye, &at, &up);
		D3DXMatrixTranspose(&pCBufEveryFrame->View, &pCBufEveryFrame->View);
		// �_�������W
		D3DXVECTOR3   vLightPos(3.0f, 3.0f, -3.0f);   // �������W(���[���h���W�n)
		D3DXVec3TransformCoord(&pCBufEveryFrame->Light, &vLightPos, &pCBufEveryFrame->View);

		// �萔�o�b�t�@�B���X�V
		cbChangesEveryObject* pCBufEveryObject = static_cast<cbChangesEveryObject*>(_renderer.GetShaderConstantBuffer(_shader, 2));
		// ���[���h�ϊ��s��
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
		// IA�ɒ��_�o�b�t�@��ݒ�
		_renderer.SetVertexBuffer(_vertices);
		// IA�ɃC���f�b�N�X�E�o�b�t�@��ݒ�
		_renderer.SetIndexBuffer(_indices);
		// IA�ɓ��̓��C�A�E�g�E�I�u�W�F�N�g��ݒ�
		_renderer.SetInputLayout(_inputLayout);
		// IA�Ƀv���~�e�B�u�̎�ނ�ݒ�
		pNativeDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// �V�F�[�_���f�o�C�X�ɓK�p�B
		_renderer.SetShader(_shader);
		_renderer.ApplyShader();

		_renderer.SetDefaultRenderTargets();

		_renderer.SetDefaultRasterizerState();
		_renderer.SetDefaultBlendState();
		_renderer.SetDefaultDepthStencilState();

		// ***************************************
		// ���_�o�b�t�@�̃f�[�^���C���f�b�N�X�E�o�b�t�@���g���ĕ`�悷��
		pNativeDevice->DrawIndexed(
				36, // �`�悷��C���f�b�N�X��(���_��)
				0,  // �C���f�b�N�X�E�o�b�t�@�̍ŏ��̃C���f�b�N�X����`��J�n
				0); // ���_�o�b�t�@���̍ŏ��̒��_�f�[�^����g�p�J�n
	}

	// �E�B���h�E���T�C�Y�B
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
		
		// �Ȃ񂩁��̏������Ȃ��Ă���肭�ł��Ă�悤�Ɍ������ł����ǁc�B
		if (failed(_device.ResizeBackBuffer(sz)))
		{
			Exit(0);
			return 0;
		}
		
		// �ˉe�g�����X�t�H�[�������Z�b�g�B
		ResetProjectionTransform(sz);
		return 0;
	}
	
	// �L�[��������ė����ꂽ�B
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

// �A�v���P�[�V�����t���[���N���X�̓]�����B
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
