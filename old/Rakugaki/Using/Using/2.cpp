#include <string>

namespace Hoge
{
	// Hoge名前空間のみで有効になる
	using namespace std;

	string s;	// OK
}

class S
{
	string _s;	// NG
};

void Func()
{
	string s;	// NG
}
