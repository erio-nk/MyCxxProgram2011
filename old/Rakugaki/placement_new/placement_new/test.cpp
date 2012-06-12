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
	// �����������m��
	printf("# allocating...\n");
	Hoge* pHoge = reinterpret_cast<Hoge*>(operator new (sizeof(Moke)));

	// ��Ƀ����o�ϐ������������Ă݂�
	pHoge->hoge = 1;

	Moke* pMoke = static_cast<Moke*>(pHoge);
	pMoke->moke = 2;

	// �z�unew
	printf("# constructing...\n");
	pHoge = new (pHoge) Moke(0);	// placement new

	// delete�i�f�X�g���N�^�Ă�� operator delete(sizeof)����Ȃ��Ă����H�j
	printf("# destructing...\n");
	delete pHoge;

	return 0;
}
