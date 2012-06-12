/**
 *	GameMain.cpp
 */

#include "GameMain.h"


//
GameMain GameMain::theInstance;

//
void Run();



/**
 *	Main()
 */
void Main()
{
	// �f�o�b�O�R���\�[��
#ifdef _DEBUG
	if (AllocConsole()) {
		freopen("CONOUT$", "w", stdout);
		freopen("CONIN$", "r", stdin);
	} else {
		// !�Ƃ肠�������s�͖���
	}
#endif

	GameMain::GetInstance().Initialize();

	MainLoop(Run);

	GameMain::GetInstance().Finalize();
}


/**
 *	Run()
 */
void Run()
{
	GameMain::GetInstance().Update();
}



/**
 *	GameMain::Initialize()
 */
bool GameMain::Initialize()
{
	// �E�B���h�E������
	wnd.Create(
		"Tank",
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
	D3DXVECTOR3 vLightDir(1.0f, -2.0f, 0.5f);
	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vLightDir);
	light.Position.x = 0.0f;
	light.Position.y = 500.0f;
	light.Position.z = 0.0f;
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

	// �J����������
	vEye.x = 0.0f;
	vEye.y = 10.0f;
	vEye.z = -30.0f;
	vAt.x = 0.0f;
	vAt.y = 5.0f;
	vAt.z = 0.0f;

	// �n�ʏ�����
	if (!ground.Initialize()) {
		return false;
	}

	// �^���N������
	if (!tank.Initialize()) {
		return false;
	}

	// �e������
	if (!Bullet::Initialize()) {
		return false;
	}

	// �I������
	if (!Target::Initialize()) {
		return false;
	}

	PushTarget(Target(D3DXVECTOR3(100.0f, -TARGET_H*0.5f, 0.0f)));
	PushTarget(Target(D3DXVECTOR3(0.0f, -TARGET_H*0.5f, 100.0f)));
	PushTarget(Target(D3DXVECTOR3(100.0f, -TARGET_H*0.5f, 100.0f)));
	PushTarget(Target(D3DXVECTOR3(5.0f, -TARGET_H*0.5f, -40.0f)));

	putTargetInter = PUT_TARGET_INTER;

	return true;
}


/**
 *	GameMain::Finalize()
 */
bool GameMain::Finalize()
{
	Target::Finalize();

	Bullet::Finalize();

	tank.Finalize();

	ground.Finalize();

	RELEASE(pD3DDev);
	RELEASE(pD3D);

	return true;
}


/**
 *	GameMain::Update()
 */
