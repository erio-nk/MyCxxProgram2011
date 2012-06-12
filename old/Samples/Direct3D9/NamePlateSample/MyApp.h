/*!
 	MyApp.h
 	MMORPGなんかでよくある頭上名前表示の板ポリを作って表示するサンプル。
 */

#pragma once

#include "Windows/Main.h"
#include "Windows/Window.h"

#include "Game/FpsTimer.h"

#include "Direct3D/Device.h"
#include "Direct3D/Mesh.h"
#include "Direct3D/Light.h"
#include "Direct3D/Texture.h"
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
	Direct3D::Mesh _mesh[MODEL_COUNT];
	Direct3D::DirectionalLight _light;

	Direct3D::Mesh _plate[MODEL_COUNT];
	Direct3D::RenderTexture _texture[MODEL_COUNT];

	//
	D3DXVECTOR3 _vEye;

	D3DXVECTOR3 _vModelPos[MODEL_COUNT];

public:
	MyApp() :
		_fps(),
		_device(),
		_mesh(),
		_light()
	{}

	static MyApp& GetInstance() { return theInstance; }

	bool Initialize();
	bool Finalize();

	void Update();
};
