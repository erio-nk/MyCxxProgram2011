#include <stdio.h>
#include "Type.h"

struct A {};
struct ADash : public A {};
struct B {};

int _tmain()
{
	_tprintf(_T("Int to Double = %d\n"), Conversion<int, double>::exists);
	_tprintf(_T("Int to Int* = %d\n"), Conversion<int, int*>::exists);
	_tprintf(_T("Int equals Int = %d\n"), Conversion<int, int>::equals);

	_tprintf(_T("ADash* to A* = %d\n"), UpCast<ADash, A>::exists);
	_tprintf(_T("A* to B* = %d\n"), UpCast<A, B>::exists);

	return 0;
}

