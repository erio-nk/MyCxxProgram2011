/**
 *	GameMain.cpp
 *
 *	@author	Dila
 */

#include <stdio.h>
#include <time.h>

#include "Windows/Main.h"

#include "GameMain.h"

#ifdef _DEBUG
	#define PRINTF printf
#else
	#define PRINTF __noop
#endif


GameMain GameMain::theInstance;


/**
 *	�u���b�N�p�^�[��
 */
#define BLOCK_PATTERN_MAX	(7)
static BLOCK blockPattern[BLOCK_PATTERN_MAX] =
{
	// L���^
	{	3,	{
		{ 1, 1, 0, 9 },
		{ 0, 1, 0, 9 },
		{ 0, 1, 0, 9 },
		{ 9, 9, 9, 9 },
	}	},

	// �����^
	{	3,	{
		{ 0, 1, 1, 9 },
		{ 0, 1, 0, 9 },
		{ 0, 1, 0, 9 },
		{ 9, 9, 9, 9 },
	}	},

	// T���^
	{	3,	{
		{ 0, 0, 0, 9 },
		{ 1, 1, 1, 9 },
		{ 0, 1, 0, 9 },
		{ 9, 9, 9, 9 },
	}	},

	// ���^�P
	{	3,	{
		{ 0, 1, 0, 9 },
		{ 0, 1, 1, 9 },
		{ 0, 0, 1, 9 },
		{ 9, 9, 9, 9 },
	}	},

	// ���^�Q
	{	3,	{
		{ 0, 1, 0, 9 },
		{ 1, 1, 0, 9 },
		{ 1, 0, 0, 9 },
		{ 9, 9, 9, 9 },
	}	},

	// �l�p�^
	{	2,	{
		{ 1, 1, 9, 9 },
		{ 1, 1, 9, 9 },
		{ 9, 9, 9, 9 },
		{ 9, 9, 9, 9 },
	}	},

	// �_�^
	{	4,	{
		{ 0, 1, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 1, 0, 0 },
	}	},
};


/**
 *	Run()
 *	���C�����[�v
 */
void Run() {
	GameMain::GetInstance().Update();
}


/**
 *	Main()
 *	���C���֐�
 */
void Main() {
	// ������
	if (!GameMain::GetInstance().Initialize()) {
		MessageBox(NULL, "���������s", "Error", MB_OK);
		if (!GameMain::GetInstance().Finalize()) {
			MessageBox(NULL, "�I���������s", "Error", MB_OK);
		}
		return;
	}

	// �A�v���P�[�V�������[�v
	MainLoop(Run);

	// �I������
	if (!GameMain::GetInstance().Finalize()) {
		MessageBox(NULL, "�I���������s", "Error", MB_OK);
	}
}



/**
 *	GameMain::Initialize()
 */
bool GameMain::Initialize()
{
	// �f�o�b�O�R���\�[��
#ifdef _DEBUG
	if (AllocConsole()) {
		freopen("CONOUT$", "w", stdout);
		freopen("CONIN$", "r", stdin);
	} else {
		// !�Ƃ肠�������s�͖���
	}
#endif

	// �E�B���h�E������
	using namespace WindowOption;
	bool b;
	b = wnd.Create(
		"Tetris",
		0, 0, SCREEN_W, SCREEN_H,
		StandardStyle |
		BGNull |
		Centering |
		ClientSize,
		NULL,
		NULL);
	if (!b) return false;

	// �o�b�N�o�b�t�@������
	backBuf.Create(SCREEN_W, SCREEN_H);
	backDC = backBuf.GetDC();

	// �E�B���h�E����
	wnd.Show();

	// ����������
	srand((unsigned int)time(NULL));

	// FPS
	fps.SetFrameRate(30);

	// �������
	scene = GAME_SCENE_START;

	// �f�[�^������
	score = 0;
	lines = 0;
	level = 1;

	nextPattern = -1;

	PRINTF("Initialized.\n");

	return true;
}


/**
 *	GameMain::Finalize()
 */
bool GameMain::Finalize()
{
	backBuf.Release();
	return true;
}


