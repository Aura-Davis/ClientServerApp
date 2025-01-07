#pragma once
#include <boost/asio.hpp>
#include <string>
#include <queue>
#include <iostream>
using namespace std;

namespace TCP {
	
	using MessageHandler = function<void(string)>;

	class TCPClient 
	{
	public:
		TCPClient(const string& address, int port);

		void Run();
		void Stop();
		void Post(const string& message);
		MessageHandler OnMessage;
	
	private:
		void Async_Read();
		void On_Read(boost::system::error_code ec, size_t bytestransferred);
		void Async_Write();
		void On_Write(boost::system::error_code ec, size_t bytestransferred);
		
		boost::asio::io_context _ioContext{};
		boost::asio::ip::tcp::socket _socket;
		boost::asio::ip::tcp::resolver::results_type _endpoints;
		boost::asio::streambuf _streambuf{ 65536 };
		queue<string> _outgoingMessages{};

	};

	


}