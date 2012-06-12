/*!
	@file	PMD.h

	@todo	とりあえずデータを読むだけ。最終的には何らかの形で Marda/Graphics に組み込みたい。
	
	@author	E.Nakayama
*/
#pragma once
	
#include "Marda/Math/Vector.h"
	
namespace MMD
{
	struct PMD
	{
		#pragma pack(push,1)		// 1 バイトアラインメント。
		//! ヘッダ。
		struct Header
		{
			char magic[3];			//!< シグネチャ。"Pmd"
			float version;			//!< バージョン。
			char model_name[20];	//!< モデル名。
			char comment[256];		//!< コメント。
		};
		#pragma pack(pop)
		Header header;
			
		#pragma pack(push,1)
		//! 頂点データ。
		struct Vertex
		{
			Marda::Vector3 pos;
			Marda::Vector3 normal_vec;
			Marda::Vector2 uv;
			unsigned short bone_num[2];
			unsigned char bone_weight;
			unsigned char edge_flag;
		};
		#pragma pack(pop)
			
		unsigned int vert_count;	//!< 頂点数。
		std::vector<Vertex> vertex;	//!< 頂点リスト。
			
		unsigned int face_vert_count;	//!< 面頂点（＝インデックス）数。
		std::vector<unsigned short> face_vert_index;	
			/*!<	インデックスリスト。
					面頂点は、同一マテリアルごとに並んでいる模様。
			*/
			
		#pragma pack(push,1)
		//! マテリアルデータ。
		struct Material
		{
			float diffuse_color[3];
			float alpha;
			float specularity;	// = shininess
			float specular_color[3];
			float mirror_color[3];	// = ambient
			unsigned char toon_index;
			unsigned char edge_flag;
			unsigned long face_vert_count;
			char texture_file_name[20];	//!< テクスチャファイル名の取得には GetTextureFileName 関数を使う事。
			
			std::string GetTextureFileName() const;
			std::string GetSphereMapFileName() const;
		};
		#pragma pack(pop)
			
		unsigned int material_count;	//!< マテリアル数。
		std::vector<Material> material;
			/*!<	マテリアルリスト。	
					マテリアルも同一面頂点ごとに並んでいる模様。
					かつ Material::face_vert_count がそのマテリアルを適用する面頂点数と同一。
					つまり、Material 情報が２つ並んでおり、それぞれの face_vert_count が
					10, 20 であった場合、インデックス 0 ～ 9 までは１つ目のマテリアル、
					10 ～ 39 までは２つ目のマテリアルを適用すれば良い、という事になる。
			*/
			

		#pragma pack(push,1)
		//! ボーンデータ。
		struct Bone
		{
			enum TYPE
			{
				TYPE_ROTATION,	//!< 0 : 回転。
				TYPE_ROTATION_TRANSLATION,	//!< 1 : 回転／移動。
				TYPE_IK_TARGET,		//!< 2 : IK。※IKの目標になる位置。MMDでは、IKのチェインの一番先端をターゲットと呼ぶ模様。某プロジェクト中はこのボーンをターゲットと呼び、Type6をエフェクタと呼んでいた。
				TYPE_UNKNOWN,	//!< 3 : 不明。
				TYPE_IK_CHAIN,	//!< 4 : IK影響下（回転）。
				TYPE_5,			//!< 5 : 回転影響下。
				TYPE_IK_EFFECTOR,	//!< 6 : IK接続先。
				TYPE_INVISIBLE,	//!< 7 : 非表示。
				TYPE_8,			//!< 8 : 捩り。
				TYPE_9,			//!< 9 : 回転連動。
			};

			static const unsigned short INVALID_PARENT = 0xffff;
			static const unsigned short INVALID_TAIL = 0;	//!< 最も先端のボーンは、全て0が設定されている模様。
			char bone_name[20];
			unsigned short parent_bone_index;
			unsigned short tail_pos_bone_index;
			unsigned char bone_type;
			unsigned short ik_parent_bone_index;
			Marda::Vector3 bone_head_pos;
		};
		#pragma pack(pop)
		unsigned short bone_count;	//!< ボーン数。
		std::vector<Bone> bone;
			
		#pragma pack(push,1)
		//! IKデータ。
		struct IKData
		{
			unsigned short ik_bone_index;
			unsigned short ik_target_bone_index;
			unsigned char ik_chain_length;
			unsigned short iterations;
			float control_weight;	// 値が1/4されてるっぽ？
			std::vector<unsigned short> ik_child_bone_index;
		};
		#pragma pack(pop)
		unsigned short ik_data_count;
		std::vector<IKData> ik_data;

		#pragma pack(push,1)
		//! 表情データ。
		struct Skin
		{
			enum TYPE
			{
				TYPE_BASE	= 0,	//!< 無表情時。この時だけ、skin_vert は、座標そのものを意味する。
				TYPE_EYEBROW= 1,	//!< 眉。
				TYPE_EYE	= 2,	//!< 目。
				TYPE_LIP	= 3,	//!< 口。
			};
			
			char skin_name[20];	//!< 表情名。
			unsigned int skin_vert_count;	//!< 頂点数。
			unsigned char skin_type;	//!< 表情タイプ。
			
			struct Vert
			{
				unsigned int index;	//!< skin_type が TYPE_BASE の場合は、対応する頂点インデックス。以外の場合は、 TYPE_BASE の skin_vert のインデックス。
				Marda::Vector3 pos;			//!< skin_type が TYPE_BASE の場合は、頂点座標。以外の場合は、オフセット。
			};
			std::vector<Vert> skin_vert;
		};
		#pragma pack(pop)
		unsigned short skin_count;	//!< 表情データ数。
		std::vector<Skin> skin;
		
	};	// end struct PMD
	
	bool LoadPMD(PMD& pmd, const TCHAR* path);
		
		
}	// end namespace MMD

