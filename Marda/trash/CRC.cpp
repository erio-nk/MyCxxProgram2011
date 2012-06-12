/*!
	@file	CRC.cpp

	@author	E.Nakayama
*/

#include "Marda/CRC.h"

namespace Marda
{
	#if defined(_DEBUG)
	#define CRC_TRACE DEBUG_TRACE
	#else
	#define CRC_TRACE(...) __noop
	#endif
	
	static const uint32 BITS_BYTE = 8;	//!< 1�o�C�g���̃r�b�g���B
	
	inline bool GetBitInBytesLSB(const uint8* bytes, size_t numBytes, unsigned int indexBits, uint32& out)
	{
		unsigned int indexBytesInArray = indexBits / BITS_BYTE;
		if (indexBytesInArray >= numBytes)
			return false;
		unsigned int indexBitsInByte = indexBits % BITS_BYTE;
		out = (bytes[indexBytesInArray] >> indexBitsInByte) & 1;
		return true;
	} 

	inline bool GetBitInBytesMSB(const uint8* bytes, size_t numBytes, unsigned int indexBits, uint32& out)
	{
		unsigned int indexBytesInArray = indexBits / BITS_BYTE;
		if (indexBytesInArray >= numBytes)
			return false;
		unsigned int indexBitsInByte = (BITS_BYTE-1) - (indexBits % BITS_BYTE);
		out = (bytes[indexBytesInArray] >> indexBitsInByte) & 1;
		return true;
	}
	
	inline bool GetBitsInBytesLSB(const uint8* bytes, size_t numBytes, unsigned int indexBits, unsigned int numBits, uint32& out)
	{
		out = 0;
		for (; numBits > 0; --numBits, ++indexBits)
		{
			uint32 bit;
			if (!GetBitInBytesLSB(bytes, numBytes, indexBits, bit))
				return false;
			out |= bit << (numBits - 1);
		}
		return true;
	} 

	inline bool GetBitsInBytesMSB(const uint8* bytes, size_t numBytes, unsigned int indexBits, unsigned int numBits, uint32& out)
	{
		out = 0;
		for (; numBits > 0; --numBits, ++indexBits)
		{
			uint32 bit;
			if (!GetBitInBytesMSB(bytes, numBytes, indexBits, bit))
				return false;
			out |= bit << (numBits - 1);
		}
		return true;
	} 
	
//	inline void GetIndicesLSB(unsigned int indexBits, unsigned int& indexBytesInArray, unsigned int& indexBitsInByte)
//	{
//		indexBytesInArray = indexBits / BITS_BYTE;
//		indexBitsInByte = indexBits % BITS_BYTE;
//	}
//	
//	inline void GetIndicesMSB(unsigned int indexBits, unsigned int& indexBytesInArray, unsigned int& indexBitsInByte)
//	{
//		indexBytesInArray = indexBits / BITS_BYTE;
//		indexBitsInByte = (BITS_BYTE-1) - (indexBits % BITS_BYTE);	// �ŏ�ʃr�b�g������́iMSB�j���Ă���̂ŁAi%8��0���o���Ȃ�A�����7�ɂ���B
//	}

	// ------------------------------------------------------------------------
	// 3�r�b�g��CRC�B
	
	static const uint32 BITS_CRC3		= 3;
	
#if 0
	static const uint32 BITS_DIV_CRC3	= 4;	//!< �����̃r�b�g���B
	static const uint32 DIV_CRC3		= 0xd;	//!< = 1101 = ���� = ����������(generating polynomial)
	static const uint32 MSB_CRC3		= 1 << (BITS_DIV_CRC3 - 1);	//!< �v�Z���Ɏg�p����擪�r�b�g�̃}�X�N�l�B�v�Z����4�r�b�g�Ōv�Z���Ă���̂ŁA���̒l��3�r�b�g�ڂ�1�ɂȂ��Ă���B
	
