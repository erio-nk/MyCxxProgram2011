#include "Marda.h"

using namespace Marda;

// --------------------------------------------------------------------------------------------- //
// Castテスト
#pragma warning(push)
#pragma warning(disable: 4189)
namespace __Cast
{
	struct Hoge
	{
		virtual ~Hoge() {};
	};

	struct Fuga : public Hoge
	{
		~Fuga() {}
	};

	struct Piyo : public Hoge
	{
		~Piyo() {}
	};

	void Test()
	{
		DBGTRACE(_T("--------------------------------------------------"));
		Fuga fuga;
		Piyo piyo;

		Hoge* pHogeFuga = up_cast<Hoge*>(&fuga);
		Hoge* pHogePiyo = up_cast<Hoge*>(&piyo);

		Fuga* pFugaFuga = down_cast<Fuga*>(pHogeFuga);	// OK
		//Fuga* pFugaPiyo = down_cast<Fuga*>(pHogePiyo);	// NG
	}
}
#pragma warning(pop)

// --------------------------------------------------------------------------------------------- //
// Delegateテスト

namespace __Delegate
{
#if 1
	void Hoge0()
	{
		DBGTRACE(_T("hoge"));
	}

	void Hoge1(int hoge)
	{
		DBGTRACE(_T("hoge %d"), hoge);
	}

	int Moke0()
	{
		DBGTRACE(_T("moke"));
		return 1;
	}

	int Moke1(int moke)
	{
		DBGTRACE(_T("moke %d"), moke);
		return moke;
	}

	int Moke3(int moke1, int moke2, int moke3)
	{
		DBGTRACE(_T("moke %d, %d, %d"), moke1, moke2, moke3);
		return 99;
	}

	struct Fuga
	{
		int _fuga;
		Fuga(int fuga) : _fuga(fuga) {}

		void Fuga0()
		{
			DBGTRACE(_T("fuga[%d]"), _fuga);
		}
		
		int Fuga3(int x, int y, int z)
		{
			DBGTRACE(_T("fuga[%d] %d, %d, %d"), _fuga, x, y, z);
			return _fuga;
		}
	};

	void Test()
	{
		DBGTRACE(_T("--------------------------------------------------"));
		Delegate<void>* del_v0 = Delegate<void>::New(&Hoge0);
		(*del_v0)();
		delete del_v0;

		Fuga fuga(100);
		del_v0 = Delegate<void>::New(&fuga, &Fuga::Fuga0);
		(*del_v0)();
		delete del_v0;


		Delegate<void, int>* del_v1 = Delegate<void, int>::New(&Hoge1);
		(*del_v1)(0);
		delete del_v1;

		int ret;
		Delegate<int>* del_n0 = Delegate<int>::New(&Moke0);
		ret = (*del_n0)();
		DBGTRACE(_T("\treturnd %d"), ret);
		delete del_n0;
		
		Delegate<int, int>* del_n1 = Delegate<int, int>::New(&Moke1);
		ret = (*del_n1)(10);
		DBGTRACE(_T("\treturnd %d"), ret);
		delete del_n1;
		
		Delegate<int, int, int, int>* del_n3 = Delegate<int, int, int, int>::New(&Moke3);
		ret = (*del_n3)(1, 2, 3);
		DBGTRACE(_T("\treturnd %d"), ret);
		delete del_n3;
		
		del_n3 = Delegate<int, int, int, int>::New(&fuga, &Fuga::Fuga3);
		ret = (*del_n3)(1, 2, 3);
		DBGTRACE(_T("\treturnd %d"), ret);
		delete del_n3;
	}
#else
	void Test();
#endif
}


// --------------------------------------------------------------------------------------------- //
// Variantテスト
namespace __Variant
{
	void Test()
	{
	
		DBGTRACE(_T("--------------------------------------------------"));

		Variant _int(99);
		DBGTRACE(_T("%s : %d"), _int.GetString().c_str(), _int.GetInt());
		
		Variant _float(1.5f);
		DBGTRACE(_T("%s : %f"), _float.GetString().c_str(), _float.GetFloat());

		Variant _bool(true);
		DBGTRACE(_T("%s : %d"), _bool.GetString().c_str(), _bool.GetBool());
	}
}

