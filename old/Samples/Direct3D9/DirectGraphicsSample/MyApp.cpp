/**
 *	MyApp.cpp
 */

#include "MyApp.h"


//
MyApp MyApp::theInstance;

//
void Run();



/**
 *	Main()
 */
void Main()
{
	MyApp::GetInstance().Initialize();

	MainLoop(Run);

	MyApp::GetInstance().Finalize();
}


/**
 *	Run()
 */
void Run()
{
	MyApp::GetInstance().Update();
}



/**
 *	MyApp::Initialize()
 */
bool MyApp::Initialize()
{
	// �E�B���h�E������
	wnd.Create(
		"DirectGraphicsSample",
		0, 0, SCREEN_W, SCREEN_H,
		WindowOption::Centering |
		WindowOption::ClientSize |
		WindowOption::StandardStyle,
		NULL,
		NULL);
	wnd.Show();

	// �t���[�����[�g�ݒ�
	fps.SetFrameRate(30);

	//
	pD3D = NULL;
	pD3DDev = NULL;

	pTeapot = NULL;

	// DirectGraphics������
	pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (pD3D == NULL) {
		return false;
	}

	ZeroMemory(&D3DPP, sizeof(D3DPP));

	D3DPP.Windowed = TRUE;	// �E�B���h�E���[�h
	D3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;	// �X���b�v�G�t�F�N�g
	D3DPP.BackBufferFormat = D3DFMT_UNKNOWN;	// �o�b�N�o�b�t�@�̃t�H�[�}�b�g

	HRESULT hr = pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL, wnd.GetHandle(),
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&D3DPP, &pD3DDev);
	if (FAILED(hr)) {
		return false;
	}


	// �r���[�|�[�g�̐ݒ�
	D3DVIEWPORT9 vp;
	vp.X = 0;
	vp.Y = 0;
	vp.Width = D3DPP.BackBufferWidth;
	vp.Height = D3DPP.BackBufferHeight;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;
	hr = pD3DDev->SetViewport(&vp);
	if (FAILED(hr)) {
		return false;
	}

	// �e�B�[�|�b�g�̍쐬
	hr = D3DXCreateTeapot(pD3DDev, &pTeapot, NULL);
	if (FAILED(hr)) {
		return false;
	}

	// �}�e���A��
	ZeroMemory(&mat, sizeof(D3DMATERIAL9));
	mat.Diffuse.r = mat.Ambient.r = 1.0f;
	mat.Diffuse.g = mat.Ambient.g = 1.0f;
	mat.Diffuse.b = mat.Ambient.b = 1.0f;
	mat.Diffuse.a = mat.Ambient.a = 1.0f;

	// ���C�g
	D3DXVECTOR3 vLightDir(-1.0f, -1.0f, 2.0f);
	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vLightDir);
	light.Position.x = -1.0f;
	light.Position.y = -1.0f;
	light.Position.z = 2.0f;
	light.Range = 1000.0f;

	pD3DDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	return true;
}


/**
 *	MyApp::Finalize()
 */
bool MyApp::Finalize()
{
	RELEASE(pTeapot);

	RELEASE(pD3DDev);
	RELEASE(pD3D);
	return true;
}


/**
 *	MyApp::Update()
 */
void MyApp::Update()
{
	// �V�[���N���A
	pD3DDev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,80), 1.0f, 0);

	// �V�[���`��J�n
	HRESULT hr = pD3DDev->BeginScene();
	if (SUCCEEDED(hr)) {
		//// �V�[���`��

		// ���[���h�ϊ��s��
		D3DXMATRIX mWorld;
		D3DXMatrixIdentity(&mWorld);
		pD3DDev->SetTransform(D3DTS_WORLD, &mWorld);

		// �r���[�ϊ��s��
		D3DXMATRIX mView;
		D3DXVECTOR3 vEye(0.0f, 0.0f, -5.0f);
		D3DXVECTOR3 vAt(0.0f, 0.0f ,0.0f);
		D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtLH(&mView, &vEye, &vAt, &vUp);
		pD3DDev->SetTransform(D3DTS_VIEW, &mView);

		// �����ϊ��s��
		D3DXMATRIX mProj;
		D3DXMatrixPerspectiveFovLH(&mProj,
			D3DXToRadian(60),	// ����p60�x
			(float)SCREEN_W/(float)SCREEN_H,
			3.0f,
			15.0f);
		#if 0
			// Fov����Ȃ����Ŏ������Ă݂�
			float nearH = tan(D3DXToRadian(30)) * 3.0f * 2.0f;		// �ߕ��ʂ̏c�� = tan(Y�̎���p/2) * nearZ * 2
			float nearW = nearH * (float)SCREEN_W/(float)SCREEN_H;	// �ߕ��ʂ̉��� = �ߕ��ʂ̏c�� * �A�X�y�N�g��
			D3DXMatrixPerspectiveLH(&mProj,	nearW, nearH, 3.0f, 15.0f);
		#endif
		#if 0
			// ���ˉe����Ă݂�
			float nearH = tan(D3DXToRadian(30)) * 3.0f * 2.0f;		// �ߕ��ʂ̏c�� = tan(Y�̎���p/2) * nearZ * 2
			float nearW = nearH * (float)SCREEN_W/(float)SCREEN_H;	// �ߕ��ʂ̉��� = �ߕ��ʂ̏c�� * �A�X�y�N�g��
			D3DXMatrixOrthoLH(&mProj, nearW, nearH, 3.0f, 15.0f);
		#endif
		pD3DDev->SetTransform(D3DTS_PROJECTION, &mProj);

		// �}�e���A��
		pD3DDev->SetMaterial(&mat);

		// ���C�g
		pD3DDev->SetLight(0, &light);
		pD3DDev->LightEnable(0, TRUE);

		// �e�B�[�|�b�g�`��
		pTeapot->DrawSubset(0);

		// �V�[���`��I��
		pD3DDev->EndScene();
	}

	// �V�[���̕\��
	hr = pD3DDev->Present(NULL, NULL, NULL, NULL);
	if (hr == D3DERR_DEVICELOST) ;

	//
	fps.Wait();
}


