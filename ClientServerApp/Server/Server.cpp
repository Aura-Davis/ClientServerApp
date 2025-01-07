#include <Networking/TCP_Server.h>
#include <iostream>

using namespace std;
int main(int argc, char* argv[])
{
	TCP::TCPServer server{ TCP::IPV::V4, 1337 };

    server.OnJoin = [](TCP::TCPConnection::pointer server) 
    {
            cout << "User has joined the server: " << server->GetUsername() << endl;
    };

    server.OnLeave = [](TCP::TCPConnection::pointer server) 
    {
            cout << "User has left the server: " << server->GetUsername() << endl;
    };

    server.OnMessage = [&server](const string& message) 
    {
        server.Broadcast(message);
    };

	server.Run();
	return 0;
}
