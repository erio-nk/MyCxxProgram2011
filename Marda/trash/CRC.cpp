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
	
	static const uint32 BITS_BYTE = 8;	//!< 1バイト中のビット数。
	
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
//		indexBitsInByte = (BITS_BYTE-1) - (indexBits % BITS_BYTE);	// 最上位ビットから入力（MSB）しているので、i%8で0が出たなら、これを7にする。
//	}

	// ------------------------------------------------------------------------
	// 3ビット長CRC。
	
	static const uint32 BITS_CRC3		= 3;
	
#if 0
	static const uint32 BITS_DIV_CRC3	= 4;	//!< 除数のビット数。
	static const uint32 DIV_CRC3		= 0xd;	//!< = 1101 = 除数 = 生成多項式(generating polynomial)
	static const uint32 MSB_CRC3		= 1 << (BITS_DIV_CRC3 - 1);	//!< 計算時に使用する先頭ビットのマスク値。計算時は4ビットで計算しているので、この値も3ビット目が1になっている。
	
	uint8 MakeCRC3(const void* pData, size_t size)
	{
		/*
			元々のinputが10110だった場合、CRC格納分の3bitを末尾に補うと、
			通信でCRCを末尾に付加して送った時、そのビット列を受信側でそのままCRC計算した時に0になる。
			下記の例で言えば、10110101のCRCは0になる。
			この様な受信側のチェック処理の為に補っているだけであり、0で補う事は必須ではない。
			
			1011000 のモジュロ2除算の例。
				    11001
				---------
			1101)10110000 <- r = 1011
				 1101
				 --------
				  1100    <- r = ((r ^ DIV_CRC3) << 1) | 次の桁
				  1101
				  -------
				     10   <- r = ((r ^ DIV_CRC3) << 1) | 次の桁
				   0000   <- 最上位ビットが0でないので、割れない（xor出来ない）。↑の商には0を付け足す。
				   ------
				     100  <- r = (r << 1) | 次の桁
				    0000  <- 最上位ビットが0でないので以下略。
				    -----
				     1000 <- r = (r << 1) | 次の桁
				     1101
				     ----
				      101 <- r = (r ^ DIV_CRC3)	次の桁はもう無いので、xorだけ行い、これが余り＝答えとなる。
		*/
		if (size == 0)
			return 0;
		
		const uint8* p = static_cast<const uint8*>(pData);
		
		uint32 r = 0;	// 割り算時の一時変数。割り切れなくなった時は、この値が答えとなる。
		uint32 i = 0;
		#if 0
		r = *p >> (BITS_BYTE-BITS_DIV_CRC3);	// 先頭の4ビットを取り出す。
		int i = BITS_DIV_CRC3;	// 処理済みのインデックス値。↑で4ビット分取り出して居るので、4で初期化しておく。
		//CRC_TRACE(_T("r=") TFMT_BIN8, format_bin8(r));
		#endif
		
		for (;;)
		{
			if ((r & MSB_CRC3) != 0)
			{
				// 先頭ビットが1なら、引き算（xor）。
				r ^= DIV_CRC3;
				//CRC_TRACE(_T("+r=") TFMT_BIN8 _T(", i=%u"), format_bin8(r), i);
			}
			// 次の桁のビットを取得する。
			uint32 bit = 0;
			if (!GetBitInBytesMSB(p, size, i, bit))
				break;	// 全ビット処理したので中断。その時の r が余り＝答え。
			r <<= 1;	// r の先頭ビットは必ず0なので、次の引き算の為（除数と桁を合わせる為）に左シフトする。
			r |= bit;
			//CRC_TRACE(_T("r=") TFMT_BIN8 _T(", i=%u"), format_bin8(r), i);
			++i;
		}
		
		return static_cast<uint8>(r);
	}
