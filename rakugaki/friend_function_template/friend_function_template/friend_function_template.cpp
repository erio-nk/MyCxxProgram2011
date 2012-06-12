// friend_function_template.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"

class Hoge
{
public:
	// �t�����h�֐��e���v���[�g�̎w��B
	template <class T> friend void CallHogeEcho(Hoge&, T);

private:
	void Echo(int x)
	{
		printf("Hoge: %d\n", x);
	}
	void Echo(const char* x)
	{
		printf("Hoge: %s\n", x);
	}
};

template <class T>
void CallHogeEcho(Hoge& hoge, T x)
{
	hoge.Echo(x);
}

int _tmain(int argc, _TCHAR* argv[])
{
	Hoge hoge;

	CallHogeEcho(hoge, 100);
	CallHogeEcho(hoge, "fuga");

	return 0;
}

