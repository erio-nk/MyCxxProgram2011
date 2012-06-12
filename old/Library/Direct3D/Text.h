/*!
 	@file Direct3D/Text.h
 
 	�e�L�X�g�\���N���X
 
 	@author Dila
 */

#pragma once

#include "Direct3D/Device.h"


namespace Direct3D
{
	//! @addtogroup Direct3D Direct3D
	//! @{

	/*!
	 	�e�L�X�g�\���N���X
	 */
	class Text
	{
		LPD3DXFONT	_pFont;
		int _size;

	public:
		Text();
		~Text();

		bool Create(Direct3D::Device& device);
		void Release();

		HRESULT Render(const TCHAR* text, float x, float y, DWORD color, DWORD format);

	};	// end class Text

	//! @}
}	// end namespace Direct3D


