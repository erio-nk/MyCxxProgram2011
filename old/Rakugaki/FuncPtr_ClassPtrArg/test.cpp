#include <stdio.h>

// 関数ポインタとクラス関係のてすと
//
// 引数がクラスのポインタ型で、かつベースクラスの
// ポインタであるとき(SayHoge)、サブクラスのポインタを引数に
// とる関数(SayFuga,SayMoke)のポインタをSayHogeの型にキャストして、
// 更に引数はそれぞれ目的のインスタンスへのポインタを与えて、
// それぞれの処理がきちんと呼び出されるかどうかのチェック。

// 関数ポインタが処理コードの先頭アドレスで、
// 引数が単なる連続したメモリ(スタック)とすれば、
// 引数データの合計サイズ(int,intなら8バイト)が
// 合致していれば、型をキャストした関数ポインタで
// あっても(引数のサイズが合っているので)呼び出しは
// 問題ないと思われる。
//
// void num(int* p) {...}
//
// void (*pNum)(int*) = num;	// OK
// void (*pNum2)(long*) = num;	// サイズがあっているのでこれも多分OK
//
// 上記の関数ポインタpNum,pNum2の呼び出し結果は、
// 結局のところnum()の処理結果となると思われる。

// SayHoge,SayFuga,SayMokeの引数は、
// どれもポインタ型4バイトのデータなので、
// SayFuga,SayMokeはSayHogeの型(void (*)(Hoge*))に
// キャストしても問題ないと思われる。
// 勿論キャストしたポインタにそれぞれFuga*と
// Moke*を渡しても、ベースクラスがHoge*なので問題ない。
// 呼出し後はそれぞれSayFuga,SayMokeが起動されるが、
// この時Hoge*として渡したポインタはそれぞれFuga*,
// Moke*に変換された状態で関数が起動する(と思われる)ので、
// SayFuga,SayMokeは最終的に正しい処理と引数として
// 呼び出されると思われる。
//
// つまり、
// void (*p)(Hoge*) = SayHoge; (*p)(&hoge);	// OK
// void (*p)(Hoge*) = SayHoge; (*p)(&fuga);	// OK HogeのサブクラスなのでOK
// void (*p)(Hoge*) = SayHoge; (*p)(&moke);	// OK HogeのサブクラスなのでOK
//
// void (*p)(Hoge*) = SayFuga; (*p)(&hoge);	// NG 継承の関係でNG？
// void (*p)(Hoge*) = SayFuga; (*p)(&fuga);	// OK
// void (*p)(Hoge*) = SayFuga; (*p)(&moke);	// OK FugaのサブクラスなのでOK
//
// void (*p)(Hoge*) = SayMoke; (*p)(&hoge);	// NG 継承の関係でNG？
// void (*p)(Hoge*) = SayMoke; (*p)(&fuga);	// NG 継承の関係でNG？
// void (*p)(Hoge*) = SayMoke; (*p)(&moke);	// OK


class Hoge
{
	int x;
public:
	Hoge() {x=0;}
	virtual void Say() {
		printf("say hoge.\n");
	}
};

class Fuga : public Hoge
{
	int y;
public:
	Fuga() {y=1;}
	void Say() {
		printf("say fuga.\n");
	}
};

class Moke : public Fuga
{
public:
	Moke() {}
	void Say() {
		printf("say moke.\n");
	}
};


void SayHoge (Hoge* p) {
	p->Say();
}

void SayFuga(Fuga* p) {
	p->Say();
}

void SayMoke(Moke* p) {
	p->Say();
}


int main()
{
	Hoge hoge;
	Fuga fuga;
	Moke moke;

	void (*pSay)(Hoge*);

	pSay = SayHoge;
	(*pSay)(&hoge);
	
	(*pSay)(&fuga);
	
	(*pSay)(&moke);
	

	pSay = (void (*)(Hoge*))SayFuga;
	(*pSay)(&fuga);
	
	(*pSay)(&moke);
	
	pSay = (void (*)(Hoge*))SayMoke;
	(*pSay)(&moke);
}

