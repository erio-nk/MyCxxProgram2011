#include <string>

//namespace Hoge
//{
//	string s0;	// NG, このコメントブロックを外すとs1,2も何故かNGになる
//}

namespace Hoge
{
	using namespace std;

	string s1;	// OK
}

namespace Hoge
{
	string s2;	// OK
}