void GameMain::Update()
{
	// ��ԑ���
	if (HIWORD(GetAsyncKeyState('W'))) {
		tank.Forward();
	}
	if (HIWORD(GetAsyncKeyState('S'))) {
		tank.Back();
	}
	if (HIWORD(GetAsyncKeyState('A'))) {
		tank.TurnLeft();
	}
	if (HIWORD(GetAsyncKeyState('D'))) {
		tank.TurnRight();
	}


	//// �X�V

	// ��ԍX�V
	tank.Update();
	
	// �e�X�V
	std::list<Bullet>::iterator itrBul = bullets.begin();
	for (; itrBul != bullets.end(); ++itrBul) {
		itrBul->Update();
	}

	// �I�o��
	if (putTargetInter-- <= 0) {
		float x = (float)((rand()%(int)(GROUND_X_MAX-GROUND_X_MIN)) + GROUND_X_MIN);
		float z = (float)((rand()%(int)(GROUND_Z_MAX-GROUND_Z_MIN)) + GROUND_Z_MIN);

		if (targets.size() < 50) {
			PushTarget(Target(D3DXVECTOR3(x, 0, z)));
		}

		putTargetInter = PUT_TARGET_INTER;
	}

	// �I�X�V
	std::list<Target>::iterator itrTgt = targets.begin();
	for (; itrTgt != targets.end(); ++itrTgt) {
		itrTgt->Update();
	}

	// �e�폜
	itrBul = bullets.begin();
	while (itrBul != bullets.end()) {
		if (itrBul->CanErase()) {
			itrBul->OnErase();
			itrBul = bullets.erase(itrBul);
		} else {
			++itrBul;
		}
	}

	// �I�폜
	itrTgt = targets.begin();
	while (itrTgt != targets.end()) {
		if (itrTgt->CanErase()) {
			itrTgt->OnErase();
			itrTgt = targets.erase(itrTgt);
		} else {
			++itrTgt;
		}
	}


	//// �`��

	// �V�[���N���A
	pD3DDev->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,	// Z�o�b�t�@���N���A����
		D3DCOLOR_XRGB(0x80,0xc0,0xff), 1.0f, 0);

	// �V�[���`��J�n
	HRESULT hr = pD3DDev->BeginScene();
	if (SUCCEEDED(hr)) {
		//// �V�[���`��

		// �r���[�ϊ��s��
		D3DXMATRIX mView;
		D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f);

		D3DXMatrixLookAtLH(&mView, &vEye, &vAt, &vUp);
		pD3DDev->SetTransform(D3DTS_VIEW, &mView);

		// �����ϊ��s��
		D3DXMATRIX mProj;
		D3DXMatrixPerspectiveFovLH(&mProj,
			60.0f/180.0f*3.141592f,	// ����p60�x
			(float)SCREEN_W/(float)SCREEN_H,
			3.0f,
			10000.0f);
		pD3DDev->SetTransform(D3DTS_PROJECTION, &mProj);

		// ���C�g
		pD3DDev->SetLight(0, &light);
		pD3DDev->LightEnable(0, TRUE);

		// �n�ʕ`��
		ground.Draw();

		// �^���N�`��
		tank.Draw();

		// �e�`��
		std::list<Bullet>::iterator itrBul = bullets.begin();
		for (; itrBul != bullets.end(); ++itrBul) {
			itrBul->Draw();
		}
		
		// �I�`��
		std::list<Target>::iterator itrTgt = targets.begin();
		for (; itrTgt != targets.end(); ++itrTgt) {
			itrTgt->Draw();
		}

		// �V�[���`��I��
		pD3DDev->EndScene();
	}

	// �V�[���̕\��
	hr = pD3DDev->Present(NULL, NULL, NULL, NULL);
	if (hr == D3DERR_DEVICELOST) ;

	// Fps�v��
	static char str[32];
	static int _fps = 0;
	if (_fps != fps.GetFps()) {
		sprintf(str, "Tank (%dFPS)", fps.GetFps());
		SetWindowText(wnd.GetHandle(), str);
		_fps = fps.GetFps();
	}

	//
	fps.Wait();
}


/**
 *	�}�E�X�����������B
 */
bool GameWindow::OnLButtonDown::Handle()
{
	GameMain::GetInstance().GetTank().Shot();
	return true;
}

/**
 *	�}�E�X�E���������B
 */
bool GameWindow::OnRButtonDown::Handle()
{
	GameWindow* This = (GameWindow*)pWnd;
	This->vPos.x = (float)GetX();
	This->vPos.y = (float)GetY();
	return true;
}


/**
 *	�}�E�X���h���b�O�����B
 */
bool GameWindow::OnMouseMove::Handle()
{
	if (!IsRButton()) return false;

	GameWindow* This = (GameWindow*)pWnd;

	// ��Ԃ̓�����
	D3DXVECTOR3 v((FLOAT)GetX(), (FLOAT)GetY(), 0);
	v = v - This->vPos;

	float angle = v.x / 3.0f;	// �l�͓K��
	if (angle >= 40.0f) angle = 40.0f;

	GameMain::GetInstance().GetTank().RotateHead(D3DXToRadian(angle));

	This->vPos.x = (float)GetX();
	This->vPos.y = (float)GetY();

	return true;
}


/**
 *	GameMain::SetCamera()
 *	�J�����ݒ�
 */
void GameMain::SetCamera(const D3DXVECTOR3& vEye, const D3DXVECTOR3& vAt)
{
	this->vEye = vEye;
	this->vAt = vAt;
}


/**
 *	GameMain::PushBullet()
 *	�e�𔭎˂���B
 */
void GameMain::PushBullet(const Bullet& bul)
{
	bullets.push_back(bul);
}


/**
 *	GameMain::PushTarget()
 *	�I�𗧂Ă�B
 */
void GameMain::PushTarget(const Target& tgt)
{
	targets.push_back(tgt);
}