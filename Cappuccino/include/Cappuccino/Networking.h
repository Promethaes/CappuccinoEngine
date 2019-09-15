#pragma once
#include <string>
#include <WS2tcpip.h>
#include <vector>
#pragma comment(lib,"ws2_32.lib")

//i followed this tutorial https://www.youtube.com/watch?v=uIanSvWou1M&

class WinSocketCommon {
public:
	~WinSocketCommon();
protected:
	static bool winSocketInitialized;
	WinSocketCommon();

};

class Server : public WinSocketCommon {
public:
	Server(unsigned port);
	void listen();
	void recvfromPackets();

	void cleanup();
private:
	SOCKET in = socket(AF_INET, SOCK_DGRAM, 0);
	sockaddr_in serverHint;
	sockaddr_in client;
	int clientLength;
	char buf[1024];

};

class Client : public WinSocketCommon {
public:
	Client(unsigned port);
	void sendMessage(const std::string& message);

private:
	sockaddr_in server;
	SOCKET out = socket(AF_INET, SOCK_DGRAM, 0);
};
class Network {
public:
	Network(unsigned port);
	~Network();
	void sendMessage(const std::string& message);
	void listen();

private:
	bool firstListen = true;
	Client* client;
	Server* server;
};