/*!
 	@file Direct3D/Mesh.h
 
 	���b�V���N���X�B
 
 	@author Dila
 */

#include "Direct3D/Device.h"

namespace Direct3D
{
	//! @addtogroup Direct3D Direct3D
	//! @{

	/*!
	 	���b�V���N���X
	 
	 	���b�V���ƃ}�e���A���̖ʓ|�Ȏ葱����
	 	���\�b�h�ɓZ�߂������̃��b�p�[�N���X�B
	 */
	class Mesh
	{
		LPD3DXMESH _pMesh;	//! ���b�V��
		D3DMATERIAL _Material; 	//! �}�e���A��

	public:
		//! �R���X�g���N�^
		Mesh();

		//! �f�X�g���N�^
		~Mesh();

		//! ���\�[�X���������
		void Release();

		//! ���b�V�����擾����
		LPD3DXMESH& GetMesh() { return _pMesh; }
		const LPD3DXMESH& GetMesh() const { return _pMesh; }

		//! �}�e���A�����擾����
		D3DMATERIAL& GetMaterial() { return _Material; }
		const D3DMATERIAL& GetMaterial() const { return _Material; }

		/*!
			���b�V���̃f�B�t���[�Y�F��ݒ肷��
		 */
		void SetDiffuseColor(float r, float g, float b, float a = 1.0f);

		/*!
			���b�V���̃f�B�t���[�Y�F���擾����
		 */
		D3DCOLOR GetDiffuseColor()
		{
			return D3DCOLOR_ARGB(
				(unsigned char)(_Material.Diffuse.a*255),
				(unsigned char)(_Material.Diffuse.r*255),
				(unsigned char)(_Material.Diffuse.g*255),
				(unsigned char)(_Material.Diffuse.b*255)
				);
		}

		/*!
			���b�V���̃A���r�G���g�F��ݒ肷��
		 */
		void SetAmbientColor(float r, float g, float b, float a = 1.0f);

		/*!
			���b�V���̃A���r�G���g�F���擾����
		 */
		D3DCOLOR GetAmbientColor()
		{
			return D3DCOLOR_ARGB(
				(unsigned char)(_Material.Ambient.a*255),
				(unsigned char)(_Material.Ambient.r*255),
				(unsigned char)(_Material.Ambient.g*255),
				(unsigned char)(_Material.Ambient.b*255)
				);
		}


		/*!
			���b�V����`�悷��
		 */
		HRESULT Render(Direct3D::Device& device);

	};	// end class Mesh

	//! @}
}	// end namespace Direct3D


