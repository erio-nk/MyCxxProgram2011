// Test_Network_Server.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

#include <iostream>
#include <boost/asio.hpp>

using namespace std;
using namespace boost::asio;
using ip::tcp;

//int _tmain(int argc, _TCHAR* argv[])
//{
//    io_service io;
//
//    tcp::acceptor acc(io, tcp::endpoint(tcp::v4(), 31400));
//
//    for (;;) {
//        tcp::iostream s;
//        acc.accept(*s.rdbuf());
//
//        cout << "[connect start]" << endl;
//
//        cout << "[connect end]" << endl;
//    }
//
//	return 0;
//}

#include "Marda/Network/Server.h"

using namespace Marda;

namespace 
{
	void OnConnect(Network::Session::Ptr pSession)
	{
		std::cout << "Connect sessionId:" << pSession->GetId() << std::endl;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	Network::Server server(31400);
	
	server.SetConnectHandler(&OnConnect);

	server.Listen();
	
	for (;;)
	{
		server.Poll();
		Sleep(100);
		//std::cout << "hoge" << std::endl;
	}
	
	return 0;
}

