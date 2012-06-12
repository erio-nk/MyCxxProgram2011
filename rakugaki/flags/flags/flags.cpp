// flags.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	// 指定のインデックスのフラグを削除して、後続のフラグをひとつ右にずらす
	// std::vector<bool> 的なものの、eraseに相当する
	int flags = 0x00f6;

	//int index = 32 + 4;	// 左シフト演算子の左辺は常に32の剰余演算されるよーだ。32->0回シフト、36->4回シフト。
	int index = 31;

	int mask = -1 << index;

	int result = (flags & (~mask)) | ((flags >> 1) & mask);

	printf("flags=%x result=%x\n", flags, result);

	return 0;
}

