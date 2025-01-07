#pragma once
#include <Networking/TCP_Server.h>
#include <iostream>

using namespace std;
using boost::asio::ip::tcp;

namespace TCP {
	TCPServer::TCPServer(IPV ipv, int port) : _ipVersion(ipv), _port(port),
		_acceptor(_ioContext, tcp::endpoint(_ipVersion == IPV::V4 ? tcp::v4() : tcp::v6(), _port))
	{

	}


	int TCPServer::Run() {
		try {
			startAccept();
			_ioContext.run();
		}
		catch (exception& e)
		{
			cerr << e.what() << endl;
			return -1;
		}
		return 0;
	}

	void TCPServer::Broadcast(const string& message)
	{
		for (auto& connection : _connections)
		{
			connection->Post(message);
		}
	}

	void TCPServer::startAccept()
	{
		_socket.emplace(_ioContext);
		
		

		_acceptor.async_accept(*_socket, [this](const boost::system::error_code& error) 
			{
				auto connection = TCPConnection::Create(move(*_socket));

				if (OnJoin)
				{
					OnJoin(connection);
				}
				_connections.insert(connection);
				
				if (!error) {
					connection->Start(
						[this](const string& message) { if (OnMessage) OnMessage(message); },
						[&, weak = weak_ptr(connection)] {
							if (auto shared = weak.lock(); shared && _connections.erase(shared)) {
								if (OnLeave) OnLeave(shared);
							}
						}
					);
				}
				startAccept();
			});
		
	}
}
