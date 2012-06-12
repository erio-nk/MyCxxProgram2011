#include <iostream>

#include "Marda.h"
#include "Marda/Util.h"

using namespace Marda;
	
namespace
{
	typedef StaticList<
			StaticValue<int, 0>,
			StaticList<
				StaticValue<int, 1>,
				StaticList<
					StaticValue<int, 2>,
					NullType
					>
				>
			> MyStaticList;
	typedef StaticList< StaticValue<int, 10>, NullType > HogeList;

	template <class T>
	struct Printer
	{
		void operator ()()
		{
			std::cout << "##" << T::Value << std::endl;
		}
	};

}

void test_staticlist()
{

	std::cout << MyStaticList::Element::Value << std::endl;
	std::cout << MyStaticList::Next::Element::Value << std::endl;
	std::cout << MyStaticList::Next::Next::Element::Value << std::endl;

	//std::cout << "length=" << StaticList_Length<MyStaticList>::Result << std::endl;
	std::cout << "length=" << MyStaticList::Length << std::endl;
	std::cout << "length=" << HogeList::Length << std::endl;
	
	std::cout << MyStaticList::At<0>::Result::Value << std::endl;
	std::cout << HogeList::At<0>::Result::Value << std::endl;
	std::cout << MyStaticList::At<1>::Result::Value << std::endl;
	std::cout << MyStaticList::At<2>::Result::Value << std::endl;

	MyStaticList::ForEach<Printer>();

	//std::cout << StaticList_At<MyStaticList, 0>::Result::Value << std::endl;
	//std::cout << StaticList_At<MyStaticList, 1>::Result::Value << std::endl;
	//std::cout << StaticList_At<MyStaticList, 2>::Result::Value << std::endl;

	//typedef StaticValue<int, 0> MyStaticInt;
	//std::cout << MyStaticInt::Value << std::endl;

	//typedef StaticValue<float, 1.0f> MyStaticFloat;
	//std::cout << MyStaticFloat::Value << std::endl;
}
