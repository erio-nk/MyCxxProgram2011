/*!
	@file	Utilities/WorkerThread.cpp

	@author	E.Nakayama
*/

#include <process.h>

#include "Marda/Utilities/WorkerThread.h"

using namespace Marda;

// ----------------------------------------------------------------------------
// WorkerThread::Work
WorkerThread::Work::Work()
	: _pOwner(NULL)
	, _state(STATE_NONE) 
{
//	_hEndEvent.reset(
//		CreateEvent(
//			NULL,
//			TRUE,	// bManualReset
//			FALSE,	// bInitialState
//			NULL
//			)
//		);
//	if (_hEndEvent.is_invalid())
//	{
//		// TODO:
//	}
}

void
WorkerThread::Work::ChangeState(STATE state)
{
	_state = state;
	OnNotify(state);
}

// ----------------------------------------------------------------------------
// WorkerThread クラスの実装部

struct WorkerThread::Impl : private Uncopyable
{
	WorkerThread& _interface;
	
	HANDLE _hThread;
	unsigned int _threadID;
	HANDLE _hPushEvent;	//!< 仕事をPushした場合にシグナル状態になるイベント。
	HANDLE _hCompleteEvent;	//!< WaitForCompletion用。
	bool _isCompleted;
	bool _needsToExit;
	
	typedef std::deque<WorkPtr> WorkQueue;
	WorkQueue _workQueue;
	GuardObject _workQueueGuard;
	
	#if !defined(DISABLE_WORKER_THREAD_SYNC_EXEC)
	HANDLE _hBeginSyncEvent;	//!< 同期処理開始用イベント。
	HANDLE _hEndSyncEvent;	//!< 同期処理終了用イベント。
	#endif
	
	//! ctor
	Impl(WorkerThread& iface)
		: _interface(iface)
		, _hThread(NULL)
		, _threadID(0)
		, _hPushEvent(NULL)
		, _hCompleteEvent(NULL)
		, _isCompleted(true)
		, _needsToExit(false)
		#if !defined(DISABLE_WORKER_THREAD_SYNC_EXEC)
		, _hBeginSyncEvent(NULL)
		, _hEndSyncEvent(NULL)
		#endif
	{
	}
	
	//! dtor
	~Impl()
	{
		Release();
	}
	
	//! ワーカースレッドを作成する
	bool Create()
	{
		if (is_null(_hThread)) 
		{
			_hThread = (HANDLE)_beginthreadex(
				NULL,	// lpThreadAttributes
				0,	// dwStackSize	; デフォルト
				&Impl::ThreadProc,	// lpStartAddress
				this,	// lpParameter
				0,	// dwCreationFlags
				&_threadID	// lpThreadId
				);
			if (is_null(_hThread))
			{
				return false;
			}
				
			_hPushEvent = CreateEvent(
				NULL,
				TRUE,	// bManualReset	; _workQueue が空になったら非シグナル状態に戻す
				FALSE,	// bInitialState
				NULL
				);
			if (is_null(_hPushEvent))
			{
				return false;
			}

			_hCompleteEvent = CreateEvent(
				NULL,
				TRUE,	// bManualReset	; Push 関数で非シグナル状態に戻す
				TRUE,	// bInitialState
				NULL
				);
			if (is_null(_hPushEvent))
			{
				return false;
			}

			#if !defined(DISABLE_WORKER_THREAD_SYNC_EXEC)
			_hBeginSyncEvent = CreateEvent(
				NULL,
				TRUE,	// bManualReset
				TRUE,	// bInitialState
				NULL
				);
			if (is_null(_hBeginSyncEvent))
			{
				return false;
			}

			_hEndSyncEvent = CreateEvent(
				NULL,
				TRUE,	// bManualReset
				FALSE,	// bInitialState
				NULL
				);
			if (is_null(_hEndSyncEvent))
			{
				return false;
			}
			#endif
		}
		return true;
	}
	
