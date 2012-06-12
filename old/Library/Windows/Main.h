/*!
 	@file Windows/Main.h
 
 	Windowsアプリケーション用Main関数
 
 	Windowsアプリケーション構築者は、
 	通常のコンソールアプリケーション同様に
 	Main関数を実装するだけで良い。
 
 	@author	Dila
 */
//! @addtogroup Windows
//! @{
//!		@addtogroup Main Main関数
//!		@{

#pragma once

#include <windows.h>


/*!
 	メイン関数
 
 	利用者はこの関数を必ず実装する事。
 	インスタンスハンドルの取得は::GetModuleHandle(NULL)で、
 	コマンドラインの取得は::GetCommandLine()で行う。
 */
void Main();


/*!
 	メッセージループ関数
 
 	メッセージループを簡略化した処理。
 	Main関数中で呼び出す。
 */
void MessageLoop();


/*!
 	メインループ関数
 
 	メインループを簡略化した処理。
 	Main関数中で呼び出す。
 	こちらは関数及びクラスメソッド用。
 
 	@param	pfnRun	メインループで呼び出す関数。
 */
void MainLoop(void (*pfnRun)());


/*!
 	メインループ関数
 
 	メインループを簡略化した処理。
 	Main関数中で呼び出す。
 	こちらは関数及びインスタンスメソッド用。
 
 	@tparam	T		メインループ用のインスタンスメソッドを持ったクラス型。
 	@param	pObj	pfnRunを呼び出すオブジェクト。
 	@param	pfnRun	メインループで呼び出すインスタンスメソッド。
 */
#pragma warning(push)
#pragma warning(disable: 4127)	// for while(true)

template <class T>
void MainLoop(T* pObj, void (T::*pfnRun)())
{
	MSG msg;

	while (true) {
		if (PeekMessage(&msg, (HWND)NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {
			(pObj->*pfnRun)();
		}
	}
}

#pragma warning(pop)

//!		@}
//!	@}

