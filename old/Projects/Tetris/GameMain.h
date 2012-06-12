/**
 *	GameMain.h
 *
 *	@author	Dila
 */

#include "Windows/Window.h"
#include "Windows/Bitmap.h"
#include "Game/FpsTimer.h"

//// define

// �f�o�b�O�r���[
//#define _DEBUG_VIEW

// �u���b�N�̃T�C�Y
#define BLOCK_W	(16)
#define BLOCK_H	(16)

// ��ʒ��̃u���b�N�ő吔
#define BLOCK_H_CNT (8+2)		// ���̍ő吔
#define	BLOCK_V_CNT	(20+1)	// �c�̍ő吔
#define BLOCK_H_CNT2 (8)	// ������͈͂̉��̍ő吔
#define BLOCK_V_CNT2 (16)	// ������͈͂̏c�̍ő吔

// �u���b�N��ʂ̃T�C�Y
#define BLOCK_VIEW_W (BLOCK_W*BLOCK_H_CNT2)
#define BLOCK_VIEW_H (BLOCK_H*BLOCK_V_CNT2)

#define BLOCK_VIEW_LEFT	(1)
#define BLOCK_VIEW_TOP	(4)
#define BLOCK_VIEW_RIGHT	(BLOCK_VIEW_LEFT+BLOCK_H_CNT2)
#define BLOCK_VIEW_BOTTOM	(BLOCK_VIEW_TOP+BLOCK_V_CNT2)

// ����ʂ̃T�C�Y
#define INFO_VIEW_W (BLOCK_W*5)
#define INFO_VIEW_H (BLOCK_H*BLOCK_V_CNT2)

#define INFO_VIEW_TOP	(0)
#ifdef _DEBUG_VIEW
	#define INFO_VIEW_LEFT	(BLOCK_H_CNT*BLOCK_W)
#else
	#define INFO_VIEW_LEFT	(BLOCK_VIEW_W)
#endif

// ��ʃT�C�Y
#ifdef _DEBUG_VIEW
	#define SCREEN_W	(BLOCK_H_CNT*BLOCK_W+INFO_VIEW_W)	
	#define SCREEN_H	(BLOCK_V_CNT*BLOCK_W)
#else
	#define SCREEN_W	(BLOCK_VIEW_W+INFO_VIEW_W)	
	#define SCREEN_H	(BLOCK_H*BLOCK_V_CNT2)
#endif

//// enum
enum GAME_SCENE {
	GAME_SCENE_START,	// �X�^�[�g���
	GAME_SCENE_MAIN,	// ���C�����
	GAME_SCENE_GAMEOVER,// �Q�[���I�[�o�[���
};

enum MAIN_PHASE {
	MAIN_PHASE_INIT,	// ���C����ʏ�����
	MAIN_PHASE_SET,		// �J�����g�u���b�N�Z�b�g
	MAIN_PHASE_PLAYER,	// �v���C���[����
	MAIN_PHASE_BLOCK,	// �u���b�N����
	MAIN_PHASE_CHECK,	// �Q�[���I�[�o�[�`�F�b�N
};

//// struct
struct BLOCK {
	char size;			// �T�C�Y
	char pattern[4][4];	// �p�^�[��
};


/**
 *	GameMain
 */
class GameMain
{
	static GameMain theInstance;

	Window wnd;		// ���C���E�B���h�E

	Bitmap backBuf;	// �o�b�N�o�b�t�@
	HDC backDC;		// �o�b�N�o�b�t�@��DC

	FpsTimer fps;

	char blocks[BLOCK_V_CNT][BLOCK_H_CNT];
	bool eraseFlags[BLOCK_V_CNT];

	int scene;		// �Q�[���V�[��
	int phase;		// �Q�[���t�F�[�Y

	// ���݂̃u���b�N
	bool cur;
	BLOCK curBlock;
	char curColor;	// �F
	int curX;		// X���W
	int curY;		// Y���W


	//
	int fallInter;	// �����C���^�[
	int fallCount;	// �����J�E���g

	//
	char blinkCount;	// �u���b�N���ŉ��o�̓_�ŃJ�E���g

	// �v���C���
	char nextPattern;	// ���̃p�^�[��
	int score;			// �X�R�A
	int lines;			// ���������C����
	int level;			// ���x��
	int exp;			// ���̃��x���܂�

public:
	static GameMain& GetInstance() { return theInstance; }

	bool Initialize();
	bool Finalize();

	void Update();

	void UpdateStart();
	void UpdateMain();
	void UpdateGameOver();

	void UpdateMainPlayer();
	void UpdateMainBlock();

	bool CheckBlocksLine();

	int GetFallInter(int level);
	int NextExp(int level);

	void DrawBackground();

	void DrawBlock(int x, int y, char color);
	void DrawBlocks();

	void DrawInfo();

	void DrawStart();
	void DrawGameOver();

	
	HFONT NewFont();
};


