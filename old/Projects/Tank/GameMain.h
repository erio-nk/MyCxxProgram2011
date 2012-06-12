/**
 *	GameMain.h
 *	�Q�[�����C���B
 */

#pragma once

#include "Windows/Main.h"
#include "Windows/Window.h"
#include "Game/FpsTimer.h"

#include "Project.h"
#include "Ground.h"
#include "Tank.h"
#include "Bullet.h"
#include "Target.h"

#include <list>

//
#define PUT_TARGET_INTER	(60)


/**
 *	GameWindow
 */
class GameWindow : public Window
{
public:
	D3DXVECTOR3 vPos;	// �߂�ǂ��̂�public

	GameWindow() {
		AddMessageHandler<OnLButtonDown>(WM_LBUTTONDOWN);
		AddMessageHandler<OnRButtonDown>(WM_RBUTTONDOWN);
		AddMessageHandler<OnMouseMove>(WM_MOUSEMOVE);
	}

	struct OnLButtonDown : public Window::OnMouseMessage {
		bool Handle();
	};
	struct OnRButtonDown : public Window::OnMouseMessage {
		bool Handle();
	};
	struct OnMouseMove : public Window::OnMouseMessage {
		bool Handle();
	};
};


/**
 *	GameMain
 */
class GameMain
{
	static GameMain theInstance;

	GameWindow wnd;

	FpsTimer fps;

	//
	LPDIRECT3D9 pD3D;
	D3DPRESENT_PARAMETERS D3DPP;
	LPDIRECT3DDEVICE9 pD3DDev;	// �f�o�C�X

	D3DLIGHT9 light;

	//
	D3DXVECTOR3 vEye;	// �J����
	D3DXVECTOR3 vAt;	// �����_

	Ground ground;		// �n��

	Tank tank;			// ���

	std::list<Bullet> bullets;	// �e���X�g

	std::list<Target> targets;	// �I���X�g
	int putTargetInter;			// �I�o���C���^�[

public:
	GameMain() {}

	static GameMain& GetInstance() { return theInstance; }
	static LPDIRECT3DDEVICE9 GetD3DDevice() { return theInstance.pD3DDev; }

	bool Initialize();
	bool Finalize();

	void Update();

	void SetCamera(const D3DXVECTOR3& vEye, const D3DXVECTOR3& vAt);
	
	Tank& GetTank() { return tank; }

	void PushBullet(const Bullet& bul);
	void PushTarget(const Target& tgt);

	std::list<Target>& GetTargets() { return targets; }
};
