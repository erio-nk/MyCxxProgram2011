/*!
	@file	Graphics/Direct3D10/Core.h
	
	@author	E.Nakayama
*/
#pragma once

#include "..\Direct3D10.h"
#include "Shader.h"
	
namespace Marda
{
	struct Color;

	namespace Direct3D10
	{
		//! �`��^�[�Q�b�g�̃I�u�W�F�N�g�z���_�[�B
		struct RenderTarget
		{
			ID3D10RenderTargetView* _pRenderTargetView;
			ID3D10ShaderResourceView* _pShaderResourceView;

			RenderTarget()
				: _pRenderTargetView(NULL)
				, _pShaderResourceView(NULL)
			{
			}

			~RenderTarget()
			{
				// std::vector<RenderTarget> �ŃR�s�[�������������ɉ������Ă��܂��ׁA������ Release �̌Ăяo���͏����Ȃ��B
			}

			void Release()
			{
				safe_release(_pRenderTargetView);
				safe_release(_pShaderResourceView);
			}
		
		};	// end struct RenderTarget
		
		//! �[�x�X�e���V���o�b�t�@�̃I�u�W�F�N�g�z���_�[�B
		struct DepthStencilBuffer
		{
			ID3D10DepthStencilView* _pDepthStencilView;
			ID3D10ShaderResourceView* _pShaderResourceView;

			DepthStencilBuffer()
				: _pDepthStencilView(NULL)
				, _pShaderResourceView(NULL)
			{
			}

			~DepthStencilBuffer()
			{
				// std::vector<DepthStencilBuffer> �ŃR�s�[�������������ɉ������Ă��܂��ׁA������ Release �̌Ăяo���͏����Ȃ��B
			}

			void Release()
			{
				safe_release(_pDepthStencilView);
				safe_release(_pShaderResourceView);
			}
		
		};	// end struct DepthStencilBuffer
		

		//! ���_�o�b�t�@�̏�񁕃I�u�W�F�N�g�z���_�[�B
		struct VertexBuffer
		{
			size_t _sizeVertex;
			ID3D10Buffer* _pInterface;

			VertexBuffer()
				: _pInterface(NULL)
			{
			}

			~VertexBuffer()
			{
				// std::vector<VertexBuffer> �ŃR�s�[�������������ɉ������Ă��܂��ׁA������ Release �̌Ăяo���͏����Ȃ��B
			}

			void Release()
			{
				safe_release(_pInterface);
			}
		
		};	// end struct VertexBuffer

		//! �C���f�b�N�X�o�b�t�@�̏�񁕃I�u�W�F�N�g�z���_�[�B
		struct IndexBuffer
		{
			DXGI_FORMAT _format;
			ID3D10Buffer* _pInterface;

			IndexBuffer()
				: _pInterface(NULL)
			{
			}

			~IndexBuffer()
			{
				// std::vector<IndexBuffer> �ŃR�s�[�������������ɉ������Ă��܂��ׁA������ Release �̌Ăяo���͏����Ȃ��B
			}

			void Release()
			{
				safe_release(_pInterface);
			}
		
		};	// end struct IndexBuffer
	
		//! �t�H���g�̏�񁕃I�u�W�F�N�g�z���_�[�B
		struct Font
		{
			FontDesc _desc;
			ID3DX10Font* _pInterface;

			Font()
				: _pInterface(NULL)
			{
			}
		
			~Font()
			{
				// std::vector<Font> �ŃR�s�[�������������ɉ������Ă��܂��ׁA������ Release �̌Ăяo���͏����Ȃ��B
			}
		
			void Release()
			{
				safe_release(_pInterface);
			}
		};	// end struct Font


		/*!
			@brief	Direct3D10�ւ���@�\���ЂƂ܂Ƃ߂ɂ����R�A�N���X�B
					�f�o�C�X�ˑ��I�u�W�F�N�g�͂��̃N���X�ŕێ�����A�n���h���x�[�X�ł������s���B
					���̃N���X�����ł� Direct3D10 �̗��p�͉\�����A��{�I�ɂ� Device / Renderer �o�R�ŗ��p���鎖�B
		*/
		class Core
		{
		public:
			typedef ID3D10Device NativeDevice;
			
			Core();
			~Core();
			
			//! �f�o�C�X�̐����Ə����������B
			bool Create(HWND hWnd);
			
