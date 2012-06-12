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
		"DirectGraphicsSample2",
		0, 0, SCREEN_W, SCREEN_H,
		WindowOption::Centering |
		WindowOption::ClientSize |
		WindowOption::StandardStyle,
		NULL,
		NULL);
	wnd.Show();

	// �t���[�����[�g�ݒ�
	fps.SetFrameRate(30);

	// �����o�ϐ��̏�����
	pD3D = NULL;
	pD3DDev = NULL;

	pVtxBuf = NULL;

	// DirectGraphics������
	pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (pD3D == NULL) {
		return false;
	}

	ZeroMemory(&D3DPP, sizeof(D3DPP));

	D3DPP.Windowed = TRUE;	// �E�B���h�E���[�h
	D3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;	// �X���b�v�G�t�F�N�g
	D3DPP.BackBufferFormat = D3DFMT_UNKNOWN;	// �o�b�N�o�b�t�@�̃t�H�[�}�b�g
	
    D3DPP.EnableAutoDepthStencil = 1;			// Z�o�b�t�@���쐬����
    D3DPP.AutoDepthStencilFormat = D3DFMT_D16;

	HRESULT hr = pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL, wnd.GetHandle(),
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&D3DPP, &pD3DDev);
	if (FAILED(hr)) {
		return false;
	}
	
	// �J�����O
	pD3DDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	// �Ȃ� 

	// ���C�e�B���O
	pD3DDev->SetRenderState(D3DRS_LIGHTING, FALSE);	// �Ȃ�

	// Z�o�b�t�@
	pD3DDev->SetRenderState(D3DRS_ZENABLE, TRUE);	// �L��


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

	// �����́i���_�o�b�t�@�j�̍쐬
	MYVERTEX v[] = {
		{  1, -1,  1, 0xFFFFFFFF},
		{  1, -1, -1, 0xFFFFFFFF},
		{ -1, -1, -1, 0xFFFFFFFF},
		{ -1, -1,  1, 0xFFFFFFFF},	// ���̖�

		{ -1, -1,  1, 0xFF00FF00},
		{  1, -1,  1, 0xFF00FF00},
		{  1,  1,  1, 0xFF00FF00},
		{ -1,  1,  1, 0xFF00FF00},	// ���̖�

		{ -1,  1,  1, 0xFFFFFF00},
		{ -1, -1,  1, 0xFFFFFF00},
		{ -1, -1, -1, 0xFFFFFF00},
		{ -1,  1, -1, 0xFFFFFF00},	// ���̖�

		{ -1,  1, -1, 0xFF00FFFF},
		{ -1,  1,  1, 0xFF00FFFF},
		{  1,  1,  1, 0xFF00FFFF},
		{  1,  1, -1, 0xFF00FFFF},	// ��̖�

		{  1,  1, -1, 0xFF0000FF},
		{  1,  1,  1, 0xFF0000FF},
		{  1, -1,  1, 0xFF0000FF},
		{  1, -1, -1, 0xFF0000FF},	// �E�̖�

		{  1, -1, -1, 0xFFFF0000},
		{  1,  1, -1, 0xFFFF0000},
		{ -1,  1, -1, 0xFFFF0000},
		{ -1, -1, -1, 0xFFFF0000}	// ��O�̖�
	};

	hr = pD3DDev->CreateVertexBuffer(
		4*6*sizeof(MYVERTEX),
		0,
		D3DFVF_MYVERTEX,
		D3DPOOL_MANAGED,
		&pVtxBuf,
		NULL);
	if (FAILED(hr)) {
		return false;
	}

	MYVERTEX* _v;
	hr = pVtxBuf->Lock(0, 0, (LPVOID*)&_v, 0);
	if (FAILED(hr)) {
		return false;
	}

	memcpy(_v, v, sizeof(v));

	hr = pVtxBuf->Unlock();
	if (FAILED(hr)) {
		return false;
	}

	return true;
}


/**
 *	MyApp::Finalize()
 */
bool MyApp::Finalize()
{
	RELEASE(pVtxBuf);

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
	pD3DDev->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,	// Z�o�b�t�@���N���A����
		D3DCOLOR_XRGB(0,0,80), 1.0f, 0);

	// �V�[���`��J�n
	HRESULT hr = pD3DDev->BeginScene();
	if (SUCCEEDED(hr)) {
		//// �V�[���`��

		// ���[���h�ϊ��s��
		D3DXMATRIX mWorld;

		static float theta = 0.0f;
		theta += 4.0f;	// Y������ɂ��邮��
		D3DXMatrixRotationY(&mWorld, D3DXToRadian(theta));

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
			60.0f/180.0f*3.141592f,	// ����p60�x
			(float)SCREEN_W/(float)SCREEN_H,
			3.0f,
			15.0f);
		pD3DDev->SetTransform(D3DTS_PROJECTION, &mProj);

		// �����̕`��
		pD3DDev->SetStreamSource(0, pVtxBuf, 0, sizeof(MYVERTEX));
		pD3DDev->SetFVF(D3DFVF_MYVERTEX);

		for (int i = 0; i < 6; ++i) {
			pD3DDev->DrawPrimitive(D3DPT_TRIANGLEFAN, 4*i,  2);
		}

		// �V�[���`��I��
		pD3DDev->EndScene();
	}

	// �V�[���̕\��
	hr = pD3DDev->Present(NULL, NULL, NULL, NULL);
	if (hr == D3DERR_DEVICELOST) ;

	//
	fps.Wait();
}


