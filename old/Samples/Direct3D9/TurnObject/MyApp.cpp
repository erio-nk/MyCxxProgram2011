/*!
 	MyApp.cpp
 */

#include "MyApp.h"


//
MyApp MyApp::theInstance;

//
namespace
{
	const float FOV_ANGLE = D3DXToRadian(60);	// 視野角
	const float NEAR_Z = 1.0f;	// ビューボリュームの近平面までの距離
	const float FAR_Z = 15.0f;	// ビューボリュームの遠平面までの距離
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
	// ウィンドウ初期化
	_wnd.Create(
		"TurnObject",
		0, 0, SCREEN_W, SCREEN_H);
	_wnd.Show();

	// フレームレート設定
	_fps.SetFrameRate(30);

	// Direct3D初期化
	_device.Create(_wnd.GetHandle());

	// メッシュ
	D3DXCreateTeapot(_device.GetInterface(), &_mesh.GetMesh(), NULL);
	_mesh.SetDiffuseColor(1.0f, 0.0f, 1.0f);
	_mesh.SetAmbientColor(0.2f, 0.0f, 0.2f);

	// ライト
	_light.SetDirection(0.0f, 1.0f, 2.0f);
	_light.SetPosition(0.0f, -2.0f, -2.0f);

	_device.GetInterface()->SetRenderState(D3DRS_LIGHTING, TRUE);
	_device.GetInterface()->SetLight(0, &_light.GetLight());
	_device.GetInterface()->LightEnable(0, TRUE);

	// Zバッファ
	_device.GetInterface()->SetRenderState(D3DRS_ZENABLE, TRUE);

	// カリング
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
		//// 旋回：姿勢ベクトルの計算

		// 現在の姿勢ベクトルvRotと目標の姿勢ベクトルvTurnの外積を取り、回転軸vAxisとする
		D3DXVECTOR3 vAxis;
		D3DXVec3Cross(&vAxis, &_vRotation, &_vTurn);
		if (D3DXVec3LengthSq(&vAxis) == 0.0f) {
			_bTurning = false;	// 外積がゼロなら、既に同じ姿勢ベクトルになっているので旋回終了
			return;
		}
		D3DXVec3Normalize(&vAxis, &vAxis);
		
		// 回転軸から、適当な角度で回転するマトリクスを計算する
		D3DXMATRIX mRot;
		D3DXMatrixRotationAxis(&mRot, &vAxis, D3DXToRadian(2.0f));

		// vRotを少し回転させたベクトルvRot'を計算する
		D3DXVECTOR3 vRotDash;
		D3DXVec3TransformCoord(&vRotDash, &_vRotation, &mRot);

		// vRot'のvTurn外積を取り、次フレームの回転軸vAxis'を得る
		D3DXVECTOR3 vAxisDash;
		D3DXVec3Cross(&vAxisDash, &vRotDash, &_vTurn);
		if (D3DXVec3LengthSq(&vAxisDash) == 0.0f) {
			_bTurning = false;	// 外積ゼロなら、ベクトルが重なったので旋回終了
			_vRotation = _vTurn;
			return;
		}
		D3DXVec3Normalize(&vAxisDash, &vAxisDash);

		// vAxisとvAxis'の内積を取り、軸の向きを比較する
		if (D3DXVec3Dot(&vAxis, &vAxisDash) < 0.0f) {
			// 内積が負＝軸が反対向きになったという事は、
			// vRotとvTurnの左右関係と、vRot'とvTurnの左右関係が変わった
			// 即ちvTurnを超えたという事になるので、旋回終了
			_bTurning = false;
			_vRotation = _vTurn;
			return;
		}

		_vRotation = vRotDash;
	}


	//// 旋回操作
	#define ANGLE 30.0f

	if (LOBYTE(GetAsyncKeyState(VK_LEFT))) {
		// 回転後のベクトルを設定する
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
	//// シーンアップデート

	// シーンクリア
	_device.GetInterface()->Clear(
		0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(0,0,80), 1.0f, 0);

	// シーン描画開始
	HRESULT hr = _device.GetInterface()->BeginScene();
	if (SUCCEEDED(hr)) {
		//// シーン描画
		
		// ビュー変換
		D3DXMATRIX mView;
		D3DXVECTOR3 vEye(0.0f, 2.0f, -5.0f);
		D3DXVECTOR3 vAt(0.0f, 0.0f ,0.0f);
		D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtLH(&mView, &vEye, &vAt, &vUp);
		_device.GetInterface()->SetTransform(D3DTS_VIEW, &mView);
		
		// 射影変換
		D3DXMATRIX mProj;
		D3DXMatrixPerspectiveFovLH(&mProj,
			FOV_ANGLE,
			(float)SCREEN_W/(float)SCREEN_H,
			NEAR_Z, FAR_Z);
		_device.GetInterface()->SetTransform(D3DTS_PROJECTION, &mProj);

		// ワールド変換
		D3DXMATRIX mWorld, mRot, mKuti;
		D3DXMatrixIdentity(&mWorld);
		
		mWorld *= *D3DXMatrixRotationY(&mKuti, D3DXToRadian(-90));	// 口の向きを(1,0,0)にしたいので-90度回転
		
		mWorld *= *D3DXPlus::MatrixRotationVector(&mRot, &_vRotation);


		_device.GetInterface()->SetTransform(D3DTS_WORLD, &mWorld);

		// メッシュ描画
		_mesh.Render(_device);

		// 向き描画
		D3DXPlus::DrawLine(_device.GetInterface(), D3DXVECTOR3(0,0,0), _vRotation * 2.0f, 
			D3DCOLOR_XRGB(0xff, 0x00, 0xff), D3DCOLOR_XRGB(0x00, 0xff, 0xff));

		if (_bTurning) {
			D3DXPlus::DrawLine(_device.GetInterface(), D3DXVECTOR3(0,0,0), _vTurn * 2.0f, 
				D3DCOLOR_XRGB(0xff, 0x00, 0xff), D3DCOLOR_XRGB(0xff, 0xff, 0x00));
		}

		// シーン描画終了
		_device.GetInterface()->EndScene();
	}

	// シーンの表示
	hr = _device.GetInterface()->Present(NULL, NULL, NULL, NULL);
	if (hr == D3DERR_DEVICELOST) ;
}