#else
	// 除数をCRC長と同じビット長の値にして計算するver.

	static const uint32 DIV_CRC3 = 0x5;	//!< = 101 = 除数 = 生成多項式(generating polynomial)
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
			// 次のビットを取り出す。
			if (!GetBitInBytesMSB(p, size, i++, bit))
				break;	// 全ビット取り出したので中断。その時の r が余り＝答え。
			
			// 先頭ビットで処理を分岐。
			if ((r & MSB_BIT3) != 0)
			{
				r <<= 1;
				r &= MASK_BIT3;
					// 除数が4ビット長の場合の計算だと、シフトの前にxorするが、MSBはxorの結果必ず0になる為、先にシフトする。
					// そうする事で、除数定数をCRCはと同じ3ビット長のデータとして保持しておける。
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
		1ビットずつの計算ではなく、CRC長のデータ毎に計算する事を考える。
	
		1011000 に対する演算を、3ビットごとに区切ると、以下のようになる。
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
		(a)の時、[1]に相当する桁の数値がどうなったかを考える。
		110 -> 001
		更に、各桁の値にどのような演算があったかを考える。
		[1]の3桁目 : 1 ^ 0 = 1, 1 ^ 0 = 1, 1 ^ 1 = 0
		[1]の2桁目 : 1 ^ 1 = 0, 0 ^ 0 = 0
		[1]の1桁目 : 0 ^ 1 = 1
		各桁に行われた演算をまとめて一つの式にすると、
		[1]の3桁目 : 1 ^ (0 ^ 0 ^ 1) = 0
		[1]の2桁目 : 1 ^ (1 ^ 0) = 0
		[1]の1桁目 : 0 ^ (1) = 1
		となる。更に括弧内を整理すると、
		[1]の3桁目 : 1 ^ 1 = 0
		[1]の2桁目 : 1 ^ 1 = 0
		[1]の1桁目 : 0 ^ 1 = 1
		となる。[1]の元の値に、111という数値をxorしてやれば、一度に計算が可能になる。
		この111は、隣である[0]の数値が何であるかによって変化するので、
		[0]の値と、[1]にxorする値の対応表を作り、それを参照する様にすれば演算コストが大幅に削減出来る事になる。
		[0]の値に対応した値は、[0]の値にCRC長分の0を補った値に、除数で割った余りとなる。
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
		[2]の値にxorする値を考える。
		[2]の3桁目 : 0 ^ (0 ^ 0 ^ 1) = 0 ^ 1 = 1
		[2]の2桁目 : 0 ^ (0 ^ 0)     = 0 ^ 0 = 0
		[2]の1桁目 : 0 ^ (1)         = 0 ^ 1 = 1
		このxorする値101は、[0]の演算終了後の[1]の値001から算出されるはずである。
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
		上記の計算で算出した値を使ってまとめて計算する。
		     [0] [1] [2]
			------------
		1101)010 110 000  
		     010 111
		     -----------
		           1 000
		           1 101
		           -----
		             101
		InitCRC3Table 関数では、このxorする値の対応表の作成を、
		MakeCRC3t 関数では、その対応表を用いた計算を行う。
	*/
	
	static std::vector<uint32> tableCRC3;
	
	void InitCRC3Table()
	{
		unsigned int size = MPHelpers::Pow<2, BITS_CRC3>::result;
		tableCRC3.resize(size);
		
		for (unsigned int i = 0; i < size; ++i)
		{
			uint8 bits = static_cast<uint8>(i << BITS_CRC3);	// 演算の為に0を補う。
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

		// ちょうど最後のビットで終わる様にしたいので、最初に取りだすビット数を調整する。
		int i = BITS_CRC3;
		int numBits = size*BITS_BYTE;
		int mod = numBits % BITS_CRC3;
		int numLoops = numBits / BITS_CRC3 - 1;	// ↓のrの初期化で1回分ループしている事になるので、-1しておく。
		if (mod > 0)
		{
			i -= BITS_CRC3 - mod;
			++numLoops;
		}
		
		r = *p >> (BITS_BYTE-i);	// 先頭のiビットを取り出す。
		//CRC_TRACE(_T("r=") TFMT_BIN8, format_bin8(r));		
		
		for (; numLoops > 0; --numLoops)
		{
			// 次の３桁にxorする値を取得する。
			uint32 div = tableCRC3[r];
			
			// 次の３桁を取得する。
			r = 0;
			GetBitsInBytesMSB(p, size, i, BITS_CRC3, r);
			i += BITS_CRC3;
			//CRC_TRACE(_T("r=") TFMT_BIN8, format_bin8(r));	

			// xorする。
			r ^= div;
			//CRC_TRACE(_T("r=") TFMT_BIN8, format_bin8(r));	
		}
		return static_cast<uint8>(r);
	}
	
	// ------------------------------------------------------------------------
	// 8ビット長CRC。
	static const uint32 BITS_CRC8		= 8;
	static const uint32 DIV_CRC8		= 0x8d;	//!< = CRC8-CCITTの除数。
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
			
			// 先頭ビットで処理を分岐。
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
			uint8 bits[] = { static_cast<uint8>(i), 0 };	// 演算の為に0を補う。
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
	// 16ビット長CRC。
	static const uint32 BITS_CRC16		= 16;
	static const uint32 DIV_CRC16		= 0x1021;	//!< = CRC16-CCITTの除数。
	static const uint32 MSB_WORD		= 0x8000;
	static const uint32 MASK_WORD		= 0xffff;

	/*
		アルゴリズムは正しいはずなのにフリーソフトなんかで計算出来るCRCとまるで一致しない…。
		仕方ないのでそのうちアルゴリズム辞典と同じのを実装して使う事にする。
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
			
			// 先頭ビットで処理を分岐。
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
	// 32ビット長CRC。
	uint32 MakeCRC32(const void* pData, size_t size)
	{
		return 0;
	}
}
