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
 *	�`��p�̃��b�V����������������B
 */
bool Bullet::Initialize()
{
	// ���b�V������
	LPDIRECT3DDEVICE9 pDev = GameMain::GetD3DDevice();
	HRESULT hr;
	
	hr = D3DXCreateSphere(pDev, 
		BULLET_RADIUS, 16, 16,
		&pMeshBullet, NULL);
	if (FAILED(hr)) {
		return false;
	}

	// �}�e���A������
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
	// �Փˌ��m
	if (TestTargets()) {
		return;
	}

	// �ړ�
	vPos += vDir * speed;

	// �ȈՏd��
	g += 0.05f;
	vPos.y -= g;

	// �X�s�[�h���C�������炷
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

	// �}�e���A���ݒ�
	pDev->SetMaterial(&material);

	// �`��
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
	// �q�b�g���Ă����������OK
	if (hit) return true;

	// ��ʊO�ɏo���������OK
	if (vPos.x < GROUND_X_MIN ||
		vPos.x > GROUND_X_MAX ||
		vPos.z < GROUND_Z_MIN ||
		vPos.z > GROUND_Z_MAX)
	{
		return true;
	}

	// �n�ʂɖ��܂����������OK
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
 *	�I�Ƃ̏Փˌ��m������B
 */
bool Bullet::TestTargets()
{
	D3DXVECTOR3 v, c;

	std::list<Target>& targets = GameMain::GetInstance().GetTargets();
	std::list<Target>::iterator itr = targets.begin();
	for (; itr != targets.end(); ++itr) {
		if (!itr->CanHit()) continue;

		v = itr->GetPos() - vPos;
		if (D3DXVec3LengthSq(&v) > 20.0f*20.0f) continue;	// ����Ȃ�ɗ���Ă��珈�����Ȃ�

		if (TestSphereBox(
			Vector3(vPos.x, vPos.y, vPos.z), BULLET_RADIUS,
			Vector3(itr->GetPos().x, itr->GetPos().y, itr->GetPos().z),
			TARGET_W, TARGET_H, TARGET_D))
		{
			// �Փ˂���
			itr->OnHitBullet(vDir);
			hit = true;
			return true;
		}
	}
	return false;
}

