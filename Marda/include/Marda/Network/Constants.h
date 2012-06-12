/*!
	@file	Network/Constants.h
	
	@author	E.Nakayama
*/
#pragma once
	
namespace Marda
{
	namespace Network
	{
		enum DISCONNECT_REASON
		{
			DISCONNECT_NO_ERROR,
			DISCONNECT_REMOTE,	//!< �����[�g�����蓮�ؒf�����B
			DISCONNECT_RESET,	//!< �����[�g���������ؒf�i�_�E�����j�����B
		};
		
	}	// end namespace Network
	
}	// end namespace Marda

