/*!
	@file	Network/Client.cpp

	@author	E.Nakayama
*/

#include <functional>

#include <boost/asio.hpp>

#include "Marda/Network/Client.h"

using namespace boost::asio;
using namespace Marda;

struct Network::Client::Impl
{
	io_service _io;
	ip::tcp::socket _socket;
	
	void Connect(const std::string& host, uint16 port)
	{
		using namespace std::placeholders;
		_socket.async_connect(ip::tcp::endpoint(ip::address::from_string(host), port)
			, std::bind(&Impl::OnConnected, this, _1));
		
	}
	
	void OnConnected(const boost::system::error_code& error)
	{
		if (error)
		{
			TTRACE(_T("connection failure"));
		}
		else
		{
			TTRACE(_T("connection success"));
		}
	}

};	// end struct Network::Client::Impl