	uint8 MakeCRC3(const void* pData, size_t size)
	{
		/*
			���X��input��10110�������ꍇ�ACRC�i�[����3bit�𖖔��ɕ₤�ƁA
			�ʐM��CRC�𖖔��ɕt�����đ��������A���̃r�b�g�����M���ł��̂܂�CRC�v�Z��������0�ɂȂ�B
			���L�̗�Ō����΁A10110101��CRC��0�ɂȂ�B
			���̗l�Ȏ�M���̃`�F�b�N�����ׂ̈ɕ���Ă��邾���ł���A0�ŕ₤���͕K�{�ł͂Ȃ��B
			
			1011000 �̃��W����2���Z�̗�B
				    11001
				---------
			1101)10110000 <- r = 1011
				 1101
				 --------
				  1100    <- r = ((r ^ DIV_CRC3) << 1) | ���̌�
				  1101
				  -------
				     10   <- r = ((r ^ DIV_CRC3) << 1) | ���̌�
				   0000   <- �ŏ�ʃr�b�g��0�łȂ��̂ŁA����Ȃ��ixor�o���Ȃ��j�B���̏��ɂ�0��t�������B
				   ------
				     100  <- r = (r << 1) | ���̌�
				    0000  <- �ŏ�ʃr�b�g��0�łȂ��̂ňȉ����B
				    -----
				     1000 <- r = (r << 1) | ���̌�
				     1101
				     ----
				      101 <- r = (r ^ DIV_CRC3)	���̌��͂��������̂ŁAxor�����s���A���ꂪ�]�聁�����ƂȂ�B
		*/
		if (size == 0)
			return 0;
		
		const uint8* p = static_cast<const uint8*>(pData);
		
		uint32 r = 0;	// ����Z���̈ꎞ�ϐ��B����؂�Ȃ��Ȃ������́A���̒l�������ƂȂ�B
		uint32 i = 0;
		#if 0
		r = *p >> (BITS_BYTE-BITS_DIV_CRC3);	// �擪��4�r�b�g�����o���B
		int i = BITS_DIV_CRC3;	// �����ς݂̃C���f�b�N�X�l�B����4�r�b�g�����o���ċ���̂ŁA4�ŏ��������Ă����B
		//CRC_TRACE(_T("r=") TFMT_BIN8, format_bin8(r));
		#endif
		
		for (;;)
		{
			if ((r & MSB_CRC3) != 0)
			{
				// �擪�r�b�g��1�Ȃ�A�����Z�ixor�j�B
				r ^= DIV_CRC3;
				//CRC_TRACE(_T("+r=") TFMT_BIN8 _T(", i=%u"), format_bin8(r), i);
			}
			// ���̌��̃r�b�g���擾����B
			uint32 bit = 0;
			if (!GetBitInBytesMSB(p, size, i, bit))
				break;	// �S�r�b�g���������̂Œ��f�B���̎��� r ���]�聁�����B
			r <<= 1;	// r �̐擪�r�b�g�͕K��0�Ȃ̂ŁA���̈����Z�ׁ̈i�����ƌ������킹��ׁj�ɍ��V�t�g����B
			r |= bit;
			//CRC_TRACE(_T("r=") TFMT_BIN8 _T(", i=%u"), format_bin8(r), i);
			++i;
		}
		
		return static_cast<uint8>(r);
	}
#else
	// ������CRC���Ɠ����r�b�g���̒l�ɂ��Čv�Z����ver.

	static const uint32 DIV_CRC3 = 0x5;	//!< = 101 = ���� = ����������(generating polynomial)
	static const uint32 MSB_BIT3 = 0x4;
	static const uint32 MASK_BIT3 = 0x7;

	uint8 MakeCRC3(const void* pData, size_t size)
	{
		if (size == 0)
			return 0;
		
		const uint8* p = static_cast<const uint8*>(pData);
		
		uint32 r = 0;
		uint32 bit = 0;
		uint32 i = 0;
		
		for (;;)
		{
			// ���̃r�b�g�����o���B
			if (!GetBitInBytesMSB(p, size, i++, bit))
				break;	// �S�r�b�g���o�����̂Œ��f�B���̎��� r ���]�聁�����B
			
			// �擪�r�b�g�ŏ����𕪊�B
			if ((r & MSB_BIT3) != 0)
			{
				r <<= 1;
				r &= MASK_BIT3;
					// ������4�r�b�g���̏ꍇ�̌v�Z���ƁA�V�t�g�̑O��xor���邪�AMSB��xor�̌��ʕK��0�ɂȂ�ׁA��ɃV�t�g����B
					// �������鎖�ŁA�����萔��CRC�͂Ɠ���3�r�b�g���̃f�[�^�Ƃ��ĕێ����Ă�����B
				r |= bit;
				r ^= DIV_CRC3;
			}
			else
			{
				r <<= 1;
				r |= bit;
			}

			//CRC_TRACE(_T("r=") TFMT_BIN8, format_bin8(r));
		}
		
		return static_cast<uint8>(r);
	}
	
#endif
	

