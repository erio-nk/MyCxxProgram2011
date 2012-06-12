/**
 *	Bullet.h
 *	�e�N���X�B
 *	@author	Dila
 */
#pragma once

#include "Project.h"

//
#define BULLET_RADIUS	(2.0f)


/**
 *	Bullet
 */
class Bullet
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vDir;
	float speed, g;

	bool hit;

	// �`��p
	static LPD3DXMESH pMeshBullet;	// ���b�V��
	static D3DMATERIAL9 material;	// �}�e���A��

public:
	Bullet(const D3DXVECTOR3& vPos, const D3DXVECTOR3& vDir, float speed)
	{
		this->vPos = vPos;
		this->vDir = vDir;
		this->speed = speed;
		g = 0.0f;
		hit = false;
	}

	static bool Initialize();
	static void Finalize();


	void Update();

	void Draw();

	
	bool CanErase();
	void OnErase();

	bool TestTargets();
};
