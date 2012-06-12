/*!
	@file	Graphics/Direct3D10/MathHelpers.h
	
			��ɁADirect3DX �̊e�\���̂Ɋւ��鐔�w�n�w���p�֐��B
	
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
