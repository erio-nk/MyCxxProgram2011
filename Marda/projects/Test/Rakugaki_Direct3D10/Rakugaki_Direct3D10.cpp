// Rakugaki_Direct3D10.cpp : �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "Rakugaki_Direct3D10.h"
	
using namespace Marda;

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
	LoadString(hInstance, IDC_RAKUGAKI_DIRECT3D10, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// �A�v���P�[�V�����̏����������s���܂�:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RAKUGAKI_DIRECT3D10));

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RAKUGAKI_DIRECT3D10));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_RAKUGAKI_DIRECT3D10);
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

   if (failed(g_app.Create(hWnd)))
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

#define SAFE_RELEASE(iface) if (is_not_null(iface)) { iface->Release(); iface = NULL; }

#define FONT_HEIGHT	16

// �A�v���P�[�V�����t���[���N���X�̎������B
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

	// �������B
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
	
	// Direct3D�������B
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

		// �o�b�N�o�b�t�@�̃����_�[�^�[�Q�b�g�r���[�B
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

		// �[�x�X�e���V���o�b�t�@�B
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

		// �[�x�X�e���V���r���[�B
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
		
		// �[�x�X�e���V���X�e�[�g�B
		// �S�ăf�t�H���g�l�B
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
		//D3DXVECTOR3 vEye(0.0f, 0.0f,  3.0f);	// ���_(�J�����̈ʒu)
		D3DXVECTOR3 vEye(0.0f, 0.0f,  -3.0f);	// ���_(�J�����̈ʒu)
		D3DXVECTOR3 vAt(0.0f, 0.0f,  0.0f);	// �����_
		D3DXVECTOR3 vUp(0.0f, 1.0f,  0.0f);		// �J�����̏����

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
			2.0f * static_cast<float>(sz.cx) / static_cast<float>(sz.cy), // ��
			2.0f,	// ����
			0.0f,	// �O�����e�ʂ܂ł̋���
			5.0f	// ������e�ʂ܂ł̋���
			);