			//! �I�������B
			void Release();
			
			//! �t���[���o�b�t�@�̓��e����ʂɏo�͂���BIsScreenOccluded �� true ��Ԃ���Ԃł��Ăяo���Ȃ���΂Ȃ�Ȃ����A���̏ꍇ�͎��ۂ̕`��͍s���Ȃ��B
			HRESULT Present();

			//! �f�o�C�X���폜����Ă��邩�ǂ�����Ԃ��B
			bool IsDeviceRemoved() const;
			//! �`��̈悪�i�E�B���h�E�ŏ������Łj�B�؂�����Ă��邩�ǂ�����Ԃ��B
			bool IsScreenOccluded() const;
			
			//! �f�o�C�X�̐��|�C���^��Ԃ��B�߂�l�͕K�v�����Ȃ�����������K�v������B
			ID3D10Device* GetNativeDevice();
			
			//! �o�b�N�o�b�t�@�̕`��^�[�Q�b�g�n���h�����擾����B
			RenderTargetHandle GetBackBuffer() const;
			//! �[�x�X�e���V���o�b�t�@�̃n���h�����擾����B
			DepthStencilBufferHandle GetDepthStencilBuffer() const;
			//! �o�b�N�o�b�t�@�Ɛ[�x�X�e���V���o�b�t�@�̃T�C�Y��ύX����B�E�B���h�E�̃T�C�Y���ύX���ꂽ�ꍇ�A���̊֐����Ăяo���K�v������B
			bool ResizeBackBuffer(SIZE sz);
			
			//! �r���[�|�[�g��ݒ肷��B
			void SetViewport(SIZE sz);

			//! �`��^�[�Q�b�g���쐬����B
			RenderTargetHandle CreateRenderTarget(unsigned int width, unsigned int height, DXGI_FORMAT format, bool useShaderResource);
			DepthStencilBufferHandle CreateDepthStencilBuffer(unsigned int width, unsigned int height, bool useShaderResource);
			//! �`��^�[�Q�b�g��ݒ肷��B
			bool SetRenderTargets(RenderTargetHandle renderTargetHandle, DepthStencilBufferHandle depthStencilBufferHandle);
			//! �`��^�[�Q�b�g���N���A����B
			void ClearRenderTarget(RenderTargetHandle handle, const Color& color);
			void ClearDepthStencil(DepthStencilBufferHandle handle, DWORD clearDepthStencilFlag, float depth, uint8 stencil);

			//! ���̓��C�A�E�g���쐬����B
			InputLayoutHandle CreateInputLayout(D3D10_INPUT_ELEMENT_DESC* inputElementDescs, UINT numInputElements, ShaderHandle shaderHandle);
			//! ���̓��C�A�E�g���������B
			void ReleaseInputLayout(InputLayoutHandle handle);
			//! ���̓��C�A�E�g��ݒ肷��B
			void SetInputLayout(InputLayoutHandle handle);
			//! ���̓��C�A�E�g�̐ݒ���N���A����B
			void ClearInputLayout();

			//! �u�����f�B���O�X�e�[�g���쐬����B
			BlendStateHandle CreateBlendState(const D3D10_BLEND_DESC& desc);
			//! �u�����f�B���O�X�e�[�g���������B
			void ReleaseBlendState(BlendStateHandle handle);
			//! �u�����f�B���O�X�e�[�g��ݒ肷��B
			void SetBlendState(BlendStateHandle handle);
			//! �u�����f�B���O�X�e�[�g�̏ڍׂ��擾����B
			const D3D10_BLEND_DESC* GetBlendStateDesc(BlendStateHandle handle) const;

			//! ���X�^���C�U�X�e�[�g���쐬����B
			RasterizerStateHandle CreateRasterizerState(const D3D10_RASTERIZER_DESC& desc);
			//! ���X�^���C�U�X�e�[�g���������B
			void ReleaseRasterizerState(RasterizerStateHandle handle);
			//! ���X�^���C�U�X�e�[�g��ݒ肷��B
			void SetRasterizerState(RasterizerStateHandle handle);
			//! ���X�^���C�U�X�e�[�g�̏ڍׂ��擾����B
			const D3D10_RASTERIZER_DESC* GetRasterizerStateDesc(RasterizerStateHandle handle) const;

