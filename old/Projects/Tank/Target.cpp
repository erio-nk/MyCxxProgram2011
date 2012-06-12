/**
 *	Target.cpp
 *	@author	Dila
 */

#include "Ground.h"
#include "GameMain.h"

//
LPD3DXMESH Target::pMeshTarget = NULL;
D3DMATERIAL9 Target::material;


/**
 *	Initialize()
 *	�`��p�̃��b�V����������������B
 */
bool Target::Initialize()
{
	// ���b�V������
	LPDIRECT3DDEVICE9 pDev = GameMain::GetD3DDevice();
	HRESULT hr;
	
	hr = D3DXCreateBox(pDev, 
		TARGET_W,
		TARGET_H,
		TARGET_D,
		&pMeshTarget, NULL);
	if (FAILED(hr)) {
		return false;
	}

	// �}�e���A������
	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	material.Diffuse.r = material.Ambient.r = 1.0f;
	material.Diffuse.g = material.Ambient.g = 0.0f;
	material.Diffuse.b = material.Ambient.b = 0.0f;
	material.Diffuse.a = material.Ambient.a = 1.0f;

	//material.Emissive.r = 0.5f;
	//material.Emissive.g = 0.0f;
	//material.Emissive.b = 0.0f;
	//material.Emissive.a = 1.0f;

	return true;
}

/**
 *	Finalize()
 */
void Target::Finalize()
{
	RELEASE(pMeshTarget);
}


/**
 *	Update()
 */
void Target::Update()
{
	switch (state) {
		case STATE_STARTING:
			// �o���A�j���[�V����
			vPos.y += 1.0f;
			if (vPos.y >= TARGET_H*0.5f) {
				vPos.y = TARGET_H*0.5f;
				state = STATE_NORMAL;
			}
			break;

		case STATE_NORMAL:
			break;

		case STATE_HIT:
			// �q�b�g�A�j���[�V����
			hitRot += 8;
			if (hitRot >= 90) {
				hitRot = 90;
				state = STATE_END;	// ����
			}
			break;
	}
}


/**
 *	Draw()
 */
void Target::Draw()
{
	LPDIRECT3DDEVICE9 pDev = GameMain::GetD3DDevice();

	D3DXMATRIX m, mPos1, mPos2, mRot;
	D3DXMatrixTranslation(&mPos1, 0, vPos.y, 0);
	D3DXMatrixTranslation(&mPos2, vPos.x, 0, vPos.z);

	D3DXVECTOR3 v;
	D3DXMatrixRotationY(&m, D3DXToRadian(90));
	D3DXVec3TransformCoord(&v, &vHitDir, &m);
	D3DXMatrixRotationAxis(&mRot, &v, D3DXToRadian(hitRot));

	// �}�e���A���ݒ�
	pDev->SetMaterial(&material);

	// �`��
	m = mPos1 * mRot * mPos2;
	pDev->SetTransform(D3DTS_WORLD, &m);
	pMeshTarget->DrawSubset(0);
}

/**
 *	CanErase()
 */
bool Target::CanErase()
{
	if (state == STATE_END) {
		return true;
	}
	return false;
}


/**
 *	OnErase()
 */
void Target::OnErase()
{
	PRINTF("Target erase!\n");
}

/**
 *	OnHitBullet()
 */
void Target::OnHitBullet(const D3DXVECTOR3& vHitDir)
{
	state = STATE_HIT;
	this->vHitDir = vHitDir;

	PRINTF("OnHitBullet!!\n");
}


