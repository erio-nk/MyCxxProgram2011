/*!
 	MyApp.cpp
 */

#include "MyApp.h"


//
MyApp MyApp::theInstance;

//
namespace
{
	const float FOV_ANGLE = D3DXToRadian(60);	// ����p
	const float NEAR_Z = 1.0f;	// �r���[�{�����[���̋ߕ��ʂ܂ł̋���
	const float FAR_Z = 15.0f;	// �r���[�{�����[���̉����ʂ܂ł̋���
}


/*!
 	Main()
 */
void Main()
{
	MyApp::GetInstance().Initialize();

	MainLoop(&MyApp::GetInstance(), &MyApp::Update);

	MyApp::GetInstance().Finalize();
}



/*!
 	MyApp::Initialize()
 */
bool MyApp::Initialize()
{
	// �E�B���h�E������
	_wnd.Create(
		"TurnObject",
		0, 0, SCREEN_W, SCREEN_H);
	_wnd.Show();

	// �t���[�����[�g�ݒ�
	_fps.SetFrameRate(30);

	// Direct3D������
	_device.Create(_wnd.GetHandle());

	// ���b�V��
	D3DXCreateTeapot(_device.GetInterface(), &_mesh.GetMesh(), NULL);
	_mesh.SetDiffuseColor(1.0f, 0.0f, 1.0f);
	_mesh.SetAmbientColor(0.2f, 0.0f, 0.2f);

	// ���C�g
	_light.SetDirection(0.0f, 1.0f, 2.0f);
	_light.SetPosition(0.0f, -2.0f, -2.0f);

	_device.GetInterface()->SetRenderState(D3DRS_LIGHTING, TRUE);
	_device.GetInterface()->SetLight(0, &_light.GetLight());
	_device.GetInterface()->LightEnable(0, TRUE);

	// Z�o�b�t�@
	_device.GetInterface()->SetRenderState(D3DRS_ZENABLE, TRUE);

	// �J�����O
	_device.GetInterface()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//
	D3DXVec3Normalize(&_vRotation, &_vRotation);

	return true;
}


/*!
 	MyApp::Finalize()
 */
bool MyApp::Finalize()
{
	return true;
}

/*!
 	MyApp::Update()
 */
void MyApp::Update()
{
	UpdateFrame();

	DrawFrame();
	
	//
	_fps.Wait();
}

/*!
 	MyApp::UpdateFrame()
 */
void MyApp::UpdateFrame()
{
	if (_bTurning) {
		//// ����F�p���x�N�g���̌v�Z

		// ���݂̎p���x�N�g��vRot�ƖڕW�̎p���x�N�g��vTurn�̊O�ς����A��]��vAxis�Ƃ���
		D3DXVECTOR3 vAxis;
		D3DXVec3Cross(&vAxis, &_vRotation, &_vTurn);
		if (D3DXVec3LengthSq(&vAxis) == 0.0f) {
			_bTurning = false;	// �O�ς��[���Ȃ�A���ɓ����p���x�N�g���ɂȂ��Ă���̂Ő���I��
			return;
		}
		D3DXVec3Normalize(&vAxis, &vAxis);
		
		// ��]������A�K���Ȋp�x�ŉ�]����}�g���N�X���v�Z����
		D3DXMATRIX mRot;
		D3DXMatrixRotationAxis(&mRot, &vAxis, D3DXToRadian(2.0f));

		// vRot��������]�������x�N�g��vRot'���v�Z����
		D3DXVECTOR3 vRotDash;
		D3DXVec3TransformCoord(&vRotDash, &_vRotation, &mRot);

		// vRot'��vTurn�O�ς����A���t���[���̉�]��vAxis'�𓾂�
		D3DXVECTOR3 vAxisDash;
		D3DXVec3Cross(&vAxisDash, &vRotDash, &_vTurn);
		if (D3DXVec3LengthSq(&vAxisDash) == 0.0f) {
			_bTurning = false;	// �O�σ[���Ȃ�A�x�N�g�����d�Ȃ����̂Ő���I��
			_vRotation = _vTurn;
			return;
		}
		D3DXVec3Normalize(&vAxisDash, &vAxisDash);

		// vAxis��vAxis'�̓��ς����A���̌������r����
		if (D3DXVec3Dot(&vAxis, &vAxisDash) < 0.0f) {
			// ���ς������������Ό����ɂȂ����Ƃ������́A
			// vRot��vTurn�̍��E�֌W�ƁAvRot'��vTurn�̍��E�֌W���ς����
			// ����vTurn�𒴂����Ƃ������ɂȂ�̂ŁA����I��
			_bTurning = false;
			_vRotation = _vTurn;
			return;
		}

		_vRotation = vRotDash;
	}


	//// ���񑀍�
	#define ANGLE 30.0f

	if (LOBYTE(GetAsyncKeyState(VK_LEFT))) {
		// ��]��̃x�N�g����ݒ肷��
		D3DXMATRIX m;
		D3DXMatrixRotationY(&m, D3DXToRadian(ANGLE));
		D3DXVec3TransformCoord(&_vTurn, &_vRotation, &m);
		_bTurning = true;
	}
	if (LOBYTE(GetAsyncKeyState(VK_RIGHT))) {
		D3DXMATRIX m;
		D3DXMatrixRotationY(&m, D3DXToRadian(-ANGLE));
		D3DXVec3TransformCoord(&_vTurn, &_vRotation, &m);
		_bTurning = true;
	}
	if (LOBYTE(GetAsyncKeyState(VK_UP))) {
		D3DXMATRIX m;
		D3DXMatrixRotationX(&m, D3DXToRadian(-ANGLE));
		D3DXVec3TransformCoord(&_vTurn, &_vRotation, &m);
		_bTurning = true;
	}
	if (LOBYTE(GetAsyncKeyState(VK_DOWN))) {
		D3DXMATRIX m;
		D3DXMatrixRotationX(&m, D3DXToRadian(ANGLE));
		D3DXVec3TransformCoord(&_vTurn, &_vRotation, &m);
		_bTurning = true;
	}
}


