/*!
 	Direct3D/Device.cpp
 
 	@author	Dila
 */

#include "Direct3D/Device.h"

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

/*!
 	�R���X�g���N�^
 */
Device::Device() : 
	_pDirect3D(NULL),
	_pDevice(NULL)
{
}

/*!
 	�f�X�g���N�^
 */
Device::~Device()
{
}

/*
 	�f�o�C�X����
 */
bool Device::Create(HWND hWnd)
{
#if _D3D_VER == _D3D_VER_9
	_pDirect3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (_pDirect3D == NULL) {
		return false;
	}
#endif

	ZeroMemory(&_PP, sizeof(_PP));

	_PP.Windowed = TRUE;	// �E�B���h�E���[�h
	_PP.SwapEffect = D3DSWAPEFFECT_DISCARD;	// �X���b�v�G�t�F�N�g
	_PP.BackBufferFormat = D3DFMT_UNKNOWN;	// �o�b�N�o�b�t�@�̃t�H�[�}�b�g

	_PP.EnableAutoDepthStencil = 1;			// Z�o�b�t�@���쐬����
	_PP.AutoDepthStencilFormat = D3DFMT_D16;

	HRESULT hr = _pDirect3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&_PP, &_pDevice);
	if (FAILED(hr)) {
		return false;
	}

	_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);	// Z�o�b�t�@�L��

	// �r���[�|�[�g�̐ݒ�
	D3DVIEWPORT9 vp;
	vp.X = 0;
	vp.Y = 0;
	vp.Width = _PP.BackBufferWidth;
	vp.Height = _PP.BackBufferHeight;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;
	hr = _pDevice->SetViewport(&vp);
	if (FAILED(hr)) {
		return false;
	}

	return true;
}

/*
 	�f�o�C�X���
 */
void Device::Release()
{
	SAFE_RELEASE(_pDevice);
	SAFE_RELEASE(_pDirect3D);
}


D3DXMATRIX* Device::SetupViewMatrix(D3DXMATRIX* m, const D3DXVECTOR3& vEye, const D3DXVECTOR3& vAt)
{
	return D3DXMatrixLookAtLH(m, &vEye, &vAt, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
}

D3DXMATRIX* Device::SetupPerspectiveMatrix(D3DXMATRIX* m, float fovV, float zn, float zf, float cx, float cy)
{
	return D3DXMatrixPerspectiveFovLH(m,
				fovV,
				(cx == 0.0f ? (float)_PP.BackBufferWidth : cx) /
				(cy == 0.0f ? (float)_PP.BackBufferHeight : cy),
				zn, zf);
}



