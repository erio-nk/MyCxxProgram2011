/**
 *	GameMain.h
 *	ゲームメイン。
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
	D3DXVECTOR3 vPos;	// めんどいのでpublic

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
	LPDIRECT3DDEVICE9 pD3DDev;	// デバイス

	D3DLIGHT9 light;

	//
	D3DXVECTOR3 vEye;	// カメラ
	D3DXVECTOR3 vAt;	// 注視点

	Ground ground;		// 地面

	Tank tank;			// 戦車

	std::list<Bullet> bullets;	// 弾リスト

	std::list<Target> targets;	// 的リスト
	int putTargetInter;			// 的出現インター

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
