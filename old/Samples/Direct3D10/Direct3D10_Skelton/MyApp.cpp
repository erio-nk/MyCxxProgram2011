/**
	@file MyApp.cpp

	@author Dila
 */

#include "MyApp.h"

MyApp MyApp::theInstance;


/**
	Main()
 */
void Main()
{
	MyApp::GetInstance().Initialize();

	MainLoop(&MyApp::GetInstance(), &MyApp::Update);

	MyApp::GetInstance().Finalize();
}


/**
	MyApp::Initialize()
 */
bool MyApp::Initialize()
{
	// �E�B���h�E������
	_wnd.Create(
		"Direct3D10 Application Skelton",
		0, 0, SCREEN_W, SCREEN_H);
	_wnd.Show();

	// �t���[�����[�g�ݒ�
	_fps.SetFrameRate(30);


	//// Direct3D10 ������

	// �X���b�v�`�F�C���̐ݒ�
	DXGI_SWAP_CHAIN_DESC sc;
	ZeroMemory(&sc, sizeof(sc));
	sc.BufferCount = 1;		// �o�b�N�o�b�t�@�̖���
	sc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// �o�b�N�o�b�t�@�̃t�H�[�}�b�g
	sc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// �����Ȃ񂶂�H
	sc.OutputWindow = _wnd.GetHandle();
	sc.SampleDesc.Count = 1;	// �Ȃ�̂�������
	sc.Windowed = TRUE;

	// �f�o�C�X�쐬�i���Ńf�o�C�X�쐬�ɂȂ����݂����j
	HRESULT hr;
	hr = D3D10CreateDeviceAndSwapChain(	// CreateDevice()�̕��̓R���\�[���A�v���ł������炵��
		NULL,
		D3D10_DRIVER_TYPE_HARDWARE,	// �n�[�h�E�F�A�ŏ���
		NULL,
		NULL,
		D3D10_SDK_VERSION,
		&sc,
		&_pSwapChain,
		&_pDevice);
	if (FAILED(hr)) {
		return false;
	}

	// �����_�[�^�[�Q�b�g�r���[�쐬�i�r���[���ĂƂ����d�v�炵�����ǂ悭�킋�����j
	ID3D10Texture2D* pBuffer;
	hr = _pSwapChain->GetBuffer(	// �o�b�N�o�b�t�@���擾����
		0,
		__uuidof(ID3D10Texture2D),	// �Ȃɂ���I
		(LPVOID*)&pBuffer);
	if (FAILED(hr)) {
		return false;
	}

	hr = _pDevice->CreateRenderTargetView(
		pBuffer,
		NULL,
		&_pRTView);
	if (FAILED(hr)) {
		return false;
	}
	
    _pDevice->OMSetRenderTargets(1, &_pRTView, NULL);	// �f�o�C�X�ɐݒ肵�Ă���̂��ȁH

	RELEASE(pBuffer);	// COM�̂����܂�H

	// �r���[�|�[�g�̐ݒ�
	D3D10_VIEWPORT v;
	v.TopLeftX = 0;		// D3DVIEWPORT9::X
	v.TopLeftY = 0;		// D3DVIEWPORT9::Y
	v.Width  = SCREEN_W;
	v.Height = SCREEN_H;
	v.MinDepth = 0.0f;	// D3DVIEWPORT9::MinZ
	v.MaxDepth = 1.0f;	// D3DVIEWPORT9::MaxZ
	_pDevice->RSSetViewports(
		1,	// �r���[�|�[�g�̔ԍ��B�����ݒ�o����l�ɂȂ����񂾂납�H	
		&v);

	return true;
}


/**
 	MyApp::Finalize()
 */
bool MyApp::Finalize()
{
	if (_pDevice) _pDevice->ClearState();	// ����Ȍ�n�����������炵��
	RELEASE(_pRTView);
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
	_pDevice->ClearRenderTargetView(_pRTView, color);

		// BeginScene��EndScene�͂Ȃ��̂��ȁH

	// �V�[���̕\���i�f�o�C�X����X���b�v�`�F�C���̃��\�b�h�ɂȂ����݂����j
	_pSwapChain->Present(0, 0);

	// FPS����
	_fps.Wait();
}


