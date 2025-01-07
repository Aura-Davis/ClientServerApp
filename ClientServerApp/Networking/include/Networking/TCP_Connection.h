#pragma once
#include <boost/asio.hpp>
#include <string>
#include <memory>
#include <queue>
using namespace std;
using boost::asio::ip::tcp;

namespace TCP {
	
	using MessageHandler = function<void(string)>;
	using ErrorHandler = function<void()>;
	
	class TCPConnection : public enable_shared_from_this<TCPConnection> {
	public:
		using pointer = shared_ptr<TCPConnection>;
		
		static pointer Create(boost::asio::ip::tcp::socket&& socket) {
			return pointer(new TCPConnection(move(socket)));
		}

		inline const string& GetUsername() const { return _username; }

		tcp::socket& Socket() {
			return _socket;
		}

		void Start(MessageHandler&& messageHandler, ErrorHandler&& errorHandler);
		void Post(const string& message);
	private:
		void Async_Read();
		void On_Read(boost::system::error_code ec, size_t bytesTransferred);
		void Async_Write();
		void On_Write(boost::system::error_code ec, size_t bytesTransferred);

		explicit TCPConnection(boost::asio::ip::tcp::socket&& socket);
			tcp::socket _socket;
			string _username;
			MessageHandler _messageHandler;
			ErrorHandler _errorHandler;
			queue<string> _outgoingMessages;
			boost::asio::streambuf _streambuf{65536};
	};
}