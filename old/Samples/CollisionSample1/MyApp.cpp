/**
 *	MyApp.cpp
 */
#include <stdio.h>

#include "MyApp.h"

#ifdef _DEBUG
	#define PRINTF printf
#else
	#define PRINTF __noop
#endif

//
struct MYVERTEX
{
	D3DXVECTOR3 v;
	DWORD color;
};

#define D3DFVF_MYVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)


//
MyApp MyApp::theInstance;

//
void Run();



/**
 *	Main()
 */
void Main()
{	// �f�o�b�O�R���\�[��
#ifdef _DEBUG
	if (AllocConsole()) {
		freopen("CONOUT$", "w", stdout);
		freopen("CONIN$", "r", stdin);
	} else {
		// !�Ƃ肠�������s�͖���
	}
#endif

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
		CreateOption::Centering |
		CreateOption::ClientSize |
		CreateOption::StandardStyle,
		NULL,
		NULL);
	wnd.Show();

	// �t���[�����[�g�ݒ�
	fps.SetFrameRate(30);

	// �����o�ϐ��̏�����
	pD3D = NULL;
	pD3DDev = NULL;

	pMeshSph = NULL;
	pMeshBox = NULL;

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
	pD3DDev->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(0x40, 0x40, 0x40));

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


	//// �����f���̏���

	// ���b�V������
	hr = D3DXCreateSphere(pD3DDev, 
		SPHERE_RADIUS, 16, 16,
		&pMeshSph, NULL);
	if (FAILED(hr)) {
		return false;
	}

	// �}�e���A������
	ZeroMemory(&matSph, sizeof(D3DMATERIAL9));
	matSph.Diffuse.r = matSph.Ambient.r = 1.0f;
	matSph.Diffuse.g = matSph.Ambient.g = 0.0f;
	matSph.Diffuse.b = matSph.Ambient.b = 0.0f;
	matSph.Diffuse.a = matSph.Ambient.a = 1.0f;

	//// BOX���f���̏���

	// ���b�V������
	hr = D3DXCreateBox(pD3DDev, 
		BOX_W,
		BOX_H,
		BOX_D,
		&pMeshBox, NULL);
	if (FAILED(hr)) {
		return false;
	}

	// �}�e���A������
	ZeroMemory(&matBox, sizeof(D3DMATERIAL9));
	matBox.Diffuse.r = matBox.Ambient.r = 0.0f;
	matBox.Diffuse.g = matBox.Ambient.g = 1.0f;
	matBox.Diffuse.b = matBox.Ambient.b = 0.0f;
	matBox.Diffuse.a = matBox.Ambient.a = 1.0f;

	//// �J�����̏����ݒ�
	vCamera.x = 0.0f;
	vCamera.y = 0.0f;
	vCamera.z = -70.0f;

	//// ���̍��W�E�p���̏����ݒ�
	//vSph.x = -20.0f;
	//vSphSpd.Set(50, 30, 0);
	vSph.x = 30.0f;
	vSphSpd.Set(-50, 20, 0);

	vBox.x = 0.0f;
	vBoxSpd.Set(0, 0, 0);

	return true;
}


/**
 *	MyApp::Finalize()
 */
bool MyApp::Finalize()
{
	RELEASE(pMeshBox);
	RELEASE(pMeshSph);

	RELEASE(pD3DDev);
	RELEASE(pD3D);
	return true;
}



/**
 *	MyApp::Update()
 */
