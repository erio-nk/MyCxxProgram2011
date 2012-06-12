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
		参考URL : http://d.hatena.ne.jp/It_lives_vainly/20070829/1188384519
			_1				, _2	, _3			, _4
		1_ |CzCy + SzSxSy	, SzCx	, -CzSy + SzSxCy, 0 |
		2_ |-SzCy + CzSxSy	, CzCx	, SzSy + CzSxCy	, 0 |
		3_ |CxSy			, -Sx	, CxCy			, 0 |
		4_ |0				, 0		, 0				, 1 |
	*/
	angles.x = asin(-m._32);			// θx = asin(-(-Sx)) = asin(-m32)
	float Cx = cos(angles.x);
	if (!Equals(Cx, 0.f))
	{
		angles.y = atan2(m._31/Cx, m._33/Cx);		// θy = atan2(CxSy, CxCy) = atan2(m31/Cx, m33/Cx)
		angles.z = atan2(m._12/Cx, m._22/Cx);		// θz = atan2(SzCx, CzCx) = atan2(m12/Cx, m22/Cx)
	}
	else
	{
		/*
			Cx がゼロの為、θyとθzを求める際にゼロ除算が発生してしまい、値が求められない。この時、

			Sx == 1 なら↓								Sx == -1 なら↓
				_1			, _2, _3			, _4		_1			, _2, _3			, _4
			1_ |CzCy + SzSy	, 0	, -CzSy + SzCy	, 0 |	1_ |CzCy - SzSy	, 0	, -CzSy - SzCy	, 0 |
			2_ |-SzCy + CzSy, 0	, SzSy + CzCy	, 0 |	2_ |-SzCy - CzSy, 0	, SzSy - CzCy	, 0 |
			3_ |0			, -1, 0				, 0 |	3_ |0			, 1	, 0				, 0 |
			4_ |0			, 0	, 0				, 1 |	4_ |0			, 0	, 0				, 1 |

			となるが、この行列になるθy, θzは無限に存在する為、θyを 0 と仮定すると、

			Sx == 1 なら↓			Sx == -1 なら↓
				_1	, _2, _3, _4		_1	, _2, _3	, _4
			1_ |Cz	, 0	, Sz, 0 |	1_ |Cz	, 0	, -Sz	, 0 |
			2_ |-Sz	, 0	, Cz, 0 |	2_ |-Sz	, 0	, -Cz	, 0 |
			3_ |0	, -1, 0	, 0 |	3_ |0	, 1	, 0		, 0 |
			4_ |0	, 0	, 0	, 1 |	4_ |0	, 0	, 0		, 1 |
			となる為、
			θz = atan2(Sz, Cz) = atan2(-m21, m11)
		*/
		angles.y = 0.f;
		angles.z = atan2(-m._21, m._11);
	}
	return angles;
}