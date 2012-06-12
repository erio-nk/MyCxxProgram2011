/**
	@file MyApp.cpp

	@author Dila
 */

#include "MyApp.h"

MyApp MyApp::theInstance;

namespace
{
	// �G�t�F�N�g�i�V�F�[�_�[�j�t�@�C����
	const TCHAR	EFFECT_FILE_NAME[] = _T("MyShader.fx");

	// �V�F�[�_�[���f��
	const char SHADER_MODEL[] = "fx_4_0";

	// �G�t�F�N�g�̃e�N�j�b�N��
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
	// �E�B���h�E������
	_wnd.Create(
		_T("Direct3D10 Render the teapot"),
		0, 0, SCREEN_W, SCREEN_H);
	_wnd.Show();

	// �t���[�����[�g�ݒ�
	_fps.SetFrameRate(30);

	// �^�C�}�[�J�n
	_timer.Start();


	//// Direct3D10 ������

	// �X���b�v�`�F�C���̐ݒ�
	DXGI_SWAP_CHAIN_DESC sc;
	ZeroMemory(&sc, sizeof(sc));
	sc.BufferCount = 1;
	sc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sc.OutputWindow = _wnd.GetHandle();
	sc.SampleDesc.Count = 1;
	sc.Windowed = TRUE;

	// �f�o�C�X�ƃX���b�v�`�F�C���̍쐬�i���Ńf�o�C�X�쐬�ɂȂ����݂����j
	HRESULT hr;
	hr = D3D10CreateDeviceAndSwapChain(	
		NULL,
		D3D10_DRIVER_TYPE_HARDWARE,	// �n�[�h�E�F�A�ŏ���
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

	// �o�b�N�o�b�t�@(�`��^�[�Q�b�g)�̃r���[���쐬
	ID3D10Texture2D* pBuffer;
	hr = _pSwapChain->GetBuffer(	// �o�b�N�o�b�t�@���擾����
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

	RELEASE(pBuffer);	// Get�����C���^�t�F�[�X�͎Q�ƃJ�E���g�����炵�Ă���

	// �[�x/�X�e���V���o�b�t�@���쐬
	D3D10_TEXTURE2D_DESC td;
	td.Width = SCREEN_W;
	td.Height = SCREEN_H;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_D32_FLOAT;	// 32bit float
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.Usage = D3D10_USAGE_DEFAULT;
	td.BindFlags = D3D10_BIND_DEPTH_STENCIL;	// �[�x/�X�e���V���Ƃ��Ďg�p
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;

	hr = _pDevice->CreateTexture2D(&td, NULL, &pBuffer);
	if (FAILED(hr)) {
		assert(0);
		return false;
	}

	// �[�x/�X�e���V���o�b�t�@�̃r���[���쐬
	D3D10_DEPTH_STENCIL_VIEW_DESC dsd;
	dsd.Format = td.Format;	// �o�b�t�@�̕��Ɠ����t�H�[�}�b�g
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

	RELEASE(pBuffer);	// Get��������

	// �o�b�N�o�b�t�@�̃r���[�Ɛ[�x/�X�e���V���o�b�t�@�̃r���[���f�o�C�X�ɐݒ肷��
    _pDevice->OMSetRenderTargets(1, &_pBackBuf, _pZBuf);


	// �r���[�|�[�g�̐ݒ�
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

	// ���b�V���쐬
	hr = DXUTCreateTeapot(_pDevice, &_pMesh);
	if (FAILED(hr)) {
		assert(0);
		return false;
	}

	// �G�t�F�N�g�̓ǂݍ���
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
	
	// ���_�f�[�^�̃��C�A�E�g��`�iFVF�ɑ�����݂̂����H�j
	// �@���_�o�b�t�@���Ŏg���A�V�F�[�_�ł̓Z�}���e�B�b�N(POSITION,COLOR)�Ɏg���邻���ȁB
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

    _pDevice->IASetInputLayout(_pLayout);	// �f�o�C�X�ɐݒ肷��

	//RELEASE(pTechnique);	// �e�N�j�b�N��Release�͗v��Ȃ��炵��

	return true;
}


/**
 	MyApp::Finalize()
 */
bool MyApp::Finalize()
{
	// ���Ԃǂ��������̂���
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
	// �V�[���N���A
	float color[] = { 0.0f, 0.0f, 0.5f, 1.0f };
	_pDevice->ClearRenderTargetView(_pBackBuf, color);	// �o�b�N�o�b�t�@�̃N���A
	_pDevice->ClearDepthStencilView(_pZBuf, D3D10_CLEAR_DEPTH, 1.0f, 0);
													// �[�x/�X�e���V���o�b�t�@�̃N���A

	// �V�[���`��
	{
		// �V�F�[�_�Ƀg�����X�t�H�[���̃p�����[�^��ݒ肷��
		D3DXMATRIX mWorld;	// ���[���h�ϊ��s��
		//D3DXMatrixIdentity(&mWorld);
		D3DXMatrixRotationY(&mWorld, D3DXToRadian(_timer.GetTime()*20));
		_pEffect->GetVariableByName("mWorld")->AsMatrix()->SetMatrix((float*)&mWorld);

		D3DXMATRIX mView;	// �r���[�ϊ��s��
		D3DXVECTOR3 Eye(0.0f, 2.0f, -4.0f);	// �J�������W
		D3DXVECTOR3 At(0.0f, 0.0f, 0.0f);	// �����_
		D3DXVECTOR3 Up(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtLH(&mView, &Eye, &At, &Up);
		_pEffect->GetVariableByName("mView")->AsMatrix()->SetMatrix((float*)&mView);

		D3DXMATRIX mProj;	// �ˉe�ϊ��s��
		D3DXMatrixPerspectiveFovLH(
			&mProj,
			D3DXToRadian(60.0f),	// ����p
			(float)SCREEN_W/(float)SCREEN_H,	// �A�X�y�N�g��
			0.1f,	// �j�A�N���b�v
			100.0f);// �t�@�[�N���b�v
		_pEffect->GetVariableByName("mProj")->AsMatrix()->SetMatrix((float*)&mProj);

		// �e�N�j�b�N���擾����
		ID3D10EffectTechnique* pTechnique;
		pTechnique = _pEffect->GetTechniqueByName(TECHNIQUE_NAME);

		D3D10_TECHNIQUE_DESC desc;
		pTechnique->GetDesc(&desc);
		for (UINT i = 0; i < desc.Passes; ++i) {
			// �e�N�j�b�N�ɏ������p�X�̎��Ǝv���
			pTechnique->GetPassByIndex(i)->Apply(0);

			// ���b�V���`��
			_pMesh->DrawSubset(0);
		}
	}

	// �V�[���̕\��
	_pSwapChain->Present(0, 0);

	// FPS����
	_fps.Wait();
}


