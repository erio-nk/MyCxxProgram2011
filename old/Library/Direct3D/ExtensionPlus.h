/*!
 	@file Direct3D/ExtensionPlus.h
 
 	D3DXに無くてちょっと欲しくなるような関数。
 
 	@author Dila
 */


#pragma once

#include "Direct3D/Device.h"	// TODO:別のヘッダのがいいかどうか要検討

namespace D3DXPlus
{
	//! @addtogroup Direct3D Direct3D
	//! @{

	// メッシュ	------------------------------------------------------------ //

	/*!
 		板ポリメッシュ作成関数
 		@param	pDevice	Direct3Dデバイス
 		@param	width	板ポリの横幅(x軸方向の長さ)
 		@param	height	板ポリの縦幅(z軸方向の長さ)
 		@param[out]	ppMesh	作成されたメッシュ
	*/
	HRESULT CreateBoard(IDirect3DDevice* pDevice, float width, float height, LPD3DXMESH* ppMesh);


	// 描画	---------------------------------------------------------------- //

	/*!
 		線分を描く関数
 		@param	pDevice	Direct3Dデバイスインタフェース
		@param	v1		線分の始点座標
		@param	v2		線分の終点座標
		@param	color1	始点の頂点カラー
		@param	color2	終点の頂点カラー
	*/
	HRESULT DrawLine(IDirect3DDevice* pDevice, const D3DXVECTOR3& v1, const D3DXVECTOR3& v2, DWORD color1, DWORD color2);


	// TODO: サクッとテキストをレンダリングする関数


	// 算術	---------------------------------------------------------------- //

	/*!
 		ベクトルを向きとした回転行列を取得する
 		@param[out]	pOut		回転行列
 		@param		pDirection	向きとするベクトル
		@return		pOutで指定した行列のポインタ。
					戻り値をそのまま関数の引数に使用出来る。
	*/
	D3DXMATRIX* MatrixRotationVector(D3DXMATRIX* pOut, const D3DXVECTOR3* pvDirection);
	
	/*!
		ベクトルを向きとした回転クォータニオンを取得する
		@param[out]	pOut		回転クォータニオン
		@param		pvDirection	向きとするベクトル
		@return		pOutで指定したクォータニオンのポインタ。
					戻り値をそのまま関数の引数に使用出来る。
		@todo		未実装なので実装する。
	*/
	D3DXQUATERNION* QuaternionRotationVector(D3DXQUATERNION* pOut, const D3DXVECTOR3* pvDirection);

	/*!
		回転クォータニオンから向きを表すベクトルを取得する
		@param[out]	pOut		向きを表すベクトル
		@param		pqRotation	回転クォータニオン
		@return		pOutで指定したクベクトルのポインタ。
					戻り値をそのまま関数の引数に使用出来る。
		@todo		未実装なので実装する。(0,0,1)のベクトルを回転クォータニオンで回転させたものを出力する。
	 */
	D3DXVECTOR3* Vector3DirectionQuaternion(D3DXVECTOR3* pOut, const D3DXQUATERNION* pqRotation);

	/*!
		ベクトルを回転クォータニオンで回転させる
		@param[out]	pOut		回転後のベクトル
		@param		pqRotation	回転クォータニオン
		@return		pOutで指定したクベクトルのポインタ。
					戻り値をそのまま関数の引数に使用出来る。
		@todo		未実装なので実装する。
	 */
	D3DXVECTOR3* Vector3TransformQuaternion(D3DXVECTOR3* pOut, const D3DXQUATERNION* pqRotation);


	// 変換行列	-------------------------------------------------------- //

	/*!
		ビューポート変換する行列を取得する
		(プロジェクション変換したベクトルをスクリーン座標に変換する行列を取得する)
		@param[out]	pOut	変換行列
		@param		cx		スクリーンの横幅
		@param		cy		スクリーンの縦幅
		@return		pOutで指定した行列のポインタ。
					戻り値をそのまま関数の引数に使用出来る。
	*/
	D3DXMATRIX* MatrixViewport(D3DXMATRIX* pOut, float cx, float cy);


	/*!
		ビルボードを作る為の姿勢変換行列を取得する
		@param[out]	pOut			変換行列
		@param		pmViewMatrix	ビュー変換行列
		@return		pOutで指定した行列のポインタ。
					戻り値をそのまま関数の引数に使用出来る。
	 */
	D3DXMATRIX* MatrixBillboard(D3DXMATRIX* pOut, const D3DMATRIX* pmViewMatrix);

	//! @}
}
