// numeric_cast.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

#include <iostream>
#include <limits>

#include "boost/cast.hpp"

template <typename Target, typename Source>
inline Target numeric_cast_ex(Source source)
{
	try
	{
		return boost::numeric_cast<Target>(source);
	}
	catch (boost::numeric::negative_overflow e)
	{
		std::cout << e.what() << std::endl;
		return std::numeric_limits<Target>::min();
	}
	catch (boost::numeric::positive_overflow e)
	{
		std::cout << e.what() << std::endl;
		return std::numeric_limits<Target>::max();
	}
	catch (boost::bad_numeric_cast e)
	{
		std::cout << e.what() << std::endl;
		return 0;
	}
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	{
		int i = 600000;
		std::cout << i << " to " << numeric_cast_ex<short>(i) << std::endl;
	}

	{
		int i = -600000;
		std::cout << i << " to " << numeric_cast_ex<short>(i) << std::endl;
	}

	return 0;
}

