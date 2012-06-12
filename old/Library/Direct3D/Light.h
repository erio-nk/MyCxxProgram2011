/*!
 	@file Direct3D/Light.h
 
 	ライトクラス。

	ライトの面倒な手続きを
	メソッドに纏めただけのラッパークラス。
 
 	@author Dila
 */

#include "Direct3D/Device.h"

namespace Direct3D
{
	//! @addtogroup Direct3D Direct3D
	//! @{

	/*!
	 	ディレクショナルライトクラス
	 */
	class DirectionalLight
	{
		D3DLIGHT _light;	//! ライト

	public:
		//! コンストラクタ
		DirectionalLight();

		//! デストラクタ
		~DirectionalLight();

		//! ライトを取得する
		D3DLIGHT& GetLight() { return _light; }
		const D3DLIGHT& GetLight() const { return _light; }

		/*!
			ライトの方向を設定する
			@param	x	x方向
			@param	y	y方向
			@param	z	z方向
		 */
		void SetDirection(float x, float y, float z);

		/*!
			ライトの座標を設定する
			@param	x	x座標
			@param	y	y座標
			@param	z	z座標
		 */
		void SetPosition(float x, float y, float z);

		/*!
			ライトのディフューズ色を設定する
		 */
		void SetDiffuseColor(float r, float g, float b, float a = 1.0f);

		/*!
			ライトのアンビエント色を設定する
		 */
		void SetAmbientColor(float r, float g, float b, float a = 1.0f);

		/*!
			ライトの範囲を設定する
			@param	range	範囲
		 */
		void SetRange(float range);

	};	// end class DirectionalLight

	//! @}
}	// end namespace Direct3D


