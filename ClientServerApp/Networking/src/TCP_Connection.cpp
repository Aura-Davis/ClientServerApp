#include "Networking/TCP_Connection.h"
#include <iostream>
using namespace std;

namespace TCP {
	TCPConnection::TCPConnection(boost::asio::ip::tcp::socket&& socket) : _socket(move(socket))
	{
		boost::system::error_code er;
		stringstream name;
		name << _socket.remote_endpoint();
		_username = name.str();
	}

	void TCPConnection::Start(MessageHandler&& messageHandler, ErrorHandler&& errorHandler) {
		_messageHandler = move(messageHandler);
		_errorHandler = move(errorHandler);
		Async_Read();
	}

	void TCPConnection::Post(const string& message)
	{
		bool queueIdle = _outgoingMessages.empty();
		_outgoingMessages.push(message);

		if (queueIdle)
		{
			Async_Write();
		}
	}

	void TCPConnection::Async_Read()
	{
		boost::asio::async_read_until(_socket, _streambuf, "\n",
			[self = shared_from_this()](boost::system::error_code ec, size_t bytesTransferred)
			{
				self->On_Read(ec, bytesTransferred);
			});
	}

	void TCPConnection::On_Read(boost::system::error_code ec, size_t bytesTransferred)
	{
		if (ec) {
			_socket.close(ec);
			_errorHandler();
			return;
		}

		stringstream message;
		message << _username << ": " << istream(&_streambuf).rdbuf();
		_streambuf.consume(bytesTransferred);
		_messageHandler(message.str());
		Async_Read();
	}

	void TCPConnection::Async_Write()
	{
		boost::asio::async_write(_socket, boost::asio::buffer(_outgoingMessages.front()),
			[self = shared_from_this()](boost::system::error_code ec, size_t bytesTransferred)
			{
				self->On_Write(ec, bytesTransferred);
			});
	}

	void TCPConnection::On_Write(boost::system::error_code ec, size_t bytesTransferred)
	{
		if (ec) {
			_socket.close(ec);
			_errorHandler();
			return;
		}

		_outgoingMessages.pop();
		if (!_outgoingMessages.empty()) Async_Write();
	}
}