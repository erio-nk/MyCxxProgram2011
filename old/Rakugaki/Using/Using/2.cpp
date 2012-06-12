#include <string>

namespace Hoge
{
	// Hoge–¼‘O‹óŠÔ‚Ì‚Ý‚Å—LŒø‚É‚È‚é
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
