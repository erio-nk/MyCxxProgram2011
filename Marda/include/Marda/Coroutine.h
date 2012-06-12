/*!
	@file	Coroutine.h
	
			簡易コルーチンクラス。

	@author	E.Nakayama
*/
#pragma once
	
#include "Types.h"

namespace Marda
{
	// ------------------------------------------------------------------------
	// 簡易コルーチンクラス。
	
	class Coroutine
	{
	public:
		Coroutine();
	
		~Coroutine();
		
		/*!
			@brief	コルーチンを実行する。
		*/
		int operator () ();
		

	protected:
		/*!
			@brief	コルーチンで実行する処理を記述する。
			@attention	この関数が終了する（関数の最後まで来るか、returnする）前にコルーチンオブジェクトが
						破棄されてしまうと、関数内で構築されたオブジェクトのデストラクタが呼ばれず、
						メモリリークを引き起こす可能性がある為注意。
						関数内でそういったオブジェクトを利用する必要がある場合は、
						メンバ変数として定義する方が安全。
			@return	コルーチン呼び出し(operator ())の戻り値。
		*/
		virtual int Run() = 0;

		/*!
			@brief	コルーチンを中断して呼び出し元に制御を返す。
			@return	コルーチン呼び出し(operator ())の戻り値。
		*/
		void Yeild(int returnValue);
		
	private:
		PVOID _pCallFiberContext;	//!< 呼び出し元ファイバ。
		PVOID _pThisFiberContext;	//!< 自身のファイバ。
		int _returnValue;	//!< コルーチンの戻り値。
		bool _exit;	//!< コルーチンが終了したかどうか。
		
		void Exit();

		static void WINAPI FiberProc(PVOID pParam);
	};	// end class Coroutine


}	// end namespace Marda
