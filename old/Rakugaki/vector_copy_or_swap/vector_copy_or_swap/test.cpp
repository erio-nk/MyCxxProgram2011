#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#if defined(_WIN32)
// for Windows
#include <windows.h>
#else
// for GCC
#include <sys/time.h>
#endif

#if defined(_WIN32)
//// for Windows
LARGE_INTEGER freq;
LARGE_INTEGER cnt;

inline double GetCurrentTimeMicros()
{
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&cnt));
	return static_cast<double>(cnt.QuadPart * 1000000L / freq.QuadPart) * 0.001f * 0.001f;
		// マイクロ秒単位に直して(1000000倍して周波数で割る)、
		// ミリ秒単位に直して、秒単位にに直して取得
}

#else
//// for GCC
timeval t;
inline double GetCurrentTimeMicros()
{
	gettimeofday(&t, 0);
	return static_cast<double>(t.tv_sec) + static_cast<double>(t.tv_usec)/1000000.0;
}

#endif


//// 文字列リストをランダム生成
typedef std::vector<std::string> Strings;
int MakeString(Strings& strings)
{
	int num = rand() % 1024;	// 生成文字列数
	int chars = 0;				// 合計文字数
	for (int i = 0; i < num; ++i) {
		int len = rand() % 2056;	// 文字数
		chars += len;
		strings.push_back(std::string());
		strings.back().resize(len);
		for (int k = 0; k < len; ++k) {
			strings.back()[k] = (rand() % 23) + 'a';	// a-zで適当に生成
		}
	}
	return chars;
}

//// 文字列リストの同値性を検証する
bool EqualsStrings(const Strings& ss1, const Strings& ss2)
{
	if (ss1.size() != ss2.size()) return false;

	Strings::const_iterator i1 = ss1.begin();
	Strings::const_iterator i2 = ss2.begin();
	for (; i1 != ss1.end(); ++i1, ++i2) {
		if (i1->compare(*i2) != 0) {
			return false;
		}
	}
	return true;
}

//// main
int main()
{
	#if defined(_WIN32)
	QueryPerformanceFrequency(&freq);
	#endif

	int testNum = 1000;	// 試行回数

	double copyTotal = 0.0;
	double swapTotal = 0.0;
	double cb, ce, ct;
	double sb, se, st;


	for (int i = 0; i < testNum; ++i) {
		// 文字列をランダム生成する
		Strings strings;
		int chars = MakeString(strings);

		// 生成した文字列のコピーを取っておく
		Strings forCopySrc, forSwapSrc;
		forCopySrc = strings;
		forSwapSrc = strings;

		Strings forCopyDst, forSwapDst;

		printf("Test %d: strs=%u, chars=%u", i, strings.size(), chars);

		// コピーの計測
		cb = GetCurrentTimeMicros();
		forCopyDst = forCopySrc;
		ce = GetCurrentTimeMicros();
		ct = ce - cb;
		copyTotal += ct;

		printf(" copy=%.15fsec. %s", ct, EqualsStrings(strings, forCopyDst) ? "[o]" : "[x]");

		// swapの計測
		sb = GetCurrentTimeMicros();
		forSwapDst.swap(forSwapSrc);
		se = GetCurrentTimeMicros();
		st = se - sb;
		swapTotal += st;

		if (ct > st)  printf(" >");
		if (ct == st) printf(" = ");
		if (ct < st)  printf(" < ");
		
		printf(" swap=%.15fsec. %s", st, EqualsStrings(strings, forSwapDst) ? "[o]" : "[x]");

		printf("\n");
	}

	printf("Copy> Total:%f\n", copyTotal);
	printf("Swap> Total:%f (%f%%)\n", swapTotal, 100.0-swapTotal/copyTotal*100.0);

	getchar();

	return 0;
}

