

#include "ptr_container.h"

struct MyInt
{
	int _value;
	MyInt(int value) : _value(value)
	{
	}
	~MyInt()
	{
		printf("MyInt(%d) destroyed.\n", _value);
	}
};

//typedef ptr_container::list<MyInt*> MyList;
typedef ptr_container::list<MyInt*, std::vector, ptr_container::Deleter/*, std::allocator<MyInt*>*/> MyList;

int main()
{
	MyList myList;

	myList.push_back(new MyInt(0));
	myList.push_back(new MyInt(1));
	myList.push_back(new MyInt(2));
	myList.push_back(new MyInt(3));

	myList.pop_back();
	//myList.pop_front();

	return 0;
}

