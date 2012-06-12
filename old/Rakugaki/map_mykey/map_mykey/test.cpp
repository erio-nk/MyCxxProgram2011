#include <stdio.h>
#include <map>


struct MyKey
{
	int _value1;
	int _value2;
	MyKey(int value1, int value2) : _value1(value1), _value2(value2) {}

	bool operator < (const MyKey& x) const
	{
		if (_value1 != x._value1)
			return _value1 < x._value1;
		return _value2 < x._value2;
	}
};

typedef std::map<MyKey, int> MyMap;

int main()
{
	MyMap map;

	map.insert(MyMap::value_type(MyKey(2, 2), 0));
	
	MyMap::const_iterator i = map.find(MyKey(2, 3));
	//map.insert(MyKey(2, 2), 0);

	return 0;
}
