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
	const float NEAR_Z = 3.0f;	// �r���[�{�����[���̋ߕ��ʂ܂ł̋���
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
		"ProjectionSample2",
		0, 0, SCREEN_W, SCREEN_H);
	_wnd.Show();

	// �t���[�����[�g�ݒ�
	_fps.SetFrameRate(30);

	// Direct3D������
	_device.Create(_wnd.GetHandle());

	// ���b�V��
	D3DXCreateTeapot(_device.GetInterface(), &_mesh.GetMesh(), NULL);
	_mesh.SetDiffuseColor(1.0f, 1.0f, 0.0f, 1.0f);
	
	// ���C�e�B���O
	D3DXVECTOR3 vLightDir(-1.0f, -1.0f, 2.0f);
	ZeroMemory(&_light, sizeof(D3DLIGHT9));
	_light.Type = D3DLIGHT_DIRECTIONAL;
	_light.Diffuse.r = 1.0f;
	_light.Diffuse.g = 1.0f;
	_light.Diffuse.b = 1.0f;
	D3DXVec3Normalize((D3DXVECTOR3*)&_light.Direction, &vLightDir);
	_light.Position.x = -1.0f;
	_light.Position.y = -1.0f;
	_light.Position.z = 2.0f;
	_light.Range = 1000.0f;
	_device.GetInterface()->SetLight(0, &_light);
	_device.GetInterface()->LightEnable(0, TRUE);
	_device.GetInterface()->SetRenderState(D3DRS_LIGHTING, TRUE);

	// Z�o�b�t�@
	_device.GetInterface()->SetRenderState(D3DRS_ZENABLE, TRUE);	// �L��

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
	// �ˉe�ϊ��^�C�v�؂�ւ�
	if (LOBYTE(GetAsyncKeyState(VK_SPACE))) {
		if (++_projType > 2) _projType = 0; 
	}

	// �V�[���N���A
	_device.GetInterface()->Clear(
		0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(0,0,80), 1.0f, 0);

	// �V�[���`��J�n
	HRESULT hr = _device.GetInterface()->BeginScene();
	if (SUCCEEDED(hr)) {
		//// �V�[���`��

		// ���[���h�ϊ��s��
		D3DXMATRIX mWorld;

		static float theta = 0.0f;
		theta += 4.0f;	// Y������ɂ��邮��
		D3DXMatrixRotationY(&mWorld, D3DXToRadian(theta));

		_device.GetInterface()->SetTransform(D3DTS_WORLD, &mWorld);

		// �r���[�ϊ��s��
		D3DXMATRIX mView;
		D3DXVECTOR3 vEye(0.0f, 0.0f, -5.0f);
		D3DXVECTOR3 vAt(0.0f, 0.0f ,0.0f);
		D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtLH(&mView, &vEye, &vAt, &vUp);
		_device.GetInterface()->SetTransform(D3DTS_VIEW, &mView);

		// �ˉe�ϊ��s��
		D3DXMATRIX mProj;

		switch (_projType) {
			case 0:	// �������e�ϊ��E���̂P
			{
				D3DXMatrixPerspectiveFovLH(&mProj,
					FOV_ANGLE,
					(float)SCREEN_W/(float)SCREEN_H,
					NEAR_Z, FAR_Z);
				break;
			}
			case 1:	// �������e�ϊ��E���̂Q
			{
				float nearH = NEAR_Z * tan(FOV_ANGLE/2.0f) * 2.0f;			// �ߕ��ʂ̏c�� = nearZ * tan(Y�̎���p/2) * 2
				float nearW = nearH * (float)SCREEN_W / (float)SCREEN_H;	// �ߕ��ʂ̉��� = �ߕ��ʂ̏c�� * �A�X�y�N�g��
				D3DXMatrixPerspectiveLH(&mProj,
					nearW,
					nearH,
					NEAR_Z, FAR_Z);

				break;
			}
			case 2:	// ���ˉe�ϊ�
			{
				float nearH = NEAR_Z * tan(FOV_ANGLE/2.0f) * 2.0f;			// �ߕ��ʂ̏c�� = nearZ * tan(Y�̎���p/2) * 2
				float nearW = nearH * (float)SCREEN_W / (float)SCREEN_H;	// �ߕ��ʂ̉��� = �ߕ��ʂ̏c�� * �A�X�y�N�g��
				D3DXMatrixOrthoLH(&mProj,
					nearW,
					nearH,
					NEAR_Z, FAR_Z);

				break;
			}
		}
		_device.GetInterface()->SetTransform(D3DTS_PROJECTION, &mProj);

		// ���b�V���`��
		_mesh.Render(_device);

		// �V�[���`��I��
		_device.GetInterface()->EndScene();
	}

	// �V�[���̕\��
	hr = _device.GetInterface()->Present(NULL, NULL, NULL, NULL);
	if (hr == D3DERR_DEVICELOST) ;

	//
	_fps.Wait();
}


