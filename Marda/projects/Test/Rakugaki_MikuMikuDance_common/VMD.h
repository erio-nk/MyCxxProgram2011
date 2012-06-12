/*!
	@file	VMD.h

	@todo	とりあえずデータを読むだけ。最終的には何らかの形で Marda/Graphics に組み込みたい。
	
	@author	E.Nakayama
*/
#pragma once
	
#include "Marda/Math/Vector.h"
	
namespace MMD
{
	struct VMD
	{
		#pragma pack(push,1)		// 1 バイトアラインメント。
		//! ヘッダ。
		struct Header
		{
			char VmdHeader[30];
			char VmdModelName[20];
		};
		#pragma pack(pop)
		Header header;
			
		#pragma pack(push,1)
		//! ボーンモーションのキーフレームデータ。
		struct Motion
		{
			char BoneName[15];			//!< ボーン名。
			unsigned int Frame;			//!< フレーム番号。
			Marda::Vector3 Location;	//!< ローカル移動値。
			float Rotation[4];			//!< ローカル回転値。
			unsigned char Interpolation[64];	
										/*!< 
											補間パラメータ。以下抜粋。
											補間パラメータは4点のベジェ曲線(0,0),(x1,y1),(x2,y2),(127,127)で
											表している．各軸のパラメータを
											X軸の補間パラメータ　(X_x1,X_y1),(X_x2,X_y2)
											Y軸の補間パラメータ　(Y_x1,Y_y1),(Y_x2,Y_y2)
											Z軸の補間パラメータ　(Z_x1,Z_y1),(Z_x2,Z_y2)
											回転の補間パラメータ (R_x1,R_y1),(R_x2,R_y2)
											とした時、補間パラメータは以下の通り.
											X_x1,Y_x1,Z_x1,R_x1,X_y1,Y_y1,Z_y1,R_y1,
											X_x2,Y_x2,Z_x2,R_x2,X_y2,Y_y2,Z_y2,R_y2,
											Y_x1,Z_x1,R_x1,X_y1,Y_y1,Z_y1,R_y1,X_x2,
											Y_x2,Z_x2,R_x2,X_y2,Y_y2,Z_y2,R_y2, 01,
											Z_x1,R_x1,X_y1,Y_y1,Z_y1,R_y1,X_x2,Y_x2,
											Z_x2,R_x2,X_y2,Y_y2,Z_y2,R_y2, 01, 00,
											R_x1,X_y1,Y_y1,Z_y1,R_y1,X_x2,Y_x2,Z_x2,
											R_x2,X_y2,Y_y2,Z_y2,R_y2, 01, 00, 00
										*/
		};
		#pragma pack(pop)
		unsigned int motion_count;
		std::vector<Motion> motion;

		#pragma pack(push,1)
		//! 表情アニメーションのキーフレームデータ。
		struct Skin
		{
			char SkinName[15];			//!< 表情名。
			unsigned int Frame;			//!< フレーム番号。
			float Weight;				//!< オフセットに掛ける重み。
		};
		#pragma pack(pop)
		unsigned int skin_count;
		std::vector<Skin> skin;
			
	};	// end struct VMD
	
	bool LoadVMD(VMD& vmd, const TCHAR* path);
		
		
}	// end namespace MMD

