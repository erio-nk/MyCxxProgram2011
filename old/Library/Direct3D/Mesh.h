/*!
 	@file Direct3D/Mesh.h
 
 	メッシュクラス。
 
 	@author Dila
 */

#include "Direct3D/Device.h"

namespace Direct3D
{
	//! @addtogroup Direct3D Direct3D
	//! @{

	/*!
	 	メッシュクラス
	 
	 	メッシュとマテリアルの面倒な手続きを
	 	メソッドに纏めただけのラッパークラス。
	 */
	class Mesh
	{
		LPD3DXMESH _pMesh;	//! メッシュ
		D3DMATERIAL _Material; 	//! マテリアル

	public:
		//! コンストラクタ
		Mesh();

		//! デストラクタ
		~Mesh();

		//! リソースを解放する
		void Release();

		//! メッシュを取得する
		LPD3DXMESH& GetMesh() { return _pMesh; }
		const LPD3DXMESH& GetMesh() const { return _pMesh; }

		//! マテリアルを取得する
		D3DMATERIAL& GetMaterial() { return _Material; }
		const D3DMATERIAL& GetMaterial() const { return _Material; }

		/*!
			メッシュのディフューズ色を設定する
		 */
		void SetDiffuseColor(float r, float g, float b, float a = 1.0f);

		/*!
			メッシュのディフューズ色を取得する
		 */
		D3DCOLOR GetDiffuseColor()
		{
			return D3DCOLOR_ARGB(
				(unsigned char)(_Material.Diffuse.a*255),
				(unsigned char)(_Material.Diffuse.r*255),
				(unsigned char)(_Material.Diffuse.g*255),
				(unsigned char)(_Material.Diffuse.b*255)
				);
		}

		/*!
			メッシュのアンビエント色を設定する
		 */
		void SetAmbientColor(float r, float g, float b, float a = 1.0f);

		/*!
			メッシュのアンビエント色を取得する
		 */
		D3DCOLOR GetAmbientColor()
		{
			return D3DCOLOR_ARGB(
				(unsigned char)(_Material.Ambient.a*255),
				(unsigned char)(_Material.Ambient.r*255),
				(unsigned char)(_Material.Ambient.g*255),
				(unsigned char)(_Material.Ambient.b*255)
				);
		}


		/*!
			メッシュを描画する
		 */
		HRESULT Render(Direct3D::Device& device);

	};	// end class Mesh

	//! @}
}	// end namespace Direct3D


