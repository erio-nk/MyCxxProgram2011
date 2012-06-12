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
			@brief	シェーダ管理クラス。
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
			
			//! デバイスにシェーダとシェーダリソースを全て適用する。
			void Apply(ID3D10Device* pDevice);
			
			//! 定数バッファのインデックスを取得する。
			unsigned int GetConstantBufferIndex(const char* cbufName) const;
			//! グローバル変数用定数バッファのインデックスを取得する。
			unsigned int GetConstantBufferIndexGlobals() const;
			//! 定数バッファの変数の内部管理用インデックスを取得する。
			unsigned int GetConstantBufferVariableIndex(unsigned int cbufIndex, const char* varName) const;
			unsigned int GetGlobalVariableIndex(const char* varName) const;
			//! 定数バッファ（のキャッシュへのポインタ）を取得する。
			void* GetConstantBuffer(unsigned int cbufIndex);
			void* GetConstantBufferVariable(unsigned int cbufIndex, unsigned int varIndex);
			void* GetGlobalVariable(unsigned int varIndex);
			
			//! テクスチャリソース変数のインデックスを取得する。
			unsigned int GetTextureIndex(const char* texName) const;
			unsigned int GetTextureIndex(const char* texName, unsigned int indexInArray) const;
			unsigned int GetTextureArrayIndex(const char* texArrayName) const;
			//! テクスチャリソース変数にシェーダリソースビューを設定する。
			void SetTexture(unsigned int texIndex, ID3D10ShaderResourceView* pTextureView);
			void SetTexture(const char* texName, ID3D10ShaderResourceView* pTextureView);
			void SetTexture(const char* texName, unsigned int indexInArray, ID3D10ShaderResourceView* pTextureView);
			void SetTextureArray(unsigned int texArrayIndex, ID3D10ShaderResourceView* pTextureArrayView);
			void SetTextureArray(const char* texArrayName, ID3D10ShaderResourceView* pTextureArrayView);

			// サンプラーステートのインデックスを取得する。
			unsigned int GetSamplerStateIndex(const char* sampName) const;
			unsigned int GetSamplerStateIndex(const char* sampName, unsigned int indexInArray) const;
			//! サンプラーステートを設定する。
			void SetSamplerState(unsigned int sampIndex, ID3D10SamplerState* pSamplerState);
			void SetSamplerState(const char* sampName, ID3D10SamplerState* pSamplerState);
			void SetSamplerState(const char* sampName, unsigned int indexInArray, ID3D10SamplerState* pSamplerState);
			
			//! 頂点シェーダのバイトコードを取得する。
			ID3D10Blob* GetVertexShaderBytecode();
			const ID3D10Blob* GetVertexShaderBytecode() const { return const_cast<Shader*>(this)->GetVertexShaderBytecode(); }

		private:
			// シェーダオブジェクト。
			ID3D10VertexShader* _pVertexShader;
			ID3D10GeometryShader* _pGeometryShader;
			ID3D10PixelShader* _pPixelShader;
			ID3D10Blob* _pVertexShaderBytecode;
			ID3D10Blob* _pGeometryShaderBytecode;
			ID3D10Blob* _pPixelShaderBytecode;
			
			// シェーダ変数名とレジスタ番号のマップ。
			struct RegisterInfo
			{
				unsigned int _number;	//!< レジスタ番号。
				unsigned int _count;	//!< レジスタ数。静的配列(Texture2D t[n]等)の場合は、2以上の値が入る。
			};
			typedef std::map<std::string, RegisterInfo> ResourceRegisterMap;

			// 定数バッファ。定数バッファは Shader クラスが自前で作成し、保持する。
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
			
			std::vector<ID3D10Buffer*> _constantBuffers;	// 添字＝レジスタ番号。
			std::vector<ConstantBufferVariableTable> _constantBufferVariableTables;
			std::vector<ConstantBufferCache> _constantBufferCaches;
			
			// テクスチャリソース。
			ResourceRegisterMap _textureRegisterMap;
			std::vector<ID3D10ShaderResourceView*> _textures;	// 添字＝レジスタ番号。

			// サンプラーステート。
			ResourceRegisterMap _samplerRegisterMap;
			std::vector<ID3D10SamplerState*> _samplers;	// 添字＝レジスタ番号。

			// シェーダリソース適用関数。
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