	/*
		1�r�b�g���̌v�Z�ł͂Ȃ��ACRC���̃f�[�^���Ɍv�Z���鎖���l����B
	
		1011000 �ɑ΂��鉉�Z���A3�r�b�g���Ƃɋ�؂�ƁA�ȉ��̂悤�ɂȂ�B
		     [0] [1] [2]
		         011 001
			------------
		1101)010 110 000  
		     000 0
		     -----------
		      10 11
		      11 01
		      ----------
		       1 100
		       1 101
		       ========= (a)
		           1 0   
		         000 0
		         -------
		           1 00
		          00 00
		          ------
		           1 000
		           1 101
		           =====
		             101
		(a)�̎��A[1]�ɑ������錅�̐��l���ǂ��Ȃ��������l����B
		110 -> 001
		�X�ɁA�e���̒l�ɂǂ̂悤�ȉ��Z�������������l����B
		[1]��3���� : 1 ^ 0 = 1, 1 ^ 0 = 1, 1 ^ 1 = 0
		[1]��2���� : 1 ^ 1 = 0, 0 ^ 0 = 0
		[1]��1���� : 0 ^ 1 = 1
		�e���ɍs��ꂽ���Z���܂Ƃ߂Ĉ�̎��ɂ���ƁA
		[1]��3���� : 1 ^ (0 ^ 0 ^ 1) = 0
		[1]��2���� : 1 ^ (1 ^ 0) = 0
		[1]��1���� : 0 ^ (1) = 1
		�ƂȂ�B�X�Ɋ��ʓ��𐮗�����ƁA
		[1]��3���� : 1 ^ 1 = 0
		[1]��2���� : 1 ^ 1 = 0
		[1]��1���� : 0 ^ 1 = 1
		�ƂȂ�B[1]�̌��̒l�ɁA111�Ƃ������l��xor���Ă��΁A��x�Ɍv�Z���\�ɂȂ�B
		����111�́A�ׂł���[0]�̐��l�����ł��邩�ɂ���ĕω�����̂ŁA
		[0]�̒l�ƁA[1]��xor����l�̑Ή��\�����A������Q�Ƃ���l�ɂ���Ή��Z�R�X�g���啝�ɍ팸�o���鎖�ɂȂ�B
		[0]�̒l�ɑΉ������l�́A[0]�̒l��CRC������0�������l�ɁA�����Ŋ������]��ƂȂ�B
		         011
		    --------
		1101)010 000
		     000 0
		      ------
		      10 00
		      11 01
		      ------
		       1 010
		       1 101
		       -----
		         111
		[2]�̒l��xor����l���l����B
		[2]��3���� : 0 ^ (0 ^ 0 ^ 1) = 0 ^ 1 = 1
		[2]��2���� : 0 ^ (0 ^ 0)     = 0 ^ 0 = 0
		[2]��1���� : 0 ^ (1)         = 0 ^ 1 = 1
		����xor����l101�́A[0]�̉��Z�I�����[1]�̒l001����Z�o�����͂��ł���B
		         001
		    --------
		1101)001 000
		     000 0
		     -------
		       1 00
		      00 00
		      ------
		       1 000
		       1 101
		       -----
		         101
		��L�̌v�Z�ŎZ�o�����l���g���Ă܂Ƃ߂Čv�Z����B
		     [0] [1] [2]
			------------
		1101)010 110 000  
		     010 111
		     -----------
		           1 000
		           1 101
		           -----
		             101
		InitCRC3Table �֐��ł́A����xor����l�̑Ή��\�̍쐬���A
		MakeCRC3t �֐��ł́A���̑Ή��\��p�����v�Z���s���B
	*/
	
	static std::vector<uint32> tableCRC3;
	
	void InitCRC3Table()
	{
		unsigned int size = MPHelpers::Pow<2, BITS_CRC3>::result;
		tableCRC3.resize(size);
		
		for (unsigned int i = 0; i < size; ++i)
		{
			uint8 bits = static_cast<uint8>(i << BITS_CRC3);	// ���Z�ׂ̈�0��₤�B
			tableCRC3[i] = MakeCRC3(&bits, 1);
			CRC_TRACE(TFMT_BIN8 _T("=") TFMT_BIN8, format_bin8(i), format_bin8(tableCRC3[i]));
		}
	}
	
	uint8 MakeCRC3t(const void* pData, size_t size)
	{
		if (size == 0)
			return 0;
		
		const uint8* p = static_cast<const uint8*>(pData);
		
		uint32 r = 0;

		// ���傤�ǍŌ�̃r�b�g�ŏI���l�ɂ������̂ŁA�ŏ��Ɏ�肾���r�b�g���𒲐�����B
		int i = BITS_CRC3;
		int numBits = size*BITS_BYTE;
		int mod = numBits % BITS_CRC3;
		int numLoops = numBits / BITS_CRC3 - 1;	// ����r�̏�������1�񕪃��[�v���Ă��鎖�ɂȂ�̂ŁA-1���Ă����B
		if (mod > 0)
		{
			i -= BITS_CRC3 - mod;
			++numLoops;
		}
		
		r = *p >> (BITS_BYTE-i);	// �擪��i�r�b�g�����o���B
		//CRC_TRACE(_T("r=") TFMT_BIN8, format_bin8(r));		
		
		for (; numLoops > 0; --numLoops)
		{
			// ���̂R����xor����l���擾����B
			uint32 div = tableCRC3[r];
			
			// ���̂R�����擾����B
			r = 0;
			GetBitsInBytesMSB(p, size, i, BITS_CRC3, r);
			i += BITS_CRC3;
			//CRC_TRACE(_T("r=") TFMT_BIN8, format_bin8(r));	

			// xor����B
			r ^= div;
			//CRC_TRACE(_T("r=") TFMT_BIN8, format_bin8(r));	
		}
		return static_cast<uint8>(r);
	}
	
