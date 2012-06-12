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

				呼び出し側スレッドで毎フレーム Sync 関数を呼び出す様にし、Work::Execute 関数内で
				Work::SyncObject を生成する事で、呼び出し側スレッドとの同期処理を行う事が出来る。
	*/
	class WorkerThread : private Uncopyable
	{
	public:
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
				STATE_QUEUEING,		//!< 実行待ち
				STATE_EXECUTING,	//!< 実行中
				STATE_COMPLETE,		//!< 実行終了
				STATE_CANCELED,		//!< 中断された
				STATE_ERROR,		//!< エラーが発生して終了した
			};
						
			Work();

			virtual ~Work()
			{
			}
			
			WorkerThread* GetOwner() const { return _pOwner; }
			
			STATE GetState() const { return _state; }
			
			/*!
				@brief	実行処理。
						サブクラスは、この関数を必ずオーバーライドしなければならない。
				@return	true を返した場合は成功し、STATE_COMPLETE になる。
						false を返した場合は失敗し、STATE_ERROR になる。
			*/
			virtual bool Execute() = 0;
			
			/*!
				@brief	状態変更通知イベント。
						状態が変わる度に呼び出されるイベントハンドラ。
						サブクラスは、必要であればこのハンドラをオーバーライドして
						適切に処理する事が出来る。
				@param	state	変更後の状態
			*/
			virtual void OnNotify(STATE state) { UNREFERENCED_PARAMETER(state); }

		#if !defined(DISABLE_WORKER_THREAD_SYNC_EXEC)
		protected:
			class SyncObject;
		#endif

		private:
			friend class WorkerThread;
			WorkerThread* _pOwner;
			STATE _state;
			AUTO_EVENT_HANDLE _hEndEvent;
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
			@brief	仕事を中断する。
					未実装→中断したい仕事が実行中だった場合は、その仕事が終わるまでブロックされる。
					現状は中断したい仕事が実行中だった場合は中断に失敗する。
			@param	pWork	中断する仕事オブジェクト。
			@todo	特定の仕事が終わったかどうか安全に待つ関数。
		*/
		bool Cancel(WorkPtr pWork);
		
		/*!
			@brief	仕事が全て終わるまで待つ
			@param	timeout	タイムアウトするミリ秒
			@return	仕事が全て終わった場合はtrue、タイムアウトした場合はfalseを返す。
		*/
		bool WaitForCompletion(DWORD timeout = INFINITE);
		
		/*!
			@brief	仕事が全て終わったかどうかを返す
			@attention	この関数の実行には最低1msを要する。
		*/
		bool IsCompleted();
		
		#if !defined(DISABLE_WORKER_THREAD_SYNC_EXEC)
		/*!
			@brief	ワーカースレッドとの同期処理を行う。
					ワーカースレッド側が同期処理を開始していない場合は、即座に制御が戻る。
					ワーカースレッド側が同期処理を開始している場合は、一時停止しているワーカースレッドが再開し、
					同期処理を終了するまで呼び出し側スレッドに制御を返さない。
					Work::Execute 内で Work::SyncObject を利用する事がある場合は、毎フレームこの関数を呼び出す必要がある。
					利用しない場合は呼び出す必要は無い。
		*/
		void Sync();
		#endif
		
	private:
		friend class Work;

		struct Impl;
		std::auto_ptr<Impl> _pimpl;	// pimpl Idiom

		#if !defined(DISABLE_WORKER_THREAD_SYNC_EXEC)
		void BeginSync();
		void EndSync();
		#endif
	};	// end class WorkerThread


	#if !defined(DISABLE_WORKER_THREAD_SYNC_EXEC)
	/*!
		@brief	同期処理用オブジェクト。
				このオブジェクトが生存している間は、呼び出し側スレッドは WorkerThread::Sync 関数で停止している。
	*/
	class WorkerThread::Work::SyncObject : public Uncopyable
	{
	public:
		/*!
			@brief	呼び出し側スレッドとの同期処理を開始する。
					このコンストラクタが呼び出されると、ワーカースレッドは一時停止する。
					一時停止後、呼び出し側スレッドで WorkerThread::Sync 関数が呼び出されると、
					処理を再開し、呼び出し側スレッドは一時停止する。
		*/
		SyncObject(WorkerThread* pOwner)
			: _pOwner(pOwner)
		{
			if (is_not_null(_pOwner))
			{
				_pOwner->BeginSync();
			}
		}
		
		/*!
			@brief	呼び出し側スレッドとの同期処理を終了する。
					このデストラクタが呼び出されると、WorkerThread::Sync 関数で停止していた
					呼び出し側スレッドは処理を再開する。
		*/
		~SyncObject()
		{
			if (is_not_null(_pOwner))
			{
				_pOwner->EndSync();
			}
		}
		
	private:
		WorkerThread* _pOwner;
	};	// end class WorkerThread::Work::SyncObject
	#endif
	
}	// end namespace Marda
