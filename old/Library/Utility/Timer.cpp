/*!
 	@file Utility/Timer.cpp
 
 	タイマークラスの実装
 
 	@author	Dila
 */

#include <windows.h>

#include "Utility/Timer.h"

//
inline float QPCounterToMSec(const unsigned long long& t, const unsigned long long& freq)
{
	// マイクロ秒単位に直して(1000000倍して周波数で割る)、
	// ミリ秒単位に直して、秒単位にに直して取得
	return (float)(t * 1000000L / freq) * 0.001f * 0.001f;
}

/// ctor
Timer::Timer():
	start(0L),
	freq(0L),
	elapsed(0.0f),
	bStop(true)
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
}

/// dtor
Timer::~Timer()
{
}

/*!
 	タイマーを開始する
 
 */
void Timer::Start()
{
	bStop = false;
	QueryPerformanceCounter((LARGE_INTEGER*)&start);
}

/*!
 	タイマーを停止する
 
 	停止している間は、時間のカウントは行われない。
 */
void Timer::Stop()
{
	if (bStop) return;
	bStop = true;

	unsigned long long t;
	QueryPerformanceCounter((LARGE_INTEGER*)&t);
	elapsed += QPCounterToMSec(t - start, freq);
}


/*!
 	時間(秒)を取得する
 
 	秒以下の単位は、小数点以下の数値として取得出来る。
 	@return	秒
 */
float Timer::GetTime() const
{
	if (start == 0L) return 0.0f;
	if (bStop) return elapsed;

	unsigned long long t;
	QueryPerformanceCounter((LARGE_INTEGER*)&t);

	return QPCounterToMSec(t - start, freq) + elapsed;
}
