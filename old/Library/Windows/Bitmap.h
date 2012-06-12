/**
 *	Bitmap
 *
 *	�r�b�g�}�b�v�C���[�W�N���X�B
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

	// �����̃r�b�g�}�b�v�쐬
	virtual bool Create(int w, int h);

	// �ǂݍ���
	virtual bool Load(const TCHAR* fileName);

	// ���
	void Release();

	// �擾
	int GetWidth() { return w; }
	int GetHeight() { return h; }
	int GetDepth() { return d; }

	HBITMAP GetHandle() { return hBmp; }
	HDC GetDC() { return hMemDC; }
};


/**
 *	DIBitmap
 *
 *	�f�o�C�X�Ɨ��r�b�g�}�b�v�N���X�B
 */
class DIBitmap : public Bitmap
{
	void *pBits;	// �r�b�g��ւ̃|�C���^
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
 *	�r�b�g�}�b�v���[�_�B
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
 *	�r�b�g�}�b�v�p�֐�
 */

// BITMAPFILEHEADER�ݒ�
void SetBitmapFileHeader(BITMAPFILEHEADER* p, int w, int h, int d);

// BITMAPINFO�ݒ�
void SetBitmapInfo(BITMAPINFO* p, int w, int h, int d);

// BITMAPINFOHEADER�ݒ�
void SetBitmapInfoHeader(BITMAPINFOHEADER* p, int w, int h, int d);

// BITMAPV4HEADER�ݒ�
void SetBitmapV4Header(BITMAPV4HEADER* p, int w, int h, int d);

// ���X�^�T�C�Y�擾
int GetBitmapRasterSize(int w, int d);

// ���X�^�̃p�f�B���O�T�C�Y�擾
int GetBitmapRasterPaddingSize(int w, int d);

// �C���[�W�T�C�Y�擾
int GetBitmapImageSize(int w, int h, int d);

// �t�@�C���T�C�Y�擾
int GetBitmapFileSize(int w, int h, int d);

// �J���[�e�[�u���T�C�Y�擾
int GetBitmapColorTableSize(int d);

// �J���[���擾
int GetBitmapColorCount(int d);

