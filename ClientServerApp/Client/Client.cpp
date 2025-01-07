#include <Networking/TCP_Client.h>
#include <iostream>
#include <thread>
using namespace std;
using namespace TCP;

int main(int argc, char* argv[])
{
	TCPClient client{ "localhost", 1337 };

	client.OnMessage = [](const string& message)
		{
			cout << message;
		};

	thread t{ [&client]() {client.Run(); } };

	while (true)
	{
		string message;
		getline(cin, message);

		if (message == "\\q") break;
		message += "\n";
		client.Post(message);
	}

	client.Stop();
	t.join();

	return 0;
}
