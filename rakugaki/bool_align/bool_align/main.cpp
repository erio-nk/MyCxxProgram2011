#include <stdio.h>

struct BoolAlign
{
	bool _bool0;
	int _int0;
	
	bool _bool1;
	bool _bool2;
	int _int1;

	bool _bool3;
	bool _bool4;
	bool _bool5;
	int _int2;

	bool _bool6;
	bool _bool7;
	short _short0;

	bool _bool8;
	short _short1;
	bool _bool9;

	int _int3;
};

int main()
{
	printf("sizeof(bool)=%u\n", sizeof(bool));
	printf("sizeof(BoolAlign)=%u\n", sizeof(BoolAlign));

	BoolAlign a;

	a._bool0 = true;
	a._int0 = -1;

	a._bool1 = true;
	a._bool2 = true;
	a._int1 = -1;

	a._bool3 = true;
	a._bool4 = true;
	a._bool5 = true;
	a._int2 = -1;

	a._bool6 = true;
	a._bool7 = true;
	a._short0 = -1;

	a._bool8 = true;
	a._short1 = -1;
	a._bool9 = true;

	a._int3 = -1;

	return 0;
}
