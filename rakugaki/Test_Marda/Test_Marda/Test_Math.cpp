#include "stdafx.h"

using namespace Marda;

bool Test_Math_Vector2()
{
	Vector2 v1(2.f, 2.f);
	Vector2 v2(3.f, 4.f);

	Vector2 vRet = v1 + v2;

	if (vRet.x != v1.x + v2.x || vRet.y != v1.y + v2.y)
	{
		TTRACE(_T("operator + # test failed!"));
		return false;
	}
		
	TTRACE(_T("OK."));

	return true;
}

bool Test_Math()
{
	if (!Test_Math_Vector2()) return false;

	return true;
}
