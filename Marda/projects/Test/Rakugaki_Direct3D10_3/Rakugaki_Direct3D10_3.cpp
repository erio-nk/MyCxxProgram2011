// Rakugaki_Direct3D10_3.cpp : �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "Rakugaki_Direct3D10_3.h"

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
                     LPTSTR    lpCmdTriangle,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdTriangle);

 	// TODO: �����ɃR�[�h��}�����Ă��������B
	MSG msg;
	HACCEL hAccelTable;

	// �O���[�o������������������Ă��܂��B
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_RAKUGAKI_DIRECT3D10_3, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// �A�v���P�[�V�����̏����������s���܂�:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RAKUGAKI_DIRECT3D10_3));

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RAKUGAKI_DIRECT3D10_3));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_RAKUGAKI_DIRECT3D10_3);
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
	
	// �I�������B
	void Release()
	{
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
				(float)D3DXToRadian(30.0f),		// ����p30��
			static_cast<float>(sz.cx) / static_cast<float>(sz.cy), // �A�X�y�N�g��
				1.0f,							// �O�����e�ʂ܂ł̋���
				100.0f);							// ������e�ʂ܂ł̋���

		D3DXMatrixLookAtLH(&mView,
			&D3DXVECTOR3(0.0f, 5.0f, -5.0f),  // ���_(�J�����̈ʒu)
			&D3DXVECTOR3(0.0f, 0.0f,  0.0f),  // �����_
			&D3DXVECTOR3(0.0f, 1.0f,  0.0f)); // �J�����̏����
			
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
