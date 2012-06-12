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
	, _needsToCancel(false)
{
	_hEndEvent.reset(
		CreateEvent(
			NULL,
			TRUE,	// bManualReset
			FALSE,	// bInitialState
			NULL
			)
		);
	if (_hEndEvent.is_invalid())
	{
		// TODO:
	}
}

void
WorkerThread::Work::ChangeState(STATE state)
{
	_state = state;
	switch (state)
	{
		case STATE_QUEUEING:
			ResetEvent(_hEndEvent);
			_needsToCancel = false;
			break;
		case STATE_COMPLETE:
		case STATE_CANCELED:
		case STATE_ERROR:
			SetEvent(_hEndEvent);
			break;
	}
	OnNotify(state);
}

// ----------------------------------------------------------------------------
// WorkerThread クラスの実装部

struct WorkerThread::Impl
{
	WorkerThread* _pinterface;

	AUTO_THREAD_HANDLE _hThread;
	unsigned int _threadID;
	AUTO_EVENT_HANDLE _hPushEvent;	//!< 仕事をPushした場合にシグナル状態になるイベント。
	AUTO_EVENT_HANDLE _hCompleteEvent;	//!< WaitForCompletion用。
	bool _isCompleted;
	bool _needsToExit;
	
	bool (*_pfnWaitForEvent)(WorkerThread::Impl* /*pthis*/, AUTO_EVENT_HANDLE& /*handle*/, DWORD /*timeout*/);
		/*	SynchronizedWorkerThread では、デッドロック回避の為に WaitForSingleObject とは異なる方法でイベントの待機を
			実現しなければならないが、この為の関数をポリモーフィックに実装してしまうと、デストラクト時のイベント待機に
			使用出来ない為、関数ポインタで実現する。
		*/
			
	typedef std::deque<WorkPtr> WorkQueue;
	WorkQueue _workQueue;	// TODO: pplの並列キューに置き換えたいところ。
	GuardObject _workQueueGuard;
	
	//! ctor
	Impl()
		: _pinterface(NULL)
		, _threadID(0)
		, _isCompleted(true)
		, _needsToExit(false)
		, _pfnWaitForEvent(&WaitForEvent)
	{
	}
	
	//! dtor
	virtual ~Impl()
	{
		ExitThread();
	}
	
	void SetInterface(WorkerThread* pinterface)
	{
		_pinterface = pinterface;
	}
	
	//! ワーカースレッドを作成する。
	virtual bool Create()
	{
		if (_hThread.is_invalid()) 
		{
			_hThread.reset(
				CreateThread(
					NULL,	// lpThreadAttributes
					0,	// dwStackSize	; デフォルト
					&Impl::ThreadProc,	// lpStartAddress
					this,	// lpParameter
					0,	// dwCreationFlags
					&_threadID	// lpThreadId
					)
				);
			if (_hThread.is_invalid())
			{
				return false;
			}
				
			_hPushEvent.reset(
				CreateEvent(
					NULL,
					TRUE,	// bManualReset	; _workQueue が空になったら非シグナル状態に戻す
					FALSE,	// bInitialState
					NULL
					)
				);
			if (_hPushEvent.is_invalid())
			{
				return false;
			}

			_hCompleteEvent.reset(
				CreateEvent(
					NULL,
					TRUE,	// bManualReset	; Push 関数で非シグナル状態に戻す
					TRUE,	// bInitialState
					NULL
					)
				);
			if (_hCompleteEvent.is_invalid())
			{
				return false;
			}
		}
		return true;
	}
	
	void ExitThread()
	{
		_needsToExit = true;
		if (_hThread.is_valid())
		{
			// 全ての仕事をキャンセルする。
			{
				Lock<GuardObject> lock(_workQueueGuard);	// ロック
				if (!_workQueue.empty())
				{
					WorkQueue::iterator itrQueueBegin = _workQueue.begin();	// 先頭要素は仕事中。
					(*itrQueueBegin)->_needsToCancel = true;	// 中断要求を出す。
					++itrQueueBegin;

					WorkQueue::iterator itrQueue = itrQueueBegin;
					WorkQueue::iterator itrQueueEnd = _workQueue.end();
					for (; itrQueue != itrQueueEnd; ++itrQueue)
					{
						(*itrQueue)->_needsToCancel = true;
						(*itrQueue)->ChangeState(Work::STATE_CANCELED);
					}
					_workQueue.erase(itrQueueBegin, itrQueueEnd);
				}
			}
			
			WaitForCompletion(INFINITE);	// 仕事が始まっちゃってるものは終了まで待つ。

			// 仕事待ちの状態解除の為、シグナル状態にする。
			SetEvent(_hPushEvent);

			// スレッド終了待ち
//			DWORD ret = 
				WaitForSingleObject(_hThread, INFINITE);
//			if (ret == WAIT_TIMEOUT)
//			{
//				// TODO:
//			}

			_hThread.reset();
		}
	}
	