/**
 *	GameMain::Update()
 *	�X�V
 */
void GameMain::Update()
{
	switch (scene) {
		case GAME_SCENE_START:
			UpdateStart();
			break;
		case GAME_SCENE_MAIN:
			UpdateMain();
			break;
		case GAME_SCENE_GAMEOVER:
			UpdateGameOver();
			break;
	}



	// �w�i�`��
	DrawBackground();

	// �ς�ł���u���b�N�`��
	DrawBlocks();

	// �v���C���`��
	DrawInfo();

	// �X�^�[�g��ʕ`��
	if (scene == GAME_SCENE_START) {
		DrawStart();
	}

	// �Q�[���I�[�o�[��ʕ`��
	if (scene == GAME_SCENE_GAMEOVER) {
		DrawGameOver();
	}


	// �t���b�v
	HDC hDC = GetDC(wnd.GetHandle());
	BitBlt(hDC, 0, 0, SCREEN_W, SCREEN_H, backDC, 0, 0, SRCCOPY);
	ReleaseDC(wnd.GetHandle(), hDC);

	// FPS����
	static char str[32];
	static int _fps = 0;
	if (_fps != fps.GetFps()) {
		sprintf(str, "Tetris (%dFPS)", fps.GetFps());
		SetWindowText(wnd.GetHandle(), str);
		_fps = fps.GetFps();
	}

	fps.Wait();
	//PRINTF("time=%d\n", fps.GetMilliSecond());
}


/**
 *	GameMain::UpdateStart()
 *	�X�^�[�g��ʏ���
 */
void GameMain::UpdateStart()
{
	if (phase++ == 0) {
		// �f�[�^�N���A
		int i, k;
		for (i = 0; i < BLOCK_VIEW_BOTTOM; ++i) {
			for (k = BLOCK_VIEW_LEFT; k < BLOCK_VIEW_RIGHT; ++k) {
				blocks[i][k] = 0;
			}
		}

		// �ǁE���ݒ�
		for (i = 0; i < BLOCK_V_CNT; ++i) {
			blocks[i][0] = 8;				// ����
			blocks[i][BLOCK_H_CNT-1] = 8;	// �E��
		}
		for (i = 0; i < BLOCK_H_CNT; ++i) {
			blocks[BLOCK_V_CNT-1][i] = 8;	// ��
		}

		nextPattern = -1;

		score = 0;
		lines = 0;
		level = 1;
	}

	if (LOBYTE(GetAsyncKeyState(VK_RETURN))) {
		// ���C����ʂ�
		scene = GAME_SCENE_MAIN;
		phase = MAIN_PHASE_INIT;
	}
}

/**
 *	GameMain::UpdateGameOver()
 *	�Q�[���I�[�o�[��ʏ���
 */
void GameMain::UpdateGameOver()
{
	if (phase < BLOCK_V_CNT2) {
		for (int i = BLOCK_VIEW_LEFT; i < BLOCK_VIEW_RIGHT; ++i) {
			blocks[BLOCK_VIEW_BOTTOM-phase-1][i] = 8;
		}
		++phase;
	} else {
		if (LOBYTE(GetAsyncKeyState(VK_RETURN))) {
			// �X�^�[�g��ʂ�
			scene = GAME_SCENE_START;
			phase = 0;
		}
	}
}


/**
 *	GameMain::UpdateMain()
 *	���C����ʏ���
 */
