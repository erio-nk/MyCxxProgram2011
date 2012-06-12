/*!
	@file	Timer.cpp

	@author	E.Nakayama
*/

#include "Marda/Misc.h"
#include "Marda/PerformanceCounter.h"
#include "Marda/Timer.h"

using namespace Marda;

// ----------------------------------------------------------------------------
// Timer クラスの実装部
struct Timer::Impl
{
	struct Times
	{
		LARGE_INTEGER _start;
		LARGE_INTEGER _total;	// 前回Stop呼び出し時の合計経過時間。
		Times()
		{
			_start.QuadPart = 0;
			_total.QuadPart = 0;
		}
	};
	Times _times;	// GetTime用。
	Times _elapsed;	// GetElapsedTime用。

	Impl()
	{
	}
	
	~Impl()
	{
	}
	
	//! タイマーを開始する。
	void Start()
	{
		PerformanceCounter::GetCurrentCount(_times._start);
		_elapsed._start = _times._start;
	}
	
	//! タイマーを停止する。
	void Stop()
	{
		if (_times._start.QuadPart > 0)
		{
			LARGE_INTEGER now;
			PerformanceCounter::GetCurrentCount(now);
			_times._total.QuadPart += now.QuadPart - _times._start.QuadPart; 
			_times._start.QuadPart = 0;
			_elapsed._total.QuadPart += now.QuadPart - _elapsed._start.QuadPart;
			_elapsed._start.QuadPart = 0;
		}
	}
	
	//! タイマーをリセットする。
	void Reset()
	{
		_times._start.QuadPart = 0;
		_times._total.QuadPart = 0;
		_elapsed._start.QuadPart = 0;
		_elapsed._total.QuadPart = 0;
	}
	
	//! タイマーを開始してからの経過時間を取得する。
	float GetTime()
	{
		if (_times._start.QuadPart > 0)
		{
			LARGE_INTEGER now;
			PerformanceCounter::GetCurrentCount(now);
			now.QuadPart = now.QuadPart - _times._start.QuadPart + _times._total.QuadPart;
			return PerformanceCounter::ToSecondsAsFloat(now);
		}
		return PerformanceCounter::ToSecondsAsFloat(_times._total);
	}
	
	//! 前回の GetElapsedTime 呼び出し時からの経過時間を取得する。
	float GetElapsedTime(bool reset)
	{
		if (_times._start.QuadPart > 0)
		{
			LARGE_INTEGER now;
			PerformanceCounter::GetCurrentCount(now);
			float result = PerformanceCounter::ToSecondsAsFloat(now.QuadPart - _elapsed._start.QuadPart + _elapsed._total.QuadPart);
			if (reset)
			{
				_elapsed._start = now;
				_elapsed._total.QuadPart = 0;	// リセット
			}
			return result;
		}
		return PerformanceCounter::ToSecondsAsFloat(_elapsed._total);
	}

};	// end struct Timer::Impl

// ----------------------------------------------------------------------------
// Timer クラスの転送部
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

float
Timer::GetTime() const
{
	return _pimpl->GetTime();
}

float
Timer::GetElapsedTime(bool reset)
{
	return _pimpl->GetElapsedTime(reset);
}

