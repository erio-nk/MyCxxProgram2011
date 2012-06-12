/*!
 	@file Direct3D/Device.h
 
 	Direct3Dデバイスクラス。
 
 	@author	Dila
 */
//! @addtogroup Direct3D Direct3D
//! @{

#pragma once

// D3Dバージョン
#define _D3D_VER_9	(9)		//! Direct3D 9
#define _D3D_VER_10	(10)	//! Direct3D 10 (とりあえず定義しただけ。)

#ifndef _D3D_VER
	#define _D3D_VER _D3D_VER_9	//! Direct3Dデフォルトバージョン
#endif

// include
#include <windows.h>

// Direct3D 9
#if _D3D_VER == _D3D_VER_9
	// include
	#include <d3d9.h>
	#include <d3dx9.h>

	// link
	#pragma comment(lib, "d3d9.lib")
	#ifdef _DEBUG
		#pragma comment(lib, "d3dx9d.lib")
	#else
		#pragma comment(lib, "d3dx9.lib")
	#endif

	// typedef
	typedef IDirect3D9 IDirect3D;	//! Direct3Dインタフェース
	typedef IDirect3DDevice9 IDirect3DDevice;	//! Direct3Dデバイスインタフェース

	typedef IDirect3DTexture9 IDirect3DTexture;		//! テクスチャ
	typedef IDirect3DSurface9 IDirect3DSurface;	//! サーフェイス
	
	typedef D3DMATERIAL9 D3DMATERIAL;	//! マテリアルクラス
	typedef D3DLIGHT9 D3DLIGHT;			//! ライトクラス
#endif


namespace Direct3D
{
	//! @addtogroup Direct3D Direct3D
	//! @{

	/*!
	 	Direct3Dデバイスクラス
	 
	 	Direct3Dデバイスの初期化時のお決まりな処理のみをまとめたクラス。
	 	詳細な処理は、デバイスへのポインタ(IDirect3DDevice9*)を取得して
	 	任意に処理する事。
	 
	 */
	class Device
	{
		IDirect3D* _pDirect3D;		//! Direct3Dオブジェクト
		IDirect3DDevice* _pDevice;	//! デバイス
		D3DPRESENT_PARAMETERS _PP;	//! プレゼンテーションパラメータ

	public:
		Device();

		~Device();

		/*!
			デバイス生成
		 	@param	hWnd	ウィンドウハンドル
		 */
		bool Create(HWND hWnd);

		/*!
		 	デバイス解放
		 */
		void Release();

		/*!
		 	Direct3Dオブジェクト取得
		 */
		IDirect3D* GetDirect3D() { return _pDirect3D; }

		/*!
		 	デバイスのインタフェース取得
		 */
		IDirect3DDevice* GetInterface() { return _pDevice; }

		/*!
		 	プレゼンテーションパラメータ取得
		 */
		const D3DPRESENT_PARAMETERS& GetPresentParameters() { return _PP; }


		/*!
		 	ビュー変換マトリクスセットアップ
		 */
		D3DXMATRIX* SetupViewMatrix(D3DXMATRIX* m, const D3DXVECTOR3& vEye, const D3DXVECTOR3& vAt);


		/*!
		 	透視投影変換マトリクスセットアップ
		 */
		D3DXMATRIX* SetupPerspectiveMatrix(D3DXMATRIX* m, float fovV,
			float zn = 1.0f, float zf = 1000.0f, float cx = 0.0f, float cy = 0.0f);

		/*!
			正射影変換マトリクスセットアップ
		 */
		//D3DXMATRIX* SetupOrthoMatrix(D3DXMATRIX* m, float fovV,
		//	float zn = 1.0f, float zf = 1000.0f, float cx = 0.0f, float cy = 0.0f);

	};	// end class Device


	//! @}
}	// end namespace Direct3D

//! @}
