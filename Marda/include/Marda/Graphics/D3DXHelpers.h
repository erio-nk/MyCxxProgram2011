/*!
	@file	Graphics/D3DXHelpers.h
	
			主に D3DX の算術計算用の各構造体に関するヘルパ関数。
	
	@author	E.Nakayama
*/
#pragma once

#include "../Math.h"

namespace Marda
{
	namespace D3DXHelpers
	{
		inline D3DXVECTOR2 ToD3DXVECTOR2(const Vector2& v)
		{
			return D3DXVECTOR2(v.x, v.y);
		}

		inline D3DXVECTOR3 ToD3DXVECTOR3(const Vector3& v)
		{
			return D3DXVECTOR3(v.x, v.y, v.z);
		}
		
		inline D3DXCOLOR ToD3DXCOLOR(const Color& color)
		{
			return D3DXCOLOR(color.red, color.green, color.blue, color.alpha);
		}
		
		
		inline D3DXVECTOR3 GetTranslationVector(const D3DXMATRIX& m)
		{
			return D3DXVECTOR3(m._41, m._42, m._43);
		}
		
		D3DXVECTOR3 GetEulerAnglesZXY(const D3DXMATRIX& m);
	
	}	// end namespace D3DXHelpers
}	// end namespace Marda
