/*!
	@file	Types.h

	@author	E.Nakayama
*/
#pragma once
	
namespace Marda
{
	// �o�C�g�T�C�Y�𖾎��������ꍇ�̌^
	typedef signed char		sint8;
	typedef unsigned char	uint8;
	typedef signed short	sint16;
	typedef unsigned short	uint16;
	typedef signed int		sint32;
	typedef unsigned int	uint32;
	typedef signed long long	sint64;
	typedef unsigned long long	uint64;
	
	// ������^
	#if defined(_UNICODE)
	typedef std::wstring TString;
	#else
	typedef std::string TString;
	#endif

	// �k���^�B�e���v���[�g�p�B
	struct NullType {};
	
	// �X���b�f�B���O�|���V�[�B�e���v���[�g�p�B
	struct ThreadSafe {};
	struct ThreadUnsafe {};
	
}	// end namespace Marda

#if _MSC_VER <= 1500	// VC++9.0�ȉ��Ȃ�
// tr1���ȗ�����ׂ̐錾������BVS10�ȍ~�ł́Atr1�͖����Ȃ�B
namespace std { namespace tr1 {}; using namespace tr1; }
#endif
