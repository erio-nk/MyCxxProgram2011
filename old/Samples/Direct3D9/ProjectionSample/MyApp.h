/*!
 	MyApp.h
 	透視投影変換・正射影変換のサンプル。
 */

#pragma once

#include "Windows/Main.h"
#include "Windows/Window.h"

#include "Game/FpsTimer.h"

#include "Direct3D/Device.h"
#include "Direct3D/Mesh.h"
#include "Direct3D/ExtensionPlus.h"

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


/*!
 	MyApp
 */
class MyApp
{
	static MyApp theInstance;

	Window _wnd;

	FpsTimer _fps;

	//
	Direct3D::Device _device;
	Direct3D::Mesh _mesh;
	D3DLIGHT _light;

	//
	int _projType;

public:
	MyApp() :
		_fps(),
		_device(),
		_mesh(),
		_light(),
		_projType(0)
	{}

	static MyApp& GetInstance() { return theInstance; }

	bool Initialize();
	bool Finalize();

	void Update();
};
