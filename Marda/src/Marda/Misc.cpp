/*!
	@file	Misc.cpp

	@author	E.Nakayama
*/

#include "Marda/Misc.h"

using namespace Marda;

// ----------------------------------------------------------------------------
// �f�o�b�O�o�́B

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
// ��O�֘A�B

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
	// get�o���Ȃ��̂ŁA��x�f�t�H���g�ɖ߂��ĈȑO�̃n���h���𒲂ׁAstructured_exception_translator �ƈ�v���Ă����� true �Ƃ���B
	_se_translator_function current = _set_se_translator(NULL);
	_set_se_translator(NULL);
	return current == structured_exception_translator;
}
