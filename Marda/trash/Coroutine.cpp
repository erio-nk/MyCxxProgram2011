/*!
	@file	Utilities/Coroutine.h

			Copyright(C) 2011 Erio Nakayama <yamadila@gmail.com>
			All rights reserved.

	@author	E.Nakayama
*/

#include "Marda/Utilities/Coroutine.h"

using namespace Marda;

CoroutineStructure::CoroutineStructure()
	: _pCallFiberContext(NULL)
	, _pThisFiberContext(NULL)
	, _pReturnValue(NULL)
	, _exit(false)
{
}

CoroutineStructure::~CoroutineStructure()
{
	if (is_not_null(_pThisFiberContext))
	{
		DeleteFiber(_pThisFiberContext);
		_pThisFiberContext = NULL;
	}
}
	
void 
CoroutineStructure::Switch(PFIBER_START_ROUTINE pFiberProc)
{
	if (_exit) return;

	// 呼び出し元スレッドがまだファイバになっていなければファイバに変換する。
	if (!IsThreadAFiber())
	{
		_pCallFiberContext = ConvertThreadToFiber(NULL);
	}
	else
	{
		_pCallFiberContext = GetCurrentFiber();
	}

	// コルーチン用のファイバがまだ作成されていなければ作成する。
	if (is_null(_pThisFiberContext))
	{
		_pThisFiberContext = CreateFiberEx(0, 0, FIBER_FLAG_FLOAT_SWITCH, pFiberProc, this);
	}
	
	// コルーチン用のファイバに切り替える。
	SwitchToFiber(_pThisFiberContext);

	// 呼び出したコルーチンが終了していたらファイバを削除する。
	if (_exit)
	{
		DeleteFiber(_pThisFiberContext);
		_pThisFiberContext = NULL;
	}
}

void 
CoroutineStructure::Yeild()
{
	// 呼び出し元ファイバに切り替える。
	SwitchToFiber(_pCallFiberContext);
}

void 
CoroutineStructure::Exit()
{
	// コルーチン終了フラグを立てて呼び出し元ファイバに切り替える。
	_exit = true;
	SwitchToFiber(_pCallFiberContext);
}

void 
Marda::YeildCoroutine()
{
	CoroutineStructure* pCoroutine = reinterpret_cast<CoroutineStructure*>(GetFiberData());
	pCoroutine->Yeild();
}

