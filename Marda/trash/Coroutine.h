/*!
	@file	Utilities/Coroutine.h
	
			コルーチン。
			Helpersの簡易コルーチンクラスの方が簡単に利用出来るので使わない事にした。

			Copyright(C) 2011 Erio Nakayama <yamadila@gmail.com>
			All rights reserved.

	@author	E.Nakayama
*/
#pragma once

namespace Marda
{
	class CoroutineStructure
	{
	protected:
		friend void YeildCoroutine();
		template <class ReturnType> friend void YeildCoroutine(ReturnType&);

		PVOID _pCallFiberContext;	//!< 呼び出し元ファイバ。
		PVOID _pThisFiberContext;	//!< 自身のファイバ。
		void* _pReturnValue;	//!< コルーチンの戻り値。
		bool _exit;	//!< コルーチンが終了したかどうか。

		CoroutineStructure();

		~CoroutineStructure();
	
		void Switch(PFIBER_START_ROUTINE pFiberProc);

		void Yeild();

		void Exit();
	};

	/*!
		@brief	コルーチンを中断し、呼び出し元に制御を返す。
				コルーチン以外の関数内で呼び出した場合の動作は未定義。
	*/
	void YeildCoroutine();

	/*!
		@brief	コルーチンを中断し、呼び出し元に制御を返す。
		@param	returnValue	コルーチン呼び出し(Call)の戻り値。
	*/
	template <class ReturnType>
	void YeildCoroutine(ReturnType& returnValue)
	{
		CoroutineStructure* pCoroutine = reinterpret_cast<CoroutineStructure*>(GetFiberData());
		*reinterpret_cast<ReturnType*>(pCoroutine->_pReturnValue) = returnValue;
		pCoroutine->Yeild();
	}


	/*!
		@brief	コルーチンクラステンプレート。
				コルーチンに渡す関数が終わる（関数の最後まで来るかreturnする）前に
				コルーチンオブジェクトが破棄されてしまうと、関数内で構築されたオブジェクトの
				デストラクタが呼ばれず、メモリリークを引き起こす可能性がある為注意。
		@code
		void Coroutine1()
		{
			for (int i = 0; i < 3; ++i)
			{
				printf("Func1\n");
				YeildCoroutine();
			}
		}

		int main()
		{	
			Coroutine<void> coroutine1(&Coroutine1);
			for (int i = 0; i < 5; ++i)
			{
				coroutine1.Call();
			}
			return 0;
		}
		@endcode
	*/
	template <class ReturnType, class ArgType1 = NullType, class ArgType2 = NullType>
	class Coroutine;

	#define Coroutine_Arg0	NullType, NullType
	#define Coroutine_Arg1	ArgType1, NullType

	//! 戻り値無し、引数無し
	template <>
	class Coroutine<void, Coroutine_Arg0>
		: public CoroutineStructure
	{
	public:
		typedef void (*FunctionType)();

		Coroutine(FunctionType function)
			: _function(function)
		{
		}

		void Call()
		{
			Switch(FiberProc);
		}

	private:
		FunctionType _function;

		static void WINAPI FiberProc(PVOID pParam)
		{
			Coroutine* pCoroutine = reinterpret_cast<Coroutine*>(pParam);
			(*pCoroutine->_function)();
			pCoroutine->Exit();
		}
	};	// end class Croutine

	//! 戻り値有り、引数無し
	template <class ReturnType>
	class Coroutine<ReturnType,	Coroutine_Arg0>
		: public CoroutineStructure
	{
	public:
		typedef ReturnType (*FunctionType)();

		Coroutine(FunctionType function)
			: _function(function)
		{
			_pReturnValue = &_returnValue;
		}

		ReturnType Call()
		{
			Switch(FiberProc);
			return _returnValue;
		}

	private:
		FunctionType _function;
		ReturnType _returnValue;

		static void WINAPI FiberProc(PVOID pParam)
		{
			Coroutine* pCoroutine = reinterpret_cast<Coroutine*>(pParam);
			*reinterpret_cast<ReturnType*>(pCoroutine->_pReturnValue)
				= (*pCoroutine->_function)();
			pCoroutine->Exit();
		}
	};	// end class Croutine

	//! 戻り値無し、引数１
	template <class ArgType1>
	class Coroutine<void, Coroutine_Arg1>
		: public CoroutineStructure
	{
	public:
		typedef void (*FunctionType)(ArgType1);

		Coroutine(FunctionType function)
			: _function(function)
		{
		}

		void Call(ArgType1 arg1)
		{
			_pArg1 = &arg1;
			Switch(FiberProc);
		}

	private:
		FunctionType _function;
		ArgType1* _pArg1;

		static void WINAPI FiberProc(PVOID pParam)
		{
			Coroutine* pCoroutine = reinterpret_cast<Coroutine*>(pParam);
			(*pCoroutine->_function)(*pCoroutine->_pArg1);
			pCoroutine->Exit();
		}
	};	// end class Croutine

	
	//! 戻り値有り、引数１
	template <class ReturnType, class ArgType1>
	class Coroutine<ReturnType, Coroutine_Arg1>
		: public CoroutineStructure
	{
	public:
		typedef ReturnType (*FunctionType)(ArgType1);

		Coroutine(FunctionType function)
			: _function(function)
		{
			_pReturnValue = &_returnValue;
		}

		ReturnType Call(ArgType1 arg1)
		{
			_pArg1 = &arg1;
			Switch(FiberProc);
			return _returnValue;
		}

	private:
		FunctionType _function;
		ReturnType _returnValue;
		ArgType1* _pArg1;

		static void WINAPI FiberProc(PVOID pParam)
		{
			Coroutine* pCoroutine = reinterpret_cast<Coroutine*>(pParam);
			*reinterpret_cast<ReturnType*>(pCoroutine->_pReturnValue)
				= (*pCoroutine->_function)(*pCoroutine->_pArg1);
			pCoroutine->Exit();
		}
	};	// end class Croutine

}	// end namespace Marda