			//! �[�x�X�e���V���X�e�[�g���쐬����B
			DepthStencilStateHandle CreateDepthStencilState(const D3D10_DEPTH_STENCIL_DESC& desc);
			//! �[�x�X�e���V���X�e�[�g���������B
			void ReleaseDepthStencilState(DepthStencilStateHandle handle);
			//! �[�x�X�e���V���X�e�[�g��ݒ肷��B
			void SetDepthStencilState(DepthStencilStateHandle handle);
			//! �[�x�X�e���V���X�e�[�g�̏ڍׂ��擾����B
			const D3D10_DEPTH_STENCIL_DESC* GetDepthStencilStateDesc(DepthStencilStateHandle handle) const;

			//! ���_�o�b�t�@���쐬����B
			VertexBufferHandle CreateVertexBuffer(D3D10_USAGE usage, UINT CPUAccessFlags, size_t sizeVertex, size_t numVertices, void* pInitialData);
			//! ���_�o�b�t�@���������B
			void ReleaseVertexBuffer(VertexBufferHandle handle);
			//! ���_�o�b�t�@�̏ڍׂ��擾����B
			const D3D10_BUFFER_DESC* GetVertexBufferDesc(VertexBufferHandle handle) const;
			//! ���_�o�b�t�@�ւ̃|�C���^���擾����B
			bool GetVertexBufferPtr(VertexBufferHandle handle, BufferPtr& bufferPtr, D3D10_MAP mapType, UINT mapFlags = 0);
			//! ���_�o�b�t�@��ݒ肷��B
			void SetVertexBuffer(unsigned int slot, VertexBufferHandle handle);
			//! ���_�o�b�t�@�̓��e���X�V����B
			void UpdateVertexBuffer(VertexBufferHandle handle, void* pNewData);

			//! �C���f�b�N�X�o�b�t�@���쐬����B
			IndexBufferHandle CreateIndexBuffer(D3D10_USAGE usage, UINT CPUAccessFlags, size_t sizeIndex, size_t numIndices, void* pInitialData = NULL);
			//! �C���f�b�N�X�o�b�t�@���������B
			void ReleaseIndexBuffer(IndexBufferHandle handle);
			//! �C���f�b�N�X�o�b�t�@�̏ڍׂ��擾����B
			const D3D10_BUFFER_DESC* GetIndexBufferDesc(IndexBufferHandle handle) const;
			//! �C���f�b�N�X�o�b�t�@�ւ̃|�C���^���擾����B
			bool GetIndexBufferPtr(IndexBufferHandle handle, BufferPtr& bufferPtr, D3D10_MAP mapType, UINT mapFlags = 0);
			//! �C���f�b�N�X�o�b�t�@��ݒ肷��B
			void SetIndexBuffer(IndexBufferHandle handle);
			
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
			//! �V�F�[�_�̐ݒ���N���A����B
			void ClearShader();
			//! �V�F�[�_�ƃV�F�[�_���\�[�X���f�o�C�X�ɓK�p����B
			void ApplyShader(ShaderHandle handle);

			//! �V�F�[�_�̒萔�o�b�t�@������C���f�b�N�X���擾����B
			unsigned int GetShaderConstantBufferIndex(ShaderHandle handle, const char* cbufName) const;
			unsigned int GetShaderConstantBufferVariableIndex(ShaderHandle handle, unsigned int cbufIndex, const char* varName)  const;
			unsigned int GetShaderGlobalVariableIndex(ShaderHandle handle, const char* varName) const;
			//! �V�F�[�_�̒萔�o�b�t�@�i�̃L���b�V���ւ̃|�C���^�j���擾����B
			void* GetShaderConstantBuffer(ShaderHandle handle, unsigned int cbufIndex);
			void* GetShaderConstantBufferVariable(ShaderHandle handle, unsigned int cbufIndex, unsigned int varIndex);
			void* GetShaderGlobalVariable(ShaderHandle handle, unsigned int varIndex);