/*!
 	MyApp::DrawFrame()
 */
void MyApp::DrawFrame()
{
	//// �V�[���A�b�v�f�[�g

	// �V�[���N���A
	_device.GetInterface()->Clear(
		0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(0,0,80), 1.0f, 0);

	// �V�[���`��J�n
	HRESULT hr = _device.GetInterface()->BeginScene();
	if (SUCCEEDED(hr)) {
		//// �V�[���`��
		
		// �r���[�ϊ�
		D3DXMATRIX mView;
		D3DXVECTOR3 vEye(0.0f, 2.0f, -5.0f);
		D3DXVECTOR3 vAt(0.0f, 0.0f ,0.0f);
		D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtLH(&mView, &vEye, &vAt, &vUp);
		_device.GetInterface()->SetTransform(D3DTS_VIEW, &mView);
		
		// �ˉe�ϊ�
		D3DXMATRIX mProj;
		D3DXMatrixPerspectiveFovLH(&mProj,
			FOV_ANGLE,
			(float)SCREEN_W/(float)SCREEN_H,
			NEAR_Z, FAR_Z);
		_device.GetInterface()->SetTransform(D3DTS_PROJECTION, &mProj);

		// ���[���h�ϊ�
		D3DXMATRIX mWorld, mRot, mKuti;
		D3DXMatrixIdentity(&mWorld);
		
		mWorld *= *D3DXMatrixRotationY(&mKuti, D3DXToRadian(-90));	// ���̌�����(1,0,0)�ɂ������̂�-90�x��]
		
		mWorld *= *D3DXPlus::MatrixRotationVector(&mRot, &_vRotation);


		_device.GetInterface()->SetTransform(D3DTS_WORLD, &mWorld);

		// ���b�V���`��
		_mesh.Render(_device);

		// �����`��
		D3DXPlus::DrawLine(_device.GetInterface(), D3DXVECTOR3(0,0,0), _vRotation * 2.0f, 
			D3DCOLOR_XRGB(0xff, 0x00, 0xff), D3DCOLOR_XRGB(0x00, 0xff, 0xff));

		if (_bTurning) {
			D3DXPlus::DrawLine(_device.GetInterface(), D3DXVECTOR3(0,0,0), _vTurn * 2.0f, 
				D3DCOLOR_XRGB(0xff, 0x00, 0xff), D3DCOLOR_XRGB(0xff, 0xff, 0x00));
		}

		// �V�[���`��I��
		_device.GetInterface()->EndScene();
	}

	// �V�[���̕\��
	hr = _device.GetInterface()->Present(NULL, NULL, NULL, NULL);
	if (hr == D3DERR_DEVICELOST) ;
}


