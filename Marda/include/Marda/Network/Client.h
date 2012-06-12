/*!
	@file	Network/Client.h
	
	@author	E.Nakayama
*/
#pragma once
	
#include "Marda/Types.h"

namespace Marda
{
	namespace Network
	{
		class Client
		{
		public:
			
			void Connect(const std::string& host, uint16 port);
			
		private:
			struct Impl;
			std::auto_ptr<Impl> _pimpl;
		
		};	// end class Client
		
	}	// end namespace Network
	
}	// end namespace Marda

