/**
 *	Bitmap.cpp
 *
 *	@author	Dila
 */

#include "Windows/Bitmap.h"


/**
 *	Bitmap::Bitmap()
 */
Bitmap::Bitmap()
{
	hBmp = NULL;
	hMemDC = NULL;
	w = h = 0;
}


/**
 *	Bitmap::Craete()
 */
bool Bitmap::Create(int w, int h)
{
	if (hMemDC != NULL || hBmp != NULL) {
		return false;	// 既に出来てるよ
	}

	// メモリDC
	hMemDC = CreateCompatibleDC(NULL);
	if (hMemDC == NULL) {
		return false;
	}

	// ビットマップ
	hBmp = CreateCompatibleBitmap(::GetDC(NULL), w, h);	// !これGetDC(NULL)でいいのかね
	if (hBmp == NULL) {
		Release();
		return false;
	}

	// 関連付け
	SelectObject(hMemDC, hBmp);

	this->w = w;
	this->h = h;

	return true;
}


/**
 *	Bitmap::Load()
 */
bool Bitmap::Load(const TCHAR* fileName)
{
	if (hBmp != NULL) {
		DeleteObject(hBmp);	// 既に作ってたら一旦削除
		hBmp = NULL;
	}

	// メモリDC
	if (hMemDC == NULL) {
		hMemDC = CreateCompatibleDC(NULL);
		if (hMemDC == NULL) {
			Release();
			return false;
		}
	}

	// ビットマップ読み込み
	hBmp = (HBITMAP)LoadImage(
		(HINSTANCE)GetModuleHandle(NULL),
		fileName, IMAGE_BITMAP,
		0, 0,	// イメージファイルのサイズと同じ
		LR_LOADFROMFILE
		);
	if (hBmp == NULL) {
		Release();
		return false;
	}

	// 関連付け
	SelectObject(hMemDC, hBmp);

	// 情報を取得する
	BITMAP bmp;
	if (!GetObject(hBmp, sizeof(bmp), &bmp)) {
		return false;	// !とりあえず解放はしないでおくか
	}
	w = bmp.bmWidth;
	h = bmp.bmHeight;
	d = bmp.bmBitsPixel;

	return true;
}


/**
 *	Bitmap::Release()
 */
void Bitmap::Release()
{
	if (hBmp != NULL) {
		DeleteObject(hBmp);
		hBmp = NULL;
	}

	if (hMemDC != NULL) {
		DeleteDC(hMemDC);
		hMemDC = NULL;
	}
}


/* ------------------------------------------------------------------------- */

/**
 *	DIBitmap::DIBitmap()
 */
DIBitmap::DIBitmap()
{
	pBits = NULL;
}


/**
 *	DIBitmap::Create()
 *
 *	@param	d	カラーモード
 *	@param	pColorTable	カラーテーブル
 */
bool DIBitmap::Create(int w, int h) {
	return DIBitmap::Create(w, h, 24, NULL);
}

bool DIBitmap::Create(int w, int h, int d, RGBQUAD* pColorTable) 
{
	if (hMemDC != NULL || hBmp != NULL) {
		return false;	// 既に出来てるよ
	}

	// メモリDC
	hMemDC = CreateCompatibleDC(NULL);
	if (hMemDC == NULL) {
		return false;
	}

	// BITMAPINFO設定
	BITMAPINFO* pInfo;
	int biSize = sizeof(BITMAPINFO);
	int dCnt = GetBitmapColorCount(d);

	if (d <= 8) {
		// カラーテーブルあり
		biSize += sizeof(RGBQUAD) * (dCnt - 1);

		pInfo = (BITMAPINFO*)malloc(biSize);
		if (pInfo == NULL) {
			return false;
		}

		if (pColorTable == NULL) {
			// テーブルの指定が無いのでとりあえず真っ黒パレット
			for (int i = 0; i < dCnt; ++i) {
				pInfo->bmiColors[i].rgbRed = 0;
				pInfo->bmiColors[i].rgbGreen = 0;
				pInfo->bmiColors[i].rgbBlue = 0;
				pInfo->bmiColors[i].rgbReserved = 0;
			}
		} else {
			// テーブルをコピー
			for (int i = 0; i < dCnt; ++i) {
				pInfo->bmiColors[i].rgbRed = pColorTable[i].rgbRed;
				pInfo->bmiColors[i].rgbGreen = pColorTable[i].rgbGreen;
				pInfo->bmiColors[i].rgbBlue = pColorTable[i].rgbBlue;
				pInfo->bmiColors[i].rgbReserved = 0;
			}
		}
	} else {
		// カラーテーブル無し
		pInfo = (BITMAPINFO*)malloc(biSize);
		if (pInfo == NULL) {
			Release();
			return false;
		}
	}

	SetBitmapInfo(pInfo, w, h, d);

	// DIBセクション作成
	pBits = NULL;

	hBmp = CreateDIBSection(NULL, pInfo, DIB_RGB_COLORS, &pBits, NULL, 0);
	if (hBmp == NULL) {
		free(pInfo);
		Release();
		return false;
	}

	free(pInfo);

	// 関連付け
	SelectObject(hMemDC, hBmp);

	this->w = w;
	this->h = h > 0 ? h : -h;
	this->d = d;

	return true;
}



