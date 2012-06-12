/*!
	@file MemoryPool.h

	Copyright(C) 2008-2009 Dila <yamadila@gmail.com>
		All rights reserved.

	@todo	色々未完。作り直さないと。
	@author Dila
 */

#pragma once

namespace Marda
{
	class IMemoryPool
	{
	public:
		virtual void* Alloc(int block_num) = 0;
		virtual void Free(void* ptr) = 0;
		virtual bool IsMyAddr(void* ptr) = 0;
	};

	/*!
		MemoryPool
	 */
	template <int SIZE, int BLOCK_NUM>
	class MemoryPool : public IMemoryPool, Uncopyable
	{
		enum
		{
			FREE = 0,
			USED = 1
		};

		static const int UNINIT_VALUE = 0xCC;	// 未初期化状態のメモリの値

	public:
		static MemoryPool<SIZE, BLOCK_NUM>& GetInstance()
		{
			static MemoryPool<SIZE, BLOCK_NUM> theInstance;
			return theInstance;
		}

		void* Alloc(int /*block_num = 1*/)
		{
			if (_cnt < BLOCK_NUM) {
				// 空きを探す
				for (int i = 0; i < BLOCK_NUM; ++i) {
					if (_flags[_pos] == FREE) {
						void* ret = &_datas[_pos*SIZE];
						_flags[_pos] = USED;
						++_cnt;
						if (++_pos >= BLOCK_NUM) _pos = 0;
						return ret;
					}
					if (++_pos >= BLOCK_NUM) _pos = 0;
				}
			}
			// 空きが無いので失敗
			return NULL;
			//throw std::bad_alloc();
		}

		void Free(void* ptr)
		{
			if (!IsMyAddr(ptr)) Assert(0, "Failed to free! Bad adress!!");
			uint8* p = reinterpret_cast<uint8*>(ptr);
			ptrdiff_t diff = (p - &_datas[0]);
			int pos = diff / SIZE;

			_flags[pos] = FREE;
			--_cnt;
			memset(ptr, UNINIT_VALUE, SIZE); 
		}

		inline void* GetMinAddr() { return &_datas[0]; }
		inline void* GetMaxAddr() { return &_datas[SIZE*BLOCK_NUM]; }
		bool IsMyAddr(void* ptr) { return GetMinAddr() <= ptr && ptr && GetMaxAddr(); }

	private:
		std::vector<uint8> _flags;
		std::vector<uint8> _datas;
		int _pos;	//!< 現在のカーソル位置
		int _cnt;	//!< 現在の確保数
		
		MemoryPool() :
			_flags(BLOCK_NUM),
			_datas(SIZE * BLOCK_NUM),
			_pos(0),
			_cnt(0)
		{
			Assert(SIZE % 4 == 0, "Bad memory size!");	// 4byteサイズに合わせてください！
			if (_datas.size() <= 0) return;
			memset(&_flags[0], FREE, BLOCK_NUM);
			memset(&_datas[0], UNINIT_VALUE, SIZE * BLOCK_NUM);
		}

		~MemoryPool()
		{
		}

	};


	template
		<
			int BLOCK_NUM_4_BYTE	= 0/*4096*/,
			int BLOCK_NUM_8_BYTE	= 0/*2048*/,
			int BLOCK_NUM_16_BYTE	= 0/*1024*/,
			int BLOCK_NUM_32_BYTE	= 0/*512*/,
			int BLOCK_NUM_64_BYTE	= 0/*256*/,
			int BLOCK_NUM_128_BYTE	= 0/*128*/,
			int BLOCK_NUM_256_BYTE	= 0/*64*/,
			int BLOCK_NUM_512_BYTE	= 0/*32*/,
			int BLOCK_NUM_1024_BYTE	= 0/*16*/,
			int BLOCK_NUM_2048_BYTE	= 0/*8*/,
			int BLOCK_NUM_4096_BYTE	= 0/*4*/
		>
	class MemoryPoolManager
	{
	public:
		MemoryPoolManager()
		{
			_poolList.push_back(&MemoryPool<4, BLOCK_NUM_4_BYTE>::GetInstance());
			_poolList.push_back(&MemoryPool<8, BLOCK_NUM_8_BYTE>::GetInstance());
			_poolList.push_back(&MemoryPool<16, BLOCK_NUM_16_BYTE>::GetInstance());
			_poolList.push_back(&MemoryPool<32, BLOCK_NUM_32_BYTE>::GetInstance());
			_poolList.push_back(&MemoryPool<64, BLOCK_NUM_64_BYTE>::GetInstance());
			_poolList.push_back(&MemoryPool<128, BLOCK_NUM_128_BYTE>::GetInstance());
			_poolList.push_back(&MemoryPool<256, BLOCK_NUM_256_BYTE>::GetInstance());
			_poolList.push_back(&MemoryPool<512, BLOCK_NUM_512_BYTE>::GetInstance());
			_poolList.push_back(&MemoryPool<1024, BLOCK_NUM_1024_BYTE>::GetInstance());
			_poolList.push_back(&MemoryPool<2048, BLOCK_NUM_2048_BYTE>::GetInstance());
			_poolList.push_back(&MemoryPool<4096, BLOCK_NUM_4096_BYTE>::GetInstance());
		}

		//static GetInstance() 
		//{
		//}

		//inline size_t Align(size_t size)
		//{
		//	//int n = (size >> 2) << 2;

		//	int 
		//}

		void* Alloc(size_t size)
		{
			int n = (size >> 2) - 1;	// TODO: だめだぁーこの計算式じゃ8以上がおかしくなる･･･順々に比較でいいんじゃねえのどうせ重いんだしnewは･･･
			if (n < 0) n = 0;
			return _poolList[n]->Alloc(1);
		}

		void Free(void* ptr)
		{
			// もっとマシなやり方ないのか･･･
			for (unsigned int i = 0; i < _poolList.size(); ++i) {
				if (_poolList[i]->IsMyAddr(ptr)) {
					_poolList[i]->Free(ptr);
					return;
				}
			}
		}

	private:
		std::vector<IMemoryPool*> _poolList;
		
	};

}	// end namespace Marda