	//! ワーカースレッドを解放する
	void Release()
	{
		_needsToExit = true;
		if (!is_null(_hThread))
		{
			// 全ての仕事をキャンセルする。
			{
				Lock<GuardObject> lock(_workQueueGuard);	// ロック
				WorkQueue::iterator itrQueue = _workQueue.begin();
				WorkQueue::iterator itrQueueEnd = _workQueue.end();
				for (; itrQueue != itrQueueEnd; ++itrQueue)
				{
					(*itrQueue)->ChangeState(Work::STATE_CANCELED);
				}
				_workQueue.clear();
			}
			
			WaitForCompletion(INFINITE);	// 仕事が始まっちゃってるものはキャンセルできないので待つ。

			// 仕事待ちの状態解除の為、シグナル状態にする。
			SetEvent(_hPushEvent);

			// スレッド終了待ち
//			DWORD ret = 
				WaitForSingleObject(_hThread, INFINITE);
//			if (ret == WAIT_TIMEOUT)
//			{
//				// TODO:
//			}

			safe_close<NULL>(_hThread);
		}
		safe_close<NULL>(_hPushEvent);
		safe_close<NULL>(_hCompleteEvent);
		#if !defined(DISABLE_WORKER_THREAD_SYNC_EXEC)
		safe_close<NULL>(_hBeginSyncEvent);
		safe_close<NULL>(_hEndSyncEvent);
		#endif
	}
	
	//! このワーカースレッドのスレッドIDを取得する
	DWORD GetThreadID()
	{
		return _threadID;
	}
	
	//! 仕事を追加する。
	bool Push(WorkPtr pWork)
	{
		Lock<GuardObject> lock(_workQueueGuard);	// ロック
		
		pWork->_pOwner = &_interface;
		
		pWork->ChangeState(Work::STATE_QUEUEING);
		_workQueue.push_back(pWork);
		
		ResetEvent(_hCompleteEvent);	// Pushした直後に同スレッドからWaitForCompletionを呼び出しても良い様にココで非シグナル状態にする
		SetEvent(_hPushEvent);	// 仕事待ちの状態解除の為、シグナル状態にする
		
		return true;
	}
	
	//! 仕事を中断する。
	bool Cancel(WorkPtr pWork)
	{
		bool result = false;
		Lock<GuardObject> lock(_workQueueGuard);	// ロック
		WorkQueue::iterator itrQueue = _workQueue.begin();
		while (itrQueue != _workQueue.end())
		{
			if (*itrQueue == pWork)
			{
				(*itrQueue)->ChangeState(Work::STATE_CANCELED);
				itrQueue = _workQueue.erase(itrQueue);
				result = true;
			}
			else
			{
				++itrQueue;
			}
		}
		return result;
	}
	
	//! 仕事が全て終わるまで待つ。
	bool WaitForCompletion(DWORD timeout)
	{
		#if !defined(DISABLE_WORKER_THREAD_SYNC_EXEC)
		DWORD begin = GetTickCount();
		DWORD current;
		for (;;)
		{
			Sync();	// 同期処理中かもしれないので呼び出す。
			
			if (IsSignaled(_hCompleteEvent))
				break;
			
			if (timeout != INFINITE)
			{
				current = GetTickCount();
				if (current < begin)
				{
					// TODO: システム起動から 49.7 日間経過してカウントがリセットされた。タイムアウトと見なすか？
				}
				else
				if (current - begin >= timeout)
				{
					// タイムアウト。
					return false;
				}
			}
			
			Sleep(0);	// スレッドコンテキストのスイッチ。
		}
		#else
		DWORD ret = WaitForSingleObject(_hCompleteEvent, timeout);
		if (ret == WAIT_TIMEOUT)
		{
			return false;
		}
		#endif
		return true;
	}
	
	//! 仕事が全て終わったかどうかを返す
	bool IsCompleted()
	{
		// 非シグナル状態（＝仕事が全て終わっている）かどうかをチェックする
		return IsNonSignaled(_hPushEvent);
		//return WaitForSingleObject(_hPushEvent, 0) != WAIT_OBJECT_0;
	}
	
