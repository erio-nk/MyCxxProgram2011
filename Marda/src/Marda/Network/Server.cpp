/*!
	@file	Network/Server.cpp

	@author	E.Nakayama
*/

#include <iostream>
#include <functional>

#include <boost/cast.hpp>
#include <boost/asio.hpp>

#include "Marda/Network/Server.h"
#include "Marda/Network/Constants.h"
#include "SessionImpl_inl.h"

using namespace boost::asio;
using namespace Marda;

namespace
{
	void DefaultConnectHandler(Network::Session::Ptr)
	{
	}
	
	void DefaultDisconnectHandler(Network::Session::Ptr, Network::DISCONNECT_REASON)
	{
	}
		
}	// end namespace

struct Network::Server::Impl
{
	typedef ip::tcp::acceptor acceptor;
	typedef SessionImpl<Impl> ServerSession;
	
	io_service _io;
	acceptor _acceptor;
	acceptor::endpoint_type _endpoint;
	
	typedef std::map<Session::Id, ServerSession::Ptr> SessionList;
	SessionList _sessions;
	
	Session::Id _nextSessionId;
	
	ConnectHandler _connectHandler;
	DisconnectHandler _disconnectHandler;
	
	Impl(uint16 port)
		: _nextSessionId(1)
		, _io()
		, _acceptor(_io, ip::tcp::endpoint(ip::tcp::v4(), port))
	{
		SetConnectHandler(&DefaultConnectHandler);
		SetDisconnectHandler(&DefaultDisconnectHandler);
	}
	
	~Impl()
	{
	}
	
	io_service& GetIOService()
	{
		return _io;
	}
	
	void SetConnectHandler(ConnectHandler handler)
	{
		_connectHandler = handler;
	}
	
	void SetDisconnectHandler(DisconnectHandler handler)
	{
		_disconnectHandler = handler;
	}

	void Listen()
	{
		using namespace std::placeholders;
		// 予め次に接続してきたクライアント用のセッションを用意しておき、bindでOnAcceptに渡す。
		ServerSession::Ptr pNewSession(new ServerSession(*this));
		_acceptor.async_accept(pNewSession->_socket
			, std::bind(&Impl::OnAccept, this, pNewSession, _1));
	}
	
	void OnAccept(ServerSession::Ptr pNewSession, const boost::system::error_code& error)
	{
		if (error) 
		{
			std::cout << error << std::endl;
			
			if (error == error::bad_descriptor)	// WSAEBADF=10009
			{
				// このケースですぐにasync_acceptすると、
				// OnAcceptを抜けた後に現在のio_service::pollがそのイベントを実行し、
				// io_service::pollから帰ってこなくなる。
			}
			throw error;	// のでとりあえず例外を投げて終了させる。
		}
		else
		{
			pNewSession->_id = _nextSessionId++;
			_sessions.insert(std::make_pair(pNewSession->GetId(), pNewSession));
			
			_connectHandler(pNewSession);

			ServerSession::AsyncRead(pNewSession);
		}
		
		// 引き続き他の接続を待機。
		Listen();
	}

	void Disconnect(ServerSession::Ptr pSession, DISCONNECT_REASON reason)
	{
		SessionList::iterator found = _sessions.find(pSession->GetId());
		if (found != _sessions.end())
		{
			std::cout << "disconnect " << pSession->GetId() << " reason:" << reason << std::endl;
			_disconnectHandler(pSession, reason);
			_sessions.erase(found);
		}
	}
	
	void OnRead(ServerSession::Ptr pSession, const boost::system::error_code& error, size_t readSize)
	{
		if (error)
		{
			std::cout << "session error " << error << std::endl;
			DISCONNECT_REASON reason = DISCONNECT_NO_ERROR;
			if (error == error::eof)
			{
				reason = DISCONNECT_REMOTE;
			}
			else
			if (error == error::connection_reset)	// WSAECONNRESET=10054
			{
				reason = DISCONNECT_RESET;
			}
			Disconnect(pSession, reason);
			return;
		}
		
		std::cout << "read " << readSize << "bytes" << std::endl;
		
		ServerSession::AsyncRead(pSession);
	} 
	
	void Poll()
	{
		_io.poll();
	}
	
};	// end struct Network::Client::Impl

Network::Server::Server(uint16 port)
	: _pimpl(new Impl(port))
{
}

Network::Server::~Server()
{
}

void
Network::Server::SetConnectHandler(ConnectHandler handler)
{
	_pimpl->SetConnectHandler(handler);
}

void
Network::Server::SetDisconnectHandler(DisconnectHandler handler)
{
	_pimpl->SetDisconnectHandler(handler);
}

void
Network::Server::Listen()
{
	_pimpl->Listen();
}

void
Network::Server::Poll()
{
	_pimpl->Poll();
}