/* ------------------------------------------------------------------------- */

// ビットマップ用関数

/**
 *	SetBitmapFileHeader()
 */
void SetBitmapFileHeader(BITMAPFILEHEADER* p, int w, int h, int d)
{
	if (p == NULL) { return; }

	ZeroMemory(p, sizeof(BITMAPFILEHEADER));

	p->bfType = 0x4d42;	// シグネチャ"BM"
	p->bfSize = GetBitmapFileSize(w, h, d);
	p->bfReserved1= p->bfReserved2 = 0;
	p->bfOffBits  = p->bfSize - GetBitmapImageSize(w, h, d);	// ビット列開始アドレス
}


/**
 *	SetBitmapInfo()
 */
void SetBitmapInfo(BITMAPINFO* p, int w, int h, int d) {
	if (p == NULL) { return; }

	SetBitmapInfoHeader(&p->bmiHeader, w, h, d);
}


/**
 *	SetBitmapInfoHeader()
 */
void SetBitmapInfoHeader(BITMAPINFOHEADER* p, int w, int h, int d) 
{
	if (p == NULL) { return; }

	ZeroMemory(p, sizeof(BITMAPINFOHEADER));

	p->biSize    = sizeof(BITMAPINFOHEADER);
	p->biWidth   = w;
	p->biHeight  = h;
	p->biPlanes  = 1;	// 1でよい
	p->biBitCount= (WORD)d;

	p->biCompression = BI_RGB;	// 圧縮形式 通常はBI_RGB
	//p->biCompression	= BI_BITFIELDS;	//ピクセルフォーマットを有する

	p->biSizeImage = GetBitmapImageSize(w, h, d);
	p->biXPelsPerMeter= 0;
	p->biYPelsPerMeter= 0;
	p->biClrUsed      = 0;	// 0でよい
	p->biClrImportant = 0;
}


/**
 *	SetBitmapV4Header()
 */
void SetBitmapV4Header(BITMAPV4HEADER* p, int w, int h, int d)
{
	if (p == NULL) { return; }

	ZeroMemory(p, sizeof(BITMAPV4HEADER));

	p->bV4Size    = sizeof(BITMAPV4HEADER);
	p->bV4Width   = w;
	p->bV4Height  = h;
	p->bV4Planes  = 1;
	p->bV4BitCount= (WORD)d;

	p->bV4V4Compression = BI_BITFIELDS;

	p->bV4SizeImage = GetBitmapImageSize(w, h, d);
	p->bV4XPelsPerMeter= 0;
	p->bV4YPelsPerMeter= 0;
	p->bV4ClrUsed      = 0;
	p->bV4ClrImportant = 0;

	p->bV4RedMask  = 0x00ff0000;
	p->bV4GreenMask= 0x0000ff00;
	p->bV4BlueMask = 0x000000ff;
	p->bV4AlphaMask= 0x00000000;
}


/**
 *	GetBitmapRasterSize()
 *
 *	ビットマップの1ラスタのサイズを取得する.
 *
 *	@param	w	横幅
 *	@param	d	カラーモード
 */
int GetBitmapRasterSize(int w, int d) {
	if ((w * d) & 31) {
		return (w * d + (32 - ((w * d) & 31))) >> 3;
	}
	return (w * d) >> 3;
}


/**
 *	GetBitmapRasterPaddingSize()
 *
 *	ビットマップの1ラスタのパディングサイズを取得する.
 *
 *	@param	w	横幅
 *	@param	d	カラーモード
 */
int GetBitmapRasterPaddingSize(int w, int d) {
	int size = 0;

	if (((w * d) & 31) == 0) {
		size = 0;
	} else {
		size = (32 - ((d * w) & 31)) >> 3;
	}

	return size;
}


/**
 *	GetBitmapImageSize()
 *
 *	ビットマップのイメージ部分(ビット列のみ)のサイズを取得する.
 *
 *	@param	w	横幅
 *	@param	h	縦幅
 *	@param	d	カラーモード
 */
int GetBitmapImageSize(int w, int h, int d) {
	return GetBitmapRasterSize(w, d) * h;
}


/**
 *	GetBitmapFileSize()
 *
 *	ビットマップファイルのサイズを取得する.
 *
 *	@param	w	横幅
 *	@param	h	縦幅
 *	@param	d	カラーモード
 */
int GetBitmapFileSize(int w, int h, int d) {
	return sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
		GetBitmapColorTableSize(d) + GetBitmapImageSize(w, h, d);
}


/**
 *	GetBitmapColorTableSize()
 *
 *	ビットマップのカラーテーブルサイズを取得する.
 *
 *	@param	d	カラーモード
 */
int GetBitmapColorTableSize(int d) {
	if (d <= 8) {
		return GetBitmapColorCount(d) << 2;
	}
	return 0;
}


/**
 *	GetBitmapColorCount()
 *
 *	ビットマップの色数を取得する.
 *
 *	@param	d	カラーモード
 */
int GetBitmapColorCount(int d) {
	switch (d) {
		case  1: return 2;
		case  4: return 16;
		case  8: return 256;
		case 16: return 256*256;
		case 24:
		case 32: return 256*256*256;
	}

	return 0;
}