	//! スレッドプロシージャ
	static unsigned int WINAPI ThreadProc(void* lpParameter)
	{
		Impl* pThis = reinterpret_cast<Impl*>(lpParameter);
		for (;;)
		{
			// 仕事が来るまで待つ
			WaitForSingleObject(pThis->_hPushEvent, INFINITE);
			
			// 終了要求が来ているか？
			if (pThis->_needsToExit)
			{
				break;
			}
			else
			{
				// 積んである仕事をすべてこなす。
				for (;;)
				{
					// 終了要求が来ていたら以降の仕事はキャンセルする。
					if (pThis->_needsToExit)
					{
						break;
					}

					WorkPtr pWork;
					{
						Lock<GuardObject> lock(pThis->_workQueueGuard);	// ロック
						if (pThis->_workQueue.empty())
						{
							break;	// 仕事なし
						}
						
						pWork = pThis->_workQueue.front();
						pThis->_workQueue.pop_front();
						pWork->ChangeState(Work::STATE_EXECUTING);
					}
					
					// 実行
					if (pWork->Execute())
					{
						pWork->ChangeState(Work::STATE_COMPLETE);
					}
					else
					{
						pWork->ChangeState(Work::STATE_ERROR);
					}
				}
			}
			ResetEvent(pThis->_hPushEvent);	// 非シグナル状態に戻して仕事待ち状態にする
			SetEvent(pThis->_hCompleteEvent);	// シグナル状態にしてWaitForCompletionの待機状態を解除させる
		}
		return 0;
	}

	#if !defined(DISABLE_WORKER_THREAD_SYNC_EXEC)
	void Sync()
	{
		// 同期処理開始用イベントが非シグナル状態かどうかをチェックする。
		if (IsNonSignaled(_hBeginSyncEvent))
		{
			ResetEvent(_hEndSyncEvent);	// 終了用イベントを非シグナル状態にする。
			SetEvent(_hBeginSyncEvent);	// 開始用イベントをシグナル状態にし、ワーカースレッド側の処理を再開させる。
			WaitForSingleObject(_hEndSyncEvent, INFINITE);	// 終了用イベントがシグナル状態になる（＝EndSyncが呼び出される）のを待つ。
		}
	}
	
	void BeginSync()
	{
		ResetEvent(_hBeginSyncEvent);	// 開始用イベントを非シグナル状態にする。
		WaitForSingleObject(_hBeginSyncEvent, INFINITE);	// 開始用イベントがシグナル状態になる（＝Syncが呼び出される）のを待つ。
	}
	
	void EndSync()
	{
		SetEvent(_hEndSyncEvent);	// 終了用イベントをシグナル状態にする。
	}
	#endif

};	// end struct WorkerThread::Impl

// ----------------------------------------------------------------------------
// WorkerThread クラスの転送部

WorkerThread::WorkerThread()
{
	_pimpl.reset(new Impl(*this));
}

WorkerThread::~WorkerThread()
{
}

bool
WorkerThread::Create()
{
	return _pimpl->Create();
}

void
WorkerThread::Release()
{
	_pimpl->Release();
}

DWORD
WorkerThread::GetThreadID() const
{
	return _pimpl->GetThreadID();
}

bool
WorkerThread::Push(WorkPtr pWork)
{
	return _pimpl->Push(pWork);
}

bool
WorkerThread::Cancel(WorkPtr pWork)
{
	return _pimpl->Cancel(pWork);
}

bool
WorkerThread::WaitForCompletion(DWORD timeout)
{
	return _pimpl->WaitForCompletion(timeout);
}

bool
WorkerThread::IsCompleted()
{
	return _pimpl->IsCompleted();
}

#if !defined(DISABLE_WORKER_THREAD_SYNC_EXEC)
void
WorkerThread::Sync()
{
	return _pimpl->Sync();
}

void
WorkerThread::BeginSync()
{
	return _pimpl->BeginSync();
}

void
WorkerThread::EndSync()
{
	return _pimpl->EndSync();
}
#endif
