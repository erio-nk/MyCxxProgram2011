/*!
	@file	Graphics/Direct3D10/Device.h
	
	@author	E.Nakayama
*/
#pragma once
	
namespace Marda
{
	namespace Direct3D10
	{
		class Core;
		class BufferPtr;
		struct FontDesc;
		
		/*!
			@brief	�f�o�C�X�ˑ��I�u�W�F�N�g�̐����^�j�����s�� Facade �N���X�B
		*/
		class Device
		{
		public:
			typedef ID3D10Device NativeDevice;
			
			Device();
			~Device();
			
			//! �f�o�C�X�̐����Ə����������B
			bool Create(HWND hWnd);
			
			//! �I�������B
			void Release();
			
			//! �f�o�C�X���폜����Ă��邩�ǂ�����Ԃ��B
			bool IsDeviceRemoved() const;
			//! �`��̈悪�i�E�B���h�E�ŏ������Łj�B�؂�����Ă��邩�ǂ�����Ԃ��B
			bool IsScreenOccluded() const;
			
			//! �f�o�C�X�̐��|�C���^��Ԃ��B�߂�l�͕K�v�����Ȃ�����������K�v������B
			ID3D10Device* GetNativeDevice();
			//! �R�A�I�u�W�F�N�g��Ԃ��B�N���X�̒P�����p�҂͂��̊֐����ĂԂׂ��ł͖����B
			Core& GetCore();

			//! �o�b�N�o�b�t�@�̕`��^�[�Q�b�g�n���h�����擾����B
			RenderTargetHandle GetBackBuffer() const;
			//! �[�x�X�e���V���o�b�t�@�̃n���h�����擾����B
			DepthStencilBufferHandle GetDepthStencilBuffer() const;
			//! �o�b�N�o�b�t�@�Ɛ[�x�X�e���V���o�b�t�@�̃T�C�Y��ύX����B�E�B���h�E�̃T�C�Y���ύX���ꂽ�ꍇ�A���̊֐����Ăяo���K�v������B
			bool ResizeBackBuffer(SIZE sz);

			//! �`��^�[�Q�b�g���쐬����B
			RenderTargetHandle CreateRenderTarget(unsigned int width, unsigned int height, DXGI_FORMAT format, bool useShaderResource);
			DepthStencilBufferHandle CreateDepthStencilBuffer(unsigned int width, unsigned int height, bool useShaderResource);
			
			//! ���̓��C�A�E�g���쐬����B
			InputLayoutHandle CreateInputLayout(D3D10_INPUT_ELEMENT_DESC* inputElementDescs, UINT numInputElements, ShaderHandle shaderHandle);
			//! ���̓��C�A�E�g���������B
			void ReleaseInputLayout(InputLayoutHandle handle);

			//! �u�����f�B���O�X�e�[�g���쐬����B
			BlendStateHandle CreateBlendState(const D3D10_BLEND_DESC& desc);
			//! �u�����f�B���O�X�e�[�g���������B
			void ReleaseBlendState(BlendStateHandle handle);
			//! �u�����f�B���O�X�e�[�g�̏ڍׂ��擾����B
			const D3D10_BLEND_DESC* GetBlendStateDesc(BlendStateHandle handle) const;

			//! ���X�^���C�U�X�e�[�g���쐬����B
			RasterizerStateHandle CreateRasterizerState(const D3D10_RASTERIZER_DESC& desc);
			//! ���X�^���C�U�X�e�[�g���������B
			void ReleaseRasterizerState(RasterizerStateHandle handle);
			//! ���X�^���C�U�X�e�[�g�̏ڍׂ��擾����B
			const D3D10_RASTERIZER_DESC* GetRasterizerStateDesc(RasterizerStateHandle handle) const;

