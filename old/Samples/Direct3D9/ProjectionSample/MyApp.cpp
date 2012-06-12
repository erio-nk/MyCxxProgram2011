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
	const float NEAR_Z = 3.0f;	// ビューボリュームの近平面までの距離
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
		"ProjectionSample2",
		0, 0, SCREEN_W, SCREEN_H);
	_wnd.Show();

	// フレームレート設定
	_fps.SetFrameRate(30);

	// Direct3D初期化
	_device.Create(_wnd.GetHandle());

	// メッシュ
	D3DXCreateTeapot(_device.GetInterface(), &_mesh.GetMesh(), NULL);
	_mesh.SetDiffuseColor(1.0f, 1.0f, 0.0f, 1.0f);
	
	// ライティング
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

	// Zバッファ
	_device.GetInterface()->SetRenderState(D3DRS_ZENABLE, TRUE);	// 有効

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
	// 射影変換タイプ切り替え
	if (LOBYTE(GetAsyncKeyState(VK_SPACE))) {
		if (++_projType > 2) _projType = 0; 
	}

	// シーンクリア
	_device.GetInterface()->Clear(
		0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(0,0,80), 1.0f, 0);

	// シーン描画開始
	HRESULT hr = _device.GetInterface()->BeginScene();
	if (SUCCEEDED(hr)) {
		//// シーン描画

		// ワールド変換行列
		D3DXMATRIX mWorld;

		static float theta = 0.0f;
		theta += 4.0f;	// Y軸周りにぐるぐる
		D3DXMatrixRotationY(&mWorld, D3DXToRadian(theta));

		_device.GetInterface()->SetTransform(D3DTS_WORLD, &mWorld);

		// ビュー変換行列
		D3DXMATRIX mView;
		D3DXVECTOR3 vEye(0.0f, 0.0f, -5.0f);
		D3DXVECTOR3 vAt(0.0f, 0.0f ,0.0f);
		D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtLH(&mView, &vEye, &vAt, &vUp);
		_device.GetInterface()->SetTransform(D3DTS_VIEW, &mView);

		// 射影変換行列
		D3DXMATRIX mProj;

		switch (_projType) {
			case 0:	// 透視投影変換・その１
			{
				D3DXMatrixPerspectiveFovLH(&mProj,
					FOV_ANGLE,
					(float)SCREEN_W/(float)SCREEN_H,
					NEAR_Z, FAR_Z);
				break;
			}
			case 1:	// 透視投影変換・その２
			{
				float nearH = NEAR_Z * tan(FOV_ANGLE/2.0f) * 2.0f;			// 近平面の縦幅 = nearZ * tan(Yの視野角/2) * 2
				float nearW = nearH * (float)SCREEN_W / (float)SCREEN_H;	// 近平面の横幅 = 近平面の縦幅 * アスペクト比
				D3DXMatrixPerspectiveLH(&mProj,
					nearW,
					nearH,
					NEAR_Z, FAR_Z);

				break;
			}
			case 2:	// 正射影変換
			{
				float nearH = NEAR_Z * tan(FOV_ANGLE/2.0f) * 2.0f;			// 近平面の縦幅 = nearZ * tan(Yの視野角/2) * 2
				float nearW = nearH * (float)SCREEN_W / (float)SCREEN_H;	// 近平面の横幅 = 近平面の縦幅 * アスペクト比
				D3DXMatrixOrthoLH(&mProj,
					nearW,
					nearH,
					NEAR_Z, FAR_Z);

				break;
			}
		}
		_device.GetInterface()->SetTransform(D3DTS_PROJECTION, &mProj);

		// メッシュ描画
		_mesh.Render(_device);

		// シーン描画終了
		_device.GetInterface()->EndScene();
	}

	// シーンの表示
	hr = _device.GetInterface()->Present(NULL, NULL, NULL, NULL);
	if (hr == D3DERR_DEVICELOST) ;

	//
	_fps.Wait();
}


