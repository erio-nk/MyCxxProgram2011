/**
 *	Tank.cpp
 *	@author	Dila
 */

#include "Tank.h"
#include "Ground.h"
#include "GameMain.h"


/**
 *	Tank()
 */
Tank::Tank()
{
	pMeshBody = NULL;
	pMeshHead = NULL;
	pMeshGun = NULL;
}


/**
 *	Initialize()
 */
bool Tank::Initialize()
{
	vPos.y = TANK_BODY_H * 0.5f;

	speed = 1.0f;
	turn = D3DXToRadian(4.0f);

	shotInter = 0.0f;

	rotBody = 0.0f;
	rotHead = 0.0f;

	SetCamera();

	//// ���b�V������

	// �{��
	LPDIRECT3DDEVICE9 pDev = GameMain::GetD3DDevice();
	HRESULT hr;
	
	hr = D3DXCreateBox(pDev, 
		TANK_BODY_W,
		TANK_BODY_H,
		TANK_BODY_D,
		&pMeshBody, NULL);
	if (FAILED(hr)) {
		return false;
	}

	// ��
	hr = D3DXCreateBox(pDev,
		TANK_HEAD_W,
		TANK_HEAD_H,
		TANK_HEAD_D,
		&pMeshHead, NULL);
	if (FAILED(hr)) {
		return false;
	}

	// �C��
	hr = D3DXCreateBox(pDev,
		TANK_GUN_W,
		TANK_GUN_H,
		TANK_GUN_D,
		&pMeshGun, NULL);
	if (FAILED(hr)) {
		return false;
	}

	//// �}�e���A������
	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	material.Diffuse.r = material.Ambient.r = 0.0f;
	material.Diffuse.g = material.Ambient.g = 0.5f;
	material.Diffuse.b = material.Ambient.b = 0.2f;
	material.Diffuse.a = material.Ambient.a = 1.0f;

	return true;
}


/**
 *	Finalize()
 */
void Tank::Finalize()
{
	RELEASE(pMeshGun);
	RELEASE(pMeshHead);
	RELEASE(pMeshBody);
}


/**
 *	Update()
 */
void Tank::Update()
{
	// ���˃C���^�[
	if (shotInter > 0) --shotInter;
}


/**
 *	Draw()
 */
void Tank::Draw()
{
	LPDIRECT3DDEVICE9 pDev = GameMain::GetD3DDevice();

	D3DXMATRIX m, mPos, mRot;
	D3DXMatrixTranslation(&mPos, vPos.x, vPos.y, vPos.z);
	D3DXMatrixRotationY(&mRot, rotBody);

	// �}�e���A���ݒ�
	pDev->SetMaterial(&material);

	// �{�̕`��
	D3DXMatrixIdentity(&m);
	m *= mRot * mPos;
	pDev->SetTransform(D3DTS_WORLD, &m);
	pMeshBody->DrawSubset(0);

	// ���`��
	D3DXMATRIX mPosHead;
	D3DXMATRIX mRotHead;
	D3DXMatrixIdentity(&m);
	D3DXMatrixTranslation(&mPosHead, 0, TANK_BODY_H, 0);
	D3DXMatrixRotationY(&mRotHead, rotHead);
	m *= mPosHead * mRot * mRotHead * mPos;
	pDev->SetTransform(D3DTS_WORLD, &m);
	pMeshHead->DrawSubset(0);

	// �C���`��
	D3DXMATRIX mPosGun;
	D3DXMatrixIdentity(&m);
	D3DXMatrixTranslation(&mPosGun, 0,
		TANK_BODY_H+(TANK_HEAD_H-TANK_GUN_H)*0.5f,
		(TANK_HEAD_D+TANK_GUN_D)*0.5f);
	m *= mPosGun * mRot * mRotHead * mPos;
	pDev->SetTransform(D3DTS_WORLD, &m);
	pMeshGun->DrawSubset(0);
}

/**
 *	Forward()
 *	��Ԃ�O�i������B
 */
