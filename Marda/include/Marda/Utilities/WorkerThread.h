/*!
	@file	Utilities/WorkerThread.h
	
			ワーカースレッドクラス。

	@author	E.Nakayama
*/
#pragma once
	

namespace Marda
{
	/*!
		@brief	ワーカースレッドクラス。
				マルチスレッドのデザインパターン Worker Thread パターンを実装したクラス。
				仕事を終えてもスレッドを終了させず、次の仕事を待ち続けるスレッドユーティリティクラス。
	*/
	class WorkerThread : private Uncopyable
	{
	public:
		struct Impl;

		/*!
			@brief	仕事インターフェース。
					このインタフェースを実装した具象クラスを Push 関数に渡していく事で処理を行う。
		*/
		class Work
		{
		public:
			enum STATE
			{
				STATE_NONE,
				STATE_QUEUEING,		//!< 実行待ち。
				STATE_EXECUTING,	//!< 実行中。
				STATE_COMPLETE,		//!< 実行完了。
				STATE_CANCELED,		//!< 中断された。
				STATE_ERROR,		//!< エラー終了した。
			};
			
			Work();

			virtual ~Work()
			{
			}
			
			WorkerThread::Impl* GetOwner() const { return _pOwner; }
			
			STATE GetState() const { return _state; }
			
			/*!
				@brief	実行処理。
						サブクラスは、この関数を必ずオーバーライドしなければならない。
				@return	エラーコード(ERROR_CODE)。 
						ERR_NOERROR を返した場合は、STATE_COMPLETE になる。
						ERR_CANCELD を返した場合は、STATE_CANCELD になる。
						ERR_UNKNOWN または上記以外のエラーコードを返した場合は、STATE_ERROR になる。
			*/
			virtual ERROR_CODE Execute() = 0;
			
			/*!
				@brief	状態変更通知イベント。
						状態が変わる度に呼び出されるイベントハンドラ。
						サブクラスは、必要であればこのハンドラをオーバーライドして
						適切に処理する事が出来る。
				@param	state	変更後の状態
			*/
			virtual void OnNotify(STATE state) { UNREFERENCED_PARAMETER(state); }

		protected:
			/*!
				@brief	処理中断要求があるかどうかを返す。
			*/
			bool NeedsToCancel() const { return _needsToCancel; }

		private:
			friend class WorkerThread;
			WorkerThread::Impl* _pOwner;
			STATE _state;
			AUTO_EVENT_HANDLE _hEndEvent;
			bool _needsToCancel;
			void ChangeState(STATE state);
		};	// end class Work
		
		typedef std::shared_ptr<Work> WorkPtr;	// shared_ptr はスレッドセーフ。
		//typedef smart_ptr<Work, ThreadSafe> WorkPtr;
		
		//! ctor
		WorkerThread();
		
		//! dtor
		~WorkerThread();
		
		/*!
			@brief	ワーカースレッドを作成する
		*/
		bool Create();
		
		/*!
			@brief	ワーカースレッドを解放する
					実行中／実行待ちの仕事があった場合は、処理を中断する。
		*/
		void Release();
		
		/*!
			@brief	このワーカースレッドのスレッドIDを取得する
		*/
		DWORD GetThreadID() const;
		
		/*!
			@brief	仕事を追加する。
			@param	pWork	仕事オブジェクト。
							引数の型は shared_ptr である為、渡したオブジェクトは処理が完了するまで生存している事に注意。
			@note	この関数はスレッドセーフである。
		*/
		bool Push(WorkPtr pWork);
		
		/*!
			@brief	指定の仕事を中断する。
					中断したい仕事が実行中だった場合は、その仕事が終わるまでブロックされる。
					各 Work サブクラスは、NeedsToCancel 関数の戻り値を調べて、処理中断要求が出ているかどうかを判別出来る。
			@param	pWork	中断する仕事オブジェクト。
			@code
			struct MyWork : public WorkerThread::Work
			{
				int Execute()
				{
					... 長い処理1 ...

					if (NeedsToCancel())
						return ERR_CANCELD;	// 中断要求があったので異常終了。

					... 長い処理2 ...

					if (NeedsToCancel())
						return ERR_CANCELD;	// 中断要求があったので異常終了。

					... 長い処理3 ...
					
					return ERR_NOERROR;	// 正常終了。
				}
			};
			@endcode
		*/
		bool Cancel(WorkPtr pWork);
		
