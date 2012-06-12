/*!
	@file	CRC.h
	
			CRC ���v�Z����֐��B�{�v���W�F�N�g�ł̎�ȗp�r�̓n�b�V���֐��B
	
	@author	E.Nakayama
*/
#pragma once
	
#include "Types.h"

namespace Marda
{
	/*!
		@brief	3�r�b�g����CRC���v�Z����B
				���̊֐��͕׋��p�B
				������������ 1101�B�r�b�g����͍�����i��MostSignificantBit First��������j�B
		@return	3�r�b�g��CRC�B���̓o�C�g�񂪋�ł������ꍇ�́A0��Ԃ��B
	*/
	uint8 MakeCRC3(const void* pData, size_t size);

	//! �e�[�u�����쐬����B
	void InitCRC3Table();
	
	//! �e�[�u���𗘗p�����������ŁB
	uint8 MakeCRC3t(const void* pData, size_t size);
	

	/*!
		@brief	8�r�b�g����CRC���v�Z����B
		@return	3�r�b�g��CRC�B���̓o�C�g�񂪋�ł������ꍇ�́A0��Ԃ��B
	*/
	uint8 MakeCRC8(const void* pData, size_t size);
	
	//! �e�[�u���𗘗p�����������ŁB
	uint8 MakeCRC8t(const void* pData, size_t size);
	
	//! �e�[�u�����쐬����B
	void InitCRC8Table();
	
	uint16 MakeCRC16(const void* pData, size_t size);
	
	uint16 MakeStrCRC16(const char* str);
	uint16 MakeStrCRC16(const wchar_t* str);
	
	uint32 MakeCRC32(const void* pData, size_t size);

	uint64 MakeCRC64(const void* pData, size_t size);

}	// end namespace Marda
