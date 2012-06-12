/**
 *	Tank.h
 *	戦車クラス。
 *	@author	Dila
 */
#pragma once

#include "Project.h"

// 形状の定数
#define TANK_BODY_W	(10.0f)
#define TANK_BODY_H	(3.0f)
#define TANK_BODY_D	(15.0f)

#define TANK_HEAD_W	(7.0f)
#define TANK_HEAD_H	(2.5f)
#define TANK_HEAD_D	(10.0f)

#define TANK_GUN_W (1.0f)
#define TANK_GUN_H (1.0f)
#define TANK_GUN_D (5.0f)


/**
 *	Tank
 */
class Tank
{
	D3DXVECTOR3 vPos;

	float speed;
	float turn;

	float rotBody;			// 本体の向き
	float rotHead;			// 頭の向き

	int shotInter;			// 発射インター

	// 描画用
	LPD3DXMESH pMeshBody;	// 本体
	LPD3DXMESH pMeshHead;	// 頭
	LPD3DXMESH pMeshGun;	// 砲塔

	D3DMATERIAL9 material;	// マテリアル

public:
	Tank();

	bool Initialize();
	void Finalize();

	void Update();
	void Draw();

	void Forward();
	void Back();
	void TurnLeft();
	void TurnRight();
	void RotateHead(float angle);

	void Shot();

	void SetCamera();
};
