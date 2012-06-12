#include "Windows/Main.h"
#include "Windows/Window.h"

#include "Game/FpsTimer.h"

#include "Direct3D/Device.h"
#include "Direct3D/Mesh.h"
#include "Direct3D/ExtensionPlus.h"


class MyWindow : public Window
{
	Direct3D::Device _device;
	Direct3D::Mesh _mesh;
	D3DLIGHT _light;

	FpsTimer _fps;

public:
	static const int CX = 400;
	static const int CY = 400;

	MyWindow() :
		_device(),
		_mesh(),
		_light(),
		_fps()
	{
	}

	~MyWindow()
	{
	}

	bool Initialize()
	{
		Show();

		_device.Create(*this);

		D3DXCreateTeapot(_device.GetInterface(), &_mesh.GetMesh(), NULL);

		// ライト
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


		return true;
	}

	void Finalize()
	{
		_mesh.Release();
		_device.Release();
	}

	void Run()
	{
		_device.GetInterface()->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
			D3DCOLOR_XRGB(0,0,80), 1.0f, 0);

		HRESULT hr = _device.GetInterface()->BeginScene();
		if (SUCCEEDED(hr)) {
			// ワールド変換行列
			D3DXMATRIX mWorld;
			D3DXMatrixIdentity(&mWorld);
			_device.GetInterface()->SetTransform(D3DTS_WORLD, &mWorld);

			// ビュー変換行列
			D3DXMATRIX mView;
			D3DXVECTOR3 vEye(0.0f, 0.0f, -5.0f);
			D3DXVECTOR3 vAt(0.0f, 0.0f ,0.0f);
			_device.GetInterface()->SetTransform(D3DTS_VIEW,
				_device.SetupViewMatrix(&mView, vEye, vAt));

			// 透視変換行列
			D3DXMATRIX mProj;
			_device.GetInterface()->SetTransform(D3DTS_PROJECTION,
				_device.SetupProjectionMatrix(&mProj, D3DXToRadian(60.0f)));


			_device.GetInterface()->SetRenderState(D3DRS_LIGHTING, TRUE);
			_device.GetInterface()->SetLight(0, &_light);
			_device.GetInterface()->LightEnable(0, TRUE);

			_mesh.Render(_device);


			_D3DXDrawLine(_device.GetInterface(),
				D3DXVECTOR3(-100.0f, 0.0f, 0.0f),
				D3DXVECTOR3(100.0f, 0.0f, 0.0f),
				D3DCOLOR_XRGB(0xff, 0, 0));

            _device.GetInterface()->EndScene();
		}

		_device.GetInterface()->Present(NULL, NULL, NULL, NULL);

		_fps.Wait();
	}

};	// end class Window


void Main()
{
	MyWindow wnd;

	wnd.Create(_T("Direct3D Test"), 0, 0, MyWindow::CX, MyWindow::CY,
		Window::Option::Centering|
		Window::Option::ClientSize|
		Window::Option::StandardStyle|
		Window::Option::BGNull);

	wnd.Initialize();

	MainLoop(&wnd, &MyWindow::Run);

	wnd.Finalize();

}

