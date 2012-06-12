/*!
 	@file Utility/FpsTimer.h
 
 	FPS制御タイマー。主にゲーム用。
 	毎フレームでWait()して下さい。
 
 	@author	Dila
 */

//! @addtogroup Game ゲーム用クラス
//! @{

#pragma once

#include <windows.h>
#include "Utility/Timer.h"

/*!
 	FPS制御＆計測クラス
 	@see Utility/Timer
 */
class FpsTimer
{
#if 0
	// 制御用
	unsigned int prevFrameTime;
	unsigned int interval;

	// カウント用
	int fps;
	int counting;
	unsigned int prevCountTime;
#endif
	Timer timer;

	// 制御用
	float interval;
	float nextFrameTime;

	// カウント用
	int fps;
	int counting;
	float prevCountTime;


public:
	FpsTimer();
	~FpsTimer();

	void SetFrameRate(int rate);
	void Wait();

	int GetFps() { return fps; }
};

/// @}
