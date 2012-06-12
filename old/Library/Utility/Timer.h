/*!
 	@file Utility/Timer.h
 
 	�^�C�}�[�N���X
 
 	@author	Dila
 */
//! @addtogroup Utility
//! @{

#pragma once

/*!
 	�^�C�}�[�N���X
 */
class Timer
{
	unsigned long long start;	//! �J�n����
	unsigned long long freq;	//! ���g��
	float elapsed;	//! �~�ς����o�ߎ���

	bool bStop;	//! ��~�t���O

public:
	Timer();
	~Timer();

	//! �^�C�}�[�J�n
	void Start();

	//! �^�C�}�[��~
	void Stop();

	//! �b�擾
	float GetTime() const;
};

//! @}
