#include <stdio.h>
#include <new>



struct Hoge
{
	int hoge;
	Hoge()
	{
		printf("Hoge.ctor; hoge=%d\n", hoge);
	}
	virtual ~Hoge()
	{
		printf("Hoge.dtor\n");
	}
};

struct Moke : public Hoge
{
	int moke;
	Moke(int m)
	{
		printf("Moke.ctor(%d); moke=%d\n", m, moke);
	}
	~Moke()
	{
		printf("Moke.dtor\n");
	}
};

int main()
{
	// メモリだけ確保
	printf("# allocating...\n");
	Hoge* pHoge = reinterpret_cast<Hoge*>(operator new (sizeof(Moke)));

	// 先にメンバ変数を初期化してみる
	pHoge->hoge = 1;

	Moke* pMoke = static_cast<Moke*>(pHoge);
	pMoke->moke = 2;

	// 配置new
	printf("# constructing...\n");
	pHoge = new (pHoge) Moke(0);	// placement new

	// delete（デストラクタ呼んで operator delete(sizeof)じゃなくておｋ？）
	printf("# destructing...\n");
	delete pHoge;

	return 0;
}