// --------------------------------------------------------------------------------------------- //
// Propertiesテスト

namespace __Properties
{
	void Test()
	{
		DBGTRACE(_T("--------------------------------------------------"));

		Properties props;

		props.AddProperty(_T("hoge"), 0);
		props.AddProperty(_T("fuga"), 1);
		
		props[_T("piyo")].SetString(_T("piyopiyo"));

		Variant* hoge = props.GetProperty(_T("hoge"));
		DBGTRACE(_T("hoge=%d"), hoge->GetInt());

		Variant* fuga = props.GetProperty(_T("fuga"));
		DBGTRACE(_T("fuga=%d"), fuga->GetInt());

		DBGTRACE(_T("piyo=%s"), props[_T("piyo")].GetString().c_str());
	}
}

// --------------------------------------------------------------------------------------------- //
// Uncopyableテスト

namespace __Uncopyable
{
	struct Hoge : Uncopyable
	{
	};

	void Test()
	{
		DBGTRACE(_T("--------------------------------------------------"));

		Hoge hoge1;
//		Hoge hoge2(hoge1);	// NG!
	}
}


// --------------------------------------------------------------------------------------------- //
// MemoryPoolテスト
#if 0
#include "Base/MemoryPool.h"
namespace __MemoryPool
{
	typedef MemoryPool<sizeof(int), 2> IntMemoryPool;

	void Test()
	{
		DBGTRACE(_T("--------------------------------------------------"));

		//IntMemoryPool& pool = IntMemoryPool::GetInstance();

		//int* pInt1 = reinterpret_cast<int*>(pool.Alloc(1));
		//*pInt1 = 1;

		//int* pInt2 = reinterpret_cast<int*>(pool.Alloc(1));
		//*pInt2 = 2;

		//pool.Free(pInt1);

		//int* pInt3 = reinterpret_cast<int*>(pool.Alloc(1));
		//*pInt3 = 3;

		//pool.Free(pInt2);

		MemoryPoolManager
			<
				16,
				16,
				16,
				16
			>
			mgr;

		int* pInt1 = reinterpret_cast<int*>(mgr.Alloc(sizeof(int)));
		*pInt1 = 1;

		llong* pLlong1 = reinterpret_cast<llong*>(mgr.Alloc(sizeof(llong)));
		*pLlong1 = 2L;

		struct Hoge
		{
			int _int;
			uint _uint;
			char _char;
		};

		Hoge* pHoge = reinterpret_cast<Hoge*>(mgr.Alloc(sizeof(Hoge)));
		pHoge->_int = 1;
		pHoge->_char = 2;
	}
}

#endif

// --------------------------------------------------------------------------------------------- //
// Threadテスト
#include "Thread/Thread.h"
namespace __Thread
{
	class MyThread : public Thread
	{
		int _num;

	public:
		MyThread() :
			_num(0)
		{
		}

		int Run()
		{
			int max = _num + 5;
			for (; _num < max; ++_num) {
				Sleep(100);
				printf("MyThread[%u] : %d\n", GetThreadID(), _num);
			}

			return 0;
		}
	};

	void Test()
	{
		DBGTRACE(_T("--------------------------------------------------"));

		MyThread my1, my2;

		my1.Start();
		Sleep(10);
		my2.Start();
		if (!my2.Start()) { DBGTRACE(_T("failed.")); }

		while (!my1.IsEnd() || !my2.IsEnd()) {
			Sleep(10);
		}

		//my1.Start();
		//while (!my1.IsEnd()) {
		//	Sleep(10);
		//}
	}
}

// --------------------------------------------------------------------------------------------- //
int main()
{
	__Cast::Test();

	__Delegate::Test();

	__Variant::Test();

	__Properties::Test();

	__Uncopyable::Test();

#if 0
	__MemoryPool::Test();
#endif

	__Thread::Test();

	DBGTRACE(_T("--------------------------------------------------"));
	DBGTRACE(_T("finish."));
	getchar();	// Pause

	return 0;
}

