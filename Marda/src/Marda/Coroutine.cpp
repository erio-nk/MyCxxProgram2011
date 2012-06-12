/*!
	@file	Coroutine.h

	@author	E.Nakayama
*/

#include "Marda/Thread.h"

using namespace Marda;

Coroutine::Coroutine()
	: _pCallFiberContext(NULL)
	, _pThisFiberContext(NULL)
	, _returnValue(0)
	, _exit(false)
{
}

Coroutine::~Coroutine()
{
	if (is_not_null(_pThisFiberContext))
	{
		DeleteFiber(_pThisFiberContext);
		_pThisFiberContext = NULL;
	}
}

int 
Coroutine::operator () ()
{
	if (_exit) return _returnValue;
	
	// 呼び出し元スレッドがまだファイバになっていなければファイバに変換する。
	if (IsThreadAFiber())
	{
		_pCallFiberContext = GetCurrentFiber();
	}
	else
	{
		_pCallFiberContext = ConvertThreadToFiber(NULL);
	}

	// コルーチン用のファイバがまだ作成されていなければ作成する。
	if (is_null(_pThisFiberContext))
	{
		_pThisFiberContext = CreateFiberEx(0, 0, FIBER_FLAG_FLOAT_SWITCH, &FiberProc, this);
	}
	
	// コルーチン用のファイバに切り替える。
	SwitchToFiber(_pThisFiberContext);

	// 呼び出したコルーチンが終了していたらファイバを削除する。
	if (_exit)
	{
		DeleteFiber(_pThisFiberContext);
		_pThisFiberContext = NULL;
	}

	return _returnValue;
}

void
Coroutine::Yeild(int returnValue)
{
	_returnValue = returnValue;
	SwitchToFiber(_pCallFiberContext);
}

void
Coroutine::Exit()
{
	_exit = true;
	SwitchToFiber(_pCallFiberContext);
}

void WINAPI 
Coroutine::FiberProc(PVOID pParam)
{
	Coroutine* pCoroutine = reinterpret_cast<Coroutine*>(pParam);
	pCoroutine->Run();
	pCoroutine->Exit();
}