			//! �[�x�X�e���V���X�e�[�g���쐬����B
			DepthStencilStateHandle CreateDepthStencilState(const D3D10_DEPTH_STENCIL_DESC& desc);
			//! �[�x�X�e���V���X�e�[�g���������B
			void ReleaseDepthStencilState(DepthStencilStateHandle handle);
			//! �[�x�X�e���V���X�e�[�g�̏ڍׂ��擾����B
			const D3D10_DEPTH_STENCIL_DESC* GetDepthStencilStateDesc(DepthStencilStateHandle handle) const;

			//! ���_�o�b�t�@���쐬����B
			VertexBufferHandle CreateVertexBuffer(D3D10_USAGE usage, UINT CPUAccessFlags, size_t sizeVertex, size_t numVertices, void* pInitialData = NULL);
			//! ���_�o�b�t�@���������B
			void ReleaseVertexBuffer(VertexBufferHandle handle);
			//! ���_�o�b�t�@�̏ڍׂ��擾����B
			const D3D10_BUFFER_DESC* GetVertexBufferDesc(VertexBufferHandle handle) const;
			//! ���_�o�b�t�@�ւ̃|�C���^���擾����B
			bool GetVertexBufferPtr(VertexBufferHandle handle, BufferPtr& bufferPtr, D3D10_MAP mapType, UINT mapFlags = 0);

			//! �C���f�b�N�X�o�b�t�@���쐬����B
			IndexBufferHandle CreateIndexBuffer(D3D10_USAGE usage, UINT CPUAccessFlags, size_t sizeIndex, size_t numIndices, void* pInitialData = NULL);
			//! �C���f�b�N�X�o�b�t�@���������B
			void ReleaseIndexBuffer(IndexBufferHandle handle);
			//! �C���f�b�N�X�o�b�t�@�̏ڍׂ��擾����B
			const D3D10_BUFFER_DESC* GetIndexBufferDesc(IndexBufferHandle handle) const;
			//! �C���f�b�N�X�o�b�t�@�ւ̃|�C���^���擾����B
			bool GetIndexBufferPtr(IndexBufferHandle handle, BufferPtr& bufferPtr, D3D10_MAP mapType, UINT mapFlags = 0);
			
			//! �V�F�[�_���쐬����B�W�I���g���V�F�[�_���s�v�ȏꍇ�́AgeometryShaderFunction �� NULL ��n�������o����B
			ShaderHandle CreateShader(
				const TCHAR* path,
				const char* vertexShaderFunction,
				const char* vertexShaderProfile,
				const char* geometryShaderFunction,
				const char* geometryShaderProfile,
				const char* pixelShaderFunction,
				const char* pixelShaderProfile
				);
			//! �V�F�[�_���������B
			void ReleaseShader(ShaderHandle handle);

			//! �e�N�X�`�����쐬����B
			TextureHandle CreateTextureFromFile(const TCHAR* filePath);
			TextureHandle CreateTextureArrayFromFiles(const TCHAR** filePaths, unsigned int numFilePaths);

			//! �T���v���[�X�e�[�g���쐬����B
			SamplerStateHandle CreateSamplerState(const D3D10_SAMPLER_DESC& desc);
			//! �T���v���[�X�e�[�g���������B
			void ReleaseSamplerState(SamplerStateHandle handle);
			//! �T���v���[�X�e�[�g�̏ڍׂ��擾����B
			const D3D10_SAMPLER_DESC* GetSamplerStateDesc(SamplerStateHandle handle) const;
			
			//! �t�H���g���쐬����B
			FontHandle CreateFont(const FontDesc& desc);
			//! �t�H���g���������B
			void ReleaseFont(FontHandle handle);
			//! �t�H���g�̏ڍׂ��擾����B
			const FontDesc* GetFontDesc(FontHandle handle) const;
			//! �t�H���g�̍������擾����B
			INT GetFontHeight(FontHandle handle) const;
			
		private:
			std::auto_ptr<Core> _core;
		};	// end class Device

	}	// end namespace Direct3D
}	// end namespace Marda