		/*!
			@brief	指定の仕事が終わるまで待つ。
			@param	pWork	仕事の終了を待ちたい仕事オブジェクト。
			@param	timeout	タイムアウトするミリ秒。
			@return	仕事が全て終わった場合はtrue、タイムアウトした場合はfalseを返す。
		*/
		bool WaitForCompletion(WorkPtr pWork, DWORD timeout = INFINITE);
		
		/*!
			@brief	仕事が全て終わるまで待つ。
			@param	timeout	タイムアウトするミリ秒。
			@return	仕事が全て終わった場合はtrue、タイムアウトした場合はfalseを返す。
		*/
		bool WaitForCompletion(DWORD timeout = INFINITE);
		
		/*!
			@brief	仕事が全て終わったかどうかを返す
			@attention	この関数の実行には最低1msを要する。
		*/
		bool IsCompleted();
		
		/*!
			@brief	スレッド起動時に呼び出されるイベント。
		*/
		virtual bool OnBeginThread() { return true; }
		
		/*!
			@brief	スレッド終了時に呼び出されるイベント。
		*/
		virtual void OnExitThread() {}
		
	protected:
		friend class Work;

		std::auto_ptr<Impl> _pimpl;	// pimpl Idiom
		
		WorkerThread(Impl* pimpl);

	};	// end class WorkerThread


	/*!
		@brief	同期可能ワーカースレッドクラス。
				呼び出し側スレッドで毎フレーム Sync 関数を呼び出す様にし、Work::Execute 関数内で
				Work::SyncObject を生成する事で、呼び出し側スレッドとの同期処理を行う事が出来る。
		@code
		SynchronizableWorkerThread worker;
		
		struct MyWork : public SynchronizableWorkerThread::Work
		{
			int Execute()
			{
				// 非同期処理。
				int i = 0;
				for (; i < 100; ++i)
					printf("%d\n", i);
					
				// メインスレッドとの同期処理。この間はメインスレッドは(Sync関数で)停止している。
				{
					SyncObject sync(this);
					
					// sync の破棄と同時に同期終了。
				}
				
				// 再び非同期処理。
				for (; i < 200; ++i)
					printf("%d\n", i);
				
				return ERR_NOERROR;
			}
		};
		
		worker.Create();
		
		WorkerThread::WorkPtr work(new MyWork());
		worker.Push(work1);
		
		// メインループ。
		for (!worker.IsCompleted())
		{
			g_worker.Sync();
			Sleep(0);
		}
		@endcode
	*/
	class SynchronizableWorkerThread : public WorkerThread
	{
	private:
		struct Impl;
	public:
		class Work : public WorkerThread::Work
		{
		protected:
			SynchronizableWorkerThread::Impl* GetOwner();

			/*!
				@brief	同期処理用オブジェクト。
						このオブジェクトが生存している間は、呼び出し側スレッドは SynchronizableWorkerThread::Sync 関数で停止している。
			*/
			class SyncObject
			{
			public:
				/*!
					@brief	呼び出し側スレッドとの同期処理を開始する。
							このコンストラクタが呼び出されると、ワーカースレッドは一時停止する。
							一時停止後、呼び出し側スレッドで SynchronizableWorkerThread::Sync 関数が呼び出されると、
							処理を再開し、呼び出し側スレッドは一時停止する。
				*/
				SyncObject(SynchronizableWorkerThread::Work* pWork);
				
				/*!
					@brief	呼び出し側スレッドとの同期処理を終了する。
							このデストラクタが呼び出されると、SynchronizableWorkerThread::Sync 関数で停止していた
							呼び出し側スレッドは処理を再開する。
				*/
				~SyncObject();
				
			private:
				SynchronizableWorkerThread::Work* _pWork;
			};	// end class SyncObject
			
		};	// end class Work

		SynchronizableWorkerThread();

		~SynchronizableWorkerThread();

		/*!
			@brief	ワーカースレッドとの同期処理を行う。
					ワーカースレッド側が同期処理を開始していない場合は、即座に制御が戻る。
					ワーカースレッド側が同期処理を開始している場合は、一時停止しているワーカースレッドが再開し、
					同期処理を終了するまで呼び出し側スレッドに制御を返さない。
					Work::Execute 内で Work::SyncObject を利用する事がある場合は、毎フレームこの関数を呼び出す必要がある。
					利用しない場合は呼び出す必要は無い。
		*/
		void Sync();
		
	private:
		friend class Work;
	};	// end class SynchronizableWorkerThread

}	// end namespace Marda
