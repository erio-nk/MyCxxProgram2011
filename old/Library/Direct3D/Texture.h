/*!
 	@file Direct3D/Texture.h
 
 	�e�N�X�`���N���X�B

	�e�N�X�`���̖ʓ|�Ȏ葱����
	���\�b�h�ɓZ�߂������̃��b�p�[�N���X�B
 
 	@author Dila
 */

#include "Direct3D/Device.h"

namespace Direct3D
{
	//! @addtogroup Direct3D Direct3D
	//! @{

	/*!
	 	�e�N�X�`���N���X

		�x�[�V�b�N�ȃe�N�X�`���Ɋւ���葱����Z�߂����b�p�[�N���X�B
		@todo	��������B
	 */
	class Texture
	{
		IDirect3DTexture* _pTexture;	//! �e�N�X�`��

	public:
		Texture();

		~Texture();

		bool Create(IDirect3DDevice* pDevice);
		bool CreateFromFile(const TCHAR* pszFilePath);

		IDirect3DTexture* GetTexture() { return _pTexture; }

	};	// end class Texture


	/*!
		�����_�����O�p�̃e�N�X�`���N���X

		�e�N�X�`���̓f�o�C�X���̊֐��ō�炸�ɁA
		���̃N���X��Create�ō�鎖�B
	 */
	class RenderTexture
	{
		IDirect3DDevice* _pDevice;		//! �f�o�C�X
		IDirect3DTexture* _pTexture;	//! �e�N�X�`��
		IDirect3DSurface* _pSurface;	//! �T�[�t�F�C�X

		friend class BeginRender;
	public:
		RenderTexture();

		~RenderTexture();

		bool Create(IDirect3DDevice* pDevice, float w, float h);

		void Release();

		IDirect3DTexture* GetTexture() { return _pTexture; }



		/*!
			�e�N�X�`�������_�����O�J�n�N���X
			@code
				RenderTexture texture;
				...
				{
					RenderTexture::BeginRender begin(texture);	// �e�N�X�`�������_�����O�J�n

					...	// �e�탌���_�����O

					// �X�R�[�v�𔲂���ƃ����_�[�^�[�Q�b�g�����ɖ߂�
				}
			@endcode
		 */
		class BeginRender
		{
			RenderTexture* _pTexture;		//! �����_�[�^�[�Q�b�g
			IDirect3DSurface* _pBackBuffer;	//! �o�b�N�o�b�t�@
		public:
			BeginRender(RenderTexture& texture) :
				_pTexture(&texture)
			{
				// �����_�[�^�[�Q�b�g�؂�ւ�
				_pTexture->_pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &_pBackBuffer);
				_pTexture->_pDevice->SetRenderTarget(0, _pTexture->_pSurface);
			}

			~BeginRender()
			{
				End();
			}

			void End()
			{
				if (_pTexture) {
					// �����_�[�^�[�Q�b�g�����ɖ߂�
					_pTexture->_pDevice->SetRenderTarget(0, _pBackBuffer);
					_pTexture = NULL;
					_pBackBuffer->Release();
				}
			}
		};	// end class BeginRender

	};	// end class RenderableTexture
    
	//! @}
}	// end namespace Texture