			//! �e�N�X�`�����쐬����B
			TextureHandle CreateTextureFromFile(const TCHAR* filePath);
			TextureHandle CreateTextureArrayFromFiles(const TCHAR** filePaths, unsigned int numFilePaths);
			//! �V�F�[�_�̃e�N�X�`���ϐ��Ƀe�N�X�`����ݒ肷��B
			void SetShaderTexture(ShaderHandle shaderHandle, unsigned int texIndex, TextureHandle textureHandle);
			void SetShaderTexture(ShaderHandle shaderHandle, const char* texName, TextureHandle textureHandle);
			void SetShaderTexture(ShaderHandle shaderHandle, const char* texName, unsigned int indexInArray, TextureHandle textureHandle);
			void SetShaderTextureArray(ShaderHandle shaderHandle, unsigned int texArrayIndex, TextureHandle textureArrayHandle);
			void SetShaderTextureArray(ShaderHandle shaderHandle, const char* texArrayName, TextureHandle textureArrayHandle);
			//! �V�F�[�_�̃e�N�X�`���ϐ��ɕ`��^�[�Q�b�g��ݒ肷��B
			void SetRenderTargetToShaderTexture(ShaderHandle shaderHandle, unsigned int texIndex, RenderTargetHandle renderTargetHandle);
			void SetRenderTargetToShaderTexture(ShaderHandle shaderHandle, const char* texName, RenderTargetHandle renderTargetHandle);
			//! �V�F�[�_�̃e�N�X�`���ϐ��ɐ[�x�X�e���V���o�b�t�@��ݒ肷��B
			void SetDepthStencilBufferToShaderTexture(ShaderHandle shaderHandle, unsigned int texIndex, DepthStencilBufferHandle depthStencilBufferHandle);
			void SetDepthStencilBufferToShaderTexture(ShaderHandle shaderHandle, const char* texName, DepthStencilBufferHandle depthStencilBufferHandle);

			//! �T���v���[�X�e�[�g���쐬����B
			SamplerStateHandle CreateSamplerState(const D3D10_SAMPLER_DESC& desc);
			//! �T���v���[�X�e�[�g���������B
			void ReleaseSamplerState(SamplerStateHandle handle);
			//! �V�F�[�_�̃T���v���[�X�e�[�g�ϐ��ɃT���v���[�X�e�[�g��ݒ肷��B
			void SetShaderSamplerState(ShaderHandle shaderHandle, unsigned int sampIndex, SamplerStateHandle samplerHandle);
			void SetShaderSamplerState(ShaderHandle shaderHandle, const char* sampName, SamplerStateHandle samplerHandle);
			void SetShaderSamplerState(ShaderHandle shaderHandle, const char* sampName, unsigned int indexInArray, SamplerStateHandle samplerHandle);
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
			//! �e�L�X�g��`�悷��B�iID3DX10Font�𗘗p�����j���̊֐����Ăяo���ƁA�����Ŋe��X�e�[�g���㏑������Ă��܂��ג��ӁB
			void DrawText(FontHandle handle, int x, int y, const Color& color, const TCHAR* format, ...);
			void DrawText(FontHandle handle, int x, int y, const Color& color, const TCHAR* format, va_list va);
			
		private:
			ID3D10Device* _pDevice;
			IDXGISwapChain* _pSwapChain;
			
			RenderTargetHandle _backBufferHandle;
			DepthStencilBufferHandle _depthStencilBufferHandle;

			std::vector<RenderTarget> _renderTargets;
			std::vector<DepthStencilBuffer> _depthStencilBuffers;
			
			std::vector<ID3D10InputLayout*> _inputLayouts;

			std::vector<ID3D10BlendState*> _blendStates;
			std::vector<ID3D10RasterizerState*> _rasterizerStates;
			std::vector<ID3D10DepthStencilState*> _depthStencilStates;
			
			std::vector<VertexBuffer> _vertexBuffers;
			std::vector<IndexBuffer> _indexBuffers;
			
			std::vector<Shader> _shaders;
			std::vector<ID3D10ShaderResourceView*> _textures;
			std::vector<ID3D10SamplerState*> _samplerStates;

			std::vector<Font> _fonts;
			
			enum FLAGS
			{
				DEVICE_REMOVED	= 0x0001,
				DEVICE_ERROR	= 0x0002,
				SCREEN_OCCLUDED	= 0x0004,
				UNKNOWN_ERROR	= 0x0008,
			};
			Flags _flags;

			bool CreateBackBufferAndDepthStencil(SIZE sz);
			bool CreateDefaultStates();

			void ReleaseBackBufferAndDepthStencil();

		};	// end class Core
		
	}	// end namespace Direct3D
}	// end namespace Marda
