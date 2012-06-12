#include <stdio.h>

struct Vector2
{
	int _x;
	int _y;

	void Set(int x, int y)
	{
		_x = x;
		_y = y;
	}
};

int Add(int a, int b)
{
	return a + b;
}

Vector2 Add(Vector2 a, Vector2 b)
{
	Vector2 ret;
	ret._x = a._x + b._x;
	ret._y = a._y + b._y;
	return ret;
}

int main()
{
	printf("Hello, Disassembler world! %d, %u\n", 0, 1);

	int ret = Add(2, 4);
	printf("%d\n", ret);

	Vector2 v1, v2;
	v1.Set(1, 2);
	v2.Set(10, 20);
	Vector2 vRet;
	vRet = Add(v1, v2);
	printf("%d, %d\n", vRet._x, vRet._y);

	return 0;
}
