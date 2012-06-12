#include <iostream>

// 1,2,4,8,16,32...2の階乗にアラインメントするメタ関数	色々やったけど結局使わない系
#if 1

/*	MetaProgramingLibs.hとか作るか･･･
template <bool Expr, int Then, int Else>
struct If;

template <int Then, int Else>
struct If<true, Then, Else>
{
	enum { result = Then };
};

template <int Then, int Else>
struct If<false, Then, Else>
{
	enum { result = Else };
};*/


template <int T>
class Align
{
	template <int T, int N, bool Expr >
	struct _Align;
public:
	enum { result = _Align<T, 1, T<=1 >::result };

private:
	template <int T, int N>
	struct _Align<T, N, true>
	{
		enum { result = N };
	};

	template <int T, int N>
	struct _Align<T, N, false>
	{
		enum { result = _Align<T, N*2, T<=(N*2) >::result };
	};
};

#else

template <int T, int N = 1, bool E = (T<=N)>
struct Align;

template <int T, int N>
struct Align<T, N, true>
{
	enum
	{
		result = N
	};
};

template <int T, int N>
struct Align<T, N, false>
{
	enum
	{
		result = Align<T, N*2>::result
	};
};

#endif


//template <size_t SIZE>
//void* operator new (const size_t size)
//{
//	std::cout << "new:" << Align<SIZE>::result << std::endl; 
//	return new size;
//}

//template <size_t SIZE>
//void* New(size_t size)
//{
//	std::cout << "new" << Align<SIZE>::result << std::endl;
//	return 
//}

#include <algorithm>

//#include "Marda/Util/MemoryPool.h"
//
//typedef Marda::MemoryPool<int> MyMemoryPool;

//
void test_memorypool()
{
	size_t int_align = Align<sizeof(long long)>::result;

	std::cout << "int_align=" << int_align << std::endl;

	//MyMemoryPool memPool;

	//int* p = new int;
}
