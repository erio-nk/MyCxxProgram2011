/*!
	@file	Graphics/Direct3D10/MathHelpers.h
	
			主に、Direct3DX の各構造体に関する数学系ヘルパ関数。
	
	@author	E.Nakayama
*/
#pragma once
	
namespace Marda
{
	inline D3DXVECTOR3 GetTranslationVectorFromMatrix(const D3DXMATRIX& matrix)
	{
		return D3DXVECTOR3(matrix._41, matrix._42, matrix._43);
	}
	
	
	
	
}	// end namespace Marda
