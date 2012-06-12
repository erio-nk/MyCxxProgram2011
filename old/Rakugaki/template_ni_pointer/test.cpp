/*!
	template_ni_pointer
 */

#include <iostream>

// 文字列 (char*)
char hoge[] = "hoge";
static char fuga[] = "fuga";
const char piyo[] = "piyo";

// 関数テンプレート
template <char* String> //<const char* String> にしてもPrint<hoge>以外はダメ。
void Print()
{
	std::cout << String << std::endl;
}

// main関数
int main()
{
	Print<hoge>();		// OK. 非constのグローバル変数のポインタは渡せる
	//Print<fuga>();	// NG. 非constでもstatic変数のポインタは渡せない
	//Print<piyo>();	// NG. constにするとグローバル変数でも渡せない
	//Print<"moke">();	// NG. 文字列リテラルも渡せない。

	return 0;
}