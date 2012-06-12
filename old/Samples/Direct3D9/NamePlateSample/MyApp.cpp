/*!
 	MyApp.cpp
 */

#include "MyApp.h"

#include "Direct3D/Text.h"

//
MyApp MyApp::theInstance;

//
namespace
{
	const float FOV_ANGLE = D3DXToRadian(80);	// ����p
	const float NEAR_Z = 1.0f;	// �r���[�{�����[���̋ߕ��ʂ܂ł̋���
	const float FAR_Z = 15.0f;	// �r���[�{�����[���̉����ʂ܂ł̋���

	const int NAMEPLATE_TEXTURE_SIZE = 64;	// �l�[���v���[�g�̃e�N�X�`���̃T�C�Y
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
		"NamePlateSample",
		0, 0, SCREEN_W, SCREEN_H);
	_wnd.Show();

	// �t���[�����[�g�ݒ�
	_fps.SetFrameRate(30);

	// Direct3D������
	_device.Create(_wnd.GetHandle());

	// ���b�V���E���f���E�l�[���v���[�g
	for (int i = 0; i < MODEL_COUNT; ++i) {
		// ���f��
		D3DXCreateTeapot(_device.GetInterface(), &_mesh[i].GetMesh(), NULL);

		static const float p = 3.0f;	// position
		static const float d = 1.0f;	// diffuse
		static const float a = 0.2f;	// ambient
		switch (i) {
			case 0:
				_mesh[i].SetDiffuseColor(d, 0.0f, 0.0f);
				_mesh[i].SetAmbientColor(a, 0.0f, 0.0f);
				_vModelPos[i] = D3DXVECTOR3(-p, 0.0f, -p);
				break;
			case 1:
				_mesh[i].SetDiffuseColor(0.0f, d, 0.0f);
				_mesh[i].SetAmbientColor(0.0f, a, 0.0f);
				_vModelPos[i] = D3DXVECTOR3(-p, 0.0f, p);
				break;
			case 2:
				_mesh[i].SetDiffuseColor(0.0f, 0.0f, d);
				_mesh[i].SetAmbientColor(0.0f, 0.0f, a);
				_vModelPos[i] = D3DXVECTOR3(p, 0.0f, p);
				break;
			case 3:
				_mesh[i].SetDiffuseColor(d, d, 0.0f);
				_mesh[i].SetAmbientColor(a, a, 0.0f);
				_vModelPos[i] = D3DXVECTOR3(p, 0.0f, -p);
				break;
		}

		// �l�[���v���[�g
		D3DXPlus::CreateBoard(_device.GetInterface(), 0.25f, 0.25f, &_plate[i].GetMesh());
		_texture[i].Create(_device.GetInterface(), NAMEPLATE_TEXTURE_SIZE, NAMEPLATE_TEXTURE_SIZE);
		
		const D3DMATERIAL& m = _mesh[i].GetMaterial();
		_plate[i].SetDiffuseColor(m.Diffuse.r, m.Diffuse.g, m.Diffuse.b);
		_plate[i].SetAmbientColor(m.Ambient.r, m.Ambient.g, m.Ambient.b);
	}
	
	// ���C�g
	_light.SetDirection(0.0f, 1.0f, 2.0f);
	_light.SetPosition(0.0f, -2.0f, -2.0f);

	_device.GetInterface()->SetLight(0, &_light.GetLight());
	_device.GetInterface()->LightEnable(0, TRUE);

	// Z�o�b�t�@
	_device.GetInterface()->SetRenderState(D3DRS_ZENABLE, TRUE);

	// �J�����O
	_device.GetInterface()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	// �A���t�@�L��
	_device.GetInterface()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	_device.GetInterface()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    _device.GetInterface()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// ���_
	_vEye = D3DXVECTOR3(0.0f, -2.0f, -8.0f);


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
	// �V�[���N���A
	_device.GetInterface()->Clear(
		0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(0,0,80), 1.0f, 0);

	// �V�[���`��J�n
	HRESULT hr = _device.GetInterface()->BeginScene();
	if (SUCCEEDED(hr)) {
		//// �V�[���`��

		static bool bTexInit = false;
		if (!bTexInit) {
			// �e�N�X�`���Ƀe�L�X�g�������_�����O
			Direct3D::Text text;
			text.Create(_device);

			for (int i = 0; i < MODEL_COUNT; ++i) {
				// �����_�����O�J�n
				Direct3D::RenderTexture::BeginRender begin(_texture[i]);
				
				// �e�N�X�`���̃N���A
				_device.GetInterface()->Clear(
					0, NULL, D3DCLEAR_TARGET,
					D3DCOLOR_ARGB(0,0,0,0), 1.0f, 0);
				
				// �e�L�X�g�`��
				text.Render(_T("hoge"), NAMEPLATE_TEXTURE_SIZE/2, 0, _mesh[i].GetDiffuseColor(), DT_CENTER);				
			}

			bTexInit = true;
		}

		// ���_�����邮���
		D3DXMATRIX m;
		D3DXMatrixRotationY(&m, D3DXToRadian(1.0f));	// Y������ɂ��邮��
		D3DXVec3TransformCoord(&_vEye, &_vEye, &m);
		
		// �r���[�ϊ�
		D3DXMATRIX mView;
		D3DXVECTOR3 vAt(0.0f, 0.0f ,0.0f);
		D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtLH(&mView, &_vEye, &vAt, &vUp);
		_device.GetInterface()->SetTransform(D3DTS_VIEW, &mView);
		
		// �ˉe�ϊ��s��
		D3DXMATRIX mProj;
		D3DXMatrixPerspectiveFovLH(&mProj,
			FOV_ANGLE,
			(float)SCREEN_W/(float)SCREEN_H,
			NEAR_Z, FAR_Z);

		// �`��
		for (int i = 0; i < MODEL_COUNT; ++i) {
			//// ���f���`��
			D3DXMATRIX mWorld;

			// ���s�ړ�
			D3DXMATRIX mTrans;
			D3DXMatrixTranslation(&mTrans, _vModelPos[i].x, _vModelPos[i].y, _vModelPos[i].z);

			mWorld = mTrans;
			_device.GetInterface()->SetTransform(D3DTS_WORLD, &mWorld);

			// �ˉe�ϊ�
			_device.GetInterface()->SetTransform(D3DTS_PROJECTION, &mProj);

			// ���C�e�B���O
			_device.GetInterface()->SetRenderState(D3DRS_LIGHTING, TRUE);

			// ���b�V���`��
			_mesh[i].Render(_device);


			//// �l�[���v���[�g�`��

			// �����ɗ��Ă�
			D3DXMATRIX mRot;
			D3DXMatrixRotationX(&mRot, D3DXToRadian(90));

			// �r���{�[�h�ɂ���
			D3DXMATRIX mBill;
			D3DXMatrixInverse(&mBill, NULL, &mView);
			mBill._41 = mBill._42 = mBill._43 = 0.0f;	// �J�����̕��s�ړ������ׂ͒�

			// �l�[���v���[�g�̃��[���h���W����
			{
				// �l�[���v���[�g��u�������ʒu��ݒ肷��
				D3DXVECTOR3 v(_vModelPos[i].x, _vModelPos[i].y + 1.0f, _vModelPos[i].z);

				// ��U�l�[���v���[�g�̈ʒu�𓧎����e�܂ōs��
				D3DXVec3TransformCoord(&v, &v, &mView);
				D3DXVec3TransformCoord(&v, &v, &mProj);

				// ���ˉe�ϊ��s������
				D3DXMATRIX mOrtho;
				float nearH = NEAR_Z * tan(FOV_ANGLE*0.5f) * 2.0f;
				float nearW = nearH * (float)SCREEN_W / (float)SCREEN_H;
				D3DXMatrixOrthoLH(&mOrtho, nearW, nearH, NEAR_Z, FAR_Z);					

				// ���ˉe�̋t�s��������āA���ˉe�����ꍇ�ɓ����ʒu�ɉf��l�ȃJ�������W�n�̍��W���v�Z����
				D3DXMATRIX mInvOrtho;
				D3DXMatrixInverse(&mInvOrtho, NULL, &mOrtho);
				D3DXVec3TransformCoord(&v, &v, &mInvOrtho);

				// �r���[�s��̋t�s��������āA���[���h���W�n�ɖ߂�
				D3DXMATRIX mInvView;
				D3DXMatrixInverse(&mInvView, NULL, &mView);
				D3DXVec3TransformCoord(&v, &v, &mInvView);

				// ��������[���h���W�Ƃ���
				D3DXMatrixTranslation(&mTrans, v.x, v.y, v.z);

				// ���ˉe�ϊ�����
				_device.GetInterface()->SetTransform(D3DTS_PROJECTION, &mOrtho);
			}

			mWorld = mRot * mBill * mTrans;
			_device.GetInterface()->SetTransform(D3DTS_WORLD, &mWorld);


			// ���C�e�B���O
			_device.GetInterface()->SetRenderState(D3DRS_LIGHTING, FALSE);

			// �e�N�X�`���Z�b�g
			_device.GetInterface()->SetTexture(0, _texture[i].GetTexture());

			// ���b�V���`��
			_plate[i].Render(_device);
			
			_device.GetInterface()->SetTexture(0, NULL);
		}

		// �V�[���`��I��
		_device.GetInterface()->EndScene();
	}

	// �V�[���̕\��
	hr = _device.GetInterface()->Present(NULL, NULL, NULL, NULL);
	if (hr == D3DERR_DEVICELOST) ;

	//
	_fps.Wait();
}


