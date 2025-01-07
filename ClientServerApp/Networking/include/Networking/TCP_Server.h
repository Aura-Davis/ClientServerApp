#pragma once
#include <boost/asio.hpp>
#include <Networking/TCP_Connection.h>
#include <functional>
#include <optional>
#include <unordered_set>
using namespace std;

namespace TCP {
	enum class IPV {
		V4,
		V6
	};

	class TCPServer {
		using ONJoinHandler = function<void(TCPConnection::pointer)>;
		using OnLeaveHandler = function<void(TCPConnection::pointer)>;
		using OnClientMessageHandler = function<void(string)>;

	public:
		TCPServer(IPV ipv, int port);
		int Run();
		void Broadcast(const string &message);
		ONJoinHandler OnJoin;
		OnLeaveHandler OnLeave;
		OnClientMessageHandler OnMessage;
	
	private:
		void startAccept();

		IPV _ipVersion;
		int _port;
		boost::asio::io_context _ioContext;
		boost::asio::ip::tcp::acceptor _acceptor;
		optional<boost::asio::ip::tcp::socket> _socket;
		unordered_set<TCPConnection::pointer> _connections {};
	};
}