// Test_Network_Client.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

#include <iostream>
#include <boost/asio.hpp>

namespace asio = boost::asio;
namespace ip = boost::asio::ip;

void connect_handle(const boost::system::error_code& error)
{
    if (error)
        std::cout << error << std::endl;
    else
        std::cout << "success" << std::endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
    asio::io_service ios;

    ip::tcp::socket soc(ios);
    soc.async_connect(ip::tcp::endpoint(ip::address::from_string("127.0.0.1"), 31400),
                      connect_handle);

	for (;;)
	{
		ios.run();
	//	ios.poll();
	//	soc.close();
		Sleep(10);
	}

	return 0;
}

