/**
 *	MyApp.h
 *	ティーポットを描画するサンプル。
 */

#pragma once

#include "Windows/Main.h"
#include "Windows/Window.h"
#include "Game/FpsTimer.h"

//
#include <d3dx9.h>
#include <dxerr9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

//
#define SCREEN_W 640
#define SCREEN_H 480

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

	LPD3DXMESH pTeapot;
	D3DMATERIAL9 mat;
	
	D3DLIGHT9 light;

public:
	MyApp() {}

	static MyApp& GetInstance() { return theInstance; }

	bool Initialize();
	bool Finalize();

	void Update();
};