	//! ワーカースレッドを解放する
	virtual void Release()
	{
		ExitThread();
		_hPushEvent.reset();
		_hCompleteEvent.reset();
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
		
		pWork->_pOwner = this;
		
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
		WorkPtr pCurrentWork;
		{
			Lock<GuardObject> lock(_workQueueGuard);	// ロック
			if (!_workQueue.empty())
			{
				WorkQueue::iterator itrQueue = _workQueue.begin();
				if (*itrQueue == pWork)
				{
					pCurrentWork = pWork;	// 実行中。
					pCurrentWork->_needsToCancel = true;	// 中断フラグを立てて、処理中断を要求する。
				}
				++itrQueue;
				
				while (itrQueue != _workQueue.end())
				{
					if (*itrQueue == pWork)
					{
						(*itrQueue)->_needsToCancel = true;
						(*itrQueue)->ChangeState(Work::STATE_CANCELED);
						itrQueue = _workQueue.erase(itrQueue);
						result = true;
					}
					else
					{
						++itrQueue;
					}
				}
			}
		}
		if (is_not_null(pCurrentWork))
		{
			// 中断は難しいので終了を待つ。
			_pfnWaitForEvent(this, pCurrentWork->_hEndEvent, INFINITE);
		}
		return result;
	}

	//! 呼び出し側からのイベント待機用関数。
	static bool WaitForEvent(WorkerThread::Impl* /*pthis*/, AUTO_EVENT_HANDLE& handle, DWORD timeout)
	{
		DWORD ret = WaitForSingleObject(handle, timeout);
		if (ret == WAIT_TIMEOUT)
		{
			return false;
		}
		return true;
	}
	
	//! 指定の仕事が終わるまで待つ。
	bool WaitForCompletion(WorkPtr pWork, DWORD timeout)
	{
		return _pfnWaitForEvent(this, pWork->_hEndEvent, timeout);;
	}
	
	//! 仕事が全て終わるまで待つ。
	bool WaitForCompletion(DWORD timeout)
	{
		return _pfnWaitForEvent(this, _hCompleteEvent, timeout);;
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
		Impl* pthis = reinterpret_cast<Impl*>(lpParameter);
		if (failed(pthis->_pinterface->OnBeginThread()))
		{
			// TODO: 失敗した時の何か…。
			return 0;
		}
		for (;;)
		{
			// 仕事が来るまで待つ
			WaitForSingleObject(pthis->_hPushEvent, INFINITE);
			
			// 終了要求が来ているか？
			if (pthis->_needsToExit)
			{
				break;
			}
			else
			{
				// 積んである仕事をすべてこなす。
				for (;;)
				{
					// 終了要求が来ていたら以降の仕事はキャンセルする。
					if (pthis->_needsToExit)
					{
						break;
					}

					WorkPtr pWork;
					{
						Lock<GuardObject> lock(pthis->_workQueueGuard);	// ロック
						if (pthis->_workQueue.empty())
						{
							break;	// 仕事なし
						}
						
						pWork = pthis->_workQueue.front();
						pWork->ChangeState(Work::STATE_EXECUTING);
					}
					
					// 実行
					ERROR_CODE result = pWork->Execute();
					switch (result)
					{
						case ERR_NOERROR:
							pWork->ChangeState(Work::STATE_COMPLETE);
							break;
						case ERR_CANCELED:
							pWork->ChangeState(Work::STATE_CANCELED);
							break;
						case ERR_UNKNOWN:
						default:
							pWork->ChangeState(Work::STATE_ERROR);
							break;
					}
					
					{
						Lock<GuardObject> lock(pthis->_workQueueGuard);	// ロック
						pthis->_workQueue.pop_front();
					}
				}
			}
			ResetEvent(pthis->_hPushEvent);	// 非シグナル状態に戻して仕事待ち状態にする
			SetEvent(pthis->_hCompleteEvent);	// シグナル状態にしてWaitForCompletionの待機状態を解除させる
		}
		pthis->_pinterface->OnExitThread();
		return 0;
	}

};	// end struct WorkerThread::Impl

