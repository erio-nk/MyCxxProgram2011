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
		return false;	// ���ɏo���Ă��
	}

	// ������DC
	hMemDC = CreateCompatibleDC(NULL);
	if (hMemDC == NULL) {
		return false;
	}

	// �r�b�g�}�b�v
	hBmp = CreateCompatibleBitmap(::GetDC(NULL), w, h);	// !����GetDC(NULL)�ł����̂���
	if (hBmp == NULL) {
		Release();
		return false;
	}

	// �֘A�t��
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
		DeleteObject(hBmp);	// ���ɍ���Ă����U�폜
		hBmp = NULL;
	}

	// ������DC
	if (hMemDC == NULL) {
		hMemDC = CreateCompatibleDC(NULL);
		if (hMemDC == NULL) {
			Release();
			return false;
		}
	}

	// �r�b�g�}�b�v�ǂݍ���
	hBmp = (HBITMAP)LoadImage(
		(HINSTANCE)GetModuleHandle(NULL),
		fileName, IMAGE_BITMAP,
		0, 0,	// �C���[�W�t�@�C���̃T�C�Y�Ɠ���
		LR_LOADFROMFILE
		);
	if (hBmp == NULL) {
		Release();
		return false;
	}

	// �֘A�t��
	SelectObject(hMemDC, hBmp);

	// �����擾����
	BITMAP bmp;
	if (!GetObject(hBmp, sizeof(bmp), &bmp)) {
		return false;	// !�Ƃ肠��������͂��Ȃ��ł�����
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
 *	@param	d	�J���[���[�h
 *	@param	pColorTable	�J���[�e�[�u��
 */
bool DIBitmap::Create(int w, int h) {
	return DIBitmap::Create(w, h, 24, NULL);
}

bool DIBitmap::Create(int w, int h, int d, RGBQUAD* pColorTable) 
{
	if (hMemDC != NULL || hBmp != NULL) {
		return false;	// ���ɏo���Ă��
	}

	// ������DC
	hMemDC = CreateCompatibleDC(NULL);
	if (hMemDC == NULL) {
		return false;
	}

	// BITMAPINFO�ݒ�
	BITMAPINFO* pInfo;
	int biSize = sizeof(BITMAPINFO);
	int dCnt = GetBitmapColorCount(d);

	if (d <= 8) {
		// �J���[�e�[�u������
		biSize += sizeof(RGBQUAD) * (dCnt - 1);

		pInfo = (BITMAPINFO*)malloc(biSize);
		if (pInfo == NULL) {
			return false;
		}

		if (pColorTable == NULL) {
			// �e�[�u���̎w�肪�����̂łƂ肠�����^�����p���b�g
			for (int i = 0; i < dCnt; ++i) {
				pInfo->bmiColors[i].rgbRed = 0;
				pInfo->bmiColors[i].rgbGreen = 0;
				pInfo->bmiColors[i].rgbBlue = 0;
				pInfo->bmiColors[i].rgbReserved = 0;
			}
		} else {
			// �e�[�u�����R�s�[
			for (int i = 0; i < dCnt; ++i) {
				pInfo->bmiColors[i].rgbRed = pColorTable[i].rgbRed;
				pInfo->bmiColors[i].rgbGreen = pColorTable[i].rgbGreen;
				pInfo->bmiColors[i].rgbBlue = pColorTable[i].rgbBlue;
				pInfo->bmiColors[i].rgbReserved = 0;
			}
		}
	} else {
		// �J���[�e�[�u������
		pInfo = (BITMAPINFO*)malloc(biSize);
		if (pInfo == NULL) {
			Release();
			return false;
		}
	}

	SetBitmapInfo(pInfo, w, h, d);

	// DIB�Z�N�V�����쐬
	pBits = NULL;

	hBmp = CreateDIBSection(NULL, pInfo, DIB_RGB_COLORS, &pBits, NULL, 0);
	if (hBmp == NULL) {
		free(pInfo);
		Release();
		return false;
	}

	free(pInfo);

	// �֘A�t��
	SelectObject(hMemDC, hBmp);

	this->w = w;
	this->h = h > 0 ? h : -h;
	this->d = d;

	return true;
}



/* ------------------------------------------------------------------------- */

// �r�b�g�}�b�v�p�֐�

/**
 *	SetBitmapFileHeader()
 */
void SetBitmapFileHeader(BITMAPFILEHEADER* p, int w, int h, int d)
{
	if (p == NULL) { return; }

	ZeroMemory(p, sizeof(BITMAPFILEHEADER));

	p->bfType = 0x4d42;	// �V�O�l�`��"BM"
	p->bfSize = GetBitmapFileSize(w, h, d);
	p->bfReserved1= p->bfReserved2 = 0;
	p->bfOffBits  = p->bfSize - GetBitmapImageSize(w, h, d);	// �r�b�g��J�n�A�h���X
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
	p->biPlanes  = 1;	// 1�ł悢
	p->biBitCount= (WORD)d;

	p->biCompression = BI_RGB;	// ���k�`�� �ʏ��BI_RGB
	//p->biCompression	= BI_BITFIELDS;	//�s�N�Z���t�H�[�}�b�g��L����

	p->biSizeImage = GetBitmapImageSize(w, h, d);
	p->biXPelsPerMeter= 0;
	p->biYPelsPerMeter= 0;
	p->biClrUsed      = 0;	// 0�ł悢
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
 *	�r�b�g�}�b�v��1���X�^�̃T�C�Y���擾����.
 *
 *	@param	w	����
 *	@param	d	�J���[���[�h
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
 *	�r�b�g�}�b�v��1���X�^�̃p�f�B���O�T�C�Y���擾����.
 *
 *	@param	w	����
 *	@param	d	�J���[���[�h
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
 *	�r�b�g�}�b�v�̃C���[�W����(�r�b�g��̂�)�̃T�C�Y���擾����.
 *
 *	@param	w	����
 *	@param	h	�c��
 *	@param	d	�J���[���[�h
 */
int GetBitmapImageSize(int w, int h, int d) {
	return GetBitmapRasterSize(w, d) * h;
}


/**
 *	GetBitmapFileSize()
 *
 *	�r�b�g�}�b�v�t�@�C���̃T�C�Y���擾����.
 *
 *	@param	w	����
 *	@param	h	�c��
 *	@param	d	�J���[���[�h
 */
int GetBitmapFileSize(int w, int h, int d) {
	return sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
		GetBitmapColorTableSize(d) + GetBitmapImageSize(w, h, d);
}


/**
 *	GetBitmapColorTableSize()
 *
 *	�r�b�g�}�b�v�̃J���[�e�[�u���T�C�Y���擾����.
 *
 *	@param	d	�J���[���[�h
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
 *	�r�b�g�}�b�v�̐F�����擾����.
 *
 *	@param	d	�J���[���[�h
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

