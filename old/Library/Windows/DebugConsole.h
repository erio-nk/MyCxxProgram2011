/*!
 	@file DebugConsole.h
 
 	デバッグ用コンソール。
 
 	@author Dila
 */
#pragma once

#include <windows.h>
#include <tchar.h>
#include <stdarg.h>

#include "Windows/_MSVC.h"

#if defined(_DEBUG) && !defined(_NO_USE_DBGCONSOLE)
	#ifndef _USE_DBGCONSOLE
		#define _USE_DBGCONSOLE
	#endif
#endif

/*!
 	デバッグ用コンソールクラス
 
 	Win32アプリケーション用のデバッグコンソールを扱うクラス。
 */
class DebugConsole
{
public:
	/*!
	 	コンストラクタ
	 */
	DebugConsole();

	//! デストラクタ
	~DebugConsole();

	/*!
	 	インスタンスを取得する
	 	@return	デバッグコンソールオブジェクト
	 */
	static DebugConsole& GetInstance();

	/*!
	 	トレース
	 */
	void Trace(const TCHAR* format, ...);

	void Trace(const TCHAR* format, va_list& va);

};	// end class DebugConsole


/*!
 	トレース関数
 	@see _MSVC.h
 */
#ifdef _USE_DBGCONSOLE
	#if _MSC_VER < _MSVC_VER_8
		// そのうち考える
	#else
		#define _DBGTRACE(format, ...)	DebugConsole::GetInstance().Trace(_T(__FUNCTION__) _T("> ") format _T("\n"), __VA_ARGS__)
	#endif
#else
	#define _DBGTRACE	_noop
#endif
