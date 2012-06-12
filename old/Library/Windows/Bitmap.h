/**
 *	Bitmap
 *
 *	ビットマップイメージクラス。
 *
 *	@author	Dila
 */

#pragma once

#include <tchar.h>
#include <windows.h>


/**
 *	Bitmap
 */
class Bitmap
{
protected:
	HBITMAP hBmp;
	HDC hMemDC;

	int w, h, d;

public:
	Bitmap();
	virtual ~Bitmap() { Release(); }

	// 白紙のビットマップ作成
	virtual bool Create(int w, int h);

	// 読み込み
	virtual bool Load(const TCHAR* fileName);

	// 解放
	void Release();

	// 取得
	int GetWidth() { return w; }
	int GetHeight() { return h; }
	int GetDepth() { return d; }

	HBITMAP GetHandle() { return hBmp; }
	HDC GetDC() { return hMemDC; }
};


/**
 *	DIBitmap
 *
 *	デバイス独立ビットマップクラス。
 */
class DIBitmap : public Bitmap
{
	void *pBits;	// ビット列へのポインタ
					// 8bit -> BYTE
					// 16bit-> WORD
					// 24bit-> RGBTRIPLE
					// 32bit-> RGBQUAD
public:
	DIBitmap();

	bool Create(int w, int h);
	bool Create(int w, int h, int d, RGBQUAD* pColorTable = NULL);

	//bool Load(const TCHAR* fileName);
	//bool Load(class BitmapLoader* pLoader);

	void* GetBits() { return pBits; }
};


/**
 *	BitmapLoader
 *
 *	ビットマップローダ。
 */
/*class BitmapLoader
{
	BITMAPFILEHEADER bf;
	BITMAPINFOHEADER bi;
	RGBQUAD* pTable;

public:
	BitmapLoader();

	Load(const TCHAR fileName);
	Load(void* pData);
};*/


/**
 *	ビットマップ用関数
 */

// BITMAPFILEHEADER設定
void SetBitmapFileHeader(BITMAPFILEHEADER* p, int w, int h, int d);

// BITMAPINFO設定
void SetBitmapInfo(BITMAPINFO* p, int w, int h, int d);

// BITMAPINFOHEADER設定
void SetBitmapInfoHeader(BITMAPINFOHEADER* p, int w, int h, int d);

// BITMAPV4HEADER設定
void SetBitmapV4Header(BITMAPV4HEADER* p, int w, int h, int d);

// ラスタサイズ取得
int GetBitmapRasterSize(int w, int d);

// ラスタのパディングサイズ取得
int GetBitmapRasterPaddingSize(int w, int d);

// イメージサイズ取得
int GetBitmapImageSize(int w, int h, int d);

// ファイルサイズ取得
int GetBitmapFileSize(int w, int h, int d);

// カラーテーブルサイズ取得
int GetBitmapColorTableSize(int d);

// カラー数取得
int GetBitmapColorCount(int d);

