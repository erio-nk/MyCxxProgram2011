#include <string>

//namespace Hoge
//{
//	string s0;	// NG, ���̃R�����g�u���b�N���O����s1,2�����̂�NG�ɂȂ�
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
