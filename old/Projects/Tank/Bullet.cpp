/**
 *	Bullet.cpp
 *	@author	Dila
 */

#include "Ground.h"
#include "GameMain.h"
#include "Game/Test3D.h"

//
LPD3DXMESH Bullet::pMeshBullet = NULL;
D3DMATERIAL9 Bullet::material;


/**
 *	Initialize()
 *	描画用のメッシュやらを初期化する。
 */
bool Bullet::Initialize()
{
	// メッシュ準備
	LPDIRECT3DDEVICE9 pDev = GameMain::GetD3DDevice();
	HRESULT hr;
	
	hr = D3DXCreateSphere(pDev, 
		BULLET_RADIUS, 16, 16,
		&pMeshBullet, NULL);
	if (FAILED(hr)) {
		return false;
	}

	// マテリアル準備
	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	material.Diffuse.r = material.Ambient.r = 0.4f;
	material.Diffuse.g = material.Ambient.g = 0.4f;
	material.Diffuse.b = material.Ambient.b = 0.4f;
	material.Diffuse.a = material.Ambient.a = 1.0f;
	return true;
}

/**
 *	Finalize()
 */
void Bullet::Finalize()
{
	RELEASE(pMeshBullet);
}



/**
 *	Update()
 */
void Bullet::Update()
{
	// 衝突検知
	if (TestTargets()) {
		return;
	}

	// 移動
	vPos += vDir * speed;

	// 簡易重力
	g += 0.05f;
	vPos.y -= g;

	// スピードも気持ち減らす
	speed -= 0.05f;

	PRINTF("Bullet: pos=(%.02f,%.02f,%.02f), spd=%.02f\n",
		vPos.x, vPos.y, vPos.z, speed);
}


/**
 *	Draw();
 */
void Bullet::Draw()
{
	LPDIRECT3DDEVICE9 pDev = GameMain::GetD3DDevice();

	D3DXMATRIX m, mPos;
	D3DXMatrixTranslation(&mPos, vPos.x, vPos.y, vPos.z);

	// マテリアル設定
	pDev->SetMaterial(&material);

	// 描画
	D3DXMatrixIdentity(&m);
	m *= mPos;
	pDev->SetTransform(D3DTS_WORLD, &m);
	pMeshBullet->DrawSubset(0);
}



/**
 *	CanErase()
 */
bool Bullet::CanErase()
{
	// ヒットしていたら消えてOK
	if (hit) return true;

	// 画面外に出たら消えてOK
	if (vPos.x < GROUND_X_MIN ||
		vPos.x > GROUND_X_MAX ||
		vPos.z < GROUND_Z_MIN ||
		vPos.z > GROUND_Z_MAX)
	{
		return true;
	}

	// 地面に埋まったら消えてOK
	if (vPos.y < -5.0f) return true;

	return false;
}

/**
 *	OnErase()
 */
void Bullet::OnErase()
{
	PRINTF("Bullet erase! (%.02f,%.02f,%.02f)\n", vPos.x, vPos.y, vPos.z);
}


/**
 *	TestTargets()
 *	的との衝突検知をする。
 */
bool Bullet::TestTargets()
{
	D3DXVECTOR3 v, c;

	std::list<Target>& targets = GameMain::GetInstance().GetTargets();
	std::list<Target>::iterator itr = targets.begin();
	for (; itr != targets.end(); ++itr) {
		if (!itr->CanHit()) continue;

		v = itr->GetPos() - vPos;
		if (D3DXVec3LengthSq(&v) > 20.0f*20.0f) continue;	// それなりに離れてたら処理しない

		if (TestSphereBox(
			Vector3(vPos.x, vPos.y, vPos.z), BULLET_RADIUS,
			Vector3(itr->GetPos().x, itr->GetPos().y, itr->GetPos().z),
			TARGET_W, TARGET_H, TARGET_D))
		{
			// 衝突した
			itr->OnHitBullet(vDir);
			hit = true;
			return true;
		}
	}
	return false;
}

