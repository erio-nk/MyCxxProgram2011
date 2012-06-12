/*!
	@file	Graphics/Direct3D10/Renderer.h
	
	@author	E.Nakayama
*/
#pragma once
	
#include "Marda/Graphics/Color.h"
#include "Marda/Graphics/Direct3D10/Device.h"
	
namespace Marda
{
	namespace Direct3D10
	{
		/*!
			@brief	�`��Ɋւ��鏈�����������N���X�B
					�A�v���P�[�V�����͖{�N���X���p�����Ċg�����Ă��ǂ��B
		*/
		class Renderer
		{
		public:
			Renderer();
			virtual ~Renderer();
			
			virtual bool Create(Device& device);

			virtual void Release();
			
			const Device* GetDevice() const;
			Device::NativeDevice* GetNativeDevice();
			
			//! �t���[���o�b�t�@�̓��e����ʂɏo�͂���BIsScreenOccluded �� true ��Ԃ���Ԃł��Ăяo���Ȃ���΂Ȃ�Ȃ����A���̏ꍇ�͎��ۂ̕`��͍s���Ȃ��B
			HRESULT Present();
			
			//! �f�t�H���g�̕`��^�[�Q�b�g���擾����B
			RenderTargetHandle GetDefaultRenderTarget();
			//! �f�t�H���g�̐[�x�X�e���V���o�b�t�@���擾����B
			DepthStencilBufferHandle GetDefaultDepthStencilBuffer();

			//! �`��^�[�Q�b�g��ݒ肷��B
			bool SetRenderTargets(RenderTargetHandle renderTargetHandle, DepthStencilBufferHandle depthStencilBufferHandle);
			//! �f�t�H���g�̕`��^�[�Q�b�g�ɐݒ肷��B
			void SetDefaultRenderTargets();
			//! ���݂̕`��^�[�Q�b�g���N���A����B
			void Clear(bool clearColor, bool clearDepth, bool clearStencil, const Color& color = Color::BLACK, float depth = DEPTH_MAX, uint8 stencil = 0);
			
			//! �r���[�|�[�g��ݒ肷��B
			void SetViewport(SIZE sz);

			//! ���̓��C�A�E�g��ݒ肷��B
			void SetInputLayout(InputLayoutHandle handle);
			//! ���̓��C�A�E�g�̐ݒ���N���A����B
			void ClearInputLayout();

			//! �u�����f�B���O�X�e�[�g��ݒ肷��B
			void SetBlendState(BlendStateHandle handle);
			//! �f�t�H���g�̃u�����f�B���O�X�e�[�g��ݒ肷��B
			void SetDefaultBlendState();
			//! �f�t�H���g�̃u�����f�B���O�X�e�[�g���擾����B
			BlendStateHandle GetDefaultBlendState() const;

			//! ���X�^���C�U�X�e�[�g��ݒ肷��B
			void SetRasterizerState(RasterizerStateHandle handle);
			//! �f�t�H���g�̃��X�^���C�U�X�e�[�g��ݒ肷��B
			void SetDefaultRasterizerState();
			//! �f�t�H���g�̃��X�^���C�U�X�e�[�g���擾����B
			RasterizerStateHandle GetDefaultRasterizerState() const;

			//! �[�x�X�e���V���X�e�[�g��ݒ肷��B
			void SetDepthStencilState(DepthStencilStateHandle handle);
			//! �f�t�H���g�̐[�x�X�e���V���X�e�[�g��ݒ肷��B
			void SetDefaultDepthStencilState();
			//! �f�t�H���g�̐[�x�X�e���V���X�e�[�g���擾����B
			DepthStencilStateHandle GetDefaultDepthStencilState() const;

			//! ���_�o�b�t�@��ݒ肷��B
			void SetVertexBuffer(VertexBufferHandle handle);
			void SetVertexBuffer(unsigned int slot, VertexBufferHandle handle);
			//! ���_�o�b�t�@�̓��e���X�V����B
			void UpdateVertexBuffer(VertexBufferHandle handle, void* pNewData);
			//! �C���f�b�N�X�o�b�t�@��ݒ肷��B
			void SetIndexBuffer(IndexBufferHandle handle);
			
			//! �V�F�[�_��ݒ肷��B
			void SetShader(ShaderHandle handle);
			//! �V�F�[�_�̐ݒ���N���A����B
			void ClearShader();
			//! ���ݐݒ肳��Ă���V�F�[�_�ƃV�F�[�_���\�[�X���f�o�C�X�ɓK�p����B
			void ApplyShader();

