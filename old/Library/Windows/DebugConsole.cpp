/*!
 	@file DebugConsole.cpp
 
 	�f�o�b�O�R���\�[���N���X�̎���
 
 	@author Dila
 */
#include <stdio.h>
#include <locale.h>
#include "Windows/DebugConsole.h"


DebugConsole::DebugConsole()
{
#ifdef _USE_DBGCONSOLE
	_tsetlocale(LC_ALL, _T("jpn"));
	if (AllocConsole()) {
		_tfreopen(_T("CONOUT$"), _T("w"), stdout);
		_tfreopen(_T("CONIN$"), _T("r"), stdin);
	} else {
		// !�Ƃ肠�������s�͖���
	}
#endif
}

DebugConsole::~DebugConsole()
{
	FreeConsole();
}

DebugConsole& DebugConsole::GetInstance()
{
	static DebugConsole theInstance;
	return theInstance;
}

void DebugConsole::Trace(const TCHAR* format, ...)
{
	va_list va;
	va_start(va, format);
	_vtprintf(format, va);
	va_end(va);
}

void DebugConsole::Trace(const TCHAR* format, va_list& va)
{
	_vtprintf(format, va);
}