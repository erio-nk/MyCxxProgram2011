/*!
	@file	PerformanceCounter.h

	@author	E.Nakayama
*/
#pragma once
	
namespace Marda
{
	//! ������\�p�t�H�[�}���X�J�E���^�����݂��Ȃ�����������O�N���X�B
	struct QueryPerformanceCounterNotSupportedException
		: std::exception
	{};
	
	/*!
		@brief	�p�t�H�[�}���X�J�E���^�p�w���p�֐��Q�B
	*/
	class PerformanceCounter
	{
	public:
		/*!
			@brief	�p�t�H�[�}���X�J�E���^�̎��g���i���P�b�ԂɃJ�E���g�A�b�v�����l�j���擾����B
		*/
		static void GetFrequency(LARGE_INTEGER& out)
		{
			out = StaticMembers::Get()._frequency;
		}
		
		/*!
			@brief	���݂̃p�t�H�[�}���X�J�E���^�l���擾����B
		*/
		static void GetCurrentCount(LARGE_INTEGER& out)
		{
			QueryPerformanceCounter(&out);
//			if (failed(QueryPerformanceCounter(&out)))
//			{
//				throw QueryPerformanceCounterNotSupportedException();
//			}
		}
		
		/*!
			@brief	�p�t�H�[�}���X�J�E���^�l��b�ɕϊ�����B
		*/
		static float ToSecondsAsFloat(const LARGE_INTEGER& count)
		{
			return static_cast<float>(count.QuadPart) / static_cast<float>(StaticMembers::Get()._frequency.QuadPart);
		}
		
		static float ToSecondsAsFloat(const LONGLONG& count)
		{
			return static_cast<float>(count) / static_cast<float>(StaticMembers::Get()._frequency.QuadPart);
		}
		
		/*!
			@brief	�p�t�H�[�}���X�J�E���^�l���~���b�ɕϊ�����B
		*/
		static float ToMilliSecondsAsFloat(const LARGE_INTEGER& count)
		{
			return static_cast<float>(count.QuadPart * 1000) / static_cast<float>(StaticMembers::Get()._frequency.QuadPart);
		}

		static float ToMilliSecondsAsFloat(const LONGLONG& count)
		{
			return static_cast<float>(count * 1000) / static_cast<float>(StaticMembers::Get()._frequency.QuadPart);
		}
		
		/*!
			@brief	���������̌o�ߕb�����擾����B
					������Ƃ́AStaticMembers �����������ꂽ�����ł���B
		*/
		static float GetCurrentSecondsAsFloat()
		{
			LARGE_INTEGER count;
			GetCurrentCount(count);
			count.QuadPart -= StaticMembers::Get()._begin.QuadPart;
			return ToSecondsAsFloat(count);
		}

		/*!
			@brief	���������̌o�߃~���b�����擾����B
					������Ƃ́AStaticMembers �����������ꂽ�����ł���B
		*/
		static float GetCurrentMiiliSecondsAsFloat()
		{
			LARGE_INTEGER count;
			GetCurrentCount(count);
			count.QuadPart -= StaticMembers::Get()._begin.QuadPart;
			return ToMilliSecondsAsFloat(count);
		}
		
	private:
		struct StaticMembers
		{
			LARGE_INTEGER _frequency;
			LARGE_INTEGER _begin;
			static StaticMembers& Get()
			{
				static StaticMembers values;
				return values;
			}
		private:
			StaticMembers()
			{
				if (failed(QueryPerformanceFrequency(&_frequency)))
				{
					throw QueryPerformanceCounterNotSupportedException();
				}
				GetCurrentCount(_begin);
			}
		};
	};	// PerformanceCounter
	
}	// end namespace Marda

