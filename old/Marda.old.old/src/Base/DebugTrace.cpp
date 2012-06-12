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
	// イニシャライザ
	struct DbgTraceInitializer
	{
		#if !defined(_CONSOLE)
			FILE* _fpStdOut;
		#endif

		DbgTraceInitializer()
		{
			// 初期化
			_tsetlocale(LC_ALL, _T("jpn"));
			#if !defined(_CONSOLE)
				// for Windows application
				if (AllocConsole()) {
					_tfreopen_s(&_fpStdOut, _T("CONOUT$"), _T("w"), stdout);
				} else {
					// とりあえず失敗は無視
				}
				DBGTRACE(_T("Debug console started.\n"));
			#endif
		}

		~DbgTraceInitializer()
		{
			// 後始末
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
	static DbgTraceInitializer initializer;	// 最初の呼び出し時のみ起動

	va_list va;
	va_start(va, format);

	_vtprintf(format, va);

	va_end(va);
}

#endif
