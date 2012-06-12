// shift_64.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	int rhs32 = 63;
	__int64 rhs64 = 63;

	// NG
	{
		__int64 flags = 1 << rhs32;
		printf("%llx\n", flags);
	}

	// OK
	{
		__int64 flags = static_cast<__int64>(1) << rhs32;
		printf("%llx\n", flags);
	}

	// NG
	{
		__int64 flags = 1 << rhs64;
		printf("%llx\n", flags);
	}

	// OK
	{
		__int64 flags = static_cast<__int64>(1) << rhs64;
		printf("%llx\n", flags);
	}

	return 0;
}

