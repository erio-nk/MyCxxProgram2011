#include <map>
#include <string>
#include <stdio.h>

int main()
{
	typedef std::multimap<std::string, std::string> Map;
	Map map;

	map.insert(std::make_pair("Hoge", "hoge1"));
	map.insert(std::make_pair("Hoge", "hoge2"));
	map.insert(std::make_pair("Hoge", "hoge3"));

	map.insert(std::make_pair("Fuga", "fuga1"));
	map.insert(std::make_pair("Fuga", "fuga2"));

	// all elements
	printf("# all elements\n");
	Map::iterator i = map.begin();
	for (; i != map.end(); ++i) {
		printf("{%s, %s}\n", i->first.c_str(), i->second.c_str());
	}
	
	// 'Hoge' elements
	printf("# 'Hoge' elements\n");
	Map::iterator b = map.lower_bound("Hoge");
	Map::iterator e = map.upper_bound("Hoge");
	i = b;
	for (; i != e; ++i) {
		printf("{%s, %s}\n", i->first.c_str(), i->second.c_str());
	}

	// 'Fuga' elements
	printf("# 'Fuga' elements\n");
	int count = map.count("Fuga");
	i = map.find("Fuga");
	for (int k = 0; k < count; ++i, ++k) {
		printf("{%s, %s}\n", i->first.c_str(), i->second.c_str());
	}

	return 0;
}