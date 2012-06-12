// shared_ptr.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

#include <iostream>

#include "boost/shared_ptr.hpp"
#include "boost/intrusive_ptr.hpp"

#include "boost/date_time/gregorian/gregorian.hpp"

struct Hoge
{
	Hoge()
	{
		boost::gregorian::date today = boost::gregorian::day_clock::local_day();

		std::cout << __FUNCTION__ << ":ctor " << today << std::endl;
	}

	~Hoge()
	{
		std::cout << __FUNCTION__ << ":dtor" << std::endl;
	}
};


struct Fuga
{
	int _refCount;
	
	Fuga()
	{
		_refCount = 1;
		std::cout << __FUNCTION__ << ":ctor" << std::endl;
	}
	
	~Fuga()
	{
		std::cout << __FUNCTION__ << ":dtor" << std::endl;
	}
	
	void AddRef()
	{
		++_refCount;
	}
	
	void Release()
	{
		if (--_refCount <= 0)
		{
			delete this;
		}
	}
};

void intrusive_ptr_add_ref(Fuga* ptr)
{
	ptr->AddRef();
}

void intrusive_ptr_release(Fuga* ptr)
{
	ptr->Release();
}


int _tmain(int argc, _TCHAR* argv[])
{
	{
		boost::shared_ptr<Hoge> pHoge(new Hoge());	
		boost::intrusive_ptr<Fuga> pFuga(new Fuga(), false);

	}
	
	std::cout << "end" << std::endl;

	return 0;
}

