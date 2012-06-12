#include <stdio.h>

#include <string>

#include <map>
#include <unordered_map>

typedef std::map<std::string, int> Map;
//typedef std::tr1::unordered_map<std::string, int> Map;

int main()
{
	Map map;

	// operator []�͗v�f��������Α}�������
	map["hoge"] = 0;
	map["fuga"] = 1;

	printf("hoge=%d\n", map["hoge"]);
	printf("fuga=%d\n", map["fuga"]);

	// insert�͏d������v�f����������A���̃C�e���[�^���������y�A��Ԃ�
	Map::_Pairib pair = map.insert(Map::value_type("hoge", -1));
	printf("key=%s, val=%d\n", pair.first->first.c_str(), pair.first->second);
	
	pair = map.insert(Map::value_type("piyo", 3));
	printf("key=%s, val=%d\n", pair.first->first.c_str(), pair.first->second);

	return 0;
}
