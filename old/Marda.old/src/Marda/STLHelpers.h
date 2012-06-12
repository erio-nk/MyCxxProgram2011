/*!
	@file	STLHelpers.h
	@author	Dila
	
	Copyright(C) 2009 Dila <yamadila@gmail.com>
		All rights reserved.
 */
#pragma once

namespace Marda
{

	/*!
		@brief	mapオブジェクトにキーがあるかどうかを返す。
	 */
	template <class Map, class Key>
	inline bool has_key(const Map& map, const Key& key)
	{
		Map::const_iterator i = map.find(key);
		if (i == map.end()) return false;
		return true;
	}

#if 0
	/*!
		@brief	ポインタコンテナ用のカスタムアロケータ。
				要素削除時にポインタも削除する。
		@todo	型タグによるディスパッチで、スカラー型（＝ポインタ）ではdestroyが呼ばれない様になっていた･･･ので計画失敗。
	 */
	template <class T, template <class> class Base = std::allocator >
	struct ptr_container_allocator
	{
		typedef typename Base<T>::size_type size_type;
		typedef typename Base<T>::difference_type difference_type;
		typedef typename Base<T>::value_type value_type;
		typedef typename Base<T>::pointer pointer;
		typedef typename Base<T>::const_pointer const_pointer;
		typedef typename Base<T>::reference reference;
		typedef typename Base<T>::const_reference const_reference;
		
		typename Base<T> _base;	///< 元のアロケータ

		template <class U>
		struct rebind {
			typedef ptr_container_allocator<U> other;
		};

		ptr_container_allocator() {}
		ptr_container_allocator(const ptr_container_allocator<T>&) {}
		//template <class U> ptr_container_allocator(const ptr_container_allocator<U>&) throw() 
		//{}

		inline pointer allocate(size_type n, const void* p = 0)
		{
			return _base.allocate(n, p);
		}

		inline void deallocate(pointer p, size_type n)
		{
			_base.deallocate(p, n);
		}

		//inline void construct(pointer p, const T& value)
		//{
		//	std::cout << "construct" << std::endl;
		//	_base.construct(p, value);
		//}

		////! カスタム破棄関数
		//inline void destroy(pointer p)
		//{
		//	// ポインタの指す先のオブジェクトを破棄する
		//	std::cout << p << std::endl;
		//	delete *p;
		//	_base.destroy(p);
		//}

		inline size_type max_size() const throw()
		{
			return _base.max_size();
		}
	};
#endif

	//template <class T>
	//struct ptr_element_destroy
	//{
	//	void operator ()(T t) { delete *T; }
	//};
	///*!
	//	@biref	ポインタコンテナ用破棄関数
	// */
	//template <class T>
	//void ptr_container_destroy(T& t)
	//{
	//	std::for_each(t.begin(), t.end(), ptr_element_destroy<typename T>());
	//	t.clear();
	//}

}	// end namespace Marda

