#include <string>

namespace
{
	// このファイル全てのスコープで有効になる
	using namespace std;

	string s;	// OK
}

class S
{
	string _s;	// OK
};

void Func()
{
	string s;	// OK
}
