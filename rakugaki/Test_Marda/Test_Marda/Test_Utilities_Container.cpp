#include "stdafx.h"

using namespace Marda;

namespace Utilities_Container_AMap
{
	typedef std::map<int, int> Map;
	typedef amap<int, int, 32> AMap;

	bool Test_lower_bound(AMap& amap, Map& map, int key)
	{
		Map::iterator answer = map.lower_bound(key);
		AMap::iterator found = amap.lower_bound(key);
		
		if (answer == map.end())
		{
			return found == amap.end();
		}
		return found->first == answer->first;
	}

	bool Test_upper_bound(AMap& amap, Map& map, int key)
	{
		Map::iterator answer = map.upper_bound(key);
		AMap::iterator found = amap.upper_bound(key);
		
		if (answer == map.end())
		{
			return found == amap.end();
		}
		return found->first == answer->first;
	}

	bool Test_find(AMap& amap, Map& map, int key)
	{
		Map::iterator answer = map.find(key);
		AMap::iterator found = amap.find(key);

		if (answer == map.end())
		{
			return found == amap.end();
		}

		return answer->first == found->first;
	}

	bool Test_insert(AMap& amap, Map& map, int key, int value)
	{
		std::pair<Map::iterator, bool> answer = map.insert(std::make_pair(key, value));

		if (amap.ELEMENTS_MAX <= amap.size())
		{
			// 例外がthrowされるはず
			try
			{
				std::pair<AMap::iterator, bool> ret = amap.insert(std::make_pair(key, value));
			} 
			catch (elements_max_exception)
			{
				return true;	// テストOK
			}
		}
		else
		{
			std::pair<AMap::iterator, bool> ret = amap.insert(std::make_pair(key, value));

			if (answer.first->first != ret.first->first)
			{
				return false;
			}
			if (answer.second != ret.second)
			{
				return false;
			}

			if (map.size() != amap.size())
			{
				return false;
			}

			Map::iterator i = map.begin();
			AMap::iterator k = amap.begin();
			for (; i != map.end(); ++i, ++k)
			{
				if (i->first != k->first || i->second != k->second)
				{
					return false;
				}
			}
		}

		return true;
	}
	
	bool Test()
	{
		// pattern 1
		{
			Map map;
			map.insert(std::make_pair(0, 0));
			map.insert(std::make_pair(1, 0));
			map.insert(std::make_pair(2, 0));
			map.insert(std::make_pair(4, 0));
			map.insert(std::make_pair(9, 0));

			AMap amap;
			amap._base[0] = std::make_pair(0, 0);	++amap._numElements;
			amap._base[1] = std::make_pair(1, 0);	++amap._numElements;
			amap._base[2] = std::make_pair(2, 0);	++amap._numElements;
			amap._base[3] = std::make_pair(4, 0);	++amap._numElements;
			amap._base[4] = std::make_pair(9, 0);	++amap._numElements;

			// begin test
			AMap::iterator begin = amap.begin();
			if (begin->first != 0)
			{
				TTRACE(_T("amap::begin# test failed! begin->first=%d"), begin->first);
				return false;
			}

			// end test
			AMap::iterator end = amap.end();
			if (end != begin + amap.size())
			{
				TTRACE(_T("amap::end# test failed!"));
				return false;
			}

			if (!Test_lower_bound(amap, map, 1))
			{
				TTRACE(_T("amap::lower_bound# test failed!"));
				return false;
			}
			if (!Test_lower_bound(amap, map, 5))
			{
				TTRACE(_T("amap::lower_bound# test failed!"));
				return false;
			}
			
			if (!Test_lower_bound(amap, map, 99))
			{
				TTRACE(_T("amap::lower_bound# test failed!"));
				return false;
			}

			if (!Test_upper_bound(amap, map, 1))
			{
				TTRACE(_T("amap::upper_bound# test failed!"));
				return false;
			}
			
			if (!Test_upper_bound(amap, map, 5))
			{
				TTRACE(_T("amap::upper_bound# test failed!"));
				return false;
			}
			
			if (!Test_upper_bound(amap, map, 99))
			{
				TTRACE(_T("amap::upper_bound# test failed!"));
				return false;
			}

			if (!Test_find(amap, map, 1))
			{
				TTRACE(_T("amap::find# test failed!"));
				return false;
			}

			if (!Test_insert(amap, map, 1, 1))
			{
				TTRACE(_T("amap::insert# test failed!"));
				return false;
			}
		}

		// pattern 2
		{
			Map map;
			map.insert(std::make_pair(0, 0));
			map.insert(std::make_pair(3, 0));
			map.insert(std::make_pair(6, 0));
			map.insert(std::make_pair(9, 0));
			map.insert(std::make_pair(10, 0));
			map.insert(std::make_pair(12, 0));

			AMap amap;
			amap._base[0] = std::make_pair(0, 0);	++amap._numElements;
			amap._base[1] = std::make_pair(3, 0);	++amap._numElements;
			amap._base[2] = std::make_pair(6, 0);	++amap._numElements;
			amap._base[3] = std::make_pair(9, 0);	++amap._numElements;
			amap._base[4] = std::make_pair(10, 0);	++amap._numElements;
			amap._base[5] = std::make_pair(12, 0);	++amap._numElements;

			if (!Test_lower_bound(amap, map, 4))
			{
				TTRACE(_T("amap::lower_bound# test failed!"));
				return false;
			}

			if (!Test_lower_bound(amap, map, 1))
			{
				TTRACE(_T("amap::lower_bound# test failed!"));
				return false;
			}

			if (!Test_find(amap, map, 4))
			{
				TTRACE(_T("amap::find# test failed!"));
				return false;
			}

			if (!Test_insert(amap, map, 1, 1))
			{
				TTRACE(_T("amap::insert# test failed!"));
				return false;
			}
		}

		return true;
	}
}	// end namespace Utilities_Container_AMap

bool Test_Utilities_Container()
{
	TTRACE(_T("====================================================="));

	if (!Utilities_Container_AMap::Test()) return false;

	return true;
}


