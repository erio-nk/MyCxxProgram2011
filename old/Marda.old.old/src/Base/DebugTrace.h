/*!
	@file DebugTrace.h

	Copyright(C) 2008-2009 Dila <yamadila@gmail.com>
		All rights reserved.

	@author Dila
 */
#pragma once

namespace Marda
{
#if (defined(_DEBUG) || defined(USE_DBGTRACE)) && !defined(NO_DBGTRACE)

	/*!
		デバッグトレース(DBGTRACE)

		デバッグ用のトレース出力を行う関数。
		Debugビルド時及びUSE_DBGTRACEマクロ定義時に出力される。
		Releaseビルド時もしくはNO_DBGTRACEマクロ定義時には出力されず、
		コード自体が__noopに置き換えられる。

		Windowsアプリケーションでの利用時は、最初のトレース呼び出し時に
		コンソールの初期化を行ってから出力される。

		@code
			DBGTRACE(_T("Message, p1=%, p2=%s"), 10, _T("hoge"));
		@endcode

		@param	format	フォーマット文字列
	 */
	void DebugTrace(const TCHAR* format, ...);

	#if MSVC_VER >= MSVC_9
		#define DBGTRACE(format, ...)	Marda::DebugTrace(_T("%-20s> ") format _T("\n"), _T(__FUNCTION__), __VA_ARGS__)
	#else
		#error // TODO: 何か考える？	Marda::DebugPreTrace(_T("%-20s> ", _T(__FUNCTION__)); Marda::DebugTrace	// \nはDebugTrace内で
	#endif

#else
	#define DBGTRACE(...) __noop
#endif

}	// end namespace Marda