			//! �V�F�[�_�̒萔�o�b�t�@������C���f�b�N�X���擾����B
			unsigned int GetShaderConstantBufferIndex(ShaderHandle handle, const char* cbufName) const;
			unsigned int GetShaderConstantBufferVariableIndex(ShaderHandle handle, unsigned int cbufIndex, const char* varName)  const;
			unsigned int GetShaderConstantBufferVariableIndex(ShaderHandle handle, const char* cbufName, const char* varName) const;
			unsigned int GetShaderGlobalVariableIndex(ShaderHandle handle, const char* varName) const;
			//! �V�F�[�_�̒萔�o�b�t�@�i�̃L���b�V���ւ̃|�C���^�j���擾����B
			void* GetShaderConstantBuffer(ShaderHandle handle, unsigned int cbufIndex);
			void* GetShaderConstantBuffer(ShaderHandle handle, const char* cbufName);
			void* GetShaderConstantBufferVariable(ShaderHandle handle, unsigned int cbufIndex, unsigned int varIndex);
			void* GetShaderConstantBufferVariable(ShaderHandle handle, unsigned int cbufIndex, const char* varName);
			void* GetShaderConstantBufferVariable(ShaderHandle handle, const char* cbufName, unsigned int varIndex);
			void* GetShaderConstantBufferVariable(ShaderHandle handle, const char* cbufName, const char* varName);
			void* GetShaderGlobalVariable(ShaderHandle handle, unsigned int varIndex);
			void* GetShaderGlobalVariable(ShaderHandle handle, const char* varName);
			const void* GetShaderConstantBuffer(ShaderHandle handle, unsigned int cbufIndex) const { return const_cast<Renderer*>(this)->GetShaderConstantBuffer(handle, cbufIndex); }
			const void* GetShaderConstantBuffer(ShaderHandle handle, const char* cbufName) const { return const_cast<Renderer*>(this)->GetShaderConstantBuffer(handle, cbufName); }
			const void* GetShaderConstantBufferVariable(ShaderHandle handle, unsigned int cbufIndex, unsigned int varIndex) const { return const_cast<Renderer*>(this)->GetShaderConstantBufferVariable(handle, cbufIndex, varIndex); }
			const void* GetShaderConstantBufferVariable(ShaderHandle handle, unsigned int cbufIndex, const char* varName) const { return const_cast<Renderer*>(this)->GetShaderConstantBufferVariable(handle, cbufIndex, varName); }
			const void* GetShaderConstantBufferVariable(ShaderHandle handle, const char* cbufName, unsigned int varIndex) const { return const_cast<Renderer*>(this)->GetShaderConstantBufferVariable(handle, cbufName, varIndex); }
			const void* GetShaderConstantBufferVariable(ShaderHandle handle, const char* cbufName, const char* varName) const { return const_cast<Renderer*>(this)->GetShaderConstantBufferVariable(handle, cbufName, varName); }
			const void* GetShaderGlobalVariable(ShaderHandle handle, unsigned int varIndex) const { return const_cast<Renderer*>(this)->GetShaderGlobalVariable(handle, varIndex); }
			const void* GetShaderGlobalVariable(ShaderHandle handle, const char* varName) const { return const_cast<Renderer*>(this)->GetShaderGlobalVariable(handle, varName); }

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

			//! �V�F�[�_�̃T���v���[�X�e�[�g�ϐ��ɃT���v���[�X�e�[�g��ݒ肷��B
			void SetShaderSamplerState(ShaderHandle shaderHandle, unsigned int sampIndex, SamplerStateHandle samplerHandle);
			void SetShaderSamplerState(ShaderHandle shaderHandle, const char* sampName, SamplerStateHandle samplerHandle);
			void SetShaderSamplerState(ShaderHandle shaderHandle, const char* sampName, unsigned int indexInArray, SamplerStateHandle samplerHandle);

			//! �f�t�H���g�̃t�H���g���擾����B
			FontHandle GetDefaultFont() const;
			//! �e�L�X�g��`�悷��B�iID3DX10Font�𗘗p�����j���̊֐����Ăяo���ƁA�����Ŋe��X�e�[�g���㏑������Ă��܂��ג��ӁB
			void DrawText(FontHandle handle, int x, int y, const Color& color, const TCHAR* format, ...);
			//! �f�t�H���g�t�H���g�Ńe�L�X�g��`�悷��B
			void DrawText(int x, int y, const Color& color, const TCHAR* format, ...);
			
		private:
			Core* _core;
			Device* _device;
			
			RenderTargetHandle _renderTargetHandleDefault;
			DepthStencilBufferHandle _depthStencilBufferHandleDefault;
			BlendStateHandle _blendStateHandleDefault;
			RasterizerStateHandle _rasterizerStateHandleDefault;
			DepthStencilStateHandle _depthStencilStateHandleDefault;
			FontHandle _fontHandleDefault;

			RenderTargetHandle _renderTargetHandleCurrent;
			DepthStencilBufferHandle _depthStencilBufferHandleCurrent;
			ShaderHandle _shaderHandleCurrent;

			bool CreateDefaultStates();
			bool CreateDefaultFont();
			void _Release();
		
		};	// end class Renderer

	}	// end namespace Direct3D
}	// end namespace Marda
