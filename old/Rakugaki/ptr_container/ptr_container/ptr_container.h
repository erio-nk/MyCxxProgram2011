/*!
	@file ptr_container.h

	ポインタを要素に持つSTLコンテナ用のラッパークラス。
	eraseやclearした際に、自動的に要素のポインタも解放する。

	@author Dila
 */

#include <list>
#include <vector>

#include <assert.h>

namespace ptr_container
{

	/*!
		要素をdeleteで解放するファンクタ
	 */
	template <class ElementPtr>
	struct Deleter
	{
		void operator () (ElementPtr pElement)
		{
			assert(pElement);
			delete pElement;
		}
	};

	/*!
		要素をReleaseで解放するファンクタ
	 */
	template <class ElementPtr>
	struct Releaser
	{
		void operator () (ElementPtr pElement)
		{
			assert(pElement);
			pElement->Release();
			delete pElement;
		}
	};

	/*!
		ポインタを要素に持つ線形リストコンテナ
		\tparam	ValuePtr	要素の型のポインタ
		\tparam	Deleter		要素の解放ファンクタ
	 */
	template < 
		class ValuePtr,
		template <class ValuePtr, class Allocator> class Container,
		template <class ValuePtr> class Deleter = ptr_container::Deleter,
		template <class ValuePtr> class Allocator = std::allocator
		>
	class list
	{
		struct Element
		{
			ValuePtr _pValue;
			Element(ValuePtr pValue) : _pValue(pValue) {}
			Element(const Element& e) : _pValue(e._pValue)
			{
				Element* p = const_cast<Element*>(&e);
				p->_pValue = NULL;
			}
			~Element()
			{
				if (_pValue) {
					Deleter<ValuePtr> deleter;
					deleter(_pValue);
				}
			}
			ValuePtr& operator -> () { return _pValue; }
		};
		typedef Container<Element, Allocator<Element>> List;
		List _list;

	public:
		typedef typename List::iterator iterator;
		typedef typename List::const_iterator const_iterator;

		typedef typename List::reverse_iterator reverse_iterator;
		typedef typename List::const_reverse_iterator const_reverse_iterator;

		typedef typename List::reference reference;
		typedef typename List::const_reference const_reference;

		typedef typename List::value_type value_type;

		list() {}
		~list() {}

		void clear() { _list.clear(); }

		iterator begin() { return _list.begin(); }
		const_iterator begin() const { return _list.begin(); }

		iterator end() { return _list.end(); }
		const_iterator end() const { return _list.end(); }
		
		reverse_iterator rbegin() { return _list.rbegin(); }
		const_reverse_iterator rbegin() const { return _list.rbegin(); }

		reverse_iterator rend() { return _list.rend(); }
		const_reverse_iterator rend() const { return _list.rend(); }

		reference front() { return _list.front(); }
		const_reference front() const { return _list.front(); }

		reference back() { return _list.back(); }
		const_reference back() const { return _list.back(); }

		void push_front(const value_type& value) { return _list.push_front(Element(value)); }	//!< \attention std::list only
		void push_back(const value_type& value) { return _list.push_back(Element(value)); }

		void pop_front() { _list.pop_front(); }
		void pop_back() { _list.pop_back(); }

		// remove()	
		// remove_if()

		iterator erase(iterator pos) { return _list.eraase(pos); }

	};	// end class list


}	// end namespace ptr_container