void MyApp::Update()
{
	// ----------------------------------------------------------- //
	// ����

	// �J������
	if (HIWORD(GetAsyncKeyState(VK_LEFT))) {
		D3DXMATRIX m;
		D3DXMatrixRotationY(&m, D3DXToRadian(-2));
		D3DXVec3TransformCoord(&vCamera, &vCamera, &m);
	}
	if (HIWORD(GetAsyncKeyState(VK_RIGHT))) {
		D3DXMATRIX m;
		D3DXMatrixRotationY(&m, D3DXToRadian(2));
		D3DXVec3TransformCoord(&vCamera, &vCamera, &m);
	}

	// �����ړ�������
	if (HIWORD(GetAsyncKeyState(VK_NUMPAD8))) {
		vSph.y += 1.0f;
	}
	if (HIWORD(GetAsyncKeyState(VK_NUMPAD2))) {
		vSph.y -= 1.0f;
	}
	if (HIWORD(GetAsyncKeyState(VK_NUMPAD4))) {
		vSph.x -= 1.0f;
	}
	if (HIWORD(GetAsyncKeyState(VK_NUMPAD6))) {
		vSph.x += 1.0f;
	}

	// �ړ���̋����ړ�������
	if (HIWORD(GetAsyncKeyState('W'))) {
		vSphSpd.y += 1.0f;
	}
	if (HIWORD(GetAsyncKeyState('S'))) {
		vSphSpd.y -= 1.0f;
	}
	if (HIWORD(GetAsyncKeyState('A'))) {
		vSphSpd.x -= 1.0f;
	}
	if (HIWORD(GetAsyncKeyState('D'))) {
		vSphSpd.x += 1.0f;
	}

	// �e�X�g
	//bool test = TestSphereBox(
	//	vSph, SPHERE_RADIUS,
	//	vBox, BOX_W*0.5f, BOX_H*0.5f, BOX_D*0.5f);
	Vector3 vTest;
	bool test = TestMovingSphereMovingBox(
		vSph, SPHERE_RADIUS, vSphSpd,
		vBox,
		Vector3(BOX_W, 0, 0),
		Vector3(0, BOX_H, 0),
		Vector3(0, 0, BOX_D),
		vBoxSpd, &vTest);


	// ----------------------------------------------------------- //
	// �`��

	// �V�[���N���A
	pD3DDev->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,	// Z�o�b�t�@���N���A����
		D3DCOLOR_XRGB(0,0,80), 1.0f, 0);

	// �V�[���`��J�n
	HRESULT hr = pD3DDev->BeginScene();
	if (SUCCEEDED(hr)) {
		//// �V�[���`��

		// �r���[�ϊ��s��
		D3DXMATRIX mView;
		D3DXVECTOR3 vAt(0.0f, 0.0f ,0.0f);
		D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtLH(&mView, &vCamera, &vAt, &vUp);
		pD3DDev->SetTransform(D3DTS_VIEW, &mView);

		// �����ϊ��s��
		D3DXMATRIX mProj;
		D3DXMatrixPerspectiveFovLH(&mProj,
			60.0f/180.0f*3.141592f,	// ����p60�x
			(float)SCREEN_W/(float)SCREEN_H,
			3.0f,
			1000.0f);
		pD3DDev->SetTransform(D3DTS_PROJECTION, &mProj);

		// ���C�g
		pD3DDev->SetLight(0, &light);
		pD3DDev->LightEnable(0, TRUE);

		//// ���`��
		D3DXMATRIX m;
		D3DXMatrixTranslation(&m, vSph.x, vSph.y, vSph.z);
		pD3DDev->SetTransform(D3DTS_WORLD, &m);

		pD3DDev->SetMaterial(&matSph);
		pMeshSph->DrawSubset(0);
		
		// �ړ���
		//Vector3 vSph2 = vSph + vSphSpd;
		//D3DXMatrixTranslation(&m, vSph2.x, vSph2.y, vSph2.z);
		//pD3DDev->SetTransform(D3DTS_WORLD, &m);

		//pD3DDev->SetMaterial(&matSph);
		//pMeshSph->DrawSubset(0);

		// �ڐG����ʒu�̋�
		if (test) {
			D3DXMatrixTranslation(&m, vTest.x, vTest.y, vTest.z);
			pD3DDev->SetTransform(D3DTS_WORLD, &m);

			pD3DDev->SetMaterial(&matSph);
			pMeshSph->DrawSubset(0);
		}


		// �O��
		MYVERTEX mv[6];
		mv[0].v.x = 0;
		mv[0].v.y = 0;
		mv[0].v.z = 0;
		mv[0].color = D3DCOLOR_XRGB(0xff, 0, 0);
		mv[1].v.x = vSphSpd.x;
		mv[1].v.y = vSphSpd.y;
		mv[1].v.z = vSphSpd.z;
		mv[1].color = D3DCOLOR_XRGB(0xff, 0, 0);

		D3DXMatrixTranslation(&m, vSph.x, vSph.y, vSph.z);
		pD3DDev->SetTransform(D3DTS_WORLD, &m);

		pD3DDev->SetFVF(D3DFVF_MYVERTEX);
		pD3DDev->DrawPrimitiveUP(D3DPT_LINELIST, 1, mv, sizeof(MYVERTEX));

		//// BOX�`��
		D3DXMatrixTranslation(&m, vBox.x, vBox.y, vBox.z);
		pD3DDev->SetTransform(D3DTS_WORLD, &m);

		pD3DDev->SetMaterial(&matBox);
		pMeshBox->DrawSubset(0);
		
		// �ړ���
		Vector3 vBox2 = vBox + vBoxSpd;
		D3DXMatrixTranslation(&m, vBox2.x, vBox2.y, vBox2.z);
		pD3DDev->SetTransform(D3DTS_WORLD, &m);

		pD3DDev->SetMaterial(&matBox);
		pMeshBox->DrawSubset(0);
		

		// �V�[���`��I��
		pD3DDev->EndScene();
	}

	// �V�[���̕\��
	hr = pD3DDev->Present(NULL, NULL, NULL, NULL);
	if (hr == D3DERR_DEVICELOST) ;

	// ���`��
	static char str[64];
	sprintf(str, "�e�X�g=%d, pos=%.2f,%.2f,%.2f",
		test, vTest.x, vTest.y, vTest.z);
	HDC hDC = GetDC(wnd.GetHandle());
	TextOut(hDC, 0, 0, str, lstrlen(str));
	ReleaseDC(wnd.GetHandle(), hDC);

	//
	fps.Wait();
}
