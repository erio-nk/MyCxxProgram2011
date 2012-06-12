/**
 *	GameMain.h
 *
 *	@author	Dila
 */

#include "Windows/Window.h"
#include "Windows/Bitmap.h"
#include "Game/FpsTimer.h"

//// define

// デバッグビュー
//#define _DEBUG_VIEW

// ブロックのサイズ
#define BLOCK_W	(16)
#define BLOCK_H	(16)

// 画面中のブロック最大数
#define BLOCK_H_CNT (8+2)		// 横の最大数
#define	BLOCK_V_CNT	(20+1)	// 縦の最大数
#define BLOCK_H_CNT2 (8)	// 見える範囲の横の最大数
#define BLOCK_V_CNT2 (16)	// 見える範囲の縦の最大数

// ブロック画面のサイズ
#define BLOCK_VIEW_W (BLOCK_W*BLOCK_H_CNT2)
#define BLOCK_VIEW_H (BLOCK_H*BLOCK_V_CNT2)

#define BLOCK_VIEW_LEFT	(1)
#define BLOCK_VIEW_TOP	(4)
#define BLOCK_VIEW_RIGHT	(BLOCK_VIEW_LEFT+BLOCK_H_CNT2)
#define BLOCK_VIEW_BOTTOM	(BLOCK_VIEW_TOP+BLOCK_V_CNT2)

// 情報画面のサイズ
#define INFO_VIEW_W (BLOCK_W*5)
#define INFO_VIEW_H (BLOCK_H*BLOCK_V_CNT2)

#define INFO_VIEW_TOP	(0)
#ifdef _DEBUG_VIEW
	#define INFO_VIEW_LEFT	(BLOCK_H_CNT*BLOCK_W)
#else
	#define INFO_VIEW_LEFT	(BLOCK_VIEW_W)
#endif

// 画面サイズ
#ifdef _DEBUG_VIEW
	#define SCREEN_W	(BLOCK_H_CNT*BLOCK_W+INFO_VIEW_W)	
	#define SCREEN_H	(BLOCK_V_CNT*BLOCK_W)
#else
	#define SCREEN_W	(BLOCK_VIEW_W+INFO_VIEW_W)	
	#define SCREEN_H	(BLOCK_H*BLOCK_V_CNT2)
#endif

//// enum
enum GAME_SCENE {
	GAME_SCENE_START,	// スタート画面
	GAME_SCENE_MAIN,	// メイン画面
	GAME_SCENE_GAMEOVER,// ゲームオーバー画面
};

enum MAIN_PHASE {
	MAIN_PHASE_INIT,	// メイン画面初期化
	MAIN_PHASE_SET,		// カレントブロックセット
	MAIN_PHASE_PLAYER,	// プレイヤー操作
	MAIN_PHASE_BLOCK,	// ブロック消滅
	MAIN_PHASE_CHECK,	// ゲームオーバーチェック
};

//// struct
struct BLOCK {
	char size;			// サイズ
	char pattern[4][4];	// パターン
};


/**
 *	GameMain
 */
class GameMain
{
	static GameMain theInstance;

	Window wnd;		// メインウィンドウ

	Bitmap backBuf;	// バックバッファ
	HDC backDC;		// バックバッファのDC

	FpsTimer fps;

	char blocks[BLOCK_V_CNT][BLOCK_H_CNT];
	bool eraseFlags[BLOCK_V_CNT];

	int scene;		// ゲームシーン
	int phase;		// ゲームフェーズ

	// 現在のブロック
	bool cur;
	BLOCK curBlock;
	char curColor;	// 色
	int curX;		// X座標
	int curY;		// Y座標


	//
	int fallInter;	// 落下インター
	int fallCount;	// 落下カウント

	//
	char blinkCount;	// ブロック消滅演出の点滅カウント

	// プレイ情報
	char nextPattern;	// 次のパターン
	int score;			// スコア
	int lines;			// 消したライン数
	int level;			// レベル
	int exp;			// 次のレベルまで

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


