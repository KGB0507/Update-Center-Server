#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <vector>
#include <thread>
#include <iostream>
#include "SocketWrapper.h"
#include "ServerException.h"
#include "DistributiveHandler.h"
#include "RequestSubroutines.h"

struct ClientInfo
{
	SOCKET clientSocket = NULL;

	char clientIP[22] = {};

	sockaddr_in clientAddress = {};
	int clientAddressSize = 0;

	int bufSize = 0;
	char* buffer = nullptr;
};

class ConnectionManager
{
private:
	std::vector<std::thread> clientThreads;
	int numOfConnections;

	std::string distributivesLocation;

public:
	ConnectionManager();

	ConnectionManager(std::string distributivesLocation);

	~ConnectionManager();

	int ManageIncomingConnections(Socket serverSocket);

	int GetNumOfConnections();

	int HandleConnection(Socket serverSocket, ClientInfo clientInfo);
};

char* InitBuffer(const int bufferSize);

void KillBuffer(char* buffer);