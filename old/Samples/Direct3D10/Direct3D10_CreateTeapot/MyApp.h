/**
	@file MyApp.h

	Direct3D10�Ńe�B�[�|�b�g�i���b�V���j������ĕ\������v���O����

	@author Dila
 */

#pragma once

#include "Windows/Main.h"
#include "Windows/Window.h"
#include "Game/FpsTimer.h"

// Direct3D10
#include <d3d10_1.h>
#include <d3d10.h>
#include <d3dx10.h>

#pragma comment(lib, "d3d10.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment( lib, "dxerr.lib" )

// DXUT
#include <DXUT/Core/DXUT.h>
#include <DXUT/Optional/DXUTShapes.h>	// for DXUTCreateTeapot()

#ifdef _DEBUG
	#pragma comment(lib, "DXUTd.lib")
	#pragma comment(lib, "DXUTOptd.lib")
#else
	#pragma comment(lib, "DXUT.lib")
	#pragma comment(lib, "DXUTOpt.lib")
#endif
#pragma comment(lib, "comctl32.lib")	// for InitCommonControls()


// �X�N���[���T�C�Y
#define SCREEN_W 640
#define SCREEN_H 480

// ����}�N��
#define RELEASE(x) if(x){x->Release(); x=NULL;}


/**
 	MyApp
 */
class MyApp
{
	static MyApp theInstance;

	Window _wnd;

	FpsTimer _fps;

	Timer _timer;

	// Direct3D10 objects
	ID3D10Device* _pDevice;			// �f�o�C�X
	IDXGISwapChain* _pSwapChain;	// �X���b�v�`�F�C��
	ID3D10RenderTargetView* _pBackBuf;	// �o�b�N�o�b�t�@�̃r���[
	ID3D10DepthStencilView* _pZBuf;		// �[�x/�X�e���V���o�b�t�@�̃r���[

	ID3DX10Mesh* _pMesh;	// ���b�V��
	ID3D10Effect* _pEffect;	// �G�t�F�N�g
	ID3D10InputLayout* _pLayout;	// ���_�f�[�^���C�A�E�g

public:
	MyApp() :
		_pDevice(NULL),
		_pSwapChain(NULL),
		_pBackBuf(NULL),
		_pZBuf(NULL),

		_pMesh(NULL),
		_pEffect(NULL),
		_pLayout(NULL)
	{}

	static MyApp& GetInstance() { return theInstance; }

	bool Initialize();
	bool Finalize();

	void Update();
};
