/*!
	@file	STLHelpers.h
	
			STL利用時のヘルパ関数・クラス群。

	@author	E.Nakayama
*/
#pragma once
	
#include "Types.h"
	
namespace Marda
{
	// ------------------------------------------------------------------------
	// std::auto_ptr, std::shared_ptr用

	/*!
		@brief	ポインタがNULLかどうかを判別する
				この関数を利用する事により、通常のポインタ版の is_null 関数と記述を統一出来る。
		@return	ポインタがNULLであればtrue
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
		@brief	ポインタがNULLでないかどうかを判別する
				使用意図としては is_null 関数と同様。
		@return	ポインタがNULLでなければtrue
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
		@brief	ポインタを解放する
				通常ポインタ版 safe_delete 関数と記述を統一出来る。
				std::for_eachの引数に渡す事も可能。
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
		@brief	ポインタの値を取得する
				通常ポインタ版 get_ptr 関数と記述を統一出来る。
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
	// std::map, std::multimap用

	//! pair_second 関数で返される実際の関数アダプタ
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
		@brief	std::pair::second に関数を適用するアダプタ
				_pair_second 構造体にテンプレート引数リストを明示すれば
				直接利用出来るが、この関数を使えばコードが簡潔になる。
		@code
		// 関数ポインタを pair_second に渡す場合
		void Print(char c)
		{
			printf("%c\n", c);
		}
		void PrintCharacters1(std::map<int, char>& characters)
		{
			std::for_each(characters.begin(), characters.end(), pair_second(&Print));
		}

		// 関数オブジェクトを pair_second に渡す場合
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
		@brief	マップの要素をランダムアクセスする為のアダプタクラステンプレート。
				マップの内容が(insert, clear 関数の呼び出し等で)変化した場合は、reset 関数を呼び出す必要がある。
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
