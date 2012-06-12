/**
 *	Ground.h
 *	�n�ʃN���X�B
 *	@author	Dila
 */
#pragma once

#include "Project.h"

// �`��
#define GROUND_TILE_SIZE	(10.0f)
#define GROUND_TILE_CNT_H	(50)
#define GROUND_TILE_CNT_V	(50)

#define GROUND_X_MIN	(-GROUND_TILE_SIZE*GROUND_TILE_CNT_H*0.5f)
#define GROUND_X_MAX	(GROUND_TILE_SIZE*GROUND_TILE_CNT_H*0.5f)
#define GROUND_Z_MIN	(-GROUND_TILE_SIZE*GROUND_TILE_CNT_V*0.5f)
#define GROUND_Z_MAX	(GROUND_TILE_SIZE*GROUND_TILE_CNT_V*0.5f)

/**
 *	Ground
 */
class Ground
{
	// �`��p
	LPDIRECT3DVERTEXBUFFER9 pVtxBuf;
	
	D3DMATERIAL9 material[2];	// �}�e���A��

public:
	Ground();

	bool Initialize();
	void Finalize();

	void Draw();
};
