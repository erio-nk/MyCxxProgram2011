/**
 *	Project.h
 *	プロジェクト共用ヘッダファイル。
 */

#pragma once

//
#include <stdio.h>
#include <d3dx9.h>
#include <dxerr9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

// スクリーンサイズ
#define SCREEN_W 640
#define SCREEN_H 480

// 解放マクロ
#define RELEASE(x) if(x){x->Release(); x=NULL;}

// デバッグプリント
#ifdef _DEBUG
	#define PRINTF printf
#else
	#define PRINTF __noop
#endif


/**
 *	MYVERTEX	
 */
struct MYVERTEX {
	D3DXVECTOR3 v;	// 座標
	D3DXVECTOR3 n;	// 法線
};

#define D3DFVF_MYVERTEX	(D3DFVF_XYZ|D3DFVF_NORMAL)

