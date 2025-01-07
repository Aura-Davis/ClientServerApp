#include "Networking/TCP_Client.h"
#include <iostream>
using namespace std;

namespace TCP{
	TCPClient::TCPClient(const string &address, int port) : _socket(_ioContext)
	{
		boost::asio::ip::tcp::resolver resolver {_ioContext};
		_endpoints = resolver.resolve(address, to_string(port));
	}
	
	void TCPClient::Run()
	{
		boost::asio::async_connect(_socket, _endpoints,
			[this](boost::system::error_code ec, boost::asio::ip::tcp::endpoint ep) {
				if (!ec) Async_Read();
			});

		_ioContext.run();
	}

	void TCPClient::Stop()
	{
		boost::system::error_code ec;
		_socket.close(ec);
	}

	void TCPClient::Post(const string& message)
	{
		bool queueIdle = _outgoingMessages.empty();
		_outgoingMessages.push(message);

		if (queueIdle) Async_Write();
	}

	void TCPClient::Async_Read()
	{
		boost::asio::async_read_until(_socket, _streambuf, "\n",
			[this](boost::system::error_code ec, size_t bytestransferred) {
			On_Read(ec, bytestransferred);
		});
	}

	void TCPClient::On_Read(boost::system::error_code ec, size_t bytestransferred)
	{
		if (ec)
		{
			Stop();
			return;
		}

		stringstream message;
		message << istream{&_streambuf}.rdbuf();
		OnMessage(message.str());
		Async_Read();
	}

	void TCPClient::Async_Write()
	{
		boost::asio::async_write(_socket, boost::asio::buffer(_outgoingMessages.front()),
			[this](boost::system::error_code ec, size_t bytestransferred) {
			On_Write(ec, bytestransferred);
		});
	}

	void TCPClient::On_Write(boost::system::error_code ec, size_t bytestransferred)
	{
		if (ec)
		{
			Stop();
			return;
		}

		_outgoingMessages.pop();

		if (!_outgoingMessages.empty()) Async_Write();
	}
}