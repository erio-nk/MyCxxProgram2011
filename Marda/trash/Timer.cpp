/*!
	@file	Timer.h

	@author	E.Nakayama
*/

#include "Marda/Misc.h"
#include "Marda/Timer.h"

using namespace Marda;

// ----------------------------------------------------------------------------
// Timer �N���X�̎�����
struct Timer::Impl
{
	struct Times
	{
		LARGE_INTEGER _start;
		LARGE_INTEGER _total;	// �O��Stop�Ăяo�����̍��v�o�ߎ��ԁB
		Times()
		{
			_start.QuadPart = 0;
			_total.QuadPart = 0;
		}
	};
	Times _times;	// GetTime�p�B
	Times _elapsed;	// GetElapsedTime�p�B

	Impl()
	{
	}
	
	~Impl()
	{
	}
	
	//! �^�C�}�[���J�n����B
	void Start()
	{
		QueryPerformanceCounter(&_times._start);
		_elapsed._start = _times._start;
	}
	
	//! �^�C�}�[���~����B
	void Stop()
	{
		if (_times._start.QuadPart > 0)
		{
			LARGE_INTEGER now;
			QueryPerformanceCounter(&now);
			_times._total.QuadPart += now.QuadPart - _times._start.QuadPart; 
			_times._start.QuadPart = 0;
			_elapsed._total.QuadPart += now.QuadPart - _elapsed._start.QuadPart;
			_elapsed._start.QuadPart = 0;
		}
	}
	
	//! �^�C�}�[�����Z�b�g����B
	void Reset()
	{
		_times._start.QuadPart = 0;
		_times._total.QuadPart = 0;
		_elapsed._start.QuadPart = 0;
		_elapsed._total.QuadPart = 0;
	}
	
	//! �^�C�}�[���J�n���Ă���̌o�ߎ��Ԃ��擾����B
	double GetTime()
	{
		if (_times._start.QuadPart > 0)
		{
			LARGE_INTEGER now;
			QueryPerformanceCounter(&now);
			return PerformanceCounterToSeconds(now.QuadPart - _times._start.QuadPart + _times._total.QuadPart);
		}
		return PerformanceCounterToSeconds(_times._total.QuadPart);
	}
	
	//! �O��� GetElapsedTime �Ăяo��������̌o�ߎ��Ԃ��擾����B
	double GetElapsedTime()
	{
		if (_times._start.QuadPart > 0)
		{
			LARGE_INTEGER now;
			QueryPerformanceCounter(&now);
			double result = PerformanceCounterToSeconds(now.QuadPart - _elapsed._start.QuadPart + _elapsed._total.QuadPart);
			_elapsed._start = now;
			_elapsed._total.QuadPart = 0;	// ���Z�b�g
			return result;
		}
		return PerformanceCounterToSeconds(_elapsed._total.QuadPart);
	}

};	// end struct Timer::Impl

// ----------------------------------------------------------------------------
// Timer �N���X�̓]����
Timer::Timer()
	: _pimpl(new Impl())
{
}

Timer::~Timer()
{
}

void 
Timer::Start()
{
	_pimpl->Start();
}

void 
Timer::Stop()
{
	_pimpl->Stop();
}

void 
Timer::Reset()
{
	_pimpl->Reset();
}

double
Timer::GetTime() const
{
	return _pimpl->GetTime();
}

double
Timer::GetElapsedTime()
{
	return _pimpl->GetElapsedTime();
}

