#pragma once
#include <string>
#include <WS2tcpip.h>
#include <vector>
#pragma comment(lib,"ws2_32.lib")


class WinSocketCommon {
public:
	~WinSocketCommon();
protected:
	static bool winSocketInitialized;
	WinSocketCommon();

};

class Network : public WinSocketCommon {
public:
	Network(unsigned port,const std::string& IP);

	void sendMessage(const std::string& message, const std::string& IP);
	void listen();

private:
	int port;
	SOCKET in = socket(AF_INET, SOCK_DGRAM, 0);
	SOCKET out = socket(AF_INET, SOCK_DGRAM, 0);
	sockaddr_in serverHint;
	sockaddr_in client;
	int clientLength;
	char buf[1024];

};