void GameMain::UpdateMain()
{
//	int i, k;

	switch (phase) {
		// ������
		case MAIN_PHASE_INIT:
			// �e�X�g
			/*for (i = 7; i < 20; ++i) {
				for (k = BLOCK_VIEW_LEFT; k < BLOCK_VIEW_RIGHT-2; ++k) {
					blocks[i][k] = 1;
				}
			}
			blocks[18][7] = 1;*/
			/*for (i = 10; i < 20; ++i) {
				for (k = BLOCK_VIEW_LEFT; k < BLOCK_VIEW_RIGHT-2; ++k) {
					blocks[i][k] = 1;
				}
				if (i&1) blocks[i][k] = 1;
			}*/

			// �����ݒ�
			phase = MAIN_PHASE_SET;
			cur = false;

			exp = NextExp(level);
			fallInter = GetFallInter(level);
			break;

		// �J�����g�u���b�N�Z�b�g
		case MAIN_PHASE_SET:
			{
				int pat;
				if (nextPattern == -1) nextPattern = rand()%BLOCK_PATTERN_MAX;
				pat = nextPattern;
					//pat = 5;	// �e�X�g
					//pat = 6;	// �e�X�g

				curBlock = blockPattern[pat];
				curX = (BLOCK_H_CNT-curBlock.size) >> 1;
				curY = BLOCK_VIEW_TOP - curBlock.size;
				curColor = pat+1;
				cur = true;
				fallCount = fallInter;
				nextPattern = rand()%BLOCK_PATTERN_MAX;
				phase = MAIN_PHASE_PLAYER;
			}
			break;

		// �v���C���[����
		case MAIN_PHASE_PLAYER:
			UpdateMainPlayer();
			break;

		// �u���b�N����
		case MAIN_PHASE_BLOCK:
			UpdateMainBlock();
			break;

		// �Q�[���I�[�o�[�`�F�b�N
		case MAIN_PHASE_CHECK:
			if (blocks[BLOCK_VIEW_TOP-1][4] != 0 ||
				blocks[BLOCK_VIEW_TOP-1][5] != 0) {
				// �Q�[���I�[�o�[
				scene = GAME_SCENE_GAMEOVER;
				phase = 0;
			}
			else {
				// �u���b�N�Z�b�g��
				phase = MAIN_PHASE_SET;
			}
			break;
	}
}

/**
 *	UpdateMainPlayer()
 *	���C����ʃv���C���[����t�F�[�Y�X�V
 */
void GameMain::UpdateMainPlayer()
{
	int i, k, x, y;

	// �u���b�N�̎���`�F�b�N
	bool leftMove = true;
	bool rightMove = true;
	for (i = curBlock.size-1; i >= 0; --i) {
		for (k = 0; k < curBlock.size; ++k) {
			if (curBlock.pattern[i][k] != 1) continue;

			x = curX + k;
			y = curY + i;

			// �u���b�N�̍��Ƀu���b�N�����邩
			if (leftMove) {
				if (blocks[y][x-1] != 0) {
					leftMove = false;
				}
			}

			// �u���b�N�̉E�Ƀu���b�N�����邩
			if (rightMove) {
				if (blocks[y][x+1] != 0) {
					rightMove = false;
				}
			}
		}
	}

	PRINTF("leftMove=%d rightMove=%d\n", leftMove, rightMove);

	// �u���b�N��]
	char key = 0;
	if (LOBYTE(GetAsyncKeyState('Z'))) {
		key = -1;
	} else if (LOBYTE(GetAsyncKeyState('X'))) {
		key = 1;
	}
	if (key != 0) {
		bool turn = true;
		BLOCK _b = curBlock;
		for (i = 0; i < _b.size; ++i) {
			if (!turn) break;
			for (k = 0; k < _b.size; ++k) {
				if (key < 0) {			// ��
					y = _b.size-k-1;
					x = i;
				} else if (key > 0) {	// �E
					y = k;
					x = _b.size-i-1;
				}

				if (blocks[curY+y][curX+x] != 0) {	// ������Ƒ�G�c�ȃ`�F�b�N������
					turn = false;
					break;
				}

				_b.pattern[y][x] = curBlock.pattern[i][k];			
			}
		}
		if (turn) {
			curBlock = _b;
		}
	}

	// �u���b�N�ړ�
	else if (LOBYTE(GetAsyncKeyState(VK_LEFT))) {
		if (leftMove) --curX;
	}
	else if (LOBYTE(GetAsyncKeyState(VK_RIGHT))) {
		if (rightMove) ++curX;
	}

	// �u���b�N�̗����`�F�b�N
	bool stop = false;
	for (i = curBlock.size-1; i >= 0; --i) {
		for (k = 0; k < curBlock.size; ++k) {
			if (curBlock.pattern[i][k] != 1) continue;

			x = curX + k;
			y = curY + i;

			// �u���b�N�̉��Ƀu���b�N�����邩
			if (!stop) {
				if (blocks[y+1][x] != 0) {
					stop = true;
				}
			}
		}
	}	

	// �u���b�N����
	--fallCount;

	if (HIBYTE(GetAsyncKeyState(VK_DOWN))) {
		if (fallCount > 1) fallCount = 1;	// ��������
	}

	PRINTF("stop=%d, fallCount=%d\n", stop, fallCount);

	if (fallCount <= 0) {
		if (stop) {
			// �u���b�N�ݒ�
			for (i = 0; i < curBlock.size; ++i) {
				for (k = 0; k < curBlock.size; ++k) {
					if (curBlock.pattern[i][k] != 1) continue;
					blocks[curY + i][curX + k] = curColor;
				}
			}
			cur = false;

			// ���Ń`�F�b�N
			if (CheckBlocksLine()) {
				// ���ŉ��o��
				phase = MAIN_PHASE_BLOCK;
				blinkCount = 16;
			} else {
				// �Q�[���I�[�o�[�`�F�b�N��
				phase = MAIN_PHASE_CHECK;
			}
		
		} else {
			++curY;
			fallCount = fallInter;
		}
	}
}


