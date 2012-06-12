#include <iostream>

#include "Marda.h"
#include "Marda/STLHelpers.h"

using namespace Marda;

struct Hoge
{
	int _val;
	Hoge(int val) : _val(val) {}
	~Hoge()
	{
		std::cout << "~Hoge [" << _val << "]" << std::endl;
	}
};

void test_stlhelpers()
{
	std::map<std::string, int> mymap;
	mymap["hoge"] = 0;
	mymap["fuga"] = 1;
	mymap["piyo"] = 2;

	std::cout << has_key(mymap, "hoge") << std::endl;

#if 0
	std::vector<Hoge*, ptr_container_allocator<Hoge*> > myhoges;
	myhoges.push_back(new Hoge(0));
	myhoges.push_back(new Hoge(1));
	myhoges.resize(0);
#endif
	//std::vector<Hoge*> myhoges;
	//myhoges.push_back(new Hoge(0));
	//myhoges.push_back(new Hoge(1));
	//ptr_container_destroy(myhoges);
}
