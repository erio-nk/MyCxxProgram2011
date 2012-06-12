/*!
	@file	Utilities/Container.h
	
			std::vector、配列をベースにしたSTLコンテナのインタフェースと互換のあるコンテナクラス群。

	@author	E.Nakayama
*/
#pragma once

namespace Marda
{	
	
	/*!
		@brief	可変長ビット配列
				vectorの用に挿入や削除が可能。
				パフォーマンス重視の為、最大サイズが固定されてしまう。
	*/
	template <typename BaseType = int>
	class tiny_bit_vector
	{
	public:
		/*!
			@brief	指定位置のビットの削除
					指定位置以降のビットは全て右にひとつシフトする。
		*/
		void erase(unsigned int pos)
		{
			if (pos >= _numBits) return;
			BaseType mask = static_cast<BaseType>(-1) << pos;
			_elements = (_elements & (~mask)) | ((_elements >> 1) & mask);
		}
		
		
	private:
		unsigned int _numBits;
		BaseType _elements;
	};	// end class tiny_bit_vector
	
	
	
	
	
	
	////////////////////////////
	
	// std::arrayがあるので、vmap,vsetだけを作り、あとはコンテナをvectorかarrayか選べるようにしとけばコードは一種類で済む。
	// ただし、arrayの場合コピーの速さ(memmove)も出来れば欲しいので、特殊化出来るならした方がいいかも。
	
	/*!
		@brief	配列の要素が最大でこれ以上挿入出来ない事を示す例外クラス
	*/
	struct elements_max_exception : public std::exception
	{
	};

	/*!
		@brief	配列ベースのmapクラス
	*/
	template <
		typename KeyType,
		typename MappedType,
		size_t ElementsMax,
		typename Predicate = std::less<KeyType>
		>
	class amap
	{
	public:	
		typedef KeyType key_type;
		typedef MappedType mapped_type;
		typedef MappedType referent_type;
		typedef Predicate key_compare;
		
		typedef std::pair<KeyType, MappedType> value_type;
		
		typedef value_type* iterator;
		typedef const value_type* const const_iterator;

		static const size_t ELEMENTS_MAX;
		
		amap()
			: _numElements(0)
		{
		}
		
		iterator begin()
		{
			return _base;
		}
		
		iterator begin() const
		{
			return const_cast<amap*>(this)->begin();
		}
		
		iterator end()
		{
			return _base + _numElements;
		}
		
		iterator end() const
		{
			return const_cast<amap*>(this)->end();
		}
		
		bool empty() const
		{
			return _numElements == 0;
		}
		
		size_t size() const
		{
			return _numElements;
		}
		
		iterator lower_bound(key_type key)
		{
			struct Adapter
			{
				bool operator () (const value_type& lhs, const key_type& rhs)
				{
					key_compare pred;
					return pred(lhs.first, rhs);
				}
			};
			return std::lower_bound(begin(), end(), key, Adapter());
		}

		iterator upper_bound(key_type key)
		{
			struct Adapter
			{
				bool operator () (const key_type& lhs, const value_type& rhs)
				{
					key_compare pred;
					return pred(lhs, rhs.first);
				}
			};
			return std::upper_bound(begin(), end(), key, Adapter());
		}

		iterator find(key_type key)
		{
			iterator result = lower_bound(key);
			if (result == end())
			{
				return end();
			}
			if (result->first != key)
			{
				return end();
			}
			return result;
		}
		
		std::pair<iterator, bool> insert(value_type val)
		{
			if (_numElements >= ElementsMax)
			{
				throw elements_max_exception();
			}

			std::pair<iterator, bool> result;
			result.first = lower_bound(val.first);
			if (result.first->first == val.first)
			{
				// 同じキーが既に存在している
				result.second = false;
				return result;
			}

			// TODO: メンバにauto_ptr のようなものを持っているとマズイかも？どっかでテストすべし
			memmove(result.first + 1, result.first, (end() - result.first) * sizeof(value_type));
			memcpy(result.first, &val, sizeof(value_type));

			result.second = true;
			++_numElements;

			return result;
		}
		
//	private:
		value_type _base[ElementsMax];
		size_t _numElements;
		
	};	// end class amap

	
	template <
		typename KeyType,
		typename MappedType,
		size_t ElementsMax,
		typename Predicate
		>
	const size_t
	amap<KeyType, MappedType, ElementsMax, Predicate>::ELEMENTS_MAX = ElementsMax;
	
}	// end namespace Marda
