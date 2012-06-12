/*!
	@file	VMD.h

	@todo	�Ƃ肠�����f�[�^��ǂނ����B�ŏI�I�ɂ͉��炩�̌`�� Marda/Graphics �ɑg�ݍ��݂����B
	
	@author	E.Nakayama
*/
#pragma once
	
#include "Marda/Math/Vector.h"
	
namespace MMD
{
	struct VMD
	{
		#pragma pack(push,1)		// 1 �o�C�g�A���C�������g�B
		//! �w�b�_�B
		struct Header
		{
			char VmdHeader[30];
			char VmdModelName[20];
		};
		#pragma pack(pop)
		Header header;
			
		#pragma pack(push,1)
		//! �{�[�����[�V�����̃L�[�t���[���f�[�^�B
		struct Motion
		{
			char BoneName[15];			//!< �{�[�����B
			unsigned int Frame;			//!< �t���[���ԍ��B
			Marda::Vector3 Location;	//!< ���[�J���ړ��l�B
			float Rotation[4];			//!< ���[�J����]�l�B
			unsigned char Interpolation[64];	
										/*!< 
											��ԃp�����[�^�B�ȉ������B
											��ԃp�����[�^��4�_�̃x�W�F�Ȑ�(0,0),(x1,y1),(x2,y2),(127,127)��
											�\���Ă���D�e���̃p�����[�^��
											X���̕�ԃp�����[�^�@(X_x1,X_y1),(X_x2,X_y2)
											Y���̕�ԃp�����[�^�@(Y_x1,Y_y1),(Y_x2,Y_y2)
											Z���̕�ԃp�����[�^�@(Z_x1,Z_y1),(Z_x2,Z_y2)
											��]�̕�ԃp�����[�^ (R_x1,R_y1),(R_x2,R_y2)
											�Ƃ������A��ԃp�����[�^�͈ȉ��̒ʂ�.
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
		//! �\��A�j���[�V�����̃L�[�t���[���f�[�^�B
		struct Skin
		{
			char SkinName[15];			//!< �\��B
			unsigned int Frame;			//!< �t���[���ԍ��B
			float Weight;				//!< �I�t�Z�b�g�Ɋ|����d�݁B
		};
		#pragma pack(pop)
		unsigned int skin_count;
		std::vector<Skin> skin;
			
	};	// end struct VMD
	
	bool LoadVMD(VMD& vmd, const TCHAR* path);
		
		
}	// end namespace MMD