/**
 *	UpdateMainBlock()
 *	���C����ʃu���b�N���ōX�V
 */
void GameMain::UpdateMainBlock()
{
	int i, k;
	if (--blinkCount == 0) {
		// �u���b�N����
		for (i = BLOCK_VIEW_TOP; i < BLOCK_VIEW_BOTTOM; ++i) {
			if (!eraseFlags[i]) continue;
			for (k = BLOCK_VIEW_LEFT; k < BLOCK_VIEW_RIGHT; ++k) {
				blocks[i][k] = 0;
			}
		}

		// �u���b�N�𗎂Ƃ�
		int x = 0;
		for (i = BLOCK_VIEW_BOTTOM-1; i >= 0; --i) {
			if (eraseFlags[i]) {
				++x;
			}
			else if (x > 0) {
				for (k = BLOCK_VIEW_LEFT; k < BLOCK_VIEW_RIGHT; ++k) {
					blocks[i+x][k] = blocks[i][k];
					blocks[i][k] = 0;
				}
			}
		}

		// �X�R�A���Z
		int point;
		point =  x*10 + (x-1)*10;	// 1���C��=10�_, 2���C������=30�_, 3���C������=50�_, 4���C������=70�_
		point *= ((level-1)>>1)+1;	// ���x���������Ƃ�
		PRINTF("x1=%d, point=%d\n", x, point);
		score += point;
		if (score > 99999) score = 99999;

		// ���C�������Z
		PRINTF("x2=%d\n", x);
		lines += x;
		if (lines > 99999) lines = 99999;

		// ���x�����Z
		exp -= x;
		if (exp <= 0) {
			++level;
			exp = NextExp(level) + exp;
			fallInter = GetFallInter(level);
		}


		// �Q�[���I�[�o�[�`�F�b�N��
		phase = MAIN_PHASE_CHECK;
	}
}

/**
 *	GameMain::CheckBlocksLine()
 *	�u���b�N�̃��C���̏��Ń`�F�b�N
 */
bool GameMain::CheckBlocksLine()
{
	int i, k;
	bool b = false;

	for (i = BLOCK_VIEW_TOP; i < BLOCK_VIEW_BOTTOM; ++i) {
		eraseFlags[i] = true;
		for (k = BLOCK_VIEW_LEFT; k < BLOCK_VIEW_RIGHT; ++k) {
			if (blocks[i][k] == 0) {
				eraseFlags[i] = false;
				break;
			}
		}
		if (eraseFlags[i]) b = true;
	}

	return b;
}

/**
 *	�����C���^�[�𓾂�
 */
int GameMain::GetFallInter(int level)
{
	int i = 45 - ((level-1)<<1);
	if (i < 0) i = 0;
	return i;
}