// ----------------------------------------------------------------------------
// WorkerThread クラスの転送部

WorkerThread::WorkerThread()
{
	_pimpl.reset(new Impl());
	_pimpl->SetInterface(this);
}

WorkerThread::WorkerThread(Impl* pimpl)
{
	_pimpl.reset(pimpl);
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
WorkerThread::WaitForCompletion(WorkPtr pWork, DWORD timeout)
{
	return _pimpl->WaitForCompletion(pWork, timeout);
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

// ----------------------------------------------------------------------------
// SynchronizableWorkerThread クラスの実装部

struct SynchronizableWorkerThread::Impl : public WorkerThread::Impl
{
	AUTO_EVENT_HANDLE _hBeginSyncEvent;	//!< 同期処理開始用イベント。
	AUTO_EVENT_HANDLE _hEndSyncEvent;	//!< 同期処理終了用イベント。
	
	Impl()
	{
		_pfnWaitForEvent = WaitForEvent;
	}
	
	~Impl()
	{
		ExitThread();
		_hBeginSyncEvent.reset();
		_hEndSyncEvent.reset();
		_pfnWaitForEvent = &WorkerThread::Impl::WaitForEvent;
	}
	
	bool Create()
	{
		if (!WorkerThread::Impl::Create())
			return false;

		if (_hBeginSyncEvent.is_invalid())
		{
			_hBeginSyncEvent.reset(
				CreateEvent(
					NULL,
					TRUE,	// bManualReset
					TRUE,	// bInitialState
					NULL
					)
				);
			if (_hBeginSyncEvent.is_invalid())
			{
				return false;
			}
		}

		if (_hEndSyncEvent.is_invalid())
		{
			_hEndSyncEvent.reset(
				CreateEvent(
					NULL,
					TRUE,	// bManualReset
					FALSE,	// bInitialState
					NULL
					)
				);
			if (_hEndSyncEvent.is_invalid())
			{
				return false;
			}
		}
		return true;
	}
	
	void Release()
	{
		ExitThread();
		_hPushEvent.reset();
		_hCompleteEvent.reset();
		_hBeginSyncEvent.reset();
		_hEndSyncEvent.reset();
	}

	static bool WaitForEvent(WorkerThread::Impl* p, AUTO_EVENT_HANDLE& handle, DWORD timeout)
	{
		SynchronizableWorkerThread::Impl* pthis = down_cast<SynchronizableWorkerThread::Impl*>(p);
		DWORD begin = GetTickCount();
		DWORD current;
		for (;;)
		{
			pthis->Sync();	// 同期処理中かもしれないので呼び出す。
			
			if (IsSignaled(handle))
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

		return true;
	}

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
};	// end struct SynchronizableWorkerThread::Impl


// ----------------------------------------------------------------------------
// SynchronizableWorkerThread::Work
SynchronizableWorkerThread::Impl*
SynchronizableWorkerThread::Work::GetOwner()
{
	return down_cast<SynchronizableWorkerThread::Impl*>(WorkerThread::Work::GetOwner());
}

SynchronizableWorkerThread::Work::SyncObject::SyncObject(SynchronizableWorkerThread::Work* pWork)
	: _pWork(pWork)
{
	if (is_not_null(_pWork))
		_pWork->GetOwner()->BeginSync();
}

SynchronizableWorkerThread::Work::SyncObject::~SyncObject()
{
	if (is_not_null(_pWork))
		_pWork->GetOwner()->EndSync();
}


// ----------------------------------------------------------------------------
// SynchronizableWorkerThread クラスの転送部

SynchronizableWorkerThread::SynchronizableWorkerThread()
	: WorkerThread(new Impl())
{
	_pimpl->SetInterface(this);
}

SynchronizableWorkerThread::~SynchronizableWorkerThread()
{
}

void
SynchronizableWorkerThread::Sync()
{
	return down_cast<SynchronizableWorkerThread::Impl*>(_pimpl.get())->Sync();
}