/*!
	@file	STLHelpers.h
	
			STL���p���̃w���p�֐��E�N���X�Q�B

	@author	E.Nakayama
*/
#pragma once
	
#include "Types.h"
	
namespace Marda
{
	// ------------------------------------------------------------------------
	// std::auto_ptr, std::shared_ptr�p

	/*!
		@brief	�|�C���^��NULL���ǂ����𔻕ʂ���
				���̊֐��𗘗p���鎖�ɂ��A�ʏ�̃|�C���^�ł� is_null �֐��ƋL�q�𓝈�o����B
		@return	�|�C���^��NULL�ł����true
	*/
	template <class T>
	inline bool is_null(const std::auto_ptr<T>& ptr)
	{
		return ptr.get() == NULL;
	}
	
	template <class T>
	inline bool is_null(const std::shared_ptr<T>& ptr)
	{
		return ptr.get() == NULL;
	}
	
	/*!
		@brief	�|�C���^��NULL�łȂ����ǂ����𔻕ʂ���
				�g�p�Ӑ}�Ƃ��Ă� is_null �֐��Ɠ��l�B
		@return	�|�C���^��NULL�łȂ����true
	*/
	template <class T>
	inline bool is_not_null(const std::auto_ptr<T>& ptr)
	{
		return ptr.get() != NULL;
	}

	template <class T>
	inline bool is_not_null(const std::shared_ptr<T>& ptr)
	{
		return ptr.get() != NULL;
	}
	
	/*!
		@brief	�|�C���^���������
				�ʏ�|�C���^�� safe_delete �֐��ƋL�q�𓝈�o����B
				std::for_each�̈����ɓn�������\�B
	*/
	template <class T>
	inline void safe_delete(std::auto_ptr<T>& ptr)
	{
		ptr.reset();
	}

	template <class T>
	inline void safe_delete(std::shared_ptr<T>& ptr)
	{
		ptr.reset();
	}
	
	/*!
		@brief	�|�C���^�̒l���擾����
				�ʏ�|�C���^�� get_ptr �֐��ƋL�q�𓝈�o����B
	*/
	template <class T>
	inline T* get_ptr(std::auto_ptr<T>& ptr)
	{
		return ptr.get();
	}

	template <class T>
	inline T* get_ptr(std::shared_ptr<T>& ptr)
	{
		return ptr.get();
	}
	
	// ------------------------------------------------------------------------
	// std::map, std::multimap�p

	//! pair_second �֐��ŕԂ������ۂ̊֐��A�_�v�^
	template <class Func>
	struct _pair_second
	{
		Func _func;
		_pair_second(Func func) : _func(func) {}
		template <class Pair>
		void operator ()(Pair& pair)
		{
			_func(pair.second);
		}
	};

	/*!
		@brief	std::pair::second �Ɋ֐���K�p����A�_�v�^
				_pair_second �\���̂Ƀe���v���[�g�������X�g�𖾎������
				���ڗ��p�o���邪�A���̊֐����g���΃R�[�h���Ȍ��ɂȂ�B
		@code
		// �֐��|�C���^�� pair_second �ɓn���ꍇ
		void Print(char c)
		{
			printf("%c\n", c);
		}
		void PrintCharacters1(std::map<int, char>& characters)
		{
			std::for_each(characters.begin(), characters.end(), pair_second(&Print));
		}

		// �֐��I�u�W�F�N�g�� pair_second �ɓn���ꍇ
		struct Printer
		{
			void operator () (char c) const
			{
				printf("%c\n", c);
			}
		};
		void PrintCharacters2(std::map<int, char>& characters)
		{
			std::for_each(characters.begin(), characters.end(), pair_second(Printer()));
		}
		@endcode
	*/
	template <class Func>
	_pair_second<Func> pair_second(Func func)
	{
		return _pair_second<Func>(func);
	}
	
	/*!
		@brief	�}�b�v�̗v�f�������_���A�N�Z�X����ׂ̃A�_�v�^�N���X�e���v���[�g�B
				�}�b�v�̓��e��(insert, clear �֐��̌Ăяo������)�ω������ꍇ�́Areset �֐����Ăяo���K�v������B
	*/
	template <class Base>
	class map_random_accessor
	{
	public:
		map_random_accessor()
			: _changed(false)
			, _lastIdx(0)
		{
		}
		
		void reset(Base& base)
		{
			_base = &base;
			_lastIdx = 0;
			_lastItr = _base->begin();
		}
		
		typename Base::value_type& at(size_t index)
		{
			if (index >= _base->size())
			{
				throw std::out_of_range("");
			}
			
			if (_lastIdx < index)
			{
				_lastItr = std::next(_lastItr, index - _lastIdx);
			}
			else
			if (_lastIdx > index)
			{
				_lastItr = std::prev(_lastItr, _lastIdx - index);
			}
			_lastIdx = index;

			return *_lastItr;
		}
		
		typename const Base::value_type& at(size_t index) const
		{
			return const_cast<random_accessable_map*>(this)->at(index);
		}
		
	private:
		Base* _base;
		bool _changed;
		size_t _lastIdx;
		typename Base::iterator _lastItr;
	};	// end class randam_accessable_map
	
}	// end namespace Marda
