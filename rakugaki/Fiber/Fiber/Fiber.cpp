// Fiber.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

#include <Windows.h>

#include <functional>
#include <typeinfo>

const int LOOP_COUNT_IN_MAIN = 5;
const int LOOP_COUNT_IN_FIBER = 2;

PVOID pMainFiber;
PVOID pFibers[4];

struct DestroyChecker
{
	~DestroyChecker()
	{
		printf("Fiber[%p]: Destroyed.\n");
	}
};

VOID WINAPI FiberFunc(PVOID pvParam)
{
	DestroyChecker d;

	for (int i = 0; i < LOOP_COUNT_IN_FIBER; ++i)
	{
		printf("Fiber[%p]: %d (%x)\n", GetCurrentFiber(), i, GetCurrentThreadId());
		SwitchToFiber(pMainFiber);
	}
	// ココに来ちゃうとスレッド自体が終わっちゃう
}

int moke()
{
}

int _tmain(int argc, _TCHAR* argv[])
{
	printf("%s\n"), typeid(std::result_of<moke>::type).name());

	pMainFiber = ConvertThreadToFiber(NULL);
	
	for (int i = 0; i < _countof(pFibers); ++i)
	{
		pFibers[i] = CreateFiber(0, FiberFunc, NULL);
	}

	for (int i = 0; i < LOOP_COUNT_IN_MAIN; ++i)
	{
		for (int k = 0; k < _countof(pFibers); ++k)
		{
			SwitchToFiber(pFibers[k]);
		}
	}

	for (int i = 0; i < _countof(pFibers); ++i)
	{
		DeleteFiber(pFibers[i]);
	}

	ConvertFiberToThread();

	printf("End.\n");

	return 0;
}