void Tank::Forward()
{
	D3DXVECTOR3 v(0, 0, speed);
	D3DXMATRIX m;
	D3DXMatrixRotationY(&m, rotBody);
	D3DXVec3TransformCoord(&v, &v, &m);
	vPos += v;
	
	if (vPos.x < GROUND_X_MIN) vPos.x = GROUND_X_MIN;
	if (vPos.x > GROUND_X_MAX) vPos.x = GROUND_X_MAX;
	if (vPos.z < GROUND_X_MIN) vPos.z = GROUND_Z_MIN;
	if (vPos.z > GROUND_X_MAX) vPos.z = GROUND_Z_MAX;

	PRINTF("pos=%.02f,%.02f\n", vPos.x, vPos.z);

	SetCamera();
}

/**
 *	Back()
 *	��Ԃ���ނ�����B
 */
void Tank::Back()
{
	D3DXVECTOR3 v(0, 0, -speed*0.5f);	// ������ƒx��
	D3DXMATRIX m;
	D3DXMatrixRotationY(&m, rotBody);
	D3DXVec3TransformCoord(&v, &v, &m);
	vPos += v;

	if (vPos.x < GROUND_X_MIN) vPos.x = GROUND_X_MIN;
	if (vPos.x > GROUND_X_MAX) vPos.x = GROUND_X_MAX;
	if (vPos.z < GROUND_X_MIN) vPos.z = GROUND_Z_MIN;
	if (vPos.z > GROUND_X_MAX) vPos.z = GROUND_Z_MAX;
	
	PRINTF("pos=%.02f,%.02f\n", vPos.x, vPos.z);

	SetCamera();
}

/**
 *	TurnLeft()
 *	��Ԃ����ɐ��񂳂���B
 */
void Tank::TurnLeft()
{
	rotBody -= turn;

	PRINTF("rotBody=%.02f\n", rotBody);

	SetCamera();
}

/**
 *	TurnRight()
 *	��Ԃ��E�ɐ��񂳂���B
 */
void Tank::TurnRight()
{
	rotBody += turn;

	PRINTF("rotBody=%.02f\n", rotBody);

	SetCamera();
}

/**
 *	RotateHead()
 *	������񂳂���B
 */
void Tank::RotateHead(float angle)
{
	rotHead += angle;
	if (D3DXToRadian(45) < rotHead) rotHead = D3DXToRadian(45);
	if (D3DXToRadian(-45)> rotHead) rotHead = D3DXToRadian(-45);
}

/**
 *	Shot()
 *	�e�𔭎˂���B
 */
void Tank::Shot()
{
	if (shotInter > 0) return;

	//// ���ˈʒu�E���������߂�
	D3DXVECTOR3 vShotPos(0, 0, 0);
	D3DXVECTOR3 vShotDir(0, 0, 1);

	D3DXMATRIX m, mPos, mPosGun, mRot, mRotHead;

	D3DXMatrixTranslation(&mPos, vPos.x, vPos.y, vPos.z);
	D3DXMatrixTranslation(&mPosGun, 0,
		TANK_BODY_H+(TANK_HEAD_H-TANK_GUN_H)*0.5f,
		(TANK_HEAD_D+TANK_GUN_D)*0.5f);

	D3DXMatrixRotationY(&mRot, rotBody);
	D3DXMatrixRotationY(&mRotHead, rotHead);
	
	// �ʒu
	m = mPosGun * mRot * mRotHead * mPos;
	D3DXVec3TransformCoord(&vShotPos, &vShotPos, &m);

	// ����
	m = mRot * mRotHead;
	D3DXVec3TransformCoord(&vShotDir,  &vShotDir, &m);

	//// ����
	Bullet bul(vShotPos, vShotDir, 10.0f);
	GameMain::GetInstance().PushBullet(bul);

	shotInter = 10;
}


/**
 *	SetCamera()
 *	�J������ݒ肷��B
 */
void Tank::SetCamera() {
	D3DXVECTOR3 vEye(0, 10.0f, -30.0f);
	D3DXVECTOR3 vAt(0, 10.0f, 10.0f);

	D3DXMATRIX m;
	D3DXMatrixRotationY(&m, rotBody);

	D3DXVec3TransformCoord(&vEye, &vEye, &m);
	D3DXVec3TransformCoord(&vAt, &vAt, &m);

	vEye += vPos;
	vAt += vPos;

	GameMain::GetInstance().SetCamera(vEye, vAt);
}