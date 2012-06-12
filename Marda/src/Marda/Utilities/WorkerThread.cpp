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
// WorkerThread �N���X�̎�����

struct WorkerThread::Impl
{
	WorkerThread* _pinterface;

	AUTO_THREAD_HANDLE _hThread;
	unsigned int _threadID;
	AUTO_EVENT_HANDLE _hPushEvent;	//!< �d����Push�����ꍇ�ɃV�O�i����ԂɂȂ�C�x���g�B
	AUTO_EVENT_HANDLE _hCompleteEvent;	//!< WaitForCompletion�p�B
	bool _isCompleted;
	bool _needsToExit;
	
	bool (*_pfnWaitForEvent)(WorkerThread::Impl* /*pthis*/, AUTO_EVENT_HANDLE& /*handle*/, DWORD /*timeout*/);
		/*	SynchronizedWorkerThread �ł́A�f�b�h���b�N����ׂ̈� WaitForSingleObject �Ƃ͈قȂ���@�ŃC�x���g�̑ҋ@��
			�������Ȃ���΂Ȃ�Ȃ����A���ׂ̈̊֐����|�����[�t�B�b�N�Ɏ������Ă��܂��ƁA�f�X�g���N�g���̃C�x���g�ҋ@��
			�g�p�o���Ȃ��ׁA�֐��|�C���^�Ŏ�������B
		*/
			
	typedef std::deque<WorkPtr> WorkQueue;
	WorkQueue _workQueue;	// TODO: ppl�̕���L���[�ɒu�����������Ƃ���B
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
	
