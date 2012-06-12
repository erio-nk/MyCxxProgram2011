/*!
	@file	Graphics/D3DXHelpers.cpp

	@author	E.Nakayama
*/

#include "Marda/Graphics/D3DXHelpers.h"

using namespace Marda;

D3DXVECTOR3 Marda::D3DXHelpers::GetEulerAnglesZXY(const D3DXMATRIX& m)
{
	D3DXVECTOR3 angles;

	/*
		�Q�lURL : http://d.hatena.ne.jp/It_lives_vainly/20070829/1188384519
			_1				, _2	, _3			, _4
		1_ |CzCy + SzSxSy	, SzCx	, -CzSy + SzSxCy, 0 |
		2_ |-SzCy + CzSxSy	, CzCx	, SzSy + CzSxCy	, 0 |
		3_ |CxSy			, -Sx	, CxCy			, 0 |
		4_ |0				, 0		, 0				, 1 |
	*/
	angles.x = asin(-m._32);			// ��x = asin(-(-Sx)) = asin(-m32)
	float Cx = cos(angles.x);
	if (!Equals(Cx, 0.f))
	{
		angles.y = atan2(m._31/Cx, m._33/Cx);		// ��y = atan2(CxSy, CxCy) = atan2(m31/Cx, m33/Cx)
		angles.z = atan2(m._12/Cx, m._22/Cx);		// ��z = atan2(SzCx, CzCx) = atan2(m12/Cx, m22/Cx)
	}
	else
	{
		/*
			Cx ���[���ׁ̈A��y�ƃ�z�����߂�ۂɃ[�����Z���������Ă��܂��A�l�����߂��Ȃ��B���̎��A

			Sx == 1 �Ȃ火								Sx == -1 �Ȃ火
				_1			, _2, _3			, _4		_1			, _2, _3			, _4
			1_ |CzCy + SzSy	, 0	, -CzSy + SzCy	, 0 |	1_ |CzCy - SzSy	, 0	, -CzSy - SzCy	, 0 |
			2_ |-SzCy + CzSy, 0	, SzSy + CzCy	, 0 |	2_ |-SzCy - CzSy, 0	, SzSy - CzCy	, 0 |
			3_ |0			, -1, 0				, 0 |	3_ |0			, 1	, 0				, 0 |
			4_ |0			, 0	, 0				, 1 |	4_ |0			, 0	, 0				, 1 |

			�ƂȂ邪�A���̍s��ɂȂ��y, ��z�͖����ɑ��݂���ׁA��y�� 0 �Ɖ��肷��ƁA

			Sx == 1 �Ȃ火			Sx == -1 �Ȃ火
				_1	, _2, _3, _4		_1	, _2, _3	, _4
			1_ |Cz	, 0	, Sz, 0 |	1_ |Cz	, 0	, -Sz	, 0 |
			2_ |-Sz	, 0	, Cz, 0 |	2_ |-Sz	, 0	, -Cz	, 0 |
			3_ |0	, -1, 0	, 0 |	3_ |0	, 1	, 0		, 0 |
			4_ |0	, 0	, 0	, 1 |	4_ |0	, 0	, 0		, 1 |
			�ƂȂ�ׁA
			��z = atan2(Sz, Cz) = atan2(-m21, m11)
		*/
		angles.y = 0.f;
		angles.z = atan2(-m._21, m._11);
	}
	return angles;
}