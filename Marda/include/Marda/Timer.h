/*!
	@file	Timer.h
	
			���Ԍv���N���X�B
			timeGetTime���p�ł͋C����������B

	@author	E.Nakayama
*/
#pragma once

namespace Marda
{
	class Timer
	{
	public:
		typedef float float_t;
		
		Timer();

		~Timer();
		
		/*!
			@brief	�^�C�}�[���J�n����B
		*/
		void Start();
		
		/*!
			@brief	�^�C�}�[���~����B
		*/
		void Stop();
		
		/*!
			@brief	�^�C�}�[�����Z�b�g����B
		*/
		void Reset();
		
		/*!
			@brief	�^�C�}�[���J�n���Ă���̌o�ߎ��Ԃ��擾����B
					�^�C�}�[���~���Ă������Ԃ͊܂܂�Ȃ��B
			@return	�o�ߎ��ԁB�b�B
		*/
		float GetTime() const;
		
		/*!
			@brief	�O��� GetElapsedTime �Ăяo��������̌o�ߎ��Ԃ��擾����B
					�^�C�}�[���~���Ă������Ԃ͊܂܂�Ȃ��B
			@param	reset	false ���w�肵���ꍇ�́A�o�ߎ��Ԃ̃��Z�b�g���s��Ȃ��B
			@return	�O��� GetElapsedTime �Ăяo��������̌o�ߎ��ԁB�b�B
		*/
		float GetElapsedTime(bool reset = true);
		
	private:
		struct Impl;
		std::auto_ptr<Impl> _pimpl;
		
	};	// end class Timer

}	// end namespace Marda
