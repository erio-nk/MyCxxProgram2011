/**
 *	Ground.cpp
 *	@author	Dila
 */

#include "Ground.h"
#include "GameMain.h"


/**
 *	Ground()
 */
Ground::Ground()
{
	pVtxBuf = NULL;
}


/**
 *	Initialize()
 */
bool Ground::Initialize()
{
	// 頂点バッファ作成
	LPDIRECT3DDEVICE9 pDev = GameMain::GetD3DDevice();
	HRESULT hr;

	hr = pDev->CreateVertexBuffer(
		4*sizeof(MYVERTEX),
		0,
		D3DFVF_MYVERTEX,
		D3DPOOL_MANAGED,
		&pVtxBuf,
		NULL);
	if (FAILED(hr)) {
		return false;
	}

	MYVERTEX* _v;
	hr = pVtxBuf->Lock(0, 0, (LPVOID*)&_v, 0);
	if (FAILED(hr)) {
		return false;
	}

	D3DXVECTOR3 n(0, 1, 0);

	_v[0].v.x = -GROUND_TILE_SIZE*0.5f;
	_v[0].v.z = -GROUND_TILE_SIZE*0.5f;
	_v[0].v.y = 0.0f;
	_v[0].n = n;

	_v[1].v.x =  GROUND_TILE_SIZE*0.5f;
	_v[1].v.z = -GROUND_TILE_SIZE*0.5f;
	_v[1].v.y = 0.0f;
	_v[1].n = n;

	_v[2].v.x = -GROUND_TILE_SIZE*0.5f;
	_v[2].v.z =  GROUND_TILE_SIZE*0.5f;
	_v[2].v.y = 0.0f;
	_v[2].n = n;
	
	_v[3].v.x =  GROUND_TILE_SIZE*0.5f;
	_v[3].v.z =  GROUND_TILE_SIZE*0.5f;
	_v[3].v.y = 0.0f;
	_v[3].n = n;

	hr = pVtxBuf->Unlock();
	if (FAILED(hr)) {
		return false;
	}

	
	ZeroMemory(&material[0], sizeof(D3DMATERIAL9));
	material[0].Diffuse.r = material[0].Ambient.r = 1.0f;
	material[0].Diffuse.g = material[0].Ambient.g = 0.5f;
	material[0].Diffuse.b = material[0].Ambient.b = 0.5f;
	material[0].Diffuse.a = material[0].Ambient.a = 1.0f;
	
	
	ZeroMemory(&material[1], sizeof(D3DMATERIAL9));
	material[1].Diffuse.r = material[1].Ambient.r = 0.5f;
	material[1].Diffuse.g = material[1].Ambient.g = 0.5f;
	material[1].Diffuse.b = material[1].Ambient.b = 1.0f;
	material[1].Diffuse.a = material[1].Ambient.a = 1.0f;

	return true;
}


/**
 *	Finalize()
 */
void Ground::Finalize()
{
	RELEASE(pVtxBuf);
}


/**
 *	Draw()
 */
void Ground::Draw()
{
	// 赤と青のタイルを敷き詰める
	LPDIRECT3DDEVICE9 pDev = GameMain::GetD3DDevice();

	D3DXMATRIX m;
	D3DXMatrixIdentity(&m);
	pDev->SetTransform(D3DTS_WORLD, &m);

    pDev->SetFVF(D3DFVF_MYVERTEX); 

	for (int i = 0; i < GROUND_TILE_CNT_V; ++i) {
		for (int k = 0; k < GROUND_TILE_CNT_H; ++k) {
			pDev->SetMaterial(&material[(i+k)&1]);
			pDev->SetStreamSource(0, pVtxBuf, 0, sizeof(MYVERTEX));
			D3DXMatrixTranslation(&m,
				(k*GROUND_TILE_SIZE)+GROUND_X_MIN+(GROUND_TILE_SIZE*0.5f),
				0,
				(i*GROUND_TILE_SIZE)+GROUND_Z_MIN+(GROUND_TILE_SIZE*0.5f));
			pDev->SetTransform(D3DTS_WORLD, &m);
		    pDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		}
	}
}