/**
 *	���̃��x���܂ł̌o���l�𓾂�
 */
int GameMain::NextExp(int level) {
	return level * 6 + level * 2;
}


/**
 *	GameMain::DrawBlock()
 *	�P��̃u���b�N�`��
 */
void GameMain::DrawBlock(int x, int y, char color)
{
	// �F��`
	static COLORREF colors[9] = {
		RGB(0x00, 0x00, 0x00),
		RGB(0xff, 0x00, 0x00),
		RGB(0x00, 0xff, 0x00),
		RGB(0x00, 0x00, 0xff),
		RGB(0xff, 0xff, 0x00),
		RGB(0x00, 0xff, 0xff),
		RGB(0xff, 0x00, 0xff),
		RGB(0xff, 0xff, 0xff),
		RGB(0x80, 0x80, 0x80),
	};

	// �`��
	RECT rc;
	rc.left = x + 1;
	rc.top = y + 1;
	rc.right = rc.left + (BLOCK_W-2);
	rc.bottom = rc.top + (BLOCK_H-2);

	HBRUSH hBrush = CreateSolidBrush(colors[color]);

	FillRect(backDC, &rc, hBrush);

	DeleteObject(hBrush);
}


/**
 *	GameMain::DrawBlocks()
 *	�ς�ł���u���b�N�Ɨ����Ă���u���b�N�̕`��
 */
void GameMain::DrawBlocks()
{
	int i, k;

#ifdef _DEBUG_VIEW
	int offsetX = 0;
	int offsetY = 0;
#else
	int offsetX = - BLOCK_W * BLOCK_VIEW_LEFT;
	int offsetY = - BLOCK_H * BLOCK_VIEW_TOP;
#endif
	
	// �����Ă���u���b�N�̕`��
	if (cur) {
		for (i = 0; i < curBlock.size; ++i) {
			for (k = 0; k < curBlock.size; ++k) {
				if (curBlock.pattern[i][k] != 1) continue;
				DrawBlock((curX+k) * BLOCK_W + offsetX, (curY+i) * BLOCK_H + offsetY, curColor);
			}
		}
	}

	// �ς�ł���u���b�N�̕`��
	int l, r, t, b;
#ifdef _DEBUG_VIEW
	l = 0;
	r = BLOCK_H_CNT;
	t = 0;
	b = BLOCK_V_CNT;
#else
	l = BLOCK_VIEW_LEFT;
	r = BLOCK_VIEW_RIGHT;
	t = BLOCK_VIEW_TOP;
	b = BLOCK_VIEW_BOTTOM;
#endif
	for (i = t; i < b; ++i) {
		if (scene == GAME_SCENE_MAIN && phase == MAIN_PHASE_BLOCK
			&& eraseFlags[i] && ((blinkCount&3)>1)) continue;	// �_��

		for (k = l; k < r; ++k) {
			if (blocks[i][k] == 0) continue;
			DrawBlock(k * BLOCK_W + offsetX, i * BLOCK_H + offsetY, blocks[i][k]);
		}
	}
}


/**
 *	GameMain::DrawBackground()
 *	�w�i�`��
 */
void GameMain::DrawBackground()
{
#ifdef _DEBUG_VIEW
	RECT rc = {0, 0, BLOCK_H_CNT*BLOCK_W, BLOCK_V_CNT*BLOCK_H};
#else
	RECT rc = {0, 0, BLOCK_VIEW_W, BLOCK_VIEW_H};
#endif
	FillRect(backDC, &rc, (HBRUSH)GetStockObject(BLACK_BRUSH));

	// 
	rc.left = INFO_VIEW_LEFT;
	rc.right = rc.left + INFO_VIEW_W;
	FillRect(backDC, &rc, (HBRUSH)GetStockObject(GRAY_BRUSH));
}


/**
 *	GameMain::DrawInfo()
 *	�v���C���`��
 */
