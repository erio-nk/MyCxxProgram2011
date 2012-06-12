/*!
	@file	Container.h
	
			汎用コンテナクラステンプレート群。
			出来る限りSTLとインタフェースを合わせてある。
			未実装部分は随時。

	@author	E.Nakayama
*/
#pragma once
	
namespace Marda
{
	// ------------------------------------------------------------------------
	// リングバッファ。
	template <class Type>
	class ring_buffer
	{
	public:
		typedef std::vector<Type> Base;
	
		class iterator
		{
		public:
			iterator()
			{
			}
			
			iterator& operator ++ ()
			{
				forward();
				return *this;
			}
			
			iterator operator ++ (int)
			{
				iterator ret = *this;
				forward();
				return ret;
			}
			
			Type& operator * ()
			{
				return _owner->base()[_base_pos];
			}
			
			Type* operator -> ()
			{
				return &_owner->base()[_base_pos];
			}
			
			bool operator == (const iterator& rhs) const
			{
				return _pos == rhs._pos;
			}
			
			bool operator != (const iterator& rhs) const
			{
				return _pos != rhs._pos;
			}
			
		private:
			friend class ring_buffer;
			ring_buffer* _owner;
			size_t _pos;
			size_t _base_pos;	// operator *, -> でのアクセスを高速にする為、ベースコンテナ上の位置を事前計算して保持しておく。
			
			iterator(ring_buffer* owner, size_t pos)
				: _owner(owner)
				, _pos(pos)
			{
				_base_pos = owner->to_base_pos(pos);
			}
			
			void forward()
			{
				_pos = _owner->forward_pos(_pos);
				_base_pos = _owner->to_base_pos(_pos);
			}
			
			void backward()
			{
				_pos = _owner->backward_pos(_pos);
				_base_pos = _owner->to_base_pos(_pos);
			}
			
		};	// end class iterator

		typedef const iterator const_iterator;

		ring_buffer(size_t capacity = 8)
		{
			_base.resize(capacity);
			_begin_base_pos = 0;
			_size = 0;
		}
		
		Base& base()
		{
			return _base;
		}
		
		const Base& base() const
		{
			return _base;
		}
		
		size_t capacity() const
		{
			return _base.size();	// vector のサイズがリングバッファの容量となる。
		}
		
		bool empty() const
		{
			return _size == 0; 
		}
		
		bool full() const
		{
			return _size == capacity();
		}
		
		size_t size() const
		{
			return _size;
		}
		
		void reserve(size_t capacity_next)
		{
			size_t capacity_prev = capacity();
			if (capacity_next > capacity_prev)
			{
				// 新しいバッファサイズに合わせて、begin_base_pos 後方の要素を移動する。
				// バッファの増加サイズよりバッファ前方の要素数が少ない時、
				// 前方要素を移動させるには複数回のコピー処理が必要となる為、後方要素の移動のみ行う様にする。
				size_t num_back_elements = capacity_prev - _begin_base_pos;

				_base.resize(capacity_next);

				Type* move_from = &_base[_begin_base_pos];

				_begin_base_pos = capacity_next - num_back_elements;  
				Type* move_to = &_base[_begin_base_pos];

				memmove(move_to, move_from, num_back_elements * sizeof(Type));
			}
		}
		
		void resize(size_t size_next)
		{
			reserve(size_next);
			_size = size_next;
		}
		
		//! 縮小する時は前方から切りつめてリサイズする。
		void resize_from_front(size_t size_next)
		{
			reserve(size_next);
			
			size_t size_prev = _size;
			if (size_next < size_prev)
			{
				_begin_base_pos = forward_base_pos(_begin_base_pos, size_prev - size_next);
			}
			
			_size = size_next;
		}
		
		void clear()
		{
			_size = 0;
		}
		
		iterator begin()
		{
			return iterator(this, 0);
		}
		
		const_iterator begin() const
		{
			return iterator(this, 0);
		}
		
		iterator end()
		{
			return iterator(this, _size);
		}
		
		const_iterator end() const
		{
			return iterator(this, _size);
		}

		Type& front()
		{
			return _base[_begin_base_pos];
		}

		Type& back() 
		{
			return _base[to_base_pos(_size - 1)];
		}
		
		void push_front(const Type& value)
		{
			if (full())
				// TODO: throw buffer_full();
				return;
			_begin_base_pos = backward_base_pos(_begin_base_pos);
			_base[_begin_base_pos] = value;
			++_size;
		}
		
		void push_back(const Type& value)
		{
			if (full())
				// TODO: throw buffer_full();
				return;
			_base[to_base_pos(_size)] = value;
			++_size;
		}
		
		void pop_front()
		{
			if (empty())
				return;
			_begin_base_pos = forward_base_pos(_begin_base_pos);
			--_size;
		}
		
		void pop_back()
		{
			if (empty())
				return;
			--_size;
		}
		
	private:
		friend class iterator;
		Base _base;
		size_t _size;
		size_t _begin_base_pos;
		
		size_t forward_pos(size_t pos)
		{
			return cursor_forward(pos, 1, _size);
		}
		
		size_t backward_pos(size_t pos)
		{
			return cursor_backward(pos, 1);
		}
		
		size_t to_base_pos(size_t pos)
		{
			return (_begin_base_pos + pos) % _base.size();
		}
		
		size_t forward_base_pos(size_t base_pos, size_t val = 1)
		{
			return cursor_forward_rot(base_pos, val, _base.size());
		}
		
		size_t backward_base_pos(size_t base_pos, size_t val = 1)
		{
			return cursor_backward_rot(base_pos, val, _base.size());
		}
	};	// class ring_buffer

}	// end namespace Marda
