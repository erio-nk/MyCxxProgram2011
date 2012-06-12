// cast.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

#include <stdio.h>

int _tmain(int argc, _TCHAR* argv[])
{
	int src = argc;//1;

	int dst32 = src;

	long long dst64 = src;

	printf("dst32=%d, dst64=%lld\n", dst32, dst64);

	return 0;
}

