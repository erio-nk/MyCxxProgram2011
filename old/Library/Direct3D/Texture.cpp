/*!
 	@file Direct3D/Texture.cpp
 
 	テクスチャクラスの実装
 
 	@author Dila
 */
#include "Direct3D/Texture.h"

using namespace Direct3D;

namespace
{
	template <class T>
	inline void SAFE_RELEASE(T*& p)
	{
		if (p) {
			p->Release();
			p = NULL;
		}
	}
}

RenderTexture::RenderTexture() :
	_pDevice(NULL),
	_pTexture(NULL),
	_pSurface(NULL)
{
}

RenderTexture::~RenderTexture()
{
	Release();
}

bool RenderTexture::Create(IDirect3DDevice* pDevice, float w, float h)
{
	// テクスチャ作成
	pDevice->CreateTexture(w, h, 0, D3DUSAGE_RENDERTARGET,
		 D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &_pTexture, NULL);

	// サーフェイス取得
	_pTexture->GetSurfaceLevel(0, &_pSurface);

	_pDevice = pDevice;

	return true;
}

void RenderTexture::Release()
{
	SAFE_RELEASE(_pSurface);
	SAFE_RELEASE(_pTexture);
	_pDevice = NULL;
}