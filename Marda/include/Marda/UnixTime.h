/*!
	@file	UnixTime.h
	
			UNIX�^�C���x�[�X�̎����N���X�B
			�����b�E���̉����Z�͉\�����A���͌����̓������Ⴄ�׌���͏o���Ȃ��B�C�����������������B
			
	@todo	�����b�E�N�����擾�֐��B

	@author	E.Nakayama
*/
#pragma once

namespace Marda
{
	class UnixTime
	{
	public:
		//! 1�b��\���萔
		static const UnixTime A_SECOND;

		//! 1��(60�b)��\���萔
		static const UnixTime A_MINUTE;
		
		//! 1����(3600�b)��\���萔
		static const UnixTime A_HOUR;
		
		//! 1��(3600*24�b)��\���萔
		static const UnixTime A_DAY;

		/*!
			@brief	���ݎ����ŃI�u�W�F�N�g���\�z����B
		*/
		UnixTime();
		
		/*!
			@brief	�w��̎����ŃI�u�W�F�N�g���\�z����B
		*/
		UnixTime(time_t time);

		//! dtor
		~UnixTime();
		
		/*!
			@brief	UNIX�^�C���̉��Z���s���B
		*/
		UnixTime operator + (const UnixTime& rhs) const
		{
			return UnixTime(_time + rhs._time);
		}
		
		/*!
			@brief	UNIT�^�C���̌��Z���s���B
		*/
		UnixTime operator - (const UnixTime& rhs) const
		{
			return UnixTime(_time - rhs._time);
		}

		/*!
			@brief	UNIX�^�C���̏�Z���s���B
		*/
		UnixTime operator * (float rhs) const
		{
			return UnixTime(static_cast<time_t>(_time * rhs));
		}

		/*!
			@brief	UNIX�^�C���̏��Z���s���B
		*/
		UnixTime operator / (float rhs) const
		{
			return UnixTime(static_cast<time_t>(_time / rhs));
		}
		
		/*!
			@brief	�����̕�����\�����擾����B
					���̊֐��̓X���b�h�Z�[�t�ł���B
		*/
		const char* ToStringA() const;
		
		const wchar_t* ToStringW() const;
		
		const TCHAR* ToString() const
		{
			#if defined(UNICODE)
			return ToStringW();
			#else
			return ToStringA();
			#endif
		}
		
	private:
		time_t _time;
		
	};	// end class Timer

}	// end namespace Marda
