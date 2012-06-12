/*!
	@file	Network/Server.h
	
	@author	E.Nakayama
*/
#pragma once

#include "Marda.h"
#include "Session.h"
#include "Constants.h"
	
namespace Marda
{
	namespace Network
	{
		class Server
		{
		public:

			Server(uint16 port);
			
			~Server();

			void SetConnectHandler(ConnectHandler handler);

			void SetDisconnectHandler(DisconnectHandler handler);

			void SetEndPoint(uint16 port);
			
			void Listen();
			
			void Poll();
			
			
		private:
			struct Impl;
			std::auto_ptr<Impl> _pimpl;
		
		};	// end class Server
		
	}	// end namespace Network
	
}	// end namespace Marda

