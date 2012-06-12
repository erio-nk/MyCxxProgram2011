/*!
	@file	Graphics/Direct3D10/Shader.h
	
	@author	E.Nakayama
*/
#pragma once
	
namespace Marda
{
	namespace Direct3D10
	{
		class Core;
		
		/*!
			@brief	�V�F�[�_�Ǘ��N���X�B
		*/
		class Shader
		{
		public:
			Shader();
			
			~Shader();
			
			bool Create(
				ID3D10Device* pDevice,
				const TCHAR* path,
				const char* vertexShaderFunction,
				const char* vertexShaderProfile,
				const char* geometryShaderFunction,
				const char* geometryShaderProfile,
				const char* pixelShaderFunction,
				const char* pixelShaderProfile
				);

			void Release();
			
			//! �f�o�C�X�ɃV�F�[�_�ƃV�F�[�_���\�[�X��S�ēK�p����B
			void Apply(ID3D10Device* pDevice);
			
			//! �萔�o�b�t�@�̃C���f�b�N�X���擾����B
			unsigned int GetConstantBufferIndex(const char* cbufName) const;
			//! �O���[�o���ϐ��p�萔�o�b�t�@�̃C���f�b�N�X���擾����B
			unsigned int GetConstantBufferIndexGlobals() const;
			//! �萔�o�b�t�@�̕ϐ��̓����Ǘ��p�C���f�b�N�X���擾����B
			unsigned int GetConstantBufferVariableIndex(unsigned int cbufIndex, const char* varName) const;
			unsigned int GetGlobalVariableIndex(const char* varName) const;
			//! �萔�o�b�t�@�i�̃L���b�V���ւ̃|�C���^�j���擾����B
			void* GetConstantBuffer(unsigned int cbufIndex);
			void* GetConstantBufferVariable(unsigned int cbufIndex, unsigned int varIndex);
			void* GetGlobalVariable(unsigned int varIndex);
			
			//! �e�N�X�`�����\�[�X�ϐ��̃C���f�b�N�X���擾����B
			unsigned int GetTextureIndex(const char* texName) const;
			unsigned int GetTextureIndex(const char* texName, unsigned int indexInArray) const;
			unsigned int GetTextureArrayIndex(const char* texArrayName) const;
			//! �e�N�X�`�����\�[�X�ϐ��ɃV�F�[�_���\�[�X�r���[��ݒ肷��B
			void SetTexture(unsigned int texIndex, ID3D10ShaderResourceView* pTextureView);
			void SetTexture(const char* texName, ID3D10ShaderResourceView* pTextureView);
			void SetTexture(const char* texName, unsigned int indexInArray, ID3D10ShaderResourceView* pTextureView);
			void SetTextureArray(unsigned int texArrayIndex, ID3D10ShaderResourceView* pTextureArrayView);
			void SetTextureArray(const char* texArrayName, ID3D10ShaderResourceView* pTextureArrayView);

			// �T���v���[�X�e�[�g�̃C���f�b�N�X���擾����B
			unsigned int GetSamplerStateIndex(const char* sampName) const;
			unsigned int GetSamplerStateIndex(const char* sampName, unsigned int indexInArray) const;
			//! �T���v���[�X�e�[�g��ݒ肷��B
			void SetSamplerState(unsigned int sampIndex, ID3D10SamplerState* pSamplerState);
			void SetSamplerState(const char* sampName, ID3D10SamplerState* pSamplerState);
			void SetSamplerState(const char* sampName, unsigned int indexInArray, ID3D10SamplerState* pSamplerState);
			
			//! ���_�V�F�[�_�̃o�C�g�R�[�h���擾����B
			ID3D10Blob* GetVertexShaderBytecode();
			const ID3D10Blob* GetVertexShaderBytecode() const { return const_cast<Shader*>(this)->GetVertexShaderBytecode(); }

		private:
			// �V�F�[�_�I�u�W�F�N�g�B
			ID3D10VertexShader* _pVertexShader;
			ID3D10GeometryShader* _pGeometryShader;
			ID3D10PixelShader* _pPixelShader;
			ID3D10Blob* _pVertexShaderBytecode;
			ID3D10Blob* _pGeometryShaderBytecode;
			ID3D10Blob* _pPixelShaderBytecode;
			
			// �V�F�[�_�ϐ����ƃ��W�X�^�ԍ��̃}�b�v�B
			struct RegisterInfo
			{
				unsigned int _number;	//!< ���W�X�^�ԍ��B
				unsigned int _count;	//!< ���W�X�^���B�ÓI�z��(Texture2D t[n]��)�̏ꍇ�́A2�ȏ�̒l������B
			};
			typedef std::map<std::string, RegisterInfo> ResourceRegisterMap;

			// �萔�o�b�t�@�B�萔�o�b�t�@�� Shader �N���X�����O�ō쐬���A�ێ�����B
			ResourceRegisterMap _constantBufferRegisterMap;

			struct ConstantBufferVariable
			{
				std::string _name;
				size_t _offset;
				size_t _size;
				ConstantBufferVariable()
					: _offset(0)
					, _size(0)
				{
				}
			};
			typedef std::vector<ConstantBufferVariable> ConstantBufferVariableTable;
			
			typedef std::vector<uint8> ConstantBufferCache;
			
			std::vector<ID3D10Buffer*> _constantBuffers;	// �Y�������W�X�^�ԍ��B
			std::vector<ConstantBufferVariableTable> _constantBufferVariableTables;
			std::vector<ConstantBufferCache> _constantBufferCaches;
			
			// �e�N�X�`�����\�[�X�B
			ResourceRegisterMap _textureRegisterMap;
			std::vector<ID3D10ShaderResourceView*> _textures;	// �Y�������W�X�^�ԍ��B

			// �T���v���[�X�e�[�g�B
			ResourceRegisterMap _samplerRegisterMap;
			std::vector<ID3D10SamplerState*> _samplers;	// �Y�������W�X�^�ԍ��B

			// �V�F�[�_���\�[�X�K�p�֐��B
			void (Shader::*_pfnApplyConstantBuffers)(ID3D10Device*);
			void (Shader::*_pfnApplyTextures)(ID3D10Device*);
			void (Shader::*_pfnApplySamplers)(ID3D10Device*);			

			bool Parse(ID3D10Device* pDevice, ID3D10Blob* pShaderBytecode);
			void Error(ID3D10Blob* pErrorsBlob);

			void _ApplyConstantBuffers(ID3D10Device* pDevice);
			void _ApplyEmptyConstantBuffers(ID3D10Device* /*pDevice*/);
			void _ApplyTextures(ID3D10Device* pDevice);
			void _ApplyEmptyTextures(ID3D10Device* /*pDevice*/);
			void _ApplySamplerStates(ID3D10Device* pDevice);
			void _ApplyEmptySamplerStates(ID3D10Device* /*pDevice*/);

		};	// end class Shader
		
	}	// end namespace Direct3D
}	// end namespace Marda