#else
		D3DXMatrixPerspectiveFovLH(
			&m,
			(float)D3DXToRadian(60.0f),		// ����p60��
			static_cast<float>(sz.cx) / static_cast<float>(sz.cy), // �A�X�y�N�g��
			1.0f,							// �O�����e�ʂ܂ł̋���
			5.0f);							// ������e�ʂ܂ł̋���
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
			FONT_HEIGHT,			// ����
			0,			// ���ϕ�����
			FW_NORMAL,	// ����(�W��)
			1,			// �~�b�v�}�b�v���x��
			FALSE,		// �Α�(�w�肵�Ȃ�)
			DEFAULT_CHARSET,				// �L�����N�^�E�Z�b�g(�f�t�H���g)
			OUT_DEFAULT_PRECIS,				// �o�͐��x
			DEFAULT_QUALITY,				// �i��
			DEFAULT_PITCH || FF_DONTCARE,	// �s�b�`
			_T("Arial"),					// �t�H���g��
			&_pFont);
		if (FAILED(hRet))
		{
			return false;
		}
		
		return true;
	}
	
	// �I�������B
	void Release()
	{
		ReleaseDirect3D();
	}
	
	// Direct3D�I�������B
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
				1.f,	// �[�x�o�b�t�@�̃N���A�l�B1.f ����ԉ����l�B
				0		// �X�e���V���o�b�t�@�̃N���A�l�B
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

		// Present �̌��ʔ���B
		switch (hRet)
		{
			case S_OK:
				_flags.reset(SCREEN_OCCLUDED);
				break;
			case DXGI_STATUS_OCCLUDED:
				// �\�����������݂��Ȃ��̂ŕ`��𖳌��ɂ���B
				_flags.set(SCREEN_OCCLUDED);
				break;
			case DXGI_ERROR_DEVICE_HUNG:
			case DXGI_ERROR_DEVICE_RESET:
				// �f�o�C�X���X�g�B
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
//			D3DXMatrixRotationY(&mRot, angle);	// �X�v���C�g���w�ʃJ�����O�����B�ݒ�H
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
	
//	// �e�N�X�`���̃I���W�i���̃T�C�Y��ŃX�N���[���ɕ\�������l�ɂ���B
//	// ���I���W�i���̃T�C�Y��ŕ\������Ȃ��̂̓X�v���C�g�Ƀr���[�g�����X�t�H�[���Ǝˉe�g�����X�t�H�[����ݒ肵�ĂȂ����������������X�B
//	// �@���J�������W�̐ݒ肪����̂ŁA�J�����𒲐����Ȃ��ƃI���W�i���T�C�Y�ŕ\������Ȃ��B���̒����͂߂�ǂ������E�E�E���_�O�����_�O����_���������B
//	// �@�@���ł��ݒ肵�Ă�ˉe�g�����X�t�H�[���͐��ˉe�Ȃ񂾂��c�B
//	D3DXVECTOR3 GetSpriteAdjustScale(D3D10_TEXTURE2D_DESC& desc)
//	{
//		SIZE sz;
//		_window.GetClientSize(sz);
//
//		D3DXVECTOR3 ret;
//		ret.x = static_cast<float>(desc.Width) / sz.cx * 2.f;	// 2�{���Ă���̂́A�X�N���[�����W�n�̉�����2(���[-1.0,�E�[1.0)�ƂȂ��Ă���ׂł���B
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
		
		// �R�����Ăяo���ƁA�ǂ��������Ńt�H���g�`��p�̐[�x�X�e���V���X�e�[�g���ݒ肳���l�ł���B
		// �[�x�X�e���V���o�b�t�@�ƃr���[�̍쐬�Ɛݒ�����āA�t�H���g�`������Ȃ���΁AOMGetDepthStencilState �ł�
		// �������� �[�x�X�e���V���X�e�[�g�C���^�[�t�F�[�X���擾�o�����A�f�t�H���g�ݒ�ɂȂ��Ă���i�[�x�o�b�t�@�L���j�B
		// �t�H���g��`�悵���r�[�ɐ[�x�X�e���V���X�e�[�g�C���^�[�t�F�[�X���擾�ł���悤�ɂȂ邪�A
		// D3D10_DEPTH_STENCIL_DESC::DepthEnable �� FALSE �ł���B
		// �܂�ADepthStencilState �����O�ō��A�t�H���g�`��̌�i�����f����X�v���C�g�̕`��O�j�ɂ�
		// ���̃X�e�[�g��ݒ肵�Ă��Ȃ���΂Ȃ�Ȃ��B
		result = _pFont->DrawText(
			NULL,
			buf,
			-1,
			&rc,
			DT_LEFT | DT_NOCLIP,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
			);
	}

	// �E�B���h�E���T�C�Y�B
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
			// �Ȃ񂩁��̏������Ȃ��Ă���肭�ł��Ă�悤�Ɍ������ł����ǁc�B
			// �`��^�[�Q�b�g����������B
			_pGraphicsDevice->OMSetRenderTargets(0, NULL, NULL);	// �`��^�[�Q�b�g�̉����B
			SAFE_RELEASE(_pRenderTargetView);	// �`��^�[�Q�b�g �r���[�̉���B
			SAFE_RELEASE(_pDepthStencilView);
			SAFE_RELEASE(_pDepthStencil);

			// �o�b�t�@�̕ύX�B
			_pSwapChain->ResizeBuffers(1, 0, 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
			
			// �o�b�N�o�b�t�@�̍č쐬�B
			if (failed(CreateBackBuffer(sz)))
			{
				Exit(0);
				return 0;
			}
		}
		
		// �X�v���C�g�̎ˉe�g�����X�t�H�[�������Z�b�g�B
		if (failed(SetSpriteProjectionTransform(sz)))
		{
			Exit(0);
			return 0;
		}
		
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
