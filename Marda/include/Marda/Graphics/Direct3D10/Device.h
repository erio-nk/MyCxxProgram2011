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
			@brief	デバイス依存オブジェクトの生成／破棄を行う Facade クラス。
		*/
		class Device
		{
		public:
			typedef ID3D10Device NativeDevice;
			
			Device();
			~Device();
			
			//! デバイスの生成と初期化処理。
			bool Create(HWND hWnd);
			
			//! 終了処理。
			void Release();
			
			//! デバイスが削除されているかどうかを返す。
			bool IsDeviceRemoved() const;
			//! 描画領域が（ウィンドウ最小化等で）隠ぺいされているかどうかを返す。
			bool IsScreenOccluded() const;
			
			//! デバイスの生ポインタを返す。戻り値は必要無くなったら解放する必要がある。
			ID3D10Device* GetNativeDevice();
			//! コアオブジェクトを返す。クラスの単純利用者はこの関数を呼ぶべきでは無い。
			Core& GetCore();

			//! バックバッファの描画ターゲットハンドルを取得する。
			RenderTargetHandle GetBackBuffer() const;
			//! 深度ステンシルバッファのハンドルを取得する。
			DepthStencilBufferHandle GetDepthStencilBuffer() const;
			//! バックバッファと深度ステンシルバッファのサイズを変更する。ウィンドウのサイズが変更された場合、この関数を呼び出す必要がある。
			bool ResizeBackBuffer(SIZE sz);

			//! 描画ターゲットを作成する。
			RenderTargetHandle CreateRenderTarget(unsigned int width, unsigned int height, DXGI_FORMAT format, bool useShaderResource);
			DepthStencilBufferHandle CreateDepthStencilBuffer(unsigned int width, unsigned int height, bool useShaderResource);
			
			//! 入力レイアウトを作成する。
			InputLayoutHandle CreateInputLayout(D3D10_INPUT_ELEMENT_DESC* inputElementDescs, UINT numInputElements, ShaderHandle shaderHandle);
			//! 入力レイアウトを解放する。
			void ReleaseInputLayout(InputLayoutHandle handle);

			//! ブレンディングステートを作成する。
			BlendStateHandle CreateBlendState(const D3D10_BLEND_DESC& desc);
			//! ブレンディングステートを解放する。
			void ReleaseBlendState(BlendStateHandle handle);
			//! ブレンディングステートの詳細を取得する。
			const D3D10_BLEND_DESC* GetBlendStateDesc(BlendStateHandle handle) const;

			//! ラスタライザステートを作成する。
			RasterizerStateHandle CreateRasterizerState(const D3D10_RASTERIZER_DESC& desc);
			//! ラスタライザステートを解放する。
			void ReleaseRasterizerState(RasterizerStateHandle handle);
			//! ラスタライザステートの詳細を取得する。
			const D3D10_RASTERIZER_DESC* GetRasterizerStateDesc(RasterizerStateHandle handle) const;

			//! 深度ステンシルステートを作成する。
			DepthStencilStateHandle CreateDepthStencilState(const D3D10_DEPTH_STENCIL_DESC& desc);
			//! 深度ステンシルステートを解放する。
			void ReleaseDepthStencilState(DepthStencilStateHandle handle);
			//! 深度ステンシルステートの詳細を取得する。
			const D3D10_DEPTH_STENCIL_DESC* GetDepthStencilStateDesc(DepthStencilStateHandle handle) const;

			//! 頂点バッファを作成する。
			VertexBufferHandle CreateVertexBuffer(D3D10_USAGE usage, UINT CPUAccessFlags, size_t sizeVertex, size_t numVertices, void* pInitialData = NULL);
			//! 頂点バッファを解放する。
			void ReleaseVertexBuffer(VertexBufferHandle handle);
			//! 頂点バッファの詳細を取得する。
			const D3D10_BUFFER_DESC* GetVertexBufferDesc(VertexBufferHandle handle) const;
			//! 頂点バッファへのポインタを取得する。
			bool GetVertexBufferPtr(VertexBufferHandle handle, BufferPtr& bufferPtr, D3D10_MAP mapType, UINT mapFlags = 0);

			//! インデックスバッファを作成する。
			IndexBufferHandle CreateIndexBuffer(D3D10_USAGE usage, UINT CPUAccessFlags, size_t sizeIndex, size_t numIndices, void* pInitialData = NULL);
			//! インデックスバッファを解放する。
			void ReleaseIndexBuffer(IndexBufferHandle handle);
			//! インデックスバッファの詳細を取得する。
			const D3D10_BUFFER_DESC* GetIndexBufferDesc(IndexBufferHandle handle) const;
			//! インデックスバッファへのポインタを取得する。
			bool GetIndexBufferPtr(IndexBufferHandle handle, BufferPtr& bufferPtr, D3D10_MAP mapType, UINT mapFlags = 0);
			
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

			//! テクスチャを作成する。
			TextureHandle CreateTextureFromFile(const TCHAR* filePath);
			TextureHandle CreateTextureArrayFromFiles(const TCHAR** filePaths, unsigned int numFilePaths);

			//! サンプラーステートを作成する。
			SamplerStateHandle CreateSamplerState(const D3D10_SAMPLER_DESC& desc);
			//! サンプラーステートを解放する。
			void ReleaseSamplerState(SamplerStateHandle handle);
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
			
		private:
			std::auto_ptr<Core> _core;
		};	// end class Device

	}	// end namespace Direct3D
}	// end namespace Marda
