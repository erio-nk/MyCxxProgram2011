/*!
	@file	Utilities/MemoryPool.h
	
			メモリプール。
	@todo	スレッディングポリシーの指定。
	@todo	TLSMemoryPool
			ただし、スレッドの起動にオーバーヘッドがかかってしまう為、WorkerThreadでのみ使用する様に気をつける。
			かつ、WorkerThread側にも、スレッド起動時のイベントハンドラを追加し、そこでTLSMemoryPoolの初期化を行うようにしておけば、万々歳。
	@author	E.Nakayama
*/
#pragma once
	

namespace Marda
{

}	// end namespace Marda
