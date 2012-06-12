/*!
	@file	Network/SessionImpl_inl.h
	
	@author	E.Nakayama
*/
#pragma once

#include <boost/utility.hpp>
#include <boost/asio.hpp>
	
#include "Marda/Network/Session.h"

using namespace boost::asio;

namespace Marda
{
	namespace Network
	{
		template <class Core>
		struct SessionImpl : public Session, boost::noncopyable
		{
			typedef std::shared_ptr<SessionImpl> Ptr;
			typedef std::array<char, 256> Buffer;	// vector“n‚¹‚é‚Ì‚©‚ÈH
			
			Core& _core;
			
			Session::Id _id;
			ip::tcp::socket _socket;
			Buffer _buffer;	
			
			SessionImpl(Core& core)
				: _core(core) 
				, _socket(core.GetIOService())
			{
			}

			Id GetId() const
			{
				return _id;
			}
			
			static void AsyncRead(Ptr pSession)
			{
				using namespace std::placeholders;
				pSession->_socket.async_read_some(boost::asio::buffer(pSession->_buffer)
					, std::bind(&Core::OnRead, &pSession->_core, pSession, _1, _2));
			}
			
		};	// end struct SessionImpl
		
	}	// end namespace Network
	
}	// end namespace Marda

