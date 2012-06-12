/**
 *	MyApp.h
 *	動く球と動くBOXの衝突検知サンプル。
 */

#pragma once

#include "Windows/Main.h"
#include "Windows/Window.h"
#include "Game/FpsTimer.h"
#include "Game/Test3D.h"

//
#include <d3dx9.h>
#include <dxerr9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

//
#define SCREEN_W 640
#define SCREEN_H 480

// 球
#define SPHERE_RADIUS	(10)

// BOX
#define BOX_W	(10)
#define BOX_H	(20)
#define BOX_D	(10)


//
#define RELEASE(x) if(x){x->Release(); x=NULL;}


/**
 *	MyApp
 */
class MyApp
{
	static MyApp theInstance;

	Window wnd;

	FpsTimer fps;

	//
	LPDIRECT3D9 pD3D;
	D3DPRESENT_PARAMETERS D3DPP;
	LPDIRECT3DDEVICE9 pD3DDev;	// デバイス

	D3DLIGHT9 light;

	LPD3DXMESH pMeshSph;
	D3DMATERIAL9 matSph;

	LPD3DXMESH pMeshBox;
	D3DMATERIAL9 matBox;

	// カメラ
	D3DXVECTOR3 vCamera;

	// 立体座標・姿勢
	Vector3 vSph;		// 球の位置
	Vector3 vSphSpd;	// 球の速度

	Vector3 vBox;		// BOXの位置
	Vector3 vBoxSpd;	// BOXの速度

public:
	MyApp() {}

	static MyApp& GetInstance() { return theInstance; }

	bool Initialize();
	bool Finalize();

	void Update();
};

