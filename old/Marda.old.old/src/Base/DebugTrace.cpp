/*!
	@file DebugTrace.cpp

	Copyright(C) 2008-2009 Dila <yamadila@gmail.com>
		All rights reserved.

	@author Dila
 */

#include "Marda.h"

using namespace Marda;

#if (defined(_DEBUG) || defined(USE_DBGTRACE)) && !defined(NO_DBGTRACE)

namespace
{
	// �C�j�V�����C�U
	struct DbgTraceInitializer
	{
		#if !defined(_CONSOLE)
			FILE* _fpStdOut;
		#endif

		DbgTraceInitializer()
		{
			// ������
			_tsetlocale(LC_ALL, _T("jpn"));
			#if !defined(_CONSOLE)
				// for Windows application
				if (AllocConsole()) {
					_tfreopen_s(&_fpStdOut, _T("CONOUT$"), _T("w"), stdout);
				} else {
					// �Ƃ肠�������s�͖���
				}
				DBGTRACE(_T("Debug console started.\n"));
			#endif
		}

		~DbgTraceInitializer()
		{
			// ��n��
			#if !defined(_CONSOLE)
				if (_fpStdOut != NULL) {
					fclose(_fpStdOut);
					_fpStdOut = NULL;
					FreeConsole();
				}
			#endif
		}
	};
}

void Marda::DebugTrace(const TCHAR* format, ...)
{
	static DbgTraceInitializer initializer;	// �ŏ��̌Ăяo�����̂݋N��

	va_list va;
	va_start(va, format);

	_vtprintf(format, va);

	va_end(va);
}

#endif
