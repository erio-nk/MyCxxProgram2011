/*!
 	@file Direct3D/Text.cpp
 
 	テキスト表示クラスの実装
 
 	@author Dila
 */

#include <TCHAR.h>
#include <windows.h>

#include "Direct3D/Text.h"

using namespace Direct3D;


Text::Text() :
	_pFont(NULL),
	_size(0)
{
}

Text::~Text()
{
}


bool Text::Create(Direct3D::Device& device)
{
	HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

	LOGFONT logFont;
	GetObject(hFont, sizeof(LOGFONT), &logFont);

	HRESULT hr = D3DXCreateFont(device.GetInterface(), 
		logFont.lfHeight, logFont.lfWidth,
		logFont.lfWeight,
		0,
		logFont.lfItalic,
		logFont.lfCharSet,
		logFont.lfOutPrecision,
		logFont.lfQuality,
		logFont.lfPitchAndFamily,
		logFont.lfFaceName,
		&_pFont);
	if (FAILED(hr)) {
		return false;
	}

	_size = logFont.lfWidth;

	return true;
}

void Text::Release()
{
	if (_pFont) {
		_pFont->Release();
		_pFont = NULL;
	}
}

HRESULT Text::Render(const TCHAR* text, float x, float y, DWORD color, DWORD format)
{
	RECT rc;
	rc.left = x;
	rc.top = y;
	rc.right = x + _tcslen(text) * _size;
	rc.bottom = y + _size;
	return _pFont->DrawText(NULL, text, -1, &rc, DT_NOCLIP | format, color);
}


