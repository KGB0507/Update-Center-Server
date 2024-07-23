#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

//#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include "ServerException.h"

#pragma comment(lib, "Ws2_32.lib")

#define STDBUFFERSIZE 4096

class Socket
{
private:
    SOCKET serverSocket;
    WSADATA wsaData;
    sockaddr_in serverAddressStruct;
    int socketId;

    std::string serverAddressLiteral;
    int serverPort;

    bool socketStatus;

public:
    Socket(const std::string servAddress, const int servPort);

    Socket();

    ~Socket();

    int GetSocketId();

    bool GetSocketStatus();

    std::string GetServerAddress();

    int GetServerPort();

    sockaddr_in GetServerAddressStruct();

    int SendFile(std::string filePath, char* buffer);

    void ReceiveRequest(SOCKET clientSocket, char* buffer);

    void SendRequestAndReceiveResponse(SOCKET clientSocket, std::string request, char* buffer);
};
