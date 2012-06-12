/*!
 	@file Utility/Timer.cpp
 
 	�^�C�}�[�N���X�̎���
 
 	@author	Dila
 */

#include <windows.h>

#include "Utility/Timer.h"

//
inline float QPCounterToMSec(const unsigned long long& t, const unsigned long long& freq)
{
	// �}�C�N���b�P�ʂɒ�����(1000000�{���Ď��g���Ŋ���)�A
	// �~���b�P�ʂɒ����āA�b�P�ʂɂɒ����Ď擾
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
 	�^�C�}�[���J�n����
 
 */
void Timer::Start()
{
	bStop = false;
	QueryPerformanceCounter((LARGE_INTEGER*)&start);
}

/*!
 	�^�C�}�[���~����
 
 	��~���Ă���Ԃ́A���Ԃ̃J�E���g�͍s���Ȃ��B
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
 	����(�b)���擾����
 
 	�b�ȉ��̒P�ʂ́A�����_�ȉ��̐��l�Ƃ��Ď擾�o����B
 	@return	�b
 */
float Timer::GetTime() const
{
	if (start == 0L) return 0.0f;
	if (bStop) return elapsed;

	unsigned long long t;
	QueryPerformanceCounter((LARGE_INTEGER*)&t);

	return QPCounterToMSec(t - start, freq) + elapsed;
}
