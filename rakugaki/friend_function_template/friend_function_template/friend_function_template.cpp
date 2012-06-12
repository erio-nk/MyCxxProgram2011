// friend_function_template.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

class Hoge
{
public:
	// フレンド関数テンプレートの指定。
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

