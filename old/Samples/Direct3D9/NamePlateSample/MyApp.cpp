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
	const float FOV_ANGLE = D3DXToRadian(80);	// 視野角
	const float NEAR_Z = 1.0f;	// ビューボリュームの近平面までの距離
	const float FAR_Z = 15.0f;	// ビューボリュームの遠平面までの距離

	const int NAMEPLATE_TEXTURE_SIZE = 64;	// ネームプレートのテクスチャのサイズ
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
		"NamePlateSample",
		0, 0, SCREEN_W, SCREEN_H);
	_wnd.Show();

	// フレームレート設定
	_fps.SetFrameRate(30);

	// Direct3D初期化
	_device.Create(_wnd.GetHandle());

	// メッシュ・モデル・ネームプレート
	for (int i = 0; i < MODEL_COUNT; ++i) {
		// モデル
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

		// ネームプレート
		D3DXPlus::CreateBoard(_device.GetInterface(), 0.25f, 0.25f, &_plate[i].GetMesh());
		_texture[i].Create(_device.GetInterface(), NAMEPLATE_TEXTURE_SIZE, NAMEPLATE_TEXTURE_SIZE);
		
		const D3DMATERIAL& m = _mesh[i].GetMaterial();
		_plate[i].SetDiffuseColor(m.Diffuse.r, m.Diffuse.g, m.Diffuse.b);
		_plate[i].SetAmbientColor(m.Ambient.r, m.Ambient.g, m.Ambient.b);
	}
	
	// ライト
	_light.SetDirection(0.0f, 1.0f, 2.0f);
	_light.SetPosition(0.0f, -2.0f, -2.0f);

	_device.GetInterface()->SetLight(0, &_light.GetLight());
	_device.GetInterface()->LightEnable(0, TRUE);

	// Zバッファ
	_device.GetInterface()->SetRenderState(D3DRS_ZENABLE, TRUE);

	// カリング
	_device.GetInterface()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	// アルファ有効
	_device.GetInterface()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	_device.GetInterface()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    _device.GetInterface()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// 視点
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
	// シーンクリア
	_device.GetInterface()->Clear(
		0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(0,0,80), 1.0f, 0);

	// シーン描画開始
	HRESULT hr = _device.GetInterface()->BeginScene();
	if (SUCCEEDED(hr)) {
		//// シーン描画

		static bool bTexInit = false;
		if (!bTexInit) {
			// テクスチャにテキストをレンダリング
			Direct3D::Text text;
			text.Create(_device);

			for (int i = 0; i < MODEL_COUNT; ++i) {
				// レンダリング開始
				Direct3D::RenderTexture::BeginRender begin(_texture[i]);
				
				// テクスチャのクリア
				_device.GetInterface()->Clear(
					0, NULL, D3DCLEAR_TARGET,
					D3DCOLOR_ARGB(0,0,0,0), 1.0f, 0);
				
				// テキスト描画
				text.Render(_T("hoge"), NAMEPLATE_TEXTURE_SIZE/2, 0, _mesh[i].GetDiffuseColor(), DT_CENTER);				
			}

			bTexInit = true;
		}

		// 視点をぐるぐる回す
		D3DXMATRIX m;
		D3DXMatrixRotationY(&m, D3DXToRadian(1.0f));	// Y軸周りにぐるぐる
		D3DXVec3TransformCoord(&_vEye, &_vEye, &m);
		
		// ビュー変換
		D3DXMATRIX mView;
		D3DXVECTOR3 vAt(0.0f, 0.0f ,0.0f);
		D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtLH(&mView, &_vEye, &vAt, &vUp);
		_device.GetInterface()->SetTransform(D3DTS_VIEW, &mView);
		
		// 射影変換行列
		D3DXMATRIX mProj;
		D3DXMatrixPerspectiveFovLH(&mProj,
			FOV_ANGLE,
			(float)SCREEN_W/(float)SCREEN_H,
			NEAR_Z, FAR_Z);

		// 描画
		for (int i = 0; i < MODEL_COUNT; ++i) {
			//// モデル描画
			D3DXMATRIX mWorld;

			// 平行移動
			D3DXMATRIX mTrans;
			D3DXMatrixTranslation(&mTrans, _vModelPos[i].x, _vModelPos[i].y, _vModelPos[i].z);

			mWorld = mTrans;
			_device.GetInterface()->SetTransform(D3DTS_WORLD, &mWorld);

			// 射影変換
			_device.GetInterface()->SetTransform(D3DTS_PROJECTION, &mProj);

			// ライティング
			_device.GetInterface()->SetRenderState(D3DRS_LIGHTING, TRUE);

			// メッシュ描画
			_mesh[i].Render(_device);


			//// ネームプレート描画

			// 垂直に立てる
			D3DXMATRIX mRot;
			D3DXMatrixRotationX(&mRot, D3DXToRadian(90));

			// ビルボードにする
			D3DXMATRIX mBill;
			D3DXMatrixInverse(&mBill, NULL, &mView);
			mBill._41 = mBill._42 = mBill._43 = 0.0f;	// カメラの平行移動成分は潰す

			// ネームプレートのワールド座標調整
			{
				// ネームプレートを置きたい位置を設定する
				D3DXVECTOR3 v(_vModelPos[i].x, _vModelPos[i].y + 1.0f, _vModelPos[i].z);

				// 一旦ネームプレートの位置を透視投影まで行う
				D3DXVec3TransformCoord(&v, &v, &mView);
				D3DXVec3TransformCoord(&v, &v, &mProj);

				// 正射影変換行列を作る
				D3DXMATRIX mOrtho;
				float nearH = NEAR_Z * tan(FOV_ANGLE*0.5f) * 2.0f;
				float nearW = nearH * (float)SCREEN_W / (float)SCREEN_H;
				D3DXMatrixOrthoLH(&mOrtho, nearW, nearH, NEAR_Z, FAR_Z);					

				// 正射影の逆行列をかけて、正射影した場合に同じ位置に映る様なカメラ座標系の座標を計算する
				D3DXMATRIX mInvOrtho;
				D3DXMatrixInverse(&mInvOrtho, NULL, &mOrtho);
				D3DXVec3TransformCoord(&v, &v, &mInvOrtho);

				// ビュー行列の逆行列をかけて、ワールド座標系に戻す
				D3DXMATRIX mInvView;
				D3DXMatrixInverse(&mInvView, NULL, &mView);
				D3DXVec3TransformCoord(&v, &v, &mInvView);

				// これをワールド座標とする
				D3DXMatrixTranslation(&mTrans, v.x, v.y, v.z);

				// 正射影変換する
				_device.GetInterface()->SetTransform(D3DTS_PROJECTION, &mOrtho);
			}

			mWorld = mRot * mBill * mTrans;
			_device.GetInterface()->SetTransform(D3DTS_WORLD, &mWorld);


			// ライティング
			_device.GetInterface()->SetRenderState(D3DRS_LIGHTING, FALSE);

			// テクスチャセット
			_device.GetInterface()->SetTexture(0, _texture[i].GetTexture());

			// メッシュ描画
			_plate[i].Render(_device);
			
			_device.GetInterface()->SetTexture(0, NULL);
		}

		// シーン描画終了
		_device.GetInterface()->EndScene();
	}

	// シーンの表示
	hr = _device.GetInterface()->Present(NULL, NULL, NULL, NULL);
	if (hr == D3DERR_DEVICELOST) ;

	//
	_fps.Wait();
}


