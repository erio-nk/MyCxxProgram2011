/*!
 	@file Direct3D/Texture.h
 
 	テクスチャクラス。

	テクスチャの面倒な手続きを
	メソッドに纏めただけのラッパークラス。
 
 	@author Dila
 */

#include "Direct3D/Device.h"

namespace Direct3D
{
	//! @addtogroup Direct3D Direct3D
	//! @{

	/*!
	 	テクスチャクラス

		ベーシックなテクスチャに関する手続きを纏めたラッパークラス。
		@todo	実装する。
	 */
	class Texture
	{
		IDirect3DTexture* _pTexture;	//! テクスチャ

	public:
		Texture();

		~Texture();

		bool Create(IDirect3DDevice* pDevice);
		bool CreateFromFile(const TCHAR* pszFilePath);

		IDirect3DTexture* GetTexture() { return _pTexture; }

	};	// end class Texture


	/*!
		レンダリング用のテクスチャクラス

		テクスチャはデバイス直の関数で作らずに、
		このクラスのCreateで作る事。
	 */
	class RenderTexture
	{
		IDirect3DDevice* _pDevice;		//! デバイス
		IDirect3DTexture* _pTexture;	//! テクスチャ
		IDirect3DSurface* _pSurface;	//! サーフェイス

		friend class BeginRender;
	public:
		RenderTexture();

		~RenderTexture();

		bool Create(IDirect3DDevice* pDevice, float w, float h);

		void Release();

		IDirect3DTexture* GetTexture() { return _pTexture; }



		/*!
			テクスチャレンダリング開始クラス
			@code
				RenderTexture texture;
				...
				{
					RenderTexture::BeginRender begin(texture);	// テクスチャレンダリング開始

					...	// 各種レンダリング

					// スコープを抜けるとレンダーターゲットが元に戻る
				}
			@endcode
		 */
		class BeginRender
		{
			RenderTexture* _pTexture;		//! レンダーターゲット
			IDirect3DSurface* _pBackBuffer;	//! バックバッファ
		public:
			BeginRender(RenderTexture& texture) :
				_pTexture(&texture)
			{
				// レンダーターゲット切り替え
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
					// レンダーターゲットを元に戻す
					_pTexture->_pDevice->SetRenderTarget(0, _pBackBuffer);
					_pTexture = NULL;
					_pBackBuffer->Release();
				}
			}
		};	// end class BeginRender

	};	// end class RenderableTexture
    
	//! @}
}	// end namespace Texture