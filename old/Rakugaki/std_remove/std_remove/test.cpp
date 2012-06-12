#include <stdio.h>
#include <map>
#include <algorithm>

typedef std::map<int, int> Map;

	struct Pred
	{
		int _value;
		Pred(int value) : _value(value) {}
		bool operator () (const Map::value_type& v)
		{
			if (v.second == _value) return true;
			return false;
		}
	};

int main()
{
	Map map;

	map[100] = 0;
	map[200] = 1;
	map[300] = 2;

	std::remove_if(map.begin(), map.end(), Pred(1));

	for (Map::iterator i = map.begin();
		i != map.end();
		++i)
	{
		printf("%s = %d\n", i->first, i->second);
	}

	return 0;
}
