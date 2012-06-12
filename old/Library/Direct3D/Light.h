/*!
 	@file Direct3D/Light.h
 
 	���C�g�N���X�B

	���C�g�̖ʓ|�Ȏ葱����
	���\�b�h�ɓZ�߂������̃��b�p�[�N���X�B
 
 	@author Dila
 */

#include "Direct3D/Device.h"

namespace Direct3D
{
	//! @addtogroup Direct3D Direct3D
	//! @{

	/*!
	 	�f�B���N�V���i�����C�g�N���X
	 */
	class DirectionalLight
	{
		D3DLIGHT _light;	//! ���C�g

	public:
		//! �R���X�g���N�^
		DirectionalLight();

		//! �f�X�g���N�^
		~DirectionalLight();

		//! ���C�g���擾����
		D3DLIGHT& GetLight() { return _light; }
		const D3DLIGHT& GetLight() const { return _light; }

		/*!
			���C�g�̕�����ݒ肷��
			@param	x	x����
			@param	y	y����
			@param	z	z����
		 */
		void SetDirection(float x, float y, float z);

		/*!
			���C�g�̍��W��ݒ肷��
			@param	x	x���W
			@param	y	y���W
			@param	z	z���W
		 */
		void SetPosition(float x, float y, float z);

		/*!
			���C�g�̃f�B�t���[�Y�F��ݒ肷��
		 */
		void SetDiffuseColor(float r, float g, float b, float a = 1.0f);

		/*!
			���C�g�̃A���r�G���g�F��ݒ肷��
		 */
		void SetAmbientColor(float r, float g, float b, float a = 1.0f);

		/*!
			���C�g�͈̔͂�ݒ肷��
			@param	range	�͈�
		 */
		void SetRange(float range);

	};	// end class DirectionalLight

	//! @}
}	// end namespace Direct3D


