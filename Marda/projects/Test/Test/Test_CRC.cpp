#include "stdafx.h"

using namespace Marda;

bool Test_CRC()
{
#if 0
	TTRACE(_T("====================================================="));
	TTRACE(_T("-----------------------------------------------------"));
	// 3�r�b�g��CRC�e�X�g�B
	{	
		{
			const uint8 input = 0x02;	// 10
			const uint8 answer = 0x2;	// 10
		
			const uint8 bits[] = { input };
			int crc3 = MakeCRC3(bits, _countof(bits));
			TTRACE(_T("crc3=") TFMT_BIN8, format_bin8(crc3));
			if (crc3 != answer)
			{
				TTRACE(_T("CRC# failed!"));
				return false;
			}
		}

		{
			//const uint8 input = 0x16;	// 10110
			//const uint8 answer = 0x5;	// 101
		
			//const uint8 input = 0x11;	// 10001
			//const uint8 answer = 0x4;	// 100

			const uint8 input = 0x12;	// 10010
			const uint8 answer = 0x6;	// 110

			const uint8 bits[] = { input << 3 };	// CRC�i�[�p��3�r�b�g���𑫂����l�B
			//const uint8 bits[] = { 0, input << 3 };	// �擪��0�����Ă����ʂ��ς��Ȃ��B
			int crc3 = MakeCRC3(bits, _countof(bits));
			TTRACE(_T("crc3=") TFMT_BIN8, format_bin8(crc3));
			if (crc3 != answer)
			{
				TTRACE(_T("CRC# failed!"));
				return false;
			}

			const uint8 bits2[] = { (input << 3) | crc3 };	// 3bit���₢�A������input��CRC�����Ă����Ƃ��̌v�Z���ʂ�0�ɂȂ�B
				// ���̗l�ɁAC++�z��̏����l�ɕϐ��͏����Ȃ������͂������A���̂��o����BMS�g���HC++0x�g���HGCC�͂���������g��������L�q���������B
			//uint8 test[crc3];	// �����΂ɔz��T�C�Y�ɕϐ��̓����Bsizeof���Z�q�̌��ʂ��ǂ��Ȃ邩�킩������ȁB
			crc3 = MakeCRC3(bits2, _countof(bits2));
			TTRACE(_T("crc3=") TFMT_BIN8, format_bin8(crc3));
			if (crc3 != 0)
			{
				TTRACE(_T("CRC# failed!"));
				return false;
			}
		
			InitCRC3Table();
			
			crc3 = MakeCRC3t(bits, _countof(bits));
			TTRACE(_T("crc3=") TFMT_BIN8, format_bin8(crc3));
			if (crc3 != answer)
			{
				TTRACE(_T("CRC# failed!"));
				return false;
			}
		}
	}

	TTRACE(_T("-----------------------------------------------------"));
	// 8�r�b�g��CRC�e�X�g�B
	{
		{
			//const uint8 bits[] = { 0xb8 };	// 1011 1000
			//const uint8 answer = 0xb8;		// 1011 1000
			///*
			//	���� 0x8d = 1000 1101
			//	                    0
			//	           ----------
			//	1 1000 1101)1011 1000
			//*/

//			const uint8 bits[] = { 0xb8, 0x9f };	// 1011 1000 1001 1111
//			const uint8 answer = 0xa1;				// 1010 0001
//			/*
//				                      1101 0110
//				           --------------------
//				1 1000 1101)1011 1000 1001 1111
//				            1100 0110 1
//				            -------------------
//				             111 1110 00
//				             110 0011 01
//				             ------------------
//				              01 1101 010
//				              00 0000 000
//				              -----------------
//				               1 1101 0101
//				               1 1000 1101
//				               ----------------
//				                 0101 1000 1
//				                 0000 0000 0
//				                 --------------
//				                  101 1000 11
//				                  110 0011 01
//				                  -------------
//				                   11 1011 101
//				                   11 0001 101
//				                   ------------
//				                    0 1010 0001
//				                    0 0000 0000
//				                    -----------
//				                      1010 0001
//			*/

			const uint8 bits[] = { 0x17, 0xc0 };	// 0001 0111 1100 0000
			const uint8 answer = 0x62;				// 0110 0010
			/*
				                         1 1010
				           --------------------
				1 1000 1101)0001 0111 1100 0000
				               1 1000 1101
				               ----------------
				                 1111 0001 0
				                 1100 0110 1
				                 --------------
				                   11 0111 100
				                   11 0001 101
				                   ------------
				                       110 0010
			*/
		
			
			int crc = MakeCRC8(bits, _countof(bits));
			TTRACE(_T("crc8=") TFMT_BIN8, format_bin8(crc));
			if (crc != answer)
			{
				TTRACE(_T("CRC# failed!"));
				return false;
			}

			InitCRC8Table();
			
			crc = MakeCRC8t(bits, _countof(bits));
			TTRACE(_T("crc8=") TFMT_BIN8, format_bin8(crc));
			if (crc != answer)
			{
				TTRACE(_T("CRC# failed!"));
				return false;
			}
		}
	}

	TTRACE(_T("-----------------------------------------------------"));
	// 16�r�b�g��CRC�e�X�g�B
	{
		{
			//const uint8 bits[] = { 0x17, 0xc0 };	// 0001 0111 1100 0000
			//const uint8 answer = 0;

			const uint8 bits[] = { 0xaa, 0x00 };
			const uint8 answer = 0;
			/*
				                         1 1010
				           --------------------
				1 1000 1101)0001 0111 1100 0000
				               1 1000 1101
				               ----------------
				                 1111 0001 0
				                 1100 0110 1
				                 --------------
				                   11 0111 100
				                   11 0001 101
				                   ------------
				                       110 0010
			*/
		
			
			int crc = MakeCRC16(bits, _countof(bits));
			TTRACE(_T("crc16=%02x(") TFMT_BIN16 _T(")"), crc, format_bin16(crc));
//			if (crc != answer)
//			{
//				TTRACE(_T("CRC# failed!"));
//				return false;
//			}
//
//			InitCRC8Table();
//			
//			crc = MakeCRC8t(bits, _countof(bits));
//			TTRACE(_T("crc8=") TFMT_BIN8, format_bin8(crc));
//			if (crc != answer)
//			{
//				TTRACE(_T("CRC# failed!"));
//				return false;
//			}
		}
		
		{
			int crc = MakeStrCRC16("aiueo");
			TTRACE(_T("crc16=%02x(") TFMT_BIN16 _T(")"), crc, format_bin16(crc));
		}
	}
#endif

	return true;
}
