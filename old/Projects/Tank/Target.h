/**
 *	Target.h
 *	的クラス。
 *	@author	Dila
 */
#pragma once

#include "Project.h"

//
#define TARGET_W	(5.0f)
#define TARGET_H	(15.0f)
#define TARGET_D	(5.0f)

/**
 *	Target
 */
class Target
{
public:
	enum STATE {
		STATE_STARTING,
		STATE_NORMAL,
		STATE_HIT,
		STATE_END,
	};

private:
	D3DXVECTOR3 vPos;
	
	float hitRot;	// 傾き
	D3DXVECTOR3 vHitDir;	// 傾く方向

	STATE state;

	// 描画用
	static LPD3DXMESH pMeshTarget;	// メッシュ
	static D3DMATERIAL9 material;	// マテリアル

public:
	Target(const D3DXVECTOR3& vPos) {
		this->vPos = vPos;
		state = STATE_STARTING;
		hitRot = 0.0f;
		vHitDir.x = vHitDir.y = 0.0f, vHitDir.z = 0.0f;
	};

	static bool Initialize();
	static void Finalize();

	void Update();

	void Draw();

	bool CanErase();
	void OnErase();

	bool CanHit() { return state == STATE_NORMAL; }

	const D3DXVECTOR3& GetPos() { return vPos; }

	void OnHitBullet(const D3DXVECTOR3& vHitDir);
};