	//! ���[�J�[�X���b�h���쐬����B
	virtual bool Create()
	{
		if (_hThread.is_invalid()) 
		{
			_hThread.reset(
				CreateThread(
					NULL,	// lpThreadAttributes
					0,	// dwStackSize	; �f�t�H���g
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
					TRUE,	// bManualReset	; _workQueue ����ɂȂ������V�O�i����Ԃɖ߂�
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
					TRUE,	// bManualReset	; Push �֐��Ŕ�V�O�i����Ԃɖ߂�
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
			// �S�Ă̎d�����L�����Z������B
			{
				Lock<GuardObject> lock(_workQueueGuard);	// ���b�N
				if (!_workQueue.empty())
				{
					WorkQueue::iterator itrQueueBegin = _workQueue.begin();	// �擪�v�f�͎d�����B
					(*itrQueueBegin)->_needsToCancel = true;	// ���f�v�����o���B
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
			
			WaitForCompletion(INFINITE);	// �d�����n�܂�������Ă���̂͏I���܂ő҂B

			// �d���҂��̏�ԉ����ׁ̈A�V�O�i����Ԃɂ���B
			SetEvent(_hPushEvent);

			// �X���b�h�I���҂�
//			DWORD ret = 
				WaitForSingleObject(_hThread, INFINITE);
//			if (ret == WAIT_TIMEOUT)
//			{
//				// TODO:
//			}

			_hThread.reset();
		}
	}
	
	//! ���[�J�[�X���b�h���������
	virtual void Release()
	{
		ExitThread();
		_hPushEvent.reset();
		_hCompleteEvent.reset();
	}
	
	//! ���̃��[�J�[�X���b�h�̃X���b�hID���擾����
	DWORD GetThreadID()
	{
		return _threadID;
	}
	
	//! �d����ǉ�����B
	bool Push(WorkPtr pWork)
	{
		Lock<GuardObject> lock(_workQueueGuard);	// ���b�N
		
		pWork->_pOwner = this;
		
		pWork->ChangeState(Work::STATE_QUEUEING);
		_workQueue.push_back(pWork);
		
		ResetEvent(_hCompleteEvent);	// Push��������ɓ��X���b�h����WaitForCompletion���Ăяo���Ă��ǂ��l�ɃR�R�Ŕ�V�O�i����Ԃɂ���
		SetEvent(_hPushEvent);	// �d���҂��̏�ԉ����ׁ̈A�V�O�i����Ԃɂ���
		
		return true;
	}
	
	//! �d���𒆒f����B
	bool Cancel(WorkPtr pWork)
	{
		bool result = false;
		WorkPtr pCurrentWork;
		{
			Lock<GuardObject> lock(_workQueueGuard);	// ���b�N
			if (!_workQueue.empty())
			{
				WorkQueue::iterator itrQueue = _workQueue.begin();
				if (*itrQueue == pWork)
				{
					pCurrentWork = pWork;	// ���s���B
					pCurrentWork->_needsToCancel = true;	// ���f�t���O�𗧂ĂāA�������f��v������B
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
			// ���f�͓���̂ŏI����҂B
			_pfnWaitForEvent(this, pCurrentWork->_hEndEvent, INFINITE);
		}
		return result;
	}

	//! �Ăяo��������̃C�x���g�ҋ@�p�֐��B
	static bool WaitForEvent(WorkerThread::Impl* /*pthis*/, AUTO_EVENT_HANDLE& handle, DWORD timeout)
	{
		DWORD ret = WaitForSingleObject(handle, timeout);
		if (ret == WAIT_TIMEOUT)
		{
			return false;
		}
		return true;
	}
	
	//! �w��̎d�����I���܂ő҂B
	bool WaitForCompletion(WorkPtr pWork, DWORD timeout)
	{
		return _pfnWaitForEvent(this, pWork->_hEndEvent, timeout);;
	}
	
	//! �d�����S�ďI���܂ő҂B
	bool WaitForCompletion(DWORD timeout)
	{
		return _pfnWaitForEvent(this, _hCompleteEvent, timeout);;
	}
	
	//! �d�����S�ďI��������ǂ�����Ԃ�
	bool IsCompleted()
	{
		// ��V�O�i����ԁi���d�����S�ďI����Ă���j���ǂ������`�F�b�N����
		return IsNonSignaled(_hPushEvent);
		//return WaitForSingleObject(_hPushEvent, 0) != WAIT_OBJECT_0;
	}
	
	//! �X���b�h�v���V�[�W��
	static unsigned int WINAPI ThreadProc(void* lpParameter)
	{
		Impl* pthis = reinterpret_cast<Impl*>(lpParameter);
		if (failed(pthis->_pinterface->OnBeginThread()))
		{
			// TODO: ���s�������̉����c�B
			return 0;
		}
		for (;;)
		{
			// �d��������܂ő҂�
			WaitForSingleObject(pthis->_hPushEvent, INFINITE);
			
			// �I���v�������Ă��邩�H
			if (pthis->_needsToExit)
			{
				break;
			}
			else
			{
				// �ς�ł���d�������ׂĂ��Ȃ��B
				for (;;)
				{
					// �I���v�������Ă�����ȍ~�̎d���̓L�����Z������B
					if (pthis->_needsToExit)
					{
						break;
					}

					WorkPtr pWork;
					{
						Lock<GuardObject> lock(pthis->_workQueueGuard);	// ���b�N
						if (pthis->_workQueue.empty())
						{
							break;	// �d���Ȃ�
						}
						
						pWork = pthis->_workQueue.front();
						pWork->ChangeState(Work::STATE_EXECUTING);
					}
					
					// ���s
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
						Lock<GuardObject> lock(pthis->_workQueueGuard);	// ���b�N
						pthis->_workQueue.pop_front();
					}
				}
			}
			ResetEvent(pthis->_hPushEvent);	// ��V�O�i����Ԃɖ߂��Ďd���҂���Ԃɂ���
			SetEvent(pthis->_hCompleteEvent);	// �V�O�i����Ԃɂ���WaitForCompletion�̑ҋ@��Ԃ�����������
		}
		pthis->_pinterface->OnExitThread();
		return 0;
	}

};	// end struct WorkerThread::Impl

// ----------------------------------------------------------------------------
// WorkerThread �N���X�̓]����

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
// SynchronizableWorkerThread �N���X�̎�����

struct SynchronizableWorkerThread::Impl : public WorkerThread::Impl
{
	AUTO_EVENT_HANDLE _hBeginSyncEvent;	//!< ���������J�n�p�C�x���g�B
	AUTO_EVENT_HANDLE _hEndSyncEvent;	//!< ���������I���p�C�x���g�B
	
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
			pthis->Sync();	// ������������������Ȃ��̂ŌĂяo���B
			
			if (IsSignaled(handle))
				break;
			
			if (timeout != INFINITE)
			{
				current = GetTickCount();
				if (current < begin)
				{
					// TODO: �V�X�e���N������ 49.7 ���Ԍo�߂��ăJ�E���g�����Z�b�g���ꂽ�B�^�C���A�E�g�ƌ��Ȃ����H
				}
				else
				if (current - begin >= timeout)
				{
					// �^�C���A�E�g�B
					return false;
				}
			}
			
			Sleep(0);	// �X���b�h�R���e�L�X�g�̃X�C�b�`�B
		}

		return true;
	}

	void Sync()
	{
		// ���������J�n�p�C�x���g����V�O�i����Ԃ��ǂ������`�F�b�N����B
		if (IsNonSignaled(_hBeginSyncEvent))
		{
			ResetEvent(_hEndSyncEvent);	// �I���p�C�x���g���V�O�i����Ԃɂ���B
			SetEvent(_hBeginSyncEvent);	// �J�n�p�C�x���g���V�O�i����Ԃɂ��A���[�J�[�X���b�h���̏������ĊJ������B
			WaitForSingleObject(_hEndSyncEvent, INFINITE);	// �I���p�C�x���g���V�O�i����ԂɂȂ�i��EndSync���Ăяo�����j�̂�҂B
		}
	}
	
	void BeginSync()
	{
		ResetEvent(_hBeginSyncEvent);	// �J�n�p�C�x���g���V�O�i����Ԃɂ���B
		WaitForSingleObject(_hBeginSyncEvent, INFINITE);	// �J�n�p�C�x���g���V�O�i����ԂɂȂ�i��Sync���Ăяo�����j�̂�҂B
	}
	
	void EndSync()
	{
		SetEvent(_hEndSyncEvent);	// �I���p�C�x���g���V�O�i����Ԃɂ���B
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
// SynchronizableWorkerThread �N���X�̓]����

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