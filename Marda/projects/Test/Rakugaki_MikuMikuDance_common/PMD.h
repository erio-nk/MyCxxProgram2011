/*!
	@file	PMD.h

	@todo	�Ƃ肠�����f�[�^��ǂނ����B�ŏI�I�ɂ͉��炩�̌`�� Marda/Graphics �ɑg�ݍ��݂����B
	
	@author	E.Nakayama
*/
#pragma once
	
#include "Marda/Math/Vector.h"
	
namespace MMD
{
	struct PMD
	{
		#pragma pack(push,1)		// 1 �o�C�g�A���C�������g�B
		//! �w�b�_�B
		struct Header
		{
			char magic[3];			//!< �V�O�l�`���B"Pmd"
			float version;			//!< �o�[�W�����B
			char model_name[20];	//!< ���f�����B
			char comment[256];		//!< �R�����g�B
		};
		#pragma pack(pop)
		Header header;
			
		#pragma pack(push,1)
		//! ���_�f�[�^�B
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
			
		unsigned int vert_count;	//!< ���_���B
		std::vector<Vertex> vertex;	//!< ���_���X�g�B
			
		unsigned int face_vert_count;	//!< �ʒ��_�i���C���f�b�N�X�j���B
		std::vector<unsigned short> face_vert_index;	
			/*!<	�C���f�b�N�X���X�g�B
					�ʒ��_�́A����}�e���A�����Ƃɕ���ł���͗l�B
			*/
			
		#pragma pack(push,1)
		//! �}�e���A���f�[�^�B
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
			char texture_file_name[20];	//!< �e�N�X�`���t�@�C�����̎擾�ɂ� GetTextureFileName �֐����g�����B
			
			std::string GetTextureFileName() const;
			std::string GetSphereMapFileName() const;
		};
		#pragma pack(pop)
			
		unsigned int material_count;	//!< �}�e���A�����B
		std::vector<Material> material;
			/*!<	�}�e���A�����X�g�B	
					�}�e���A��������ʒ��_���Ƃɕ���ł���͗l�B
					���� Material::face_vert_count �����̃}�e���A����K�p����ʒ��_���Ɠ���B
					�܂�AMaterial ��񂪂Q����ł���A���ꂼ��� face_vert_count ��
					10, 20 �ł������ꍇ�A�C���f�b�N�X 0 �` 9 �܂ł͂P�ڂ̃}�e���A���A
					10 �` 39 �܂ł͂Q�ڂ̃}�e���A����K�p����Ηǂ��A�Ƃ������ɂȂ�B
			*/
			

		#pragma pack(push,1)
		//! �{�[���f�[�^�B
		struct Bone
		{
			enum TYPE
			{
				TYPE_ROTATION,	//!< 0 : ��]�B
				TYPE_ROTATION_TRANSLATION,	//!< 1 : ��]�^�ړ��B
				TYPE_IK_TARGET,		//!< 2 : IK�B��IK�̖ڕW�ɂȂ�ʒu�BMMD�ł́AIK�̃`�F�C���̈�Ԑ�[���^�[�Q�b�g�ƌĂԖ͗l�B�^�v���W�F�N�g���͂��̃{�[�����^�[�Q�b�g�ƌĂсAType6���G�t�F�N�^�ƌĂ�ł����B
				TYPE_UNKNOWN,	//!< 3 : �s���B
				TYPE_IK_CHAIN,	//!< 4 : IK�e�����i��]�j�B
				TYPE_5,			//!< 5 : ��]�e�����B
				TYPE_IK_EFFECTOR,	//!< 6 : IK�ڑ���B
				TYPE_INVISIBLE,	//!< 7 : ��\���B
				TYPE_8,			//!< 8 : ����B
				TYPE_9,			//!< 9 : ��]�A���B
			};

			static const unsigned short INVALID_PARENT = 0xffff;
			static const unsigned short INVALID_TAIL = 0;	//!< �ł���[�̃{�[���́A�S��0���ݒ肳��Ă���͗l�B
			char bone_name[20];
			unsigned short parent_bone_index;
			unsigned short tail_pos_bone_index;
			unsigned char bone_type;
			unsigned short ik_parent_bone_index;
			Marda::Vector3 bone_head_pos;
		};
		#pragma pack(pop)
		unsigned short bone_count;	//!< �{�[�����B
		std::vector<Bone> bone;
			
		#pragma pack(push,1)
		//! IK�f�[�^�B
		struct IKData
		{
			unsigned short ik_bone_index;
			unsigned short ik_target_bone_index;
			unsigned char ik_chain_length;
			unsigned short iterations;
			float control_weight;	// �l��1/4����Ă���ہH
			std::vector<unsigned short> ik_child_bone_index;
		};
		#pragma pack(pop)
		unsigned short ik_data_count;
		std::vector<IKData> ik_data;

		#pragma pack(push,1)
		//! �\��f�[�^�B
		struct Skin
		{
			enum TYPE
			{
				TYPE_BASE	= 0,	//!< ���\��B���̎������Askin_vert �́A���W���̂��̂��Ӗ�����B
				TYPE_EYEBROW= 1,	//!< ���B
				TYPE_EYE	= 2,	//!< �ځB
				TYPE_LIP	= 3,	//!< ���B
			};
			
			char skin_name[20];	//!< �\��B
			unsigned int skin_vert_count;	//!< ���_���B
			unsigned char skin_type;	//!< �\��^�C�v�B
			
			struct Vert
			{
				unsigned int index;	//!< skin_type �� TYPE_BASE �̏ꍇ�́A�Ή����钸�_�C���f�b�N�X�B�ȊO�̏ꍇ�́A TYPE_BASE �� skin_vert �̃C���f�b�N�X�B
				Marda::Vector3 pos;			//!< skin_type �� TYPE_BASE �̏ꍇ�́A���_���W�B�ȊO�̏ꍇ�́A�I�t�Z�b�g�B
			};
			std::vector<Vert> skin_vert;
		};
		#pragma pack(pop)
		unsigned short skin_count;	//!< �\��f�[�^���B
		std::vector<Skin> skin;
		
	};	// end struct PMD
	
	bool LoadPMD(PMD& pmd, const TCHAR* path);
		
		
}	// end namespace MMD

