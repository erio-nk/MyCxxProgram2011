/*!
 	@file Direct3D/Device.h
 
 	Direct3D�f�o�C�X�N���X�B
 
 	@author	Dila
 */
//! @addtogroup Direct3D Direct3D
//! @{

#pragma once

// D3D�o�[�W����
#define _D3D_VER_9	(9)		//! Direct3D 9
#define _D3D_VER_10	(10)	//! Direct3D 10 (�Ƃ肠������`���������B)

#ifndef _D3D_VER
	#define _D3D_VER _D3D_VER_9	//! Direct3D�f�t�H���g�o�[�W����
#endif

// include
#include <windows.h>

// Direct3D 9
#if _D3D_VER == _D3D_VER_9
	// include
	#include <d3d9.h>
	#include <d3dx9.h>

	// link
	#pragma comment(lib, "d3d9.lib")
	#ifdef _DEBUG
		#pragma comment(lib, "d3dx9d.lib")
	#else
		#pragma comment(lib, "d3dx9.lib")
	#endif

	// typedef
	typedef IDirect3D9 IDirect3D;	//! Direct3D�C���^�t�F�[�X
	typedef IDirect3DDevice9 IDirect3DDevice;	//! Direct3D�f�o�C�X�C���^�t�F�[�X

	typedef IDirect3DTexture9 IDirect3DTexture;		//! �e�N�X�`��
	typedef IDirect3DSurface9 IDirect3DSurface;	//! �T�[�t�F�C�X
	
	typedef D3DMATERIAL9 D3DMATERIAL;	//! �}�e���A���N���X
	typedef D3DLIGHT9 D3DLIGHT;			//! ���C�g�N���X
#endif


namespace Direct3D
{
	//! @addtogroup Direct3D Direct3D
	//! @{

	/*!
	 	Direct3D�f�o�C�X�N���X
	 
	 	Direct3D�f�o�C�X�̏��������̂����܂�ȏ����݂̂��܂Ƃ߂��N���X�B
	 	�ڍׂȏ����́A�f�o�C�X�ւ̃|�C���^(IDirect3DDevice9*)���擾����
	 	�C�ӂɏ������鎖�B
	 
	 */
	class Device
	{
		IDirect3D* _pDirect3D;		//! Direct3D�I�u�W�F�N�g
		IDirect3DDevice* _pDevice;	//! �f�o�C�X
		D3DPRESENT_PARAMETERS _PP;	//! �v���[���e�[�V�����p�����[�^

	public:
		Device();

		~Device();

		/*!
			�f�o�C�X����
		 	@param	hWnd	�E�B���h�E�n���h��
		 */
		bool Create(HWND hWnd);

		/*!
		 	�f�o�C�X���
		 */
		void Release();

		/*!
		 	Direct3D�I�u�W�F�N�g�擾
		 */
		IDirect3D* GetDirect3D() { return _pDirect3D; }

		/*!
		 	�f�o�C�X�̃C���^�t�F�[�X�擾
		 */
		IDirect3DDevice* GetInterface() { return _pDevice; }

		/*!
		 	�v���[���e�[�V�����p�����[�^�擾
		 */
		const D3DPRESENT_PARAMETERS& GetPresentParameters() { return _PP; }


		/*!
		 	�r���[�ϊ��}�g���N�X�Z�b�g�A�b�v
		 */
		D3DXMATRIX* SetupViewMatrix(D3DXMATRIX* m, const D3DXVECTOR3& vEye, const D3DXVECTOR3& vAt);


		/*!
		 	�������e�ϊ��}�g���N�X�Z�b�g�A�b�v
		 */
		D3DXMATRIX* SetupPerspectiveMatrix(D3DXMATRIX* m, float fovV,
			float zn = 1.0f, float zf = 1000.0f, float cx = 0.0f, float cy = 0.0f);

		/*!
			���ˉe�ϊ��}�g���N�X�Z�b�g�A�b�v
		 */
		//D3DXMATRIX* SetupOrthoMatrix(D3DXMATRIX* m, float fovV,
		//	float zn = 1.0f, float zf = 1000.0f, float cx = 0.0f, float cy = 0.0f);

	};	// end class Device


	//! @}
}	// end namespace Direct3D

//! @}
