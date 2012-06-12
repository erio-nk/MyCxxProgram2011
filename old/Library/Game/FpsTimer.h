/*!
 	@file Utility/FpsTimer.h
 
 	FPS����^�C�}�[�B��ɃQ�[���p�B
 	���t���[����Wait()���ĉ������B
 
 	@author	Dila
 */

//! @addtogroup Game �Q�[���p�N���X
//! @{

#pragma once

#include <windows.h>
#include "Utility/Timer.h"

/*!
 	FPS���䁕�v���N���X
 	@see Utility/Timer
 */
class FpsTimer
{
#if 0
	// ����p
	unsigned int prevFrameTime;
	unsigned int interval;

	// �J�E���g�p
	int fps;
	int counting;
	unsigned int prevCountTime;
#endif
	Timer timer;

	// ����p
	float interval;
	float nextFrameTime;

	// �J�E���g�p
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