void GameMain::DrawInfo()
{
	int i, k, x, y;

	// �t�H���g
	HFONT hFont = NewFont();
	HFONT hFontOld = (HFONT)SelectObject(backDC, hFont);
	
	//
	x = INFO_VIEW_LEFT + BLOCK_W/2;
	y = INFO_VIEW_TOP + BLOCK_H/2;

	// ���̃u���b�N

	RECT rc;
	rc.left = x;
	rc.top  = y + BLOCK_H;
	rc.right = rc.left + BLOCK_W*4;
	rc.bottom= rc.top  + BLOCK_H*4;
	FillRect(backDC, &rc, (HBRUSH)GetStockObject(BLACK_BRUSH));

	if (nextPattern >= 0) {
		BLOCK* pNext = &blockPattern[nextPattern];
		int offX = ((4-pNext->size)*BLOCK_W)>>1;
		int offY = ((4-pNext->size)*BLOCK_H)>>1;
		for (i = 0; i < pNext->size; ++i) {
			for (k = 0; k < pNext->size; ++k) {
				if (pNext->pattern[i][k] != 1) continue;
				DrawBlock(rc.left+BLOCK_W*k+offX, rc.top+BLOCK_H*i+offY, nextPattern+1);
			}
		}
	}

	SetTextColor(backDC, RGB(0xff, 0xff, 0xff));
	SetBkMode(backDC, TRANSPARENT);
	TextOut(backDC, x, y, "Next", lstrlen("Next"));

	y = rc.bottom;

	// �X�R�A
	y += BLOCK_H;

	char s[32];
	sprintf(s, "Score:% 5d", score);
	TextOut(backDC, x, y, s, lstrlen(s));

	// ���C����
	y += BLOCK_H;

	sprintf(s, "Lines:% 5d", lines);
	TextOut(backDC, x, y, s, lstrlen(s));

	// ���x��
	y += BLOCK_H;

	sprintf(s, "Level:% 5d", level);
	TextOut(backDC, x, y, s, lstrlen(s));


	// �t�H���g�̍폜
	SelectObject(backDC, hFontOld);
	DeleteObject(hFont);

}


/**
 *	GameMain::DrawStart()
 *	�X�^�[�g��ʕ`��
 */
void GameMain::DrawStart()
{
	// �t�H���g
	HFONT hFont = NewFont();
	HFONT hFontOld = (HFONT)SelectObject(backDC, hFont);

	// �e�L�X�g
	SetTextColor(backDC, RGB(0xff, 0xff, 0xff));
	SetBkMode(backDC, TRANSPARENT);
	TextOut(backDC, 15, 120, "Press enter key.", strlen("Press enter key."));

	// �t�H���g�̍폜
	SelectObject(backDC, hFontOld);
	DeleteObject(hFont);
}


/**
 *	GameMain::DrawGameOver()
 *	�Q�[���I�[�o�[��ʕ`��
 */
void GameMain::DrawGameOver()
{
	if (phase < BLOCK_V_CNT2) return;

	RECT rc = {0, 120-2, BLOCK_VIEW_W, 120+14};
	FillRect(backDC, &rc, (HBRUSH)GetStockObject(BLACK_BRUSH));

	// �t�H���g
	HFONT hFont = NewFont();
	HFONT hFontOld = (HFONT)SelectObject(backDC, hFont);

	// �e�L�X�g
	SetTextColor(backDC, RGB(0xff, 0x00, 0x00));
	SetBkMode(backDC, TRANSPARENT);
	TextOut(backDC, 40, 120, "Game Over", strlen("Game Over"));

	// �t�H���g�̍폜
	SelectObject(backDC, hFontOld);
	DeleteObject(hFont);
}


/**
 *	GameMain::NewFont()
 *	�t�H���g���쐬����
 */
HFONT GameMain::NewFont()
{
	return CreateFont(
		12,	// ����
		0,	// ������
		0,	// �e�L�X�g�̊p�x
		0,	
		FW_REGULAR,	// �t�H���g�̏d��
		FALSE, FALSE, FALSE,	// I, U, S
		SHIFTJIS_CHARSET,		// �����Z�b�g
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY,
		FIXED_PITCH | FF_MODERN,
		"�l�r�@�S�V�b�N");
}


