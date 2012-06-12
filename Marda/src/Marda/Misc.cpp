/*!
	@file	Misc.cpp

	@author	E.Nakayama
*/

#include "Marda/Misc.h"

using namespace Marda;

// ----------------------------------------------------------------------------
// デバッグ出力。

void 
Marda::DebugTraceA(const char* format, ...)
{
	static char buf[1024];
	va_list va;
	va_start(va, format);
	vsprintf_s(buf, format, va);
	va_end(va);
	OutputDebugStringA(buf);
	OutputDebugStringA("\n");
}

void 
Marda::DebugTraceW(const wchar_t* format, ...)
{
	static wchar_t buf[256];
	va_list va;
	va_start(va, format);
	vswprintf_s(buf, format, va);
	va_end(va);
	OutputDebugStringW(buf);
	OutputDebugStringW(L"\n");
}


// ----------------------------------------------------------------------------
// 例外関連。

namespace 
{
	void structured_exception_translator(unsigned int code, _EXCEPTION_POINTERS* information)
	{
		throw StructuredException(code, information);
	}
}

void
Marda::EnableStructuredExceptionTranslation(bool enable)
{
	if (enable)
	{
		_set_se_translator(structured_exception_translator);
	}
	else
	{
		_set_se_translator(NULL);
	}
}
	
bool
Marda::IsEnabledStructuredExceptionTranslation()
{
	// get出来ないので、一度デフォルトに戻して以前のハンドラを調べ、structured_exception_translator と一致していたら true とする。
	_se_translator_function current = _set_se_translator(NULL);
	_set_se_translator(NULL);
	return current == structured_exception_translator;
}
