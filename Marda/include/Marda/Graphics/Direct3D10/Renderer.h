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
			@brief	描画に関する処理を持ったクラス。
					アプリケーションは本クラスを継承して拡張しても良い。
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
			
			//! フレームバッファの内容を画面に出力する。IsScreenOccluded が true を返す状態でも呼び出さなければならないが、その場合は実際の描画は行われない。
			HRESULT Present();
			
			//! デフォルトの描画ターゲットを取得する。
			RenderTargetHandle GetDefaultRenderTarget();
			//! デフォルトの深度ステンシルバッファを取得する。
			DepthStencilBufferHandle GetDefaultDepthStencilBuffer();

			//! 描画ターゲットを設定する。
			bool SetRenderTargets(RenderTargetHandle renderTargetHandle, DepthStencilBufferHandle depthStencilBufferHandle);
			//! デフォルトの描画ターゲットに設定する。
			void SetDefaultRenderTargets();
			//! 現在の描画ターゲットをクリアする。
			void Clear(bool clearColor, bool clearDepth, bool clearStencil, const Color& color = Color::BLACK, float depth = DEPTH_MAX, uint8 stencil = 0);
			
			//! ビューポートを設定する。
			void SetViewport(SIZE sz);

			//! 入力レイアウトを設定する。
			void SetInputLayout(InputLayoutHandle handle);
			//! 入力レイアウトの設定をクリアする。
			void ClearInputLayout();

			//! ブレンディングステートを設定する。
			void SetBlendState(BlendStateHandle handle);
			//! デフォルトのブレンディングステートを設定する。
			void SetDefaultBlendState();
			//! デフォルトのブレンディングステートを取得する。
			BlendStateHandle GetDefaultBlendState() const;

			//! ラスタライザステートを設定する。
			void SetRasterizerState(RasterizerStateHandle handle);
			//! デフォルトのラスタライザステートを設定する。
			void SetDefaultRasterizerState();
			//! デフォルトのラスタライザステートを取得する。
			RasterizerStateHandle GetDefaultRasterizerState() const;

			//! 深度ステンシルステートを設定する。
			void SetDepthStencilState(DepthStencilStateHandle handle);
			//! デフォルトの深度ステンシルステートを設定する。
			void SetDefaultDepthStencilState();
			//! デフォルトの深度ステンシルステートを取得する。
			DepthStencilStateHandle GetDefaultDepthStencilState() const;

			//! 頂点バッファを設定する。
			void SetVertexBuffer(VertexBufferHandle handle);
			void SetVertexBuffer(unsigned int slot, VertexBufferHandle handle);
			//! 頂点バッファの内容を更新する。
			void UpdateVertexBuffer(VertexBufferHandle handle, void* pNewData);
			//! インデックスバッファを設定する。
			void SetIndexBuffer(IndexBufferHandle handle);
			
			//! シェーダを設定する。
			void SetShader(ShaderHandle handle);
			//! シェーダの設定をクリアする。
			void ClearShader();
			//! 現在設定されているシェーダとシェーダリソースをデバイスに適用する。
			void ApplyShader();

			//! シェーダの定数バッファ名からインデックスを取得する。
			unsigned int GetShaderConstantBufferIndex(ShaderHandle handle, const char* cbufName) const;
			unsigned int GetShaderConstantBufferVariableIndex(ShaderHandle handle, unsigned int cbufIndex, const char* varName)  const;
			unsigned int GetShaderConstantBufferVariableIndex(ShaderHandle handle, const char* cbufName, const char* varName) const;
			unsigned int GetShaderGlobalVariableIndex(ShaderHandle handle, const char* varName) const;
			//! シェーダの定数バッファ（のキャッシュへのポインタ）を取得する。
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

			//! シェーダのテクスチャ変数にテクスチャを設定する。
			void SetShaderTexture(ShaderHandle shaderHandle, unsigned int texIndex, TextureHandle textureHandle);
			void SetShaderTexture(ShaderHandle shaderHandle, const char* texName, TextureHandle textureHandle);
			void SetShaderTexture(ShaderHandle shaderHandle, const char* texName, unsigned int indexInArray, TextureHandle textureHandle);
			void SetShaderTextureArray(ShaderHandle shaderHandle, unsigned int texArrayIndex, TextureHandle textureArrayHandle);
			void SetShaderTextureArray(ShaderHandle shaderHandle, const char* texArrayName, TextureHandle textureArrayHandle);
			//! シェーダのテクスチャ変数に描画ターゲットを設定する。
			void SetRenderTargetToShaderTexture(ShaderHandle shaderHandle, unsigned int texIndex, RenderTargetHandle renderTargetHandle);
			void SetRenderTargetToShaderTexture(ShaderHandle shaderHandle, const char* texName, RenderTargetHandle renderTargetHandle);
			//! シェーダのテクスチャ変数に深度ステンシルバッファを設定する。
			void SetDepthStencilBufferToShaderTexture(ShaderHandle shaderHandle, unsigned int texIndex, DepthStencilBufferHandle depthStencilBufferHandle);
			void SetDepthStencilBufferToShaderTexture(ShaderHandle shaderHandle, const char* texName, DepthStencilBufferHandle depthStencilBufferHandle);

			//! シェーダのサンプラーステート変数にサンプラーステートを設定する。
			void SetShaderSamplerState(ShaderHandle shaderHandle, unsigned int sampIndex, SamplerStateHandle samplerHandle);
			void SetShaderSamplerState(ShaderHandle shaderHandle, const char* sampName, SamplerStateHandle samplerHandle);
			void SetShaderSamplerState(ShaderHandle shaderHandle, const char* sampName, unsigned int indexInArray, SamplerStateHandle samplerHandle);

			//! デフォルトのフォントを取得する。
			FontHandle GetDefaultFont() const;
			//! テキストを描画する。（ID3DX10Fontを利用した）この関数を呼び出すと、内部で各種ステートが上書きされてしまう為注意。
			void DrawText(FontHandle handle, int x, int y, const Color& color, const TCHAR* format, ...);
			//! デフォルトフォントでテキストを描画する。
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
