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
 *	ブロックパターン
 */
#define BLOCK_PATTERN_MAX	(7)
static BLOCK blockPattern[BLOCK_PATTERN_MAX] =
{
	// L字型
	{	3,	{
		{ 1, 1, 0, 9 },
		{ 0, 1, 0, 9 },
		{ 0, 1, 0, 9 },
		{ 9, 9, 9, 9 },
	}	},

	// Γ字型
	{	3,	{
		{ 0, 1, 1, 9 },
		{ 0, 1, 0, 9 },
		{ 0, 1, 0, 9 },
		{ 9, 9, 9, 9 },
	}	},

	// T字型
	{	3,	{
		{ 0, 0, 0, 9 },
		{ 1, 1, 1, 9 },
		{ 0, 1, 0, 9 },
		{ 9, 9, 9, 9 },
	}	},

	// 鍵型１
	{	3,	{
		{ 0, 1, 0, 9 },
		{ 0, 1, 1, 9 },
		{ 0, 0, 1, 9 },
		{ 9, 9, 9, 9 },
	}	},

	// 鍵型２
	{	3,	{
		{ 0, 1, 0, 9 },
		{ 1, 1, 0, 9 },
		{ 1, 0, 0, 9 },
		{ 9, 9, 9, 9 },
	}	},

	// 四角型
	{	2,	{
		{ 1, 1, 9, 9 },
		{ 1, 1, 9, 9 },
		{ 9, 9, 9, 9 },
		{ 9, 9, 9, 9 },
	}	},

	// 棒型
	{	4,	{
		{ 0, 1, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 1, 0, 0 },
	}	},
};


/**
 *	Run()
 *	メインループ
 */
void Run() {
	GameMain::GetInstance().Update();
}


/**
 *	Main()
 *	メイン関数
 */
void Main() {
	// 初期化
	if (!GameMain::GetInstance().Initialize()) {
		MessageBox(NULL, "初期化失敗", "Error", MB_OK);
		if (!GameMain::GetInstance().Finalize()) {
			MessageBox(NULL, "終了処理失敗", "Error", MB_OK);
		}
		return;
	}

	// アプリケーションループ
	MainLoop(Run);

	// 終了処理
	if (!GameMain::GetInstance().Finalize()) {
		MessageBox(NULL, "終了処理失敗", "Error", MB_OK);
	}
}



/**
 *	GameMain::Initialize()
 */
