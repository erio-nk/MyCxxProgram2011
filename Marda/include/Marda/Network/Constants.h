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
			DISCONNECT_REMOTE,	//!< リモート側が手動切断した。
			DISCONNECT_RESET,	//!< リモート側が強制切断（ダウン等）した。
		};
		
	}	// end namespace Network
	
}	// end namespace Marda

