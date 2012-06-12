/**
 *	MyApp.h
 *	�������Ɠ���BOX�̏Փˌ��m�T���v���B
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

// ��
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
	LPDIRECT3DDEVICE9 pD3DDev;	// �f�o�C�X

	D3DLIGHT9 light;

	LPD3DXMESH pMeshSph;
	D3DMATERIAL9 matSph;

	LPD3DXMESH pMeshBox;
	D3DMATERIAL9 matBox;

	// �J����
	D3DXVECTOR3 vCamera;

	// ���̍��W�E�p��
	Vector3 vSph;		// ���̈ʒu
	Vector3 vSphSpd;	// ���̑��x

	Vector3 vBox;		// BOX�̈ʒu
	Vector3 vBoxSpd;	// BOX�̑��x

public:
	MyApp() {}

	static MyApp& GetInstance() { return theInstance; }

	bool Initialize();
	bool Finalize();

	void Update();
};

