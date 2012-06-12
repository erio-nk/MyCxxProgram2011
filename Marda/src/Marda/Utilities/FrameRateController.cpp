/*!
	@file	Utilities/FrameRateController.cpp

	@author	E.Nakayama
*/

#include <MMSystem.h>
#pragma comment(lib, "winmm.lib")

#include "Marda/Timer.h"
#include "Marda/Utilities/FrameRateController.h"

using namespace Marda;

namespace
{
	static const float ONE_SECOND = 1.f;
	static const float FPS_DEFAULT = 60.f;
	static const size_t NUM_FRAME_TIME_SAMPLES_DEFAULT = 8;
	static const size_t NUM_SLEEP_TIME_SAMPLES_DEFAULT = 10;
}

// ----------------------------------------------------------------------------
// FrameRateController クラスの実装部。

struct FrameRateController::Impl
{
	Timer _timer;
	float _frameTime;
	float _currentFPS;
	float _elapsedTime;
	float _sleepTime;
	
	typedef ring_buffer<float> FrameTimeSamples;
	FrameTimeSamples _frameTimeSamples;
	float _frameTimeSum;
	
	Impl()
		: _currentFPS(0.f)
		, _elapsedTime(0.f)
		, _frameTimeSum(0.f)
	{
		SetFrameRate(FPS_DEFAULT);
		SetNumFrameTimeSamples(NUM_FRAME_TIME_SAMPLES_DEFAULT);
		
		// Sleep(1) の精度計測。
		timeBeginPeriod(1);
		
		_sleepTime = 0.f;
		for (int i = 0; i < NUM_SLEEP_TIME_SAMPLES_DEFAULT; ++i)
		{
			_timer.Start();
			Sleep(1);
			_sleepTime += _timer.GetTime();
			_timer.Reset();
		}
		_sleepTime /= NUM_SLEEP_TIME_SAMPLES_DEFAULT;

		// 開始。
		_timer.Reset();
		_timer.Start();
	}
	
	~Impl()
	{
		timeEndPeriod(1);
	}
	
	void SetFrameRate(float fps)
	{
		_frameTime = ONE_SECOND / fps;
	}
	
	void SetNumFrameTimeSamples(size_t num)
	{
		_frameTimeSamples.resize_from_front(num);
	}
	
	float GetFrameTime()
	{
		return _frameTime;
	}
	
	float GetCurrentFPS()
	{
		return _currentFPS;
	}
	
	float GetElapsedTime()
	{
		return _elapsedTime;
	}
	
	void ChangeFrame()
	{
		// FPS 計算。
		_frameTimeSum -= _frameTimeSamples.front();
		_frameTimeSamples.pop_front();
		_frameTimeSum += _elapsedTime;
		_frameTimeSamples.push_back(_elapsedTime);
		float frameTimeAverage = _frameTimeSum / _frameTimeSamples.size();
		if (frameTimeAverage > 0.f)
			_currentFPS = ONE_SECOND / frameTimeAverage;
		
		// フレームレート制御。
		for (;;)
		{
			float frameTimeCurrent = _timer.GetElapsedTime(false);
			if (_frameTime >= frameTimeCurrent)
			{
				if (_frameTime - frameTimeCurrent > _sleepTime)
				{
					Sleep(1);
					continue;
				}
				continue;
			}
			break;
		}
		
		// 経過時間保存。
		_elapsedTime = _timer.GetElapsedTime();
	}

};	// end struct FrameRateController::Impl

// ----------------------------------------------------------------------------
// FrameRateController クラスの転送部。

FrameRateController::FrameRateController()
{
	_pimpl.reset(new Impl());
}

FrameRateController::~FrameRateController()
{
}

void
FrameRateController::SetFrameRate(float fps)
{
	_pimpl->SetFrameRate(fps);
}

void
FrameRateController::SetNumFrameTimeSamples(size_t num)
{
	_pimpl->SetNumFrameTimeSamples(num);
}

float
FrameRateController::GetFrameTime() const
{
	return _pimpl->GetFrameTime();
}

float
FrameRateController::GetCurrentFPS() const
{
	return _pimpl->GetCurrentFPS();
}

float
FrameRateController::GetElapsedTime() const
{
	return _pimpl->GetElapsedTime();
}

void
FrameRateController::ChangeFrame()
{
	_pimpl->ChangeFrame();
}
