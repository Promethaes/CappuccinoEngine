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

Server::Server(unsigned port)
{
	if (!winSocketInitialized)
		return;


	serverHint.sin_addr.S_un.S_addr = ADDR_ANY;
	serverHint.sin_family = AF_INET;
	serverHint.sin_port = htons(port);

	if (bind(in, (sockaddr*)& serverHint, sizeof(serverHint)) == SOCKET_ERROR) {
		std::cout << "can't find socket " << WSAGetLastError() << "\n";
		return;
	}
	clientLength = sizeof(client);
	ZeroMemory(&client, clientLength);



}

void Server::listen()
{
	ZeroMemory(&buf, 1024);

	recvfromPackets();

	char clientIp[256];
	ZeroMemory(&clientIp, 256);

	inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);

	std::cout << clientIp << " : " << buf << "\n";
}
void Server::recvfromPackets()
{
	int bytesIn = recvfrom(in, buf, 1024, 0, (sockaddr*)& client, &clientLength);
	if (bytesIn == SOCKET_ERROR)
		std::cout << "error recieving from client " << WSAGetLastError() << "\n";
}

void Server::cleanup()
{
	closesocket(in);
}

Client::Client(unsigned port)
{
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);
}

void Client::sendMessage(const std::string& message)
{
	int sendOk = sendto(out, message.c_str(), message.size() + 1, 0, (sockaddr*)& server, sizeof(server));

	if (sendOk == SOCKET_ERROR)
		std::cout << "failed " << WSAGetLastError() << "\n";
}

Network::Network(unsigned port)
	:server(new Server(port))
{
	client = (new Client(port));
}

Network::~Network()
{
	delete client;
	delete server;
}

void Network::sendMessage(const std::string& message)
{
	client->sendMessage(message);
}

void Network::listen()
{
	if (firstListen) {
		client->sendMessage("ping");
		firstListen = false;
	}

	server->listen();
}