bool GameMain::Initialize()
{
	// デバッグコンソール
#ifdef _DEBUG
	if (AllocConsole()) {
		freopen("CONOUT$", "w", stdout);
		freopen("CONIN$", "r", stdin);
	} else {
		// !とりあえず失敗は無視
	}
#endif

	// ウィンドウ初期化
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

	// バックバッファ初期化
	backBuf.Create(SCREEN_W, SCREEN_H);
	backDC = backBuf.GetDC();

	// ウィンドウ可視化
	wnd.Show();

	// 乱数初期化
	srand((unsigned int)time(NULL));

	// FPS
	fps.SetFrameRate(30);

	// 初期画面
	scene = GAME_SCENE_START;

	// データ初期化
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
 *	更新
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



	// 背景描画
	DrawBackground();

	// 積んであるブロック描画
	DrawBlocks();

	// プレイ情報描画
	DrawInfo();

	// スタート画面描画
	if (scene == GAME_SCENE_START) {
		DrawStart();
	}

	// ゲームオーバー画面描画
	if (scene == GAME_SCENE_GAMEOVER) {
		DrawGameOver();
	}


	// フリップ
	HDC hDC = GetDC(wnd.GetHandle());
	BitBlt(hDC, 0, 0, SCREEN_W, SCREEN_H, backDC, 0, 0, SRCCOPY);
	ReleaseDC(wnd.GetHandle(), hDC);

	// FPS制御
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
 *	スタート画面処理
 */
void GameMain::UpdateStart()
{
	if (phase++ == 0) {
		// データクリア
		int i, k;
		for (i = 0; i < BLOCK_VIEW_BOTTOM; ++i) {
			for (k = BLOCK_VIEW_LEFT; k < BLOCK_VIEW_RIGHT; ++k) {
				blocks[i][k] = 0;
			}
		}

		// 壁・床設定
		for (i = 0; i < BLOCK_V_CNT; ++i) {
			blocks[i][0] = 8;				// 左壁
			blocks[i][BLOCK_H_CNT-1] = 8;	// 右壁
		}
		for (i = 0; i < BLOCK_H_CNT; ++i) {
			blocks[BLOCK_V_CNT-1][i] = 8;	// 床
		}

		nextPattern = -1;

		score = 0;
		lines = 0;
		level = 1;
	}

	if (LOBYTE(GetAsyncKeyState(VK_RETURN))) {
		// メイン画面へ
		scene = GAME_SCENE_MAIN;
		phase = MAIN_PHASE_INIT;
	}
}

/**
 *	GameMain::UpdateGameOver()
 *	ゲームオーバー画面処理
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
			// スタート画面へ
			scene = GAME_SCENE_START;
			phase = 0;
		}
	}
}


/**
 *	GameMain::UpdateMain()
 *	メイン画面処理
 */
void GameMain::UpdateMain()
{
//	int i, k;

	switch (phase) {
		// 初期化
		case MAIN_PHASE_INIT:
			// テスト
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

			// 初期設定
			phase = MAIN_PHASE_SET;
			cur = false;

			exp = NextExp(level);
			fallInter = GetFallInter(level);
			break;

		// カレントブロックセット
		case MAIN_PHASE_SET:
			{
				int pat;
				if (nextPattern == -1) nextPattern = rand()%BLOCK_PATTERN_MAX;
				pat = nextPattern;
					//pat = 5;	// テスト
					//pat = 6;	// テスト

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

		// プレイヤー操作
		case MAIN_PHASE_PLAYER:
			UpdateMainPlayer();
			break;

		// ブロック消滅
		case MAIN_PHASE_BLOCK:
			UpdateMainBlock();
			break;

		// ゲームオーバーチェック
		case MAIN_PHASE_CHECK:
			if (blocks[BLOCK_VIEW_TOP-1][4] != 0 ||
				blocks[BLOCK_VIEW_TOP-1][5] != 0) {
				// ゲームオーバー
				scene = GAME_SCENE_GAMEOVER;
				phase = 0;
			}
			else {
				// ブロックセットへ
				phase = MAIN_PHASE_SET;
			}
			break;
	}
}

/**
 *	UpdateMainPlayer()
 *	メイン画面プレイヤー操作フェーズ更新
 */
void GameMain::UpdateMainPlayer()
{
	int i, k, x, y;

	// ブロックの周りチェック
	bool leftMove = true;
	bool rightMove = true;
	for (i = curBlock.size-1; i >= 0; --i) {
		for (k = 0; k < curBlock.size; ++k) {
			if (curBlock.pattern[i][k] != 1) continue;

			x = curX + k;
			y = curY + i;

			// ブロックの左にブロックがあるか
			if (leftMove) {
				if (blocks[y][x-1] != 0) {
					leftMove = false;
				}
			}

			// ブロックの右にブロックがあるか
			if (rightMove) {
				if (blocks[y][x+1] != 0) {
					rightMove = false;
				}
			}
		}
	}

	PRINTF("leftMove=%d rightMove=%d\n", leftMove, rightMove);

	// ブロック回転
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
				if (key < 0) {			// 左
					y = _b.size-k-1;
					x = i;
				} else if (key > 0) {	// 右
					y = k;
					x = _b.size-i-1;
				}

				if (blocks[curY+y][curX+x] != 0) {	// ちょっと大雑把なチェックだけど
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

	// ブロック移動
	else if (LOBYTE(GetAsyncKeyState(VK_LEFT))) {
		if (leftMove) --curX;
	}
	else if (LOBYTE(GetAsyncKeyState(VK_RIGHT))) {
		if (rightMove) ++curX;
	}

	// ブロックの落下チェック
	bool stop = false;
	for (i = curBlock.size-1; i >= 0; --i) {
		for (k = 0; k < curBlock.size; ++k) {
			if (curBlock.pattern[i][k] != 1) continue;

			x = curX + k;
			y = curY + i;

			// ブロックの下にブロックがあるか
			if (!stop) {
				if (blocks[y+1][x] != 0) {
					stop = true;
				}
			}
		}
	}	

	// ブロック落下
	--fallCount;

	if (HIBYTE(GetAsyncKeyState(VK_DOWN))) {
		if (fallCount > 1) fallCount = 1;	// 強制落下
	}

	PRINTF("stop=%d, fallCount=%d\n", stop, fallCount);

	if (fallCount <= 0) {
		if (stop) {
			// ブロック設定
			for (i = 0; i < curBlock.size; ++i) {
				for (k = 0; k < curBlock.size; ++k) {
					if (curBlock.pattern[i][k] != 1) continue;
					blocks[curY + i][curX + k] = curColor;
				}
			}
			cur = false;

			// 消滅チェック
			if (CheckBlocksLine()) {
				// 消滅演出へ
				phase = MAIN_PHASE_BLOCK;
				blinkCount = 16;
			} else {
				// ゲームオーバーチェックへ
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
 *	メイン画面ブロック消滅更新
 */
void GameMain::UpdateMainBlock()
{
	int i, k;
	if (--blinkCount == 0) {
		// ブロック消滅
		for (i = BLOCK_VIEW_TOP; i < BLOCK_VIEW_BOTTOM; ++i) {
			if (!eraseFlags[i]) continue;
			for (k = BLOCK_VIEW_LEFT; k < BLOCK_VIEW_RIGHT; ++k) {
				blocks[i][k] = 0;
			}
		}

		// ブロックを落とす
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

		// スコア加算
		int point;
		point =  x*10 + (x-1)*10;	// 1ライン=10点, 2ライン同時=30点, 3ライン同時=50点, 4ライン同時=70点
		point *= ((level-1)>>1)+1;	// レベルをかけとく
		PRINTF("x1=%d, point=%d\n", x, point);
		score += point;
		if (score > 99999) score = 99999;

		// ライン数加算
		PRINTF("x2=%d\n", x);
		lines += x;
		if (lines > 99999) lines = 99999;

		// レベル加算
		exp -= x;
		if (exp <= 0) {
			++level;
			exp = NextExp(level) + exp;
			fallInter = GetFallInter(level);
		}


		// ゲームオーバーチェックへ
		phase = MAIN_PHASE_CHECK;
	}
}

/**
 *	GameMain::CheckBlocksLine()
 *	ブロックのラインの消滅チェック
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
 *	落下インターを得る
 */
int GameMain::GetFallInter(int level)
{
	int i = 45 - ((level-1)<<1);
	if (i < 0) i = 0;
	return i;
}

/**
 *	次のレベルまでの経験値を得る
 */
int GameMain::NextExp(int level) {
	return level * 6 + level * 2;
}


/**
 *	GameMain::DrawBlock()
 *	単一のブロック描画
 */
void GameMain::DrawBlock(int x, int y, char color)
{
	// 色定義
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

	// 描画
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
 *	積んであるブロックと落ちてくるブロックの描画
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
	
	// 落ちてくるブロックの描画
	if (cur) {
		for (i = 0; i < curBlock.size; ++i) {
			for (k = 0; k < curBlock.size; ++k) {
				if (curBlock.pattern[i][k] != 1) continue;
				DrawBlock((curX+k) * BLOCK_W + offsetX, (curY+i) * BLOCK_H + offsetY, curColor);
			}
		}
	}

	// 積んであるブロックの描画
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
			&& eraseFlags[i] && ((blinkCount&3)>1)) continue;	// 点滅

		for (k = l; k < r; ++k) {
			if (blocks[i][k] == 0) continue;
			DrawBlock(k * BLOCK_W + offsetX, i * BLOCK_H + offsetY, blocks[i][k]);
		}
	}
}


/**
 *	GameMain::DrawBackground()
 *	背景描画
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
 *	プレイ情報描画
 */
void GameMain::DrawInfo()
{
	int i, k, x, y;

	// フォント
	HFONT hFont = NewFont();
	HFONT hFontOld = (HFONT)SelectObject(backDC, hFont);
	
	//
	x = INFO_VIEW_LEFT + BLOCK_W/2;
	y = INFO_VIEW_TOP + BLOCK_H/2;

	// 次のブロック

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

	// スコア
	y += BLOCK_H;

	char s[32];
	sprintf(s, "Score:% 5d", score);
	TextOut(backDC, x, y, s, lstrlen(s));

	// ライン数
	y += BLOCK_H;

	sprintf(s, "Lines:% 5d", lines);
	TextOut(backDC, x, y, s, lstrlen(s));

	// レベル
	y += BLOCK_H;

	sprintf(s, "Level:% 5d", level);
	TextOut(backDC, x, y, s, lstrlen(s));


	// フォントの削除
	SelectObject(backDC, hFontOld);
	DeleteObject(hFont);

}


/**
 *	GameMain::DrawStart()
 *	スタート画面描画
 */
void GameMain::DrawStart()
{
	// フォント
	HFONT hFont = NewFont();
	HFONT hFontOld = (HFONT)SelectObject(backDC, hFont);

	// テキスト
	SetTextColor(backDC, RGB(0xff, 0xff, 0xff));
	SetBkMode(backDC, TRANSPARENT);
	TextOut(backDC, 15, 120, "Press enter key.", strlen("Press enter key."));

	// フォントの削除
	SelectObject(backDC, hFontOld);
	DeleteObject(hFont);
}


/**
 *	GameMain::DrawGameOver()
 *	ゲームオーバー画面描画
 */
void GameMain::DrawGameOver()
{
	if (phase < BLOCK_V_CNT2) return;

	RECT rc = {0, 120-2, BLOCK_VIEW_W, 120+14};
	FillRect(backDC, &rc, (HBRUSH)GetStockObject(BLACK_BRUSH));

	// フォント
	HFONT hFont = NewFont();
	HFONT hFontOld = (HFONT)SelectObject(backDC, hFont);

	// テキスト
	SetTextColor(backDC, RGB(0xff, 0x00, 0x00));
	SetBkMode(backDC, TRANSPARENT);
	TextOut(backDC, 40, 120, "Game Over", strlen("Game Over"));

	// フォントの削除
	SelectObject(backDC, hFontOld);
	DeleteObject(hFont);
}


/**
 *	GameMain::NewFont()
 *	フォントを作成する
 */
HFONT GameMain::NewFont()
{
	return CreateFont(
		12,	// 高さ
		0,	// 文字幅
		0,	// テキストの角度
		0,	
		FW_REGULAR,	// フォントの重さ
		FALSE, FALSE, FALSE,	// I, U, S
		SHIFTJIS_CHARSET,		// 文字セット
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY,
		FIXED_PITCH | FF_MODERN,
		"ＭＳ　ゴシック");
}


