/*!
 	@file Game/FpsTimer.cpp
 
 	@author	Dila
 */
#include <stdio.h>	// for debug print

#include <windows.h>

#pragma comment(lib, "winmm.lib")
#include <mmsystem.h>

#include "Game/FpsTimer.h"


//! ctor()
FpsTimer::FpsTimer() :
	timer(),
	interval(0.0f),
	nextFrameTime(0.0f),
	fps(0),
	counting(0),
	prevCountTime(0)
{
	SetFrameRate(30);
	
	timer.Start();

	nextFrameTime = timer.GetTime() + interval;
			
	timeBeginPeriod(1);
}

//! dtor()
FpsTimer::~FpsTimer()
{
	timeEndPeriod(1);
}


/*!
 	フレームレートを設定する。
 	@param	fps	FPS
 */
void FpsTimer::SetFrameRate(int rate)
{
	interval = 1.0f / (float)rate;
}


/*!
 	次のフレームまで待つ。
 
 	サンプル
 	@code
 		FpsCounter fps;
 		// メインループ
 		while (true) {
 			...
 			fps.Wait();
 		}
 	@endcode
 */
void FpsTimer::Wait()
{
	// フレームカウント
	++counting;

	// 制御
	while (true) {
		if (timer.GetTime() >= nextFrameTime) {
			nextFrameTime = timer.GetTime() + interval;
			break;
		}
		if (nextFrameTime - timer.GetTime() >= 0.01f) {
			// 十分に時間が残っている時だけSleep
			//printf("\tsleep start %f\n", timer.GetTime());
			Sleep(1);
			//printf("\tsleep end   %f\n", timer.GetTime());
		}
	}

	// FPS更新
	if (timer.GetTime() - prevCountTime >= 1.0f) {
		fps = counting;
		counting = 0;
		prevCountTime = timer.GetTime();
	}
}
