/**
	@file MyApp.h

	Direct3D10�A�v���P�[�V�����̃X�P���g���v���O����

	@author Dila
 */

#pragma once

#include "Windows/Main.h"
#include "Windows/Window.h"
#include "Game/FpsTimer.h"

// Direct3D10
#include <d3d10.h>

#pragma comment(lib, "d3d10.lib")

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

	// Direct3D10 objects
	ID3D10Device* _pDevice;			// �f�o�C�X
	IDXGISwapChain* _pSwapChain;	// �X���b�v�`�F�C���i���o�b�N�o�b�t�@�Ƃ��̉�ʂ̎��j
	ID3D10RenderTargetView* _pRTView;	// �`��Ώ�

public:
	MyApp() :
		_pDevice(NULL),
		_pSwapChain(NULL),
		_pRTView(NULL)
	{}

	static MyApp& GetInstance() { return theInstance; }

	bool Initialize();
	bool Finalize();

	void Update();
};
