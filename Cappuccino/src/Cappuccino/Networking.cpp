#include "Cappuccino/Networking.h"

#include <iostream>

bool WinSocketCommon::winSocketInitialized = false;

WinSocketCommon::~WinSocketCommon()
{
	WSACleanup();
}
WinSocketCommon::WinSocketCommon()
{
	if (!winSocketInitialized) {
		WSADATA data;
		WORD version = MAKEWORD(2, 2);
		int wsOk = WSAStartup(version, &data);

		if (wsOk != 0) {
			std::cout << "can't start winsock " << wsOk << "\n";
			return;
		}
		winSocketInitialized = true;
	}
}

Network::Network(unsigned port, const std::string& IP)
{
	if (!winSocketInitialized)
		return;
	this->port = port;

	serverHint.sin_addr.S_un.S_addr = ADDR_ANY;
	serverHint.sin_family = AF_INET;
	serverHint.sin_port = htons(port);

	inet_pton(AF_INET, IP.c_str(), &serverHint.sin_addr);

	if (bind(in, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR) {
		std::cout << "can't find socket " << WSAGetLastError() << "\n";
		return;
	}
	clientLength = sizeof(client);
	ZeroMemory(&client, clientLength);
}

void Network::sendMessage(const std::string& message, const std::string& IP)
{
	sockaddr_in temp;
	temp.sin_addr.S_un.S_addr = ADDR_ANY;
	temp.sin_family = AF_INET;
	temp.sin_port = htons(port);

	inet_pton(AF_INET, IP.c_str(), &temp.sin_addr);

	int sendOk = sendto(out, message.c_str(), message.size() + 1, 0, (sockaddr*)&temp, sizeof(temp));

	if (sendOk == SOCKET_ERROR)
		std::cout << "failed " << WSAGetLastError() << "\n";
}

void Network::listen()
{
	ZeroMemory(&buf, 1024);

	int bytesIn = recvfrom(in, buf, 1024, 0, (sockaddr*)&client, &clientLength);
	if (bytesIn == SOCKET_ERROR)
		std::cout << "error recieving from client " << WSAGetLastError() << "\n";

	char clientIp[256];
	ZeroMemory(&clientIp, 256);

	inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);

	std::cout << clientIp << " : " << buf << "\n";
	sendMessage("Message Recieved Anthony", clientIp);
}

std::string Network::listen(bool returnString)
{
	ZeroMemory(&buf, 1024);

	int bytesIn = recvfrom(in, buf, 1024, 0, (sockaddr*)&client, &clientLength);
	if (bytesIn == SOCKET_ERROR)
		std::cout << "error recieving from client " << WSAGetLastError() << "\n";

	char clientIp[256];
	ZeroMemory(&clientIp, 256);

	inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);

	std::cout << clientIp << " : " << buf << "\n";
	return std::string(buf);
}