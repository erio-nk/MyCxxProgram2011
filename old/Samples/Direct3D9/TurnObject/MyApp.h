/*!
 	MyApp.h
 	目標の姿勢ベクトルに徐々に近づけながら旋回するサンプル。
 */

#pragma once

#include "Windows/Main.h"
#include "Windows/Window.h"

#include "Game/FpsTimer.h"

#include "Direct3D/Device.h"
#include "Direct3D/Mesh.h"
#include "Direct3D/Light.h"
#include "Direct3D/ExtensionPlus.h"

//
#include <d3dx9.h>
#include <dxerr9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

//
#define SCREEN_W (float)640
#define SCREEN_H (float)480

//
#define MODEL_COUNT	4

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
	Direct3D::DirectionalLight _light;

	bool _bTurning;
	D3DXVECTOR3 _vRotation;
	D3DXVECTOR3 _vTurn;

public:
	MyApp() :
		_fps(),
		_device(),
		_mesh(),
		_light(),
		_bTurning(false),
		_vRotation(1.0f, 0.0f, 0.0f) 
	{}

	static MyApp& GetInstance() { return theInstance; }

	bool Initialize();
	bool Finalize();

	void Update();

	void UpdateFrame();
	void DrawFrame();
};
