/**
 *	Project.h
 *	�v���W�F�N�g���p�w�b�_�t�@�C���B
 */

#pragma once

//
#include <stdio.h>
#include <d3dx9.h>
#include <dxerr9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

// �X�N���[���T�C�Y
#define SCREEN_W 640
#define SCREEN_H 480

// ����}�N��
#define RELEASE(x) if(x){x->Release(); x=NULL;}

// �f�o�b�O�v�����g
#ifdef _DEBUG
	#define PRINTF printf
#else
	#define PRINTF __noop
#endif


/**
 *	MYVERTEX	
 */
struct MYVERTEX {
	D3DXVECTOR3 v;	// ���W
	D3DXVECTOR3 n;	// �@��
};

#define D3DFVF_MYVERTEX	(D3DFVF_XYZ|D3DFVF_NORMAL)

