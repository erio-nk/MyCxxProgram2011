#include <string>

namespace Hoge
{
	// Hoge���O��Ԃ݂̂ŗL���ɂȂ�
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
