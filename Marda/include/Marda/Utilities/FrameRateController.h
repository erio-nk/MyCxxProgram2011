/*!
	@file	Utilities/FrameRateController.h
	
			�t���[�����[�g����B

	@author	E.Nakayama
*/
#pragma once

namespace Marda
{
	/*!
		@brief	�t���[�����[�g����N���X�B
	*/
	class FrameRateController
	{
	public:
		FrameRateController();
	
		~FrameRateController();
		
		/*!
			@brief	�t���[�����[�g��ݒ肷��B
			@param	fps	FPS�B
		*/
		void SetFrameRate(float fps);
		
		/*!
			@brief	FPS�Z�o�ׂ̈̃T���v���t���[������ݒ肷��B
			@param	num	�T���v�����B�f�t�H���g�ł�10�B
		*/
		void SetNumFrameTimeSamples(size_t num);
		
		/*!
			@brief	�P�t���[���̖ڕW���Ԃ��擾����B
					GetElapsedTime �̖߂�l�����̊֐����傫���ꍇ�A�t���[���X�L�b�v����������K�v������B
		*/
		float GetFrameTime() const;
		
		/*!
			@brief	����FPS���擾����B
		*/
		float GetCurrentFPS() const;
		
		/*!
			@brief	�O��t���[������̌o�ߎ��Ԃ��擾����B
		*/
		float GetElapsedTime() const;
		
		/*!
			@brief	�t���[���̐؂�ւ���ʒm���A�t���[�����[�g�𐧌䂷��ׂɕK�v�ł���Αҋ@����B
		*/
		void ChangeFrame();

	private:
		struct Impl;
		std::auto_ptr<Impl> _pimpl;
		
	};	// end class FrameRateController
}	// end namespace Marda
