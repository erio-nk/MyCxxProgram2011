#include <string>

namespace
{
	// ���̃t�@�C���S�ẴX�R�[�v�ŗL���ɂȂ�
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
