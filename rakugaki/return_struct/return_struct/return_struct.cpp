// return_struct.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

struct Vector2
{
	int _x;
	int _y;
};

struct Vector3
{
	int _x;
	int _y;
	int _z;
};

int Add(int lhs, int rhs)
{
	int ret;
	ret = lhs + rhs;
	return ret;
}

Vector2 AddVector2(const Vector2& lhs, const Vector2& rhs)
{
	Vector2 ret;
	ret._x = lhs._x + rhs._x;
	ret._y = lhs._y + rhs._y;
	return ret;
}

Vector3 AddVector3(const Vector3& lhs, const Vector3& rhs)
{
	Vector3 ret;
	ret._x = lhs._x + rhs._x;
	ret._y = lhs._y + rhs._y;
	ret._z = lhs._z + rhs._z;
	return ret;
}

int _tmain(int argc, _TCHAR* argv[])
{
	// return int
	{
		int i1;
		int i2;
		i1 = 1;
		i2 = 2;

		int ret;
		ret = Add(i1, i2);

		printf("%d\n", ret);
	}

	// return Vector2
	{
		Vector2 v1;
		v1._x = 1;
		v1._y = 2;
		Vector2 v2;
		v2._x = 3;
		v2._y = 4;

		Vector2 ret;
		ret = AddVector2(v1, v2);

		printf("(%d, %d)\n", ret._x, ret._y);
	}

	// return Vector3
	{
		Vector3 v1;
		v1._x = 1;
		v1._y = 2;
		v1._y = 3;
		Vector3 v2;
		v2._x = 4;
		v2._y = 5;
		v2._y = 6;

		Vector3 ret;
		ret = AddVector3(v1, v2);

		printf("(%d, %d)\n", ret._x, ret._y);
	}

	return 0;
}

