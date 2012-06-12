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
		//! 描画ターゲットのオブジェクトホルダー。
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
				// std::vector<RenderTarget> でコピーが発生した時に解放されてしまう為、ここに Release の呼び出しは書かない。
			}

			void Release()
			{
				safe_release(_pRenderTargetView);
				safe_release(_pShaderResourceView);
			}
		
		};	// end struct RenderTarget
		
		//! 深度ステンシルバッファのオブジェクトホルダー。
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
				// std::vector<DepthStencilBuffer> でコピーが発生した時に解放されてしまう為、ここに Release の呼び出しは書かない。
			}

			void Release()
			{
				safe_release(_pDepthStencilView);
				safe_release(_pShaderResourceView);
			}
		
		};	// end struct DepthStencilBuffer
		

		//! 頂点バッファの情報＆オブジェクトホルダー。
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
				// std::vector<VertexBuffer> でコピーが発生した時に解放されてしまう為、ここに Release の呼び出しは書かない。
			}

			void Release()
			{
				safe_release(_pInterface);
			}
		
		};	// end struct VertexBuffer

		//! インデックスバッファの情報＆オブジェクトホルダー。
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
				// std::vector<IndexBuffer> でコピーが発生した時に解放されてしまう為、ここに Release の呼び出しは書かない。
			}

			void Release()
			{
				safe_release(_pInterface);
			}
		
		};	// end struct IndexBuffer
	
		//! フォントの情報＆オブジェクトホルダー。
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
				// std::vector<Font> でコピーが発生した時に解放されてしまう為、ここに Release の呼び出しは書かない。
			}
		
			void Release()
			{
				safe_release(_pInterface);
			}
		};	// end struct Font


		/*!
			@brief	Direct3D10関する機能をひとまとめにしたコアクラス。
					デバイス依存オブジェクトはこのクラスで保持され、ハンドルベースでやり取りを行う。
					このクラスだけでも Direct3D10 の利用は可能だが、基本的には Device / Renderer 経由で利用する事。
		*/
		class Core
		{
		public:
			typedef ID3D10Device NativeDevice;
			
			Core();
			~Core();
			
			//! デバイスの生成と初期化処理。
			bool Create(HWND hWnd);
			
			//! 終了処理。
			void Release();
			
			//! フレームバッファの内容を画面に出力する。IsScreenOccluded が true を返す状態でも呼び出さなければならないが、その場合は実際の描画は行われない。
			HRESULT Present();

			//! デバイスが削除されているかどうかを返す。
			bool IsDeviceRemoved() const;
			//! 描画領域が（ウィンドウ最小化等で）隠ぺいされているかどうかを返す。
			bool IsScreenOccluded() const;
			
			//! デバイスの生ポインタを返す。戻り値は必要無くなったら解放する必要がある。
			ID3D10Device* GetNativeDevice();
			
			//! バックバッファの描画ターゲットハンドルを取得する。
			RenderTargetHandle GetBackBuffer() const;
			//! 深度ステンシルバッファのハンドルを取得する。
			DepthStencilBufferHandle GetDepthStencilBuffer() const;
			//! バックバッファと深度ステンシルバッファのサイズを変更する。ウィンドウのサイズが変更された場合、この関数を呼び出す必要がある。
			bool ResizeBackBuffer(SIZE sz);
			
			//! ビューポートを設定する。
			void SetViewport(SIZE sz);

			//! 描画ターゲットを作成する。
			RenderTargetHandle CreateRenderTarget(unsigned int width, unsigned int height, DXGI_FORMAT format, bool useShaderResource);
			DepthStencilBufferHandle CreateDepthStencilBuffer(unsigned int width, unsigned int height, bool useShaderResource);
			//! 描画ターゲットを設定する。
			bool SetRenderTargets(RenderTargetHandle renderTargetHandle, DepthStencilBufferHandle depthStencilBufferHandle);
			//! 描画ターゲットをクリアする。
			void ClearRenderTarget(RenderTargetHandle handle, const Color& color);
			void ClearDepthStencil(DepthStencilBufferHandle handle, DWORD clearDepthStencilFlag, float depth, uint8 stencil);

			//! 入力レイアウトを作成する。
			InputLayoutHandle CreateInputLayout(D3D10_INPUT_ELEMENT_DESC* inputElementDescs, UINT numInputElements, ShaderHandle shaderHandle);
			//! 入力レイアウトを解放する。
			void ReleaseInputLayout(InputLayoutHandle handle);
			//! 入力レイアウトを設定する。
			void SetInputLayout(InputLayoutHandle handle);
			//! 入力レイアウトの設定をクリアする。
			void ClearInputLayout();

			//! ブレンディングステートを作成する。
			BlendStateHandle CreateBlendState(const D3D10_BLEND_DESC& desc);
			//! ブレンディングステートを解放する。
			void ReleaseBlendState(BlendStateHandle handle);
			//! ブレンディングステートを設定する。
			void SetBlendState(BlendStateHandle handle);
			//! ブレンディングステートの詳細を取得する。
			const D3D10_BLEND_DESC* GetBlendStateDesc(BlendStateHandle handle) const;

			//! ラスタライザステートを作成する。
			RasterizerStateHandle CreateRasterizerState(const D3D10_RASTERIZER_DESC& desc);
			//! ラスタライザステートを解放する。
			void ReleaseRasterizerState(RasterizerStateHandle handle);
			//! ラスタライザステートを設定する。
			void SetRasterizerState(RasterizerStateHandle handle);
			//! ラスタライザステートの詳細を取得する。
			const D3D10_RASTERIZER_DESC* GetRasterizerStateDesc(RasterizerStateHandle handle) const;

			//! 深度ステンシルステートを作成する。
			DepthStencilStateHandle CreateDepthStencilState(const D3D10_DEPTH_STENCIL_DESC& desc);
			//! 深度ステンシルステートを解放する。
			void ReleaseDepthStencilState(DepthStencilStateHandle handle);
			//! 深度ステンシルステートを設定する。
			void SetDepthStencilState(DepthStencilStateHandle handle);
			//! 深度ステンシルステートの詳細を取得する。
			const D3D10_DEPTH_STENCIL_DESC* GetDepthStencilStateDesc(DepthStencilStateHandle handle) const;

			//! 頂点バッファを作成する。
			VertexBufferHandle CreateVertexBuffer(D3D10_USAGE usage, UINT CPUAccessFlags, size_t sizeVertex, size_t numVertices, void* pInitialData);
			//! 頂点バッファを解放する。
			void ReleaseVertexBuffer(VertexBufferHandle handle);
			//! 頂点バッファの詳細を取得する。
			const D3D10_BUFFER_DESC* GetVertexBufferDesc(VertexBufferHandle handle) const;
			//! 頂点バッファへのポインタを取得する。
			bool GetVertexBufferPtr(VertexBufferHandle handle, BufferPtr& bufferPtr, D3D10_MAP mapType, UINT mapFlags = 0);
			//! 頂点バッファを設定する。
			void SetVertexBuffer(unsigned int slot, VertexBufferHandle handle);
			//! 頂点バッファの内容を更新する。
			void UpdateVertexBuffer(VertexBufferHandle handle, void* pNewData);

			//! インデックスバッファを作成する。
			IndexBufferHandle CreateIndexBuffer(D3D10_USAGE usage, UINT CPUAccessFlags, size_t sizeIndex, size_t numIndices, void* pInitialData = NULL);
			//! インデックスバッファを解放する。
			void ReleaseIndexBuffer(IndexBufferHandle handle);
			//! インデックスバッファの詳細を取得する。
			const D3D10_BUFFER_DESC* GetIndexBufferDesc(IndexBufferHandle handle) const;
			//! インデックスバッファへのポインタを取得する。
			bool GetIndexBufferPtr(IndexBufferHandle handle, BufferPtr& bufferPtr, D3D10_MAP mapType, UINT mapFlags = 0);
			//! インデックスバッファを設定する。
			void SetIndexBuffer(IndexBufferHandle handle);
			
			//! シェーダを作成する。ジオメトリシェーダが不要な場合は、geometryShaderFunction に NULL を渡す事が出来る。
			ShaderHandle CreateShader(
				const TCHAR* path,
				const char* vertexShaderFunction,
				const char* vertexShaderProfile,
				const char* geometryShaderFunction,
				const char* geometryShaderProfile,
				const char* pixelShaderFunction,
				const char* pixelShaderProfile
				);
			//! シェーダを解放する。
			void ReleaseShader(ShaderHandle handle);
			//! シェーダの設定をクリアする。
			void ClearShader();
			//! シェーダとシェーダリソースをデバイスに適用する。
			void ApplyShader(ShaderHandle handle);

			//! シェーダの定数バッファ名からインデックスを取得する。
			unsigned int GetShaderConstantBufferIndex(ShaderHandle handle, const char* cbufName) const;
			unsigned int GetShaderConstantBufferVariableIndex(ShaderHandle handle, unsigned int cbufIndex, const char* varName)  const;
			unsigned int GetShaderGlobalVariableIndex(ShaderHandle handle, const char* varName) const;
			//! シェーダの定数バッファ（のキャッシュへのポインタ）を取得する。
			void* GetShaderConstantBuffer(ShaderHandle handle, unsigned int cbufIndex);
			void* GetShaderConstantBufferVariable(ShaderHandle handle, unsigned int cbufIndex, unsigned int varIndex);
			void* GetShaderGlobalVariable(ShaderHandle handle, unsigned int varIndex);

			//! テクスチャを作成する。
			TextureHandle CreateTextureFromFile(const TCHAR* filePath);
			TextureHandle CreateTextureArrayFromFiles(const TCHAR** filePaths, unsigned int numFilePaths);
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

			//! サンプラーステートを作成する。
			SamplerStateHandle CreateSamplerState(const D3D10_SAMPLER_DESC& desc);
			//! サンプラーステートを解放する。
			void ReleaseSamplerState(SamplerStateHandle handle);
			//! シェーダのサンプラーステート変数にサンプラーステートを設定する。
			void SetShaderSamplerState(ShaderHandle shaderHandle, unsigned int sampIndex, SamplerStateHandle samplerHandle);
			void SetShaderSamplerState(ShaderHandle shaderHandle, const char* sampName, SamplerStateHandle samplerHandle);
			void SetShaderSamplerState(ShaderHandle shaderHandle, const char* sampName, unsigned int indexInArray, SamplerStateHandle samplerHandle);
			//! サンプラーステートの詳細を取得する。
			const D3D10_SAMPLER_DESC* GetSamplerStateDesc(SamplerStateHandle handle) const;
			
			//! フォントを作成する。
			FontHandle CreateFont(const FontDesc& desc);
			//! フォントを解放する。
			void ReleaseFont(FontHandle handle);
			//! フォントの詳細を取得する。
			const FontDesc* GetFontDesc(FontHandle handle) const;
			//! フォントの高さを取得する。
			INT GetFontHeight(FontHandle handle) const;
			//! テキストを描画する。（ID3DX10Fontを利用した）この関数を呼び出すと、内部で各種ステートが上書きされてしまう為注意。
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