	// ------------------------------------------------------------------------
	// 8�r�b�g��CRC�B
	static const uint32 BITS_CRC8		= 8;
	static const uint32 DIV_CRC8		= 0x8d;	//!< = CRC8-CCITT�̏����B
	static const uint32 MSB_BYTE		= 0x80;
	static const uint32 MASK_BYTE		= 0xff;

	uint8 MakeCRC8(const void* pData, size_t size)
	{
		if (size == 0)
			return 0;
		
		const uint8* p = static_cast<const uint8*>(pData);
		
		uint32 r = 0;
		uint32 bit = 0;
		uint32 i = 0;
		
		for (;;)
		{
			if (!GetBitInBytesMSB(p, size, i++, bit))
				break;
			
			// �擪�r�b�g�ŏ����𕪊�B
			if ((r & MSB_BYTE) != 0)
			{
				r <<= 1;
				r &= MASK_BYTE;
				r |= bit;
				r ^= DIV_CRC8;
			}
			else
			{
				r <<= 1;
				r |= bit;
			}
			//CRC_TRACE(_T("r=") TFMT_BIN8, format_bin8(r));
		}
		
		return static_cast<uint8>(r);
	}

	static std::vector<uint32> tableCRC8;
	
	void InitCRC8Table()
	{
		unsigned int size = MPHelpers::Pow<2, BITS_BYTE>::result;
		tableCRC8.resize(size);
		
		for (unsigned int i = 0; i < size; ++i)
		{
			uint8 bits[] = { static_cast<uint8>(i), 0 };	// ���Z�ׂ̈�0��₤�B
			tableCRC8[i] = MakeCRC8(bits, _countof(bits));
			//CRC_TRACE(TFMT_BIN8 _T("=") TFMT_BIN8, format_bin8(i), format_bin8(tableCRC8[i]));
		}
	}
	
	uint8 MakeCRC8t(const void* pData, size_t size)
	{
		if (size == 0)
			return 0;
		
		const uint8* p = static_cast<const uint8*>(pData);
		
		uint32 r = p[0];
		for (size_t i = 1; i < size; ++i)
		{
			uint32 div = tableCRC8[r];
			r = p[i];
			r ^= div;
		}

		return static_cast<uint8>(r);
	}

	// ------------------------------------------------------------------------
	// 16�r�b�g��CRC�B
	static const uint32 BITS_CRC16		= 16;
	static const uint32 DIV_CRC16		= 0x1021;	//!< = CRC16-CCITT�̏����B
	static const uint32 MSB_WORD		= 0x8000;
	static const uint32 MASK_WORD		= 0xffff;

	/*
		�A���S���Y���͐������͂��Ȃ̂Ƀt���[�\�t�g�Ȃ񂩂Ōv�Z�o����CRC�Ƃ܂�ň�v���Ȃ��c�B
		�d���Ȃ��̂ł��̂����A���S���Y�����T�Ɠ����̂��������Ďg�����ɂ���B
	*/
	uint16 MakeCRC16(const void* pData, size_t size)
	{
		if (size == 0)
			return 0;
		
		const uint8* p = static_cast<const uint8*>(pData);
		
		uint32 r = 0;
		uint32 bit = 0;
		uint32 i = 0;
		
		for (;;)
		{
			if (!GetBitInBytesMSB(p, size, i++, bit))
				break;
			
			// �擪�r�b�g�ŏ����𕪊�B
			if ((r & MSB_WORD) != 0)
			{
				r <<= 1;
				r &= MASK_WORD;
				r |= bit;
				r ^= DIV_CRC16;
			}
			else
			{
				r <<= 1;
				r |= bit;
			}
			//CRC_TRACE(_T("r=") TFMT_BIN8, format_bin8(r));
		}
		
		return static_cast<uint16>(r);
	}
	
	uint16 MakeStrCRC16(const char* str)
	{
		return MakeCRC16(str, strlen(str));
	}

	uint16 MakeStrCRC16(const wchar_t* str)
	{
		return MakeCRC16(str, (wcslen(str))*sizeof(wchar_t));
	}

	// ------------------------------------------------------------------------
	// 32�r�b�g��CRC�B
	uint32 MakeCRC32(const void* pData, size_t size)
	{
		return 0;
	}
}
