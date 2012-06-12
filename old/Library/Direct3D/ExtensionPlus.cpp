/**
 	@file Direct3D/ExtensionPlus.cpp
 
 	@author Dila
 */

#include "Direct3D/ExtensionPlus.h"

// 板ポリメッシュ作成関数
HRESULT D3DXPlus::CreateBoard(IDirect3DDevice* pDevice, float width, float height, LPD3DXMESH* ppMesh)
{
	struct D3DVERTEX {
		D3DXVECTOR3 v;	// 頂点
		D3DXVECTOR3 n;	// 法線
		float tu, tv;	// テクスチャUV
		enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 };
	};
	HRESULT hr = D3DXCreateMeshFVF(2, 4, D3DXMESH_MANAGED, D3DVERTEX::FVF, pDevice, ppMesh);
	if (FAILED(hr)) {
		return hr;
	}
	
	D3DVERTEX* pVertex;
	WORD* pIndex;
	(*ppMesh)->LockVertexBuffer(D3DLOCK_DISCARD, (void**)&pVertex);
	(*ppMesh)->LockIndexBuffer(D3DLOCK_DISCARD, (void**)&pIndex);

	float hlen = width / 2.0f;
	float vlen = height/ 2.0f;

	pVertex[0].v = D3DXVECTOR3(-hlen, 0.0f, -vlen);
	pVertex[1].v = D3DXVECTOR3(-hlen, 0.0f,  vlen);
	pVertex[2].v = D3DXVECTOR3( hlen, 0.0f,  vlen);
	pVertex[3].v = D3DXVECTOR3( hlen, 0.0f, -vlen);

	pVertex[0].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVertex[1].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVertex[2].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVertex[3].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	pVertex[0].tu = 0.0f;	pVertex[0].tv = 0.0f;
	pVertex[1].tu = 0.0f;	pVertex[1].tv = 1.0f;
	pVertex[2].tu = 1.0f;	pVertex[2].tv = 1.0f;
	pVertex[3].tu = 1.0f;	pVertex[3].tv = 0.0f;

	pIndex[0] = 0;
	pIndex[1] = 1;
	pIndex[2] = 2;

	pIndex[3] = 2;
	pIndex[4] = 3;
	pIndex[5] = 0;

	(*ppMesh)->UnlockIndexBuffer();
	(*ppMesh)->UnlockVertexBuffer();

	return hr;
}

// 線分描画
HRESULT D3DXPlus::DrawLine(IDirect3DDevice* pDevice, const D3DXVECTOR3& v1, const D3DXVECTOR3& v2, DWORD color1, DWORD color2)
{
	struct D3DVERTEX
	{
		D3DXVECTOR3 v;
		DWORD color;
		enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };
	} line[2];
	
	line[0].v = v1;
	line[0].color = color1;
	line[1].v = v2;
	line[1].color = color2;

	D3DXMATRIX m;
	pDevice->SetTransform(D3DTS_WORLD, D3DXMatrixIdentity(&m));

	DWORD dwPrevLighting;
	pDevice->GetRenderState(D3DRS_LIGHTING, &dwPrevLighting);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	pDevice->SetFVF(D3DVERTEX::FVF);
	pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, line, sizeof(D3DVERTEX));
	
	pDevice->SetRenderState(D3DRS_LIGHTING, dwPrevLighting);

	return D3D_OK;	// TODO: エラー処理
}



// ベクトルを向きとした回転行列を取得する
D3DXMATRIX* D3DXPlus::MatrixRotationVector(D3DXMATRIX* pOut, const D3DXVECTOR3* pvDirection)
{
	D3DXMATRIX& m = *pOut;

	// 向きのベクトルをz軸の正方向として、その座標系に変換する行列を作る
	D3DXVECTOR3 vz;
		D3DXVec3Normalize(&vz, pvDirection);	// z軸決定

	D3DXVECTOR3  vy(0.0f, 1.0f, 0.0f);	// 適当に上側を向いたベクトル(外積計算用)
	D3DXVECTOR3  vx;
		D3DXVec3Cross(&vx, &vy, &vz);
		D3DXVec3Normalize(&vx, &vx);	// x軸決定
		D3DXVec3Cross(&vy, &vz, &vx);
		D3DXVec3Normalize(&vy, &vy);	// y軸決定

	// TODO: pDirectionがY軸と同じ場合の処理＝外積がゼロになる

	m(0, 0) = vx.x;
	m(0, 1) = vx.y;
	m(0, 2) = vx.z;
	m(0, 3) = 0.0f;

	m(1, 0) = vy.x;
	m(1, 1) = vy.y;
	m(1, 2) = vy.z;
	m(1, 3) = 0.0f;

	m(2, 0) = vz.x;
	m(2, 1) = vz.y;
	m(2, 2) = vz.z;
	m(2, 3) = 0.0f;

	m(3, 0) = 0.0f;
	m(3, 1) = 0.0f;
	m(3, 2) = 0.0f;
	m(3, 3) = 1.0f;

	return pOut;
}


// ビューポート変換行列を取得する
D3DXMATRIX* D3DXPlus::MatrixViewport(D3DXMATRIX* pOut, float cx, float cy)
{
	D3DXMATRIX& m = *pOut;

	D3DXMatrixIdentity(&m);

	m(0, 0) = cx / 2.0f;
	m(1, 1) = -cy / 2.0f;
	m(3, 0) = cx / 2.0f;
	m(3, 1) = cy / 2.0f;

	return pOut;
}
