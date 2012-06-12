/*!
	@file	Utilities/ThreadPool.h
	
			スレッドプール。
			ppl使えば良くね？

	@author	E.Nakayama
*/
#pragma once
	
#include "WorkerThread.h"

namespace Marda
{
	/*!
		@brief	スレッドプールクラス。
	*/
	class ThreadPool
	{
	public:
		//! ctor
		ThreadPool();
		
		//! dtor
		~ThreadPool();
		
		/*!
			@brief	スレッドプールを作成する。
			@param	numThreads	作成するワーカースレッド数。-1を渡した場合は、CPUの論理コア数分だけワーカースレッドを作成する。
		*/
		bool Create(int numThreads = -1);
		
		/*!
			@brief	スレッドプールを解放する。
					実行中／実行待ちの仕事があった場合は、処理を中断する。
		*/
		void Release();
		
		/*!
			@brief	このワーカースレッドのスレッドIDを取得する
		*/
		DWORD GetThreadID() const;
		
		/*!
			@brief	仕事をプッシュする
			@param	pWork	仕事オブジェクト。
							このパラメータに渡したオブジェクトは、処理が完了するか中断されるまで生存している必要がある。
			@note	この関数はスレッドセーフである。
		*/
		bool Push(WorkerThread::Work* pWork);
		
//		/*!
//			@brief	仕事を中断する
//					中断したい仕事が実行中だった場合は、その仕事が終わるまでブロックされる。
//			@param	pWork	中断する仕事オブジェクト。
//		*/
//		void Cancel(Work* pWork);
//		
//		/*!
//			@brief	仕事が全て終わるまで待つ
//			@param	timeout	タイムアウトするミリ秒
//			@return	仕事が全て終わった場合はtrue、タイムアウトした場合はfalseを返す。
//		*/
//		bool WaitForCompletion(DWORD timeout = INFINITE);
//		
//		/*!
//			@brief	仕事が全て終わったかどうかを返す
//			@attention	この関数の実行には最低1msを要する。
//		*/
//		bool IsCompleted();
		
	private:
		struct Impl;
		std::auto_ptr<Impl> _pimpl;	// pimpl Idiom
	};	// end class ThreadPool
	
}	// end namespace Marda